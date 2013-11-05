#include "FortuneFunctions.h"
#include <File.h>
#include <Path.h>
#include <Directory.h>
#include <Entry.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <OS.h>

FortuneAccess::FortuneAccess(const char *folder)
{
	SetFolder(folder);
}

FortuneAccess::~FortuneAccess(void)
{
	MakeEmpty();
}

status_t FortuneAccess::SetFolder(const char *folder)
{
	if(!folder)
		return B_BAD_VALUE;
	
	fPath = folder; 
	ScanFolder();
	
	return B_OK;
}

status_t FortuneAccess::GetFortune(BString *target)
{
	if(!target)
		return B_BAD_VALUE;
	
	if(fRefList.CountItems()<1)
		return B_ERROR;
	
	int32 index = int32(float(rand()) / RAND_MAX * fRefList.CountItems());
	
	entry_ref *ref = (entry_ref*)fRefList.ItemAt(index);
	
	BFile file(ref,B_READ_ONLY);
	if(file.InitCheck()!=B_OK)
		return file.InitCheck();
	
	fLastFile = ref->name;
	
	off_t size;
	file.GetSize(&size);
	
	if(size<1)
		return B_ERROR;
	
	BString data;
	
	char *buffer = data.LockBuffer(size + 10);
	file.Read(buffer,size);
	data.UnlockBuffer();
	buffer = NULL;
	
	// We can't depend on a .dat file, so calculate the number of entries manually
	int32 entrycount = 0;
	int32 entrystart = 0;
	
	do
	{
		entrystart = data.FindFirst("%\n",entrystart + 1);
		entrycount++;
	} while(entrystart>0);
	
	int32 entry = int32(float(rand()) / RAND_MAX * (entrycount-1));

	entrystart = 0;
	for(int32 i=0; i<entry; i++)
		entrystart = data.FindFirst("%\n",entrystart + 1);
		
	BString entrydata;
	entrydata = data.String() + entrystart + 2;
	int32 entrylength = entrydata.FindFirst("%\n");
	if(entrylength>0)
		entrydata.Truncate(entrylength);
	
	*target = entrydata;
	return B_OK;
}

void FortuneAccess::ScanFolder(void)
{
	BDirectory dir(fPath.String());
	if(dir.InitCheck()!=B_OK)
		return;
	
	MakeEmpty();
	
	entry_ref ref;
	while(dir.GetNextRef(&ref)==B_OK)
	{
		BEntry entry(&ref);
		if(entry.IsFile())
			fRefList.AddItem(new entry_ref(ref));
	}
}

void FortuneAccess::MakeEmpty(void)
{
	for(int32 i=0; i<fRefList.CountItems(); i++)
	{
		entry_ref *ref = (entry_ref*)fRefList.ItemAt(i);
		delete ref;
	}
	fRefList.MakeEmpty();
}

int32 FortuneAccess::CountFiles(void) const
{
	return fRefList.CountItems();
}

status_t FortuneAccess::LastFilename(BString *target)
{
	if(!target)
		return B_BAD_VALUE;
	
	*target = fLastFile;
	
	return B_OK;
}
