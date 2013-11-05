#include "FortuneWindow.h"
#include <Alert.h>
#include <View.h>
#include <Button.h>
#include <ScrollView.h>
#include <Application.h>
#include <Screen.h>

#define M_GET_ANOTHER_FORTUNE 'gafn'
#define M_ABOUT_REQUESTED 'abrq'

FortuneWindow::FortuneWindow(void)
 : BWindow(BRect(0,0,300,300),"Fortune",B_DOCUMENT_WINDOW,B_ASYNCHRONOUS_CONTROLS),
 	fFortune("/boot/beos/etc/fortunes")
{
	BView *back = new BView(Bounds(),"background",B_FOLLOW_ALL, B_WILL_DRAW);
	back->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(back);
	
	BButton *close = new BButton(BRect(0,0,1,1),"closebutton","Close",
								new BMessage(B_QUIT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	close->ResizeToPreferred();
	close->MoveTo(Bounds().right - 15 - close->Frame().Width(), 
				Bounds().bottom - 15 - close->Frame().Height());
	back->AddChild(close);
	close->MakeDefault(true);
	
	BButton *next = new BButton(BRect(0,0,1,1),"nextbutton","Get Another",
								new BMessage(M_GET_ANOTHER_FORTUNE), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	next->ResizeToPreferred();
	next->MoveTo(close->Frame().left - 15 - next->Frame().Width(), 
				Bounds().bottom - 15 - next->Frame().Height());
	back->AddChild(next);
	
	BButton *about = new BButton(BRect(0,0,1,1),"aboutbutton","About" B_UTF8_ELLIPSIS,
								new BMessage(M_ABOUT_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	about->ResizeToPreferred();
	about->MoveTo(next->Frame().left - 15 - about->Frame().Width(), 
				Bounds().bottom - 15 - about->Frame().Height());
	back->AddChild(about);
	
	
	BRect r(15,15,Bounds().right - B_V_SCROLL_BAR_WIDTH - 15, next->Frame().top - 15);
	fTextView = new BTextView(r,"textview",r.OffsetToCopy(0,0).InsetByCopy(10,10),B_FOLLOW_ALL,
								B_WILL_DRAW | B_PULSE_NEEDED | B_FRAME_EVENTS);
	fTextView->MakeEditable(false);
	
	BScrollView *sv = new BScrollView("scrollview",fTextView,B_FOLLOW_ALL,0,false,true);
	back->AddChild(sv);
	
	BString fortune;
	status_t status = fFortune.GetFortune(&fortune);
	if(status == B_OK)
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
			"the folder /boot/beos/etc/fortunes.");
	}
	
	next->MakeFocus(true);
	
	// Calculate a good width for the window
	SetSizeLimits(45 + close->Bounds().Width() + next->Bounds().Width(),30000,200,30000 ) ;
	r = BScreen().Frame();
	MoveTo((r.Width()-Bounds().Width())/2,r.Height() / 4);
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
			fTextView->SetText("Fortuna had a problem getting a fortune.\n\n"
				"Please make sure that you have installed fortune files to "
				"the folder /boot/beos/etc/fortunes.");
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

void FortuneWindow::FrameResized(float w, float h)
{
	BRect textrect = fTextView->TextRect();
	
	textrect.right = textrect.left + (w - B_V_SCROLL_BAR_WIDTH - 40);
	fTextView->SetTextRect(textrect);
}
