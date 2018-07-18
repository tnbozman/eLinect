/*
 * objectHunter.h
 *
 *  Created on: 02/02/2012
 *      Author: troy
 */

#ifndef OBJECTHUNTER_H_
#define OBJECTHUNTER_H_

#include <iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "occupancyMap.h"
using namespace std;
using namespace cv;

#define     TESTING_PRINT			    true

#define 	PRINT_COMMENTS  			false
#define 	X_RES 						640
#define     X_RES_LIMIT 				631
#define 	Y_RES 						480
#define     DETECTION_PIXEL_OFFSET 		10
#define 	DRAW_GREY					120
// Y = 0 degrees
//#define 	INIT_DETECTION_LINE 		290
// Y = -8 degrees
#define 	INIT_DETECTION_LINE 		200
#define 	WHITE 						cvScalar(255,255,255)
#define 	EDGE_THRESHOLD 				10
#define		depth2mm_FORMULA 			(75.0 /(tan((-(double)N /4690.4175)+(1092.356263/4690.4175))))
#define 	YAXIS_THETA 				28.45
#define 	PI							3.14

enum classification{AVOIDANCE,INTEREST, UNKNOWN};


class foundObjects{
public:
			foundObjects(){
				id = -1;
				x1 = 0;
				x2 = 0;
				y1 = 0;
				y2 = 0;
				objectClassification = -1;
				mapped = false;
				next = NULL;
				previous = NULL;
			}
			void showObject(){
				cout<<"id = "<< id<<endl;
				cout<<"x1 = "<< x1<<endl;
				cout<<"x2 = "<< x2<<endl;
				cout<<"y1 = "<< y1<<endl;
				cout<<"y2 = "<< y2<<endl;
				cout<<"objectClassification = "<<objectClassification<<endl;
				cout<<"mapped = "<<mapped<<endl;
			}
			int id;
			int x1;
			int x2;
			short unsigned int y1;
			short unsigned int y2;
			int objectClassification;
			bool mapped;
			foundObjects *next;
			foundObjects *previous;
};

class objectHunter{
	public:

		objectHunter(){
			listHeadObject = NULL;
			listTailObject = NULL;
			setDetectionLine(INIT_DETECTION_LINE);
			setEdgeThreshold(EDGE_THRESHOLD);

		}

		void setEdgeThreshold(int val){edge_thresh =val;}
		int getEdgeThreshold(){return edge_thresh;}
		void setDetectionLine(int val){ detection_line = val;}
		int getDetectionLine(){ return detection_line;}

		void createObjectList(){}
		void printObjectList(){}
		void addObjectToList(){}
		void searchObjectList(){}
		void classifyObjects(){}

		void addToOccupancyMap(){}
		void updateCurrentMap(occupancyMap* input){currentMap = input;}

		int findObjectsOfAvoidance(IplImage *edge,Mat* depthimg, int x, int y, int direction);
		void edgeDetector(IplImage* input, IplImage* dst);
		double depth2mm(uint N);
		double mmPerPixel(uint N);

		foundObjects* listHeadObject;
		foundObjects* listTailObject;
		foundObjects* currentObject;
		foundObjects* workingObject;
		occupancyMap* currentMap;

	private:
		int 	detection_line;
		int 	edge_thresh;

};


#endif /* OBJECTHUNTER_H_ */
