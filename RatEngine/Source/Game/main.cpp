#include "AppWindow.h"

int main()
{
	AppWindow app;
	if (app.init())
	{
		// game loop
		while (app.isRunning())
		{
			app.broadcast();
		}
	}

	return 0;
}