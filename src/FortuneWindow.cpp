#include "FortuneWindow.h"
#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <FindDirectory.h>
#include <LayoutBuilder.h>
#include <Path.h>
#include <Screen.h>
#include <ScrollView.h>
#include <View.h>

#define M_GET_ANOTHER_FORTUNE 'gafn'
#define M_ABOUT_REQUESTED 'abrq'

FortuneWindow::FortuneWindow(void)
	:
	BWindow(BRect(0, 0, 300, 400), "Fortuna", B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS | B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS),
	fFortune()
{
	BPath path;
	find_directory(B_SYSTEM_DATA_DIRECTORY, &path);
	path.Append("fortunes");

	fFortune.SetFolder(path.Path());

	BButton* next = new BButton("nextbutton", "Get Another",
		new BMessage(M_GET_ANOTHER_FORTUNE));

	BButton* about = new BButton("aboutbutton", "About" B_UTF8_ELLIPSIS,
		new BMessage(M_ABOUT_REQUESTED));

	fTextView = new BTextView("textview", B_WILL_DRAW | B_PULSE_NEEDED | B_FRAME_EVENTS);
	fTextView->MakeEditable(false);
	fTextView->SetInsets(10, 10, 10, 10);

	BScrollView* sv = new BScrollView("scrollview", fTextView, 0, false, true);

	BString fortune;
	status_t status = fFortune.GetFortune(&fortune);

	if (status == B_OK)
	{
		BString title;
		fFortune.LastFilename(&title);
		title.Prepend("Fortune: ");
		SetTitle(title.String());

		fTextView->SetText(fortune.String());
	}
	else
	{
		fTextView->SetText("Fortuna had a problem getting a fortune.\n\n"
			"Please make sure that you have installed fortune files to "
			"the folder ");
		fTextView->Insert(path.Path());
		fTextView->Insert(".");
	}

	next->MakeFocus(true);
	next->MakeDefault(true);

	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(sv)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(next)
			.Add(about)
			.AddGlue()
			.End();
}

bool FortuneWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void FortuneWindow::MessageReceived(BMessage *msg)
{
	if(msg->what == M_GET_ANOTHER_FORTUNE)
	{
		BString fortune;
		status_t status = fFortune.GetFortune(&fortune);
		if(status == B_OK)
		{
			BString filename(fortune);
			int32 lastslash = filename.FindLast("/");
			if(lastslash>=0)
			{
				filename = filename.String() + lastslash + 1;
				filename.Prepend("Fortune: ");
				SetTitle(filename.String());
			}
			fTextView->SetText(fortune.String());
		}
		else
		{
			BPath path;
			find_directory(B_SYSTEM_DATA_DIRECTORY, &path);
			path.Append("fortunes");

			fTextView->SetText("Fortuna had a problem getting a fortune.\n\n"
				"Please make sure that you have installed fortune files to "
				"the folder ");
			fTextView->Insert(path.Path());
			fTextView->Insert(".");
		}
	}
	else
	if(msg->what == M_ABOUT_REQUESTED)
	{
		BAlert *alert = new BAlert("Fortuna","Fortuna\n\nA fortune program for BeOS\n\n"
									"(C) 2006 DarkWyrm", "OK");
		alert->Go();
	}
	else
		BWindow::MessageReceived(msg);
}
