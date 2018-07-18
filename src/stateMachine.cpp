/*
 * stateMachine.cpp
 *
 *  Created on: 02/02/2012
 *      Author: troy
 */

#include "stateMachine.h"



IplImage *cedge = 0;

bool state_mach(char* state,MyFreenectDevice& device,objectHunter *objects ){
	bool hold(true);
	int x =0;
	int y = 0;
	int direction = 0;

	if(strlen(state) > 3){
		int x = charToint(state[1]);
		int y = charToint(state[2]);
		int direction = charToint(state[3]);// false = Y dir  True = X dir

		if(x == ERROR){}
		else if(y == ERROR){}
		else if(direction == ERROR){}
		else{
			device.setDistanceThreshold(device.updateDistanceThreshold(x, y, direction));
		}
	}

	IplImage *depthimg = cvCreateImageHeader(cvSize(X_RES,Y_RES), 8, 1);
	Mat depthf  (Size(640,480),CV_8UC1);
	Mat depthMat(Size(640,480),CV_16UC1);
	Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));

	// UDP control character buf[0]
	switch(state[0]){
		// buf[0] == 'q' is quit the running program gracefully
		case 'q':
			if(PRINT_COMMENTS)
				cout << "Exiting"<<endl;
			return true;
			break;

		// buf[0] == '1' captures the current value of the rgb camera
		case '1':
		// Issue = Fixed
		// Issue: 	resides in while loop as the images cannot be guaranteed to be valid due to
		// 		  	the use of a synchronous udp recvfrom causing freenect to drop frames
			if(CONFIRM_CURRENT_FRAMES)
				while (!device.getVideo(rgbMat));
			cv::imshow("rgb processed", rgbMat);
			break;

		// buf[0] == '2' captures the current value of the depth camera
		case '2':
		// Issue = Fixed
		// Issue: 	resides in while loop as the images cannot be guaranteed to be valid due to
		// 		  	the use of a synchronous udp recvfrom causing freenect to drop frames
			while (!device.getDepth(depthMat));

				depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
				cv::imshow("depth  processed",depthf);

			break;

		// buf[0] == '3' captures the current value of the rgb and depth image simultaneously
		// Issue = Fixed
		// ISSUE: 	the call the retrieve the most recent depth image needs to be called twice due to the
		//        	current capture sometimes being that of an older image, this simple implementation seems
		// 		  	to correct the problem but will need to be examined to see what are the timing implications.
		case '3':
		// Issue = Fixed
		// Issue: 	resides in while loop as the images cannot be guaranteed to be valid due to
		// 		  	the use of a synchronous udp recvfrom causing freenect to drop frames

		// Issue = Fixed
		// Issue: 	if the get functions are not simultaneously valid no image will be captured
		//        	but both need to be valid to ensure that two images are simultaneously captured
			hold = true;
			if(CONFIRM_CURRENT_FRAMES)
				while(device.getDepth(depthMat));
			while(hold){
				if ((device.getVideo(rgbMat)) && (device.getDepth(depthMat))){
					cv::imshow("rgb processed", rgbMat);

					depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
					cv::imshow("depth  processed",depthf);
					hold = false;
				}
			}

			break;
			// 'a'  = capture + process RGB
		case 'a':
			while (!device.getVideo(rgbMat));
			cv::imshow("rgb  processed", rgbMat);
			break;
			break;
		 // 'b'  = capture + process depth
		case 'b':
			hold = true;
			if(CONFIRM_CURRENT_FRAMES)
				while(device.getDepth(depthMat));
			while(hold){

				if ((device.getDepth(depthMat))){

						depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
						cv::imshow("depth  processed",depthf);

					hold = false;
				}
			}
			break;
		// 'c'  = capture + process RGB + process depth
		case 'c':
			hold = true;
			if(CONFIRM_CURRENT_FRAMES)
				while(device.getDepth(depthMat));
			while(hold){

				if ((device.getVideo(rgbMat)) && (device.getDepth(depthMat))){

					cv::imshow("rgb  processed", rgbMat);
					depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
					depthimg->imageData = (char *)depthf.data;
					cedge = cvCreateImageHeader(cvSize(X_RES,Y_RES), 8, 1);;
					char data[640*480];
					cedge->imageData= data;
					objects->edgeDetector(depthimg,cedge);
					objects->findObjectsOfAvoidance(cedge,&depthMat, x, y, direction);
					cvShowImage("depth  processed", cedge);

					hold = false;
				}
			}
			break;
		case 'd':
			break;
		case 'e':
			break;
		case 'f':
			break;
		case 'g':
			break;
		case 'h':
			break;

		// buf[0] == 'Y' Set the kinect Y axis motor (Example Y+20 = 20 degrees)
		case 'Y':
			if((sizeof(state) > 3) ){
				char val[3];
				double num;
				int sign;
				val[0] = state[2];
				val[1] = state[3];
				val[2] = 0;

				if(state[1] == '-')
					sign = NEGATIVE;
				else
					sign = POSITIVE;

				num = atof(val);
				num = num * sign;
				cout << num << endl;
				if(num >-MAX_KINECT_TILT_ANGLE && num < MAX_KINECT_TILT_ANGLE){
					device.setTiltDegrees(num);
					if(PRINT_COMMENTS)
						cout << "Kinect Angle = "<< num << endl;

				}else{
					if(PRINT_COMMENTS)
						cout << "invalid angle for option Y."<< endl;
				}



			}
			break;
		case 'M':
			objects->currentMap->printOccupancyMap();
			break;
		case 'O':
			break;

		default:
			if(PRINT_COMMENTS)
			cout << " State Machine Failure, Default Case Reached" << endl;
			break;
	}

	return false;
}

int charToint(char in){
	int output;
	switch(in)
	{
		case '0':
			output = 0;
			break;
		case '1':
			output = 1;
			break;
		case '2':
			output = 2;
			break;
		case '3':
			output = 3;
			break;
		case '4':
			output = 4;
			break;
		case '5':
			output = 5;
			break;
		case '6':
			output = 6;
			break;
		case '7':
			output = 7;
			break;
		case '8':
			output = 8;
			break;
		case '9':
			output = 9;
			break;
		case 'a':
			output = 10;
			break;
		case 'b':
			output = 11;
			break;
		case 'c':
			output = 12;
			break;
		case 'd':
			output = 13;
			break;
		case 'e':
			output = 14;
			break;
		case 'f':
			output = 15;
			break;
		default:
			output = ERROR;
			break;
	}
	return output;
}

