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

static bool facedetected = false;


void detectAndDraw( Mat img, CascadeClassifier cascade, double scale )
{
    int i = 0;
    double t = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    //equalizeHist( smallImg, smallImg );

   // t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.2, 2, 0
        |CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );
    //t = (double)cvGetTickCount() - t;
    //printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;

        double aspect_ratio = (double)r->width/r->height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
                       cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
                       color, 3, 8, 0);
    }
}




/**
 * Main class of project FaceTracker
 *
 * @param argc the number of arguments
 * @param argv the arguments from the commandline
 * @returns exit code of the application
 */
int main(int argc, char **argv) {
	
	/*Camera *cam = NULL;
	cam = Camera::getInstance();
	cam->launch();
	
	Display *disp = NULL;
	disp = Display::getInstance();
	
	while(1){usleep(1000);}*/
	
	string cascadeName = "/usr/share/lbpcascades/lbpcascade_frontalface.xml";
	

    bool tryflip = false;
    
    CascadeClassifier cascade;
    double scale = 1.5;
    
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
		detectAndDraw(bgrImage, cascade, scale);
		imshow( "Display FaceTracker", bgrImage );
		waitKey(1);
	}

	return 0;
}
