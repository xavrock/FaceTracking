/*
 * Display.h
 *
 *  Created on: Oct 1, 2013
 *      Author: Xavier Roche
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "Camera.h"
#include <pthread.h>
#include <queue>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class Display
{
private:
	static bool 			instanceFlagDisplay;
	static Display	 		*single;
	bool					stop_HalDisplay;
	bool					initialized;
	Camera					*camera;
	Mat						RGBImg;
	deque<Rect>				candidates;

	Display();
	void init();

public:

	pthread_mutex_t			mutexDisplay;

	static Display*	 		getInstance();
	void 					launch();
	bool					getHalDisplay_status();
	void 					retrieveImg();
	void					displayCandidates();
	void					showImg();
	~Display();
};

#endif /* DISPLAY_H_ */
