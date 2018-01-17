//--------------------------- filterdesigner.cpp ---------------------------

#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "appwin.h"

int main(int argc, char *argv[])
{
    for (int i= 0; i < argc; i++)
    {
	char *Current= argv[i];

	if (! (strcmp(Current, "-v") && strcmp(Current, "--version")))
	{
	    printf("Filter Designer Version: %s\n", VERSION);
	    return 0;
	}
    }

    FXApp MyApp("FilterDesigner", "Simo");
    MyApp.init(argc, argv);
    AppWin *Awin= new AppWin(&MyApp);
    MyApp.addSignal(SIGINT, Awin, AppWin::ID_EXIT);
    MyApp.create();

    return MyApp.run();
}

//--------------------------- FilterDesigner.cpp ---------------------------
