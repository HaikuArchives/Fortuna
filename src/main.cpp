#include <Application.h>
#include <OS.h>
#include <stdlib.h>
#include "FortuneWindow.h"

class App : public BApplication
{
public:
	App(void);
};

App::App(void)
 : BApplication("application/x-vnd.wgp-Fortuna")
{
	FortuneWindow *win = new FortuneWindow();

	win->CenterOnScreen();
	win->Show();
}

int main(void)
{
	srand(system_time());
	
	App app;
	app.Run();
	return 0;
}
