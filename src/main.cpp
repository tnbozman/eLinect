/*
 * main.cpp
 *
 *  Created on: 01/02/2012
 *      Author: Troy Boswell
 */



// Standard libs
#include <iostream>
// Libfreenect
#include <libfreenect.hpp>
#include <cmath>
// Opencv
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
// elinux headers

#include "kinect_device.h"
#include "udpNetworkInterface.h"
#include "objectHunter.h"
#include "occupancyMap.h"
#include "stateMachine.h"


using namespace std;
using namespace cv;


#define 	PRINT_COMMENTS  false
#define 	X_RES 			640
#define 	Y_RES 			480
#define 	DEPTH_PIXELS 	X_RES*Y_RES
#define 	EDGE_THRESHOLD 	10
#define		ms_DELAY 		1
#define		ESC				27

// Local Global Variables
char *host_name = "127.0.0.1" ; // local host



int main() //========================================================
{
//--- local data.
	bool temp;

	occupancyMap *map = new occupancyMap;
	objectHunter *objects = new objectHunter;
	objects->updateCurrentMap(map);

	// Create a UDP network connection
	udpNetworkInterface *udpSocket = new udpNetworkInterface;

	if(!udpSocket->getHostByName(host_name)){
		exit(-1);
	}
	if(!udpSocket->sin_init(&udpSocket->sin)){
		exit(-1);
	}
	if(!udpSocket->createSocket()){
		exit(-1);
	}
	if(!udpSocket->bindSocket()){
		exit(-1);
	}
	udpSocket->setSinLength();


   bool die(false);

   Freenect::Freenect freenect;
   MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);

   namedWindow("depth",CV_WINDOW_AUTOSIZE);

   device.startVideo();
   device.startDepth();
   Mat depthf(Size(X_RES,Y_RES),CV_8UC1);

   while(!die){

	  if(PRINT_COMMENTS)
		  printf("\n   Awaiting UDP packets from talker ...\n\n") ;
	  // Check if a UDP packet is ready to be read, I have set the flag to MSG_DONTWAIT which
	  // causes recvfrom not to block the until a packet is received but to simply continue
	  temp = udpSocket->recvFrom();
   	   /*********************************************************/
   	   /*
    	* buf holds the udp payload that is the control information for the HHH-ekinux capture and process
    	* buf[0] = ekinux function to be performed
    	* 			- 'q'  = 	Exit (quit program)
    	* 			- '1'  = 	capture RGB
    	* 			- '2'  = 	capture depth
    	* 			- '3'  = 	capture RGB + depth
		*			- 'a'  = capture + process RGB
    	* 			- 'b'  = capture + process depth
    	* 			- 'c'  = capture + process RGB + process depth
    	* 			- 'd'  = move to 0 degrees
    	* 			- 'e'  = move to -45 degrees
    	* 			- 'f'  = move to -90 degrees
    	* 			- 'g'  = move to 45 degrees
    	* 			- 'h'  = move to 90 degrees
    	* 			- 'Y'  = Adjust tilt degrees of the kinect motor -30 > x > 30 (example Y+10 = 10 deg)
    	* 			- 'M'  = Print Occupancy Map
    	* 			- 'O'  = Print Objects
    	*
    	* For all inputs except 'Y'
    	* buf[1] = x co-ordinate
    	* buf[2] = y co-ordinate
    	*
    	*
    	*/

   	   //  	buf[0] = '3';
   	   if(temp > 0){
   		die = state_mach(udpSocket->buf, device,objects);

   		   if(PRINT_COMMENTS)
   			printf("   Received packet was \"%s\".\n\n", udpSocket->buf) ;

   		   // Clear recv buffer
   		   udpSocket->clearBuf();

   	   }else{
   		   if(PRINT_COMMENTS)
   			   printf("No Packet Received") ;
   		   imshow("depth",depthf);
   	   }
   	   char key = cvWaitKey(ms_DELAY);
   	   if( key == ESC ){
   	   			break;
   	   	}
   	   temp = 0;
   }
	device.stopVideo();
	device.stopDepth();
	cvDestroyWindow("depth");
	return 0;

} //main, all file and socket descriptors automatically closed.



