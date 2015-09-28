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

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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
	
	/*Camera *cam = NULL;
	cam = Camera::getInstance();
	cam->launch();
	
	Display *disp = NULL;
	disp = Display::getInstance();
	
	while(1){usleep(1000);}*/
	
	string cascadeName = "/usr/share/haarcascades/haarcascade_frontalface_alt2.xml";
	

    bool tryflip = false;
    
    CascadeClassifier cascade;
    double scale = 1.4;
    
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }
    
    
	
	VideoCapture capture;
	capture.open(0);
	
	
		int capture_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
		int capture_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);

		Mat depthImage = Mat(Size(capture_width,capture_height),CV_8UC1);
		Mat grayImage = Mat(Size(capture_width,capture_height),CV_8UC1);
		Mat bgrImage = Mat(Size(capture_width,capture_height),CV_8UC3);
	
	cvNamedWindow( "Display FaceTracker", 1 );
	
	while(1)
	{
		capture.grab();
		capture.retrieve( bgrImage );
		
		imshow( "Display FaceTracker", bgrImage );
		waitKey(1);
	}

	return 0;
}
