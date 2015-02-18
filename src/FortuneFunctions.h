#ifndef FORTUNEFUNCTIONS_H
#define FORTUNEFUNCTIONS_H

#include <String.h>
#include <List.h>

class FortuneAccess
{
public:
	FortuneAccess(const char *folder);
	FortuneAccess();
	~FortuneAccess(void);
	
	status_t SetFolder(const char *folder);
	status_t GetFortune(BString *target);
	int32 CountFiles(void) const;
	status_t LastFilename(BString *target);
	
private:
	void ScanFolder(void);
	void MakeEmpty(void);
	
	BString fPath, fLastFile;
	BList fRefList;
	
};

#endif
