/*
 * Camera.cpp
 *
 *  Created on: Sep 28, 2015
 *      Author: xro
 */

#include "Camera.h"

#include "Camera.h"
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

bool Camera::instanceFlagCam = false;
Camera* Camera::single = NULL;

static pthread_attr_t attr;
sched_param paramThreadProcsCam;
/* Threading for AlVoice */
static pthread_t threadHalCamera;
void* thread_HalCamera(void* dataHalCamera);


Camera* Camera::getInstance()
{
	if(! instanceFlagCam)
	{
		single = new Camera();
		instanceFlagCam = true;
		single->launch();
		return single;
	}
	else
	{
		return single;
	}
}

Camera::Camera()
{
	mutexCamera = PTHREAD_MUTEX_INITIALIZER;
	stop_HalCamera = false;
	initialized = false;
	init();
}

void Camera::init()
{
	capture.open( CV_CAP_OPENNI );
	if( !capture.isOpened() )
	{
		printf("[Camera] No Kinect device has been detected...\n[Camera] -> Testing common video capture devices...\n");
		capture.open(0);
		if( !capture.isOpened() )
		{
			printf("[Camera] Cannot find any video capture devices... Exiting camera module\n");
			has_kinect = false;
			return;
		}
		else
		{
			printf("[Camera] Video capture device has been detected.\n");
			initialized = true;
			has_kinect = false;
		}
	}
	else
	{
		printf("[Camera] Kinect has been detected.\n");
		initialized = true;
		has_kinect = true;
		capture.set( CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ );
	}
}

void Camera::launch()
{
	if(initialized)
	{
		capture_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
		capture_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);

		depthImage = Mat(Size(capture_width,capture_height),CV_8UC1);
		grayImage = Mat(Size(capture_width,capture_height),CV_8UC1);
		bgrImage = Mat(Size(capture_width,capture_height),CV_8UC3);

		pthread_attr_init(&attr);
		int newprio = -20;
		paramThreadProcsCam.sched_priority = newprio;
		pthread_attr_setschedparam (&attr, &paramThreadProcsCam);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

		pthread_create(&threadHalCamera, &attr, thread_HalCamera, NULL);
	}
}


void*  thread_HalCamera(void* dataHalCamera)
{
	Camera *cap = Camera::getInstance();
	float t = 0;
	while(!cap->getHalCamera_status())
	{
		//struct timeval start, end;
		//gettimeofday(&start, NULL);

		pthread_mutex_lock(&cap->mutexCamera);
		if( !cap->capture.grab() )
		{
			printf("Can not grab images.\n");
			break;
		}
		pthread_mutex_unlock(&cap->mutexCamera);

		//gettimeofday(&end, NULL);
		//t = (float)(end.tv_sec-start.tv_sec)*1000.0f + (float)(end.tv_usec - start.tv_usec)/1000.0f;
		waitKey(10);		// Be sure to wait max 30ms before new grab
	}
	return NULL;
}


Size Camera::getCameraSize()
{
	return Size(capture_width, capture_height);
}


void Camera::retrieveDepthImg(Mat img)
{
	if(has_kinect)
	{
		pthread_mutex_lock(&mutexCamera);
		if( capture.retrieve( depthImage, CV_CAP_OPENNI_DEPTH_MAP ) )
		{
			depthImage.convertTo( img, CV_8UC1, 0.05f/*scale factor*/ );
		}
		else
		{
			printf("Error while retrieving Depth image\n");
		}
		pthread_mutex_unlock(&mutexCamera);
	}
	else
	{
		pthread_mutex_lock(&mutexCamera);
		if( capture.retrieve( grayImage) )
		{
			cv::cvtColor(grayImage, img, CV_BGR2GRAY);
		}
		else
		{
			printf("Error while retrieving Gray image\n");
		}
		pthread_mutex_unlock(&mutexCamera);
	}
}

void Camera::retrieveRGBImg(Mat img)
{
	if(has_kinect)
	{
		pthread_mutex_lock(&mutexCamera);
		if( capture.retrieve( bgrImage, CV_CAP_OPENNI_BGR_IMAGE ) )
		{
			bgrImage.copyTo(img);
		}
		else
		{
			printf("Error while retrieving RGB image\n");
		}
		pthread_mutex_unlock(&mutexCamera);
	}
	else
	{
		pthread_mutex_lock(&mutexCamera);
		if( capture.retrieve( bgrImage ) )
		{
			bgrImage.copyTo(img);
		}
		else
		{
			printf("Error while retrieving RGB image\n");
		}
		pthread_mutex_unlock(&mutexCamera);
	}
}

bool Camera::retrieveGrayImg(Mat img)
{
	bool ret = false;
	if(has_kinect)
	{
		pthread_mutex_lock(&mutexCamera);
		if( capture.retrieve( grayImage, CV_CAP_OPENNI_GRAY_IMAGE ) )
		{
			grayImage.copyTo(img);
			ret = true;
		}
		else
		{
			printf("Error while retrieving Gray image\n");
		}
		pthread_mutex_unlock(&mutexCamera);
	}
	else
	{
		pthread_mutex_lock(&mutexCamera);
		if( capture.retrieve( grayImage ) )
		{
			cvtColor(grayImage, img, CV_BGR2GRAY);
			ret = true;
		}
		else
		{
			printf("Error while retrieving Gray image\n");
		}
		pthread_mutex_unlock(&mutexCamera);
	}
	return ret;
}

bool Camera::getHalCamera_status()
{
	return stop_HalCamera;
}
