/*
 * objectHunter.cpp
 *
 *  Created on: 02/02/2012
 *      Author: troy
 */

#include "objectHunter.h"
void objectHunter::edgeDetector(IplImage* input, IplImage* dst){

	IplImage *gray = cvCreateImage(cvSize(X_RES,Y_RES), IPL_DEPTH_8U, 1);
	IplImage *edge = cvCreateImage(cvSize(X_RES,Y_RES), IPL_DEPTH_8U, 1);
	IplImage *white = cvCreateImage(cvSize(X_RES,Y_RES), IPL_DEPTH_8U, 1);

	cvCopy(input,gray);
	cvSmooth( gray, edge, CV_BLUR, 3, 3, 0, 0 );
	cvNot( gray, edge );

	cvCanny(gray, edge, (float)edge_thresh, (float)edge_thresh*3, 3);

	cvZero( dst );
	cvSet( white, WHITE );
	// copy edge points
	cvCopy( white, dst, edge );

}



// Needs to handle more situations.
// Currently only handles objects that fall into a single detection line.
// However over the 50cm - 4 meter range the detection line needs to be variable.
/*
int objectHunter::findObjectsOfAvoidance(IplImage *edge,Mat* depthimg, int x = 0, int y = 0, int direction = 0){

	bool foundLeft = false;
	bool foundRight = false;
	bool firstObject = true;
	bool maxPixel = false;
	int leftX,rightX,leftY,rightY;
	int objects = 0;
	int id= 0;
	int i = detection_line;
	workingObject = new foundObjects;
	short unsigned int* depthPtr;
	unsigned char* ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);

	for(int j =0 ; j < X_RES_LIMIT; j+=edge->nChannels){

			if(j == X_RES_LIMIT - 1){
				maxPixel = true;
				if(PRINT_COMMENTS)
					cout << "maxpixel"<<endl;
			}

			if(ptr[j] == 255)
			{
				int temp = j;
				if(PRINT_COMMENTS)
					cout<<"found at: "<< " X = " << j << " Y = " << i <<endl;

				j -= DETECTION_PIXEL_OFFSET;

				for( i = (detection_line- 1); i > 1; i--){
					ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);
					if(ptr[j] == 255){
						if(PRINT_COMMENTS)
							cout<<"found left : " << " X = " << j << " Y = " << i <<endl;
						leftX = temp;
						leftY = i;
						i = -1;
						foundLeft = true;

					}
					ptr[j] = DRAW_GREY;

				}

				j += (DETECTION_PIXEL_OFFSET * 2);
				if(j < X_RES_LIMIT){

					for( i = (detection_line - 1); i > 1; i--){
					ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);

					if(ptr[j] == 255){
						if(PRINT_COMMENTS)
							cout<<"found right : " << " X = " << j << " Y = " << i <<endl;
						rightX = temp;
						rightY = i;
						i = -1;
						foundRight = true;
					}

						ptr[j] = DRAW_GREY;

					}

				}

				i = detection_line;
				ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);
			}
			if(firstObject && foundLeft && !foundRight){
				firstObject = false;
				foundLeft = false;
				maxPixel = false;
				id++;
				depthPtr = (short unsigned int*)(depthimg->data + leftX*depthimg->cols*2);
				cout <<"object " << depth2mm(depthPtr[leftY]) <<" mm away."<< endl;

				int pixels = 315 - leftX;
					if(pixels < 0)
						pixels *= -1;

				cout <<"width = "<< mmPerPixel(depthPtr[leftY]) * leftX << endl;
				cout <<"mmPerPixel = "<< mmPerPixel(depthPtr[leftY])<< endl;


				workingObject->id = id;
				workingObject->x1 = 0;
				workingObject->x2 = leftX;
				workingObject->y1 = depthPtr[leftY];
				workingObject->y2 = depthPtr[leftY] ;
				workingObject->mapped = false;
				workingObject->showObject();
				objects++;
				if(TESTING_PRINT || PRINT_COMMENTS)
					cout<< "object off Left of screen"<<endl;
			}else if(foundRight && foundLeft){
				firstObject = false;
				foundLeft = false;
				foundRight = false;
				maxPixel = false;
				objects++;
				if(TESTING_PRINT || PRINT_COMMENTS)
					cout<< "object center of screen"<<endl;
			}else if(maxPixel && foundRight && !foundLeft){
				firstObject = false;
				foundLeft = false;
				foundRight = false;
				maxPixel = false;
				objects++;
				if(TESTING_PRINT || PRINT_COMMENTS)
					cout<< "object off Right of screen"<<endl;
			}
			ptr[j] = DRAW_GREY;

	}
	if(TESTING_PRINT || PRINT_COMMENTS)
		cout<< "Number of objects found = " << objects << endl;
	if(objects == 0)
		delete workingObject;
	return objects;
}
*/
//enum direction{POSTIVE_X,NEGATIVE_X,POSTIVE_Y,NEGATIVE_Y};
void off_left_coordinates(int x, int y, int dir, double distance_to , double distance_across,char *output){

	double to_coordinate = (distance_to + 250.0/2 )/ 250.0;
	double across_coordinate = (distance_across + 250.0/2)/250.0;
	int x1,x2,y1,y2;


	bool exit(false);
	switch(dir){
		case 0:
			// +'ve X
			x1 = x + (int)to_coordinate;
			if(x1 > 15)
				x1 = 15;
			x2 = x1;

			y1 = y - (int)across_coordinate;
			if(y1 < 0)
				y1 = 0;
			y2 = y1 - 1;
			if(y2 < 0)
				y2 = 0;
			cout<<"y = "<<y<<" y1 = "<< y1<< " y2 = "<< y2<<" acc = "<<across_coordinate <<endl;
			break;
		case 1:
			// -'ve X
			x1 = x - to_coordinate;
			if(x1 < 0)
				x1 = 0;
			x2 = x1;

			y1 = y + across_coordinate;
			if(y1 > 13)
				y1 = 13;
			y2 = y1 + 1;
			if(y2 > 13)
				y2 = 13;
			break;
		case 2:
			// +'ve Y
			y1 = y + to_coordinate;
			if(y1 > 13)
				y1 = 13;
			y2 = y1;

			x1 = x + across_coordinate;
			if(x1 > 15)
				x1 = 15;
			x2 = x1 + 1;
			if(x2 > 15)
				x2 = 15;
			break;
		case 3:
			// -'ve Y
			y1 = y - to_coordinate;
			if(y1 < 0)
				y1 = 0;
			y2 = y1 ;

			x1 = x - across_coordinate;
			if(x1 < 0)
				x1 = 0;
			x2 = x1 - 1;
			if(x2 < 0)
				x2 = 0;

			break;
		default:
			exit = true;
			break;
	}

	if(exit){

	}else{
		output[0] = x1;
		output[1] = x2;
		output[2] = y1;
		output[3] = y2;
	}

}

