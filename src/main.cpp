/** @mainpage FaceTracker - None
 *
 * @author Xavier Roche <anony@mo.us>
 * @version 1.0.0
**/


#include <cstdio>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "Camera.h"
#include "Display.h"

/**
 * Main class of project FaceTracker
 *
 * @param argc the number of arguments
 * @param argv the arguments from the commandline
 * @returns exit code of the application
 */
int main(int argc, char **argv) {
	// print a greeting to the console
	printf("Hello World!\n");
	
	Camera *cam = NULL;
	cam = Camera::getInstance();
	cam->launch();
	
	//Display *disp = NULL;
	//disp = Display::getInstance();
	
	while(1){usleep(1000);}

	return 0;
}
