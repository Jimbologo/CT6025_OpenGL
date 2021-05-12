#include "App.h"

#define DEFAULT_SCREENWIDTH 1000
#define DEFAULT_SCREENHEIGHT 700
// main that controls the creation/destruction of an application
int WinMain(int , char* )
{
	// explicitly control the creation of our application
	App* app = new App();
	app->run("CT6025 Graphics Programming", DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, false);

	// explicitly control the destruction of our application
	delete app;

	return 0;
}