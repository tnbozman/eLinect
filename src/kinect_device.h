/*
 * kinect_device.h
 *
 *  Created on: 01/02/2012
 *      Author: troy
 */

#ifndef KINECT_DEVICE_H_
#define KINECT_DEVICE_H_

#include <libfreenect.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace std;
using namespace cv;

#define     PRINT_TEST			    	true

#define 	PRINT_COMMENTS  			false
#define 	X_RES 						640
#define     X_RES_LIMIT 				631
#define 	Y_RES 						480
#define 	DEPTH_PIXELS 				X_RES*Y_RES
#define 	MAX_Y						350
#define 	MAX_X   					400
#define 	MM_CONVERSION 				10.0
#define 	COORDINATE_SIZE 			25.0 // cm
#define 	EXTENSION_FACTOR 			(COORDINATE_SIZE * MM_CONVERSION * 0.65)
#define     DETECTION_PIXEL_OFFSET 		8
#define 	INT_DETECTION_LINE  		315 /*315 370*/
#define     INT_DISTANCE_THRESHOLD 		500
#define		mm2Depth_FORMULA 			(-4690.4175 * atan(75.0 / mm) + 1092.356263)
#define		depth2mm_FORMULA 			(75.0 /(tan((-(double)N /4690.4175)+(1092.356263/4690.4175))))
#define 	YAXIS_THETA 				28.45
#define		SHADOW_FILTER_pixelFactor 	1
#define     SHADOW_FILTER_shadowFactor 	0.35

//http://www.morethantechnical.com/2010/11/22/kinect-and-opencv-2-1/
#define 	FREENECT_COUNTS_PER_G   			819
#define 	FREENECT_DEPTH_10BIT_PACKED_SIZE   	384000
#define 	FREENECT_DEPTH_10BIT_SIZE   		FREENECT_DEPTH_11BIT_SIZE
#define 	FREENECT_DEPTH_11BIT_PACKED_SIZE   	422400
#define 	FREENECT_DEPTH_11BIT_SIZE   		(FREENECT_FRAME_PIX*sizeof(uint16_t))
#define 	FREENECT_FRAME_H   					Y_RES
#define 	FREENECT_FRAME_PIX   				(FREENECT_FRAME_H*FREENECT_FRAME_W)
#define 	FREENECT_FRAME_W   					X_RES
#define 	FREENECT_IR_FRAME_H   				488
#define 	FREENECT_IR_FRAME_PIX   			(FREENECT_IR_FRAME_H*FREENECT_IR_FRAME_W)
#define 	FREENECT_IR_FRAME_W   				X_RES
#define 	FREENECT_VIDEO_BAYER_SIZE   		(FREENECT_FRAME_PIX)
#define 	FREENECT_VIDEO_IR_10BIT_PACKED_SIZE 390400
#define 	FREENECT_VIDEO_IR_10BIT_SIZE   		(FREENECT_IR_FRAME_PIX*sizeof(uint16_t))
#define 	FREENECT_VIDEO_IR_8BIT_SIZE   		(FREENECT_IR_FRAME_PIX)
#define 	FREENECT_VIDEO_RGB_SIZE   			(FREENECT_FRAME_PIX*3)
#define 	FREENECT_VIDEO_YUV_RAW_SIZE  	 	(FREENECT_FRAME_PIX*2)
#define 	FREENECT_VIDEO_YUV_RGB_SIZE   		(FREENECT_VIDEO_RGB_SIZE)

enum direction{POSTIVE_X,NEGATIVE_X,POSTIVE_Y,NEGATIVE_Y};

class Mutex {
public:
	Mutex() {
		pthread_mutex_init( &m_mutex, NULL );
	}
	void lock() ;
	void unlock();
private:
	pthread_mutex_t m_mutex;
};

class MyFreenectDevice : public Freenect::FreenectDevice {

  public:

	// Constructor
	MyFreenectDevice(freenect_context *_ctx, int _index)
		: Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(FREENECT_DEPTH_11BIT_SIZE),m_buffer_rgb(FREENECT_VIDEO_RGB_SIZE), m_new_rgb_frame(false), m_new_depth_frame(false),
		  depthMat(Size(X_RES,Y_RES),CV_16UC1), rgbMat(Size(X_RES,Y_RES),CV_8UC3,Scalar(0)), ownMat(Size(X_RES,Y_RES),CV_8UC3,Scalar(0))
	{
		 // setDetectionLine(INT_DETECTION_LINE);
		  setDistanceThreshold(INT_DISTANCE_THRESHOLD);
	}
	// Public Member Function
	// Do not call directly even in child
	void VideoCallback(void* _rgb, uint32_t timestamp) ;
	// Do not call directly even in child
	void DepthCallback(void* _depth, uint32_t timestamp) ;

	bool getVideo(Mat& output) ;
	bool getDepth(Mat& output) ;

	void setDistanceThreshold(uint val){distance_threshold = val;}

	uint getDistanceThreshold(){return distance_threshold;}

	double depth2mm(uint N);
	double updateDistanceThreshold(int x, int y, int dir);

  private:
	// Private Member Function
	uint shadowFilter(Mat &inMat,  int i, int j, uint current,uint previous);
	uint depthFilter(uint input,uint threshold);
	double mm2Depth(double mm);

	// Private member variables
	std::vector<uint8_t> m_buffer_depth;
	std::vector<uint8_t> m_buffer_rgb;
	Mat depthMat;
	Mat rgbMat;
	Mat ownMat;
	Mutex m_rgb_mutex;
	Mutex m_depth_mutex;
	bool m_new_rgb_frame;
	bool m_new_depth_frame;
	char depth_data[DEPTH_PIXELS * 3];
	uint distance_threshold;

};

#endif /* KINECT_DEVICE_H_ */
