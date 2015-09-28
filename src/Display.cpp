
#include "Display.h"
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

bool Display::instanceFlagDisplay = false;
Display* Display::single = NULL;

static pthread_attr_t attr;
sched_param paramThreadProcsDisplay;
/* Threading for AlVoice */
static pthread_t threadHalDisplay;
void* thread_HalDisplay(void* dataHalDisplay);


Display* Display::getInstance()
{
	if(! instanceFlagDisplay)
	{
		single = new Display();
		instanceFlagDisplay = true;
		single->launch();
		return single;
	}
	else
	{
		return single;
	}
}

Display::Display()
{
	mutexDisplay = PTHREAD_MUTEX_INITIALIZER;
	stop_HalDisplay = false;
	initialized = false;
	init();
}

void Display::init()
{
	camera = Camera::getInstance();
	RGBImg = Mat(camera->getCameraSize(),CV_8UC3);
	initialized = true;
}

void Display::launch()
{
	if(initialized)
	{


		pthread_attr_init(&attr);
		int newprio = -10;
		paramThreadProcsDisplay.sched_priority = newprio;
		pthread_attr_setschedparam (&attr, &paramThreadProcsDisplay);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

		pthread_create(&threadHalDisplay, &attr, thread_HalDisplay, NULL);
	}
}


void Display::retrieveImg()
{
	camera->retrieveRGBImg(RGBImg);
}

void Display::showImg()
{
	imshow( "Display FaceTracker", RGBImg );
}

void Display::displayCandidates()
{
	/*tracker->getCandidates(&candidates);
	for( int i = 0 ; i < candidates.size(); i++)
	{
		Rect r = Rect(candidates[i]);
		Point center;
		int radius;
		center.x = r.x + (int)(r.width * 0.5);
		center.y = r.y + (int)(r.height * 0.5);
		radius = cvRound((r.width + r.height) * 0.25);
		//circle( RGBImg, center, radius, CV_RGB(255,128,255), 2, 8, 0 );
		rectangle( RGBImg, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), CV_RGB(0,255,0), 1, 8, 0 );

		candidates.erase(candidates.begin() + i);
		//i--;
	}*/
}

void*  thread_HalDisplay(void* dataHalDisplay)
{
	Display *disp = Display::getInstance();
	float t = 0;
	while(!disp->getHalDisplay_status())
	{
		//struct timeval start, end, endtempo;
		//gettimeofday(&start, NULL);

		disp->retrieveImg();
		//disp->displayCandidates();
		disp->showImg();

		//pthread_mutex_unlock(&cap->mutexDisplay);

		//gettimeofday(&end, NULL);
		//t = (float)(end.tv_sec-start.tv_sec)*1000.0f + (float)(end.tv_usec - start.tv_usec)/1000.0f;
		//printf("Display time : %f\n", t);
		//if( t > 20 )
		//	usleep(abs(20 - (int)t)*1000);
		waitKey( 10 );		// Be sure to wait max 30ms before new grab

		//gettimeofday(&endtempo, NULL);
		//t = (float)(endtempo.tv_sec-start.tv_sec)*1000.0f + (float)(endtempo.tv_usec - start.tv_usec)/1000.0f;
		//printf("Display tempo time : %f\n", t);
	}
	return NULL;
}


bool Display::getHalDisplay_status()
{
	return stop_HalDisplay;
}
