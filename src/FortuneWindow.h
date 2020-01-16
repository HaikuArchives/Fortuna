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

private:
	BTextView *fTextView;
	FortuneAccess fFortune;
};

#endif
