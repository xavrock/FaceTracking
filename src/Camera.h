/*
 * Camera.h
 *
 *  Created on: Sep 28, 2015
 *      Author: xro
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <pthread.h>
#include <queue>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class Camera
{
private:
	static bool 			instanceFlagCam;
	static Camera	 		*single;
	bool					stop_HalCamera;
	bool					initialized;
	Mat						depthImage;
	Mat						bgrImage;
	Mat						grayImage;

	int						capture_height;
	int						capture_width;
	bool					has_kinect;

	Camera();
	void init();

public:

	pthread_mutex_t			mutexCamera;
	VideoCapture 			capture;

	static Camera*	 		getInstance();
	void 					launch();
	bool					getHalCamera_status();
	Size					getCameraSize();
	void 					retrieveDepthImg(Mat img);
	void 					retrieveRGBImg(Mat img);
	bool 					retrieveGrayImg(Mat img);
	~Camera();
};

#endif /* CAMERA_H_ */