void off_right_coordinates(int x, int y, int dir, double distance_to , double distance_across,char *output){

	double to_coordinate = (distance_to + 250.0/2 )/ 250.0;
	double across_coordinate = (distance_across + 250.0/2)/250.0;
	int x1,x2,y1,y2;


	bool exit(false);
	switch(dir){
		case 0:
			// +'ve X
			x1 = x + (int)to_coordinate;
			if(x1 > 15)
				x1 = 15;
			x2 = x1;

			y1 = y + (int)across_coordinate;
			if(y1 < 0)
				y1 = 0;
			y2 = y1 + 1;
			if(y2 < 0)
				y2 = 0;
			cout<<"y = "<<y<<" y1 = "<< y1<< " y2 = "<< y2<<" acc = "<<across_coordinate <<endl;
			break;
		case 1:
			// -'ve X
			x1 = x - to_coordinate;
			if(x1 < 0)
				x1 = 0;
			x2 = x1;

			y1 = y - across_coordinate;
			if(y1 > 13)
				y1 = 13;
			y2 = y1 - 1;
			if(y2 > 13)
				y2 = 13;
			break;
		case 2:
			// +'ve Y
			y1 = y + to_coordinate;
			if(y1 > 13)
				y1 = 13;
			y2 = y1;

			x1 = x - across_coordinate;
			if(x1 > 15)
				x1 = 15;
			x2 = x1 - 1;
			if(x2 > 15)
				x2 = 15;
			break;
		case 3:
			// -'ve Y
			y1 = y - to_coordinate;
			if(y1 < 0)
				y1 = 0;
			y2 = y1 ;

			x1 = x + across_coordinate;
			if(x1 < 0)
				x1 = 0;
			x2 = x1 + 1;
			if(x2 < 0)
				x2 = 0;

			break;
		default:
			exit = true;
			break;
	}

	if(exit){

	}else{
		output[0] = x1;
		output[1] = x2;
		output[2] = y1;
		output[3] = y2;
	}

}

