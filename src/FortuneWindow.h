#ifndef FORTUNEWINDOW_H
#define FORTUNEWINDOW_H

#include <Window.h>
#include <TextView.h>
#include "FortuneFunctions.h"

class FortuneWindow : public BWindow
{
public:
	FortuneWindow(void);
	bool QuitRequested(void);
	void MessageReceived(BMessage *msg);
	void FrameResized(float w, float h);
	
private:
	BTextView *fTextView;
	FortuneAccess fFortune;
};

#endif