int objectHunter::findObjectsOfAvoidance(IplImage *edge,Mat* depthimg, int x = 0, int y = 0, int direction = 0){

	bool foundLeft = false;
	bool foundRight = false;
	bool firstObject = true;
	bool maxPixel = false;
	int leftX,rightX,leftY,rightY;
	int objects = 0;
	int id= 0;
	int i = detection_line;
	workingObject = new foundObjects;
	short unsigned int* depthPtr;
	unsigned char* ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);
	int center_of_object;
	double pixels_from_center;
	double distance_across;
	double distance_to;
	char output[4];

	for(int j =0 ; j < X_RES_LIMIT; j+=edge->nChannels){

			if(j == X_RES_LIMIT - 1){
				maxPixel = true;
				if(PRINT_COMMENTS)
					cout << "maxpixel"<<endl;
			}

			if(ptr[j] == 255)
			{
				int temp = j;
				if(PRINT_COMMENTS)
					cout<<"found at: "<< " X = " << j << " Y = " << i <<endl;

				j -= DETECTION_PIXEL_OFFSET;

				for( i = (detection_line- 1); i > 1; i--){
					ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);
					if(ptr[j] == 255){
						if(PRINT_COMMENTS)
							cout<<"found left : " << " X = " << j << " Y = " << i <<endl;
						leftX = temp;
						leftY = i;
						i = -1;
						foundLeft = true;

					}
					ptr[j] = DRAW_GREY;

				}

				j += (DETECTION_PIXEL_OFFSET * 2);
				if(j < X_RES_LIMIT){

					for( i = (detection_line - 1); i > 1; i--){
					ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);

					if(ptr[j] == 255){
						if(PRINT_COMMENTS)
							cout<<"found right : " << " X = " << j << " Y = " << i <<endl;
						rightX = temp;
						rightY = i;
						i = -1;
						foundRight = true;
					}

						ptr[j] = DRAW_GREY;

					}

				}

				i = detection_line;
				ptr = (unsigned char* )(edge->imageData + i*edge->widthStep);
			}
			if(firstObject && foundLeft && !foundRight){
				firstObject = false;
				foundLeft = false;
				maxPixel = false;
				//***************************//
				// Find object co-ordinates
				id++;
				depthPtr = (short unsigned int*)(depthimg->data + detection_line*depthimg->cols*2);
				center_of_object = leftX/2;
				cout <<"object " << depth2mm(depthPtr[center_of_object ]) <<" mm away."<< endl;
				pixels_from_center = 315.0 - (double)leftX;


				cout <<"Distance from center = "<< mmPerPixel(depthPtr[leftY]) * (double)(315 - leftX) <<" mm"<< endl;
				cout <<"mmPerPixel = "<< mmPerPixel(depthPtr[leftY])<< endl;

				distance_across = mmPerPixel(depthPtr[center_of_object])*pixels_from_center;
				distance_to = depth2mm(depthPtr[center_of_object]);
				output[4];

				off_left_coordinates(1, 2, 0, distance_to , distance_across,output);

				workingObject->id = id;
				workingObject->x1 = output[0];
				workingObject->x2 = output[1];
				workingObject->y1 = output[2];
				workingObject->y2 = output[3];
				workingObject->mapped = false;
				workingObject->showObject();
				objects++;
				if(TESTING_PRINT || PRINT_COMMENTS)
					cout<< "object off Left of screen"<<endl;
			}else if(foundRight && foundLeft){
				firstObject = false;
				foundLeft = false;
				foundRight = false;
				maxPixel = false;

				objects++;
				if(TESTING_PRINT || PRINT_COMMENTS)
					cout<< "object center of screen"<<endl;
			}else if(maxPixel && foundRight && !foundLeft){
				firstObject = false;
				foundLeft = false;
				foundRight = false;
				maxPixel = false;
				//***************************//
				// Find object co-ordinates
				id++;
				depthPtr = (short unsigned int*)(depthimg->data + detection_line*depthimg->cols*2);
				center_of_object = (rightX+631)/2;
				cout <<"object " << depth2mm(depthPtr[center_of_object]) <<" mm away."<<depthPtr[center_of_object]<< endl;
				pixels_from_center = (double)rightX - 315;


				cout <<"Distance from center = "<< mmPerPixel(depthPtr[rightY]) * (double)(rightX - 315) <<" mm"<< endl;
				cout <<"mmPerPixel = "<< mmPerPixel(depthPtr[rightY])<< endl;

				distance_across = mmPerPixel(depthPtr[center_of_object])*pixels_from_center;
				distance_to = depth2mm(depthPtr[center_of_object]);
				output[4];

				off_right_coordinates(1, 2, 0, distance_to , distance_across,output);

				workingObject->id = id;
				workingObject->x1 = output[0];
				workingObject->x2 = output[1];
				workingObject->y1 = output[2];
				workingObject->y2 = output[3];
				workingObject->mapped = false;
				workingObject->showObject();
				objects++;

				if(TESTING_PRINT || PRINT_COMMENTS)
					cout<< "object off Right of screen"<<endl;
			}
			ptr[j] = DRAW_GREY;

	}
	if(TESTING_PRINT || PRINT_COMMENTS)
		cout<< "Number of objects found = " << objects << endl;
	if(objects == 0)
		delete workingObject;
	return objects;
}
double objectHunter::depth2mm(uint N){
	double output = depth2mm_FORMULA;
	return output;
}

double objectHunter::mmPerPixel(uint N){
	uint pixelRange = (X_RES_LIMIT-1)/2;
	double length = depth2mm(N);
	double theta = (YAXIS_THETA * PI)/180.0;
	double fullWidth = tan(theta) * length;
	double mmPerPixel = fullWidth / (double)pixelRange;
	return mmPerPixel;

}
