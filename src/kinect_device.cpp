/*
 * kinect_device.cpp
 *
 *  Created on: 01/02/2012
 *      Author: troy
 */
#include "kinect_device.h"

void Mutex::lock() {
	pthread_mutex_lock( &m_mutex );
}
void Mutex::unlock() {
	pthread_mutex_unlock( &m_mutex );
}

// Do not call directly even in child
void MyFreenectDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
	if(PRINT_COMMENTS)
		std::cout << "RGB callback" << std::endl;
	m_rgb_mutex.lock();
	uint8_t* rgb = static_cast<uint8_t*>(_rgb);
	rgbMat.data = rgb;
	m_new_rgb_frame = true;
	m_rgb_mutex.unlock();
}
// Do not call directly even in child
void MyFreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
	if(PRINT_COMMENTS)
		std::cout << "Depth callback" << std::endl;
	m_depth_mutex.lock();
	uint16_t* depth = static_cast<uint16_t*>(_depth);
	depthMat.data = (uchar*) depth;
	m_new_depth_frame = true;
	m_depth_mutex.unlock();
}

bool MyFreenectDevice::getVideo(Mat& output) {
	m_rgb_mutex.lock();
	if(m_new_rgb_frame) {
		cv::cvtColor(rgbMat, output, CV_RGB2BGR);
		m_new_rgb_frame = false;
		m_rgb_mutex.unlock();
		return true;
	} else {
		m_rgb_mutex.unlock();
		return false;
	}
}

bool MyFreenectDevice::getDepth(Mat& output) {

	uint previous = 0;
	m_depth_mutex.lock();

	if(m_new_depth_frame) {
		depthMat.copyTo(output);
		m_new_depth_frame = false;
		m_depth_mutex.unlock();


		for (int i=0; i < output.rows ; i++) {
			short unsigned int* ptr = (short unsigned int* )(output.data + i*output.cols*2);
			for(int j =0 ; j < output.cols; j++){
				ptr[j] = depthFilter(ptr[j],distance_threshold);
				ptr[j] = shadowFilter(output, i, j, ptr[j],previous);
				previous = ptr[j];
				}
				previous = 0;
			}

		return true;
	} else {
		m_depth_mutex.unlock();
		return false;
	}

}

uint MyFreenectDevice::shadowFilter(Mat &inMat,  int i, int j, uint current,uint previous){
	int pixelFactor = SHADOW_FILTER_pixelFactor;
	double shadowFactor = SHADOW_FILTER_shadowFactor;
	if(current == 0){
		if(previous == 0){
			current = 0;

		}else{
			uint temp1,temp2,temp3,temp4;
			double val;
			int y_axis, x_axis;
			short unsigned int* ptr;
			double divide_factor = 4;


			x_axis = j;
			y_axis = i + pixelFactor;
			if(y_axis > inMat.rows){
				//y_axis = inMat.rows - 1;
				divide_factor--;
			}else{
				ptr = (short unsigned int* )(inMat.data + (y_axis)*inMat.cols*2);
				temp1 = ptr[x_axis];
				if(temp1 > distance_threshold)
					temp1 = 0;
			}


			y_axis = i - pixelFactor*2;
			if(y_axis < 0){
				//y_axis = 0;
				divide_factor--;
			}else{
				ptr = (short unsigned int* )(inMat.data + (y_axis)*inMat.cols*2);
				temp2 = ptr[x_axis];
				if(temp2 > distance_threshold)
					temp2 = 0;
			}

			y_axis = i;
			x_axis = j + pixelFactor;
			if(x_axis > X_RES_LIMIT){
			//	x_axis = X_RES_LIMIT - 1;
				divide_factor--;
			}else{
				ptr = (short unsigned int* )(inMat.data + (y_axis)*inMat.cols*2);
				temp3 = ptr[x_axis];
				if(temp3 > distance_threshold)
					temp3 = 0;
			}

			x_axis = j - pixelFactor*2;
			if(x_axis < 0){
				//x_axis = 0;
				divide_factor--;
			}else{
				ptr = (short unsigned int* )(inMat.data + (y_axis)*inMat.cols*2);
				temp4 = ptr[x_axis];
				if(temp4 > distance_threshold)
					temp4 = 0;
			}

			if(divide_factor != 0.0)
				val = ((double)temp1+(double)temp2+(double)temp3+(double)temp4)/divide_factor;


			if(val > (((double)previous)*shadowFactor)){
				current = previous;
			}

		}
	}
	return current;
}

uint MyFreenectDevice::depthFilter(uint input,uint threshold){

	if(input > threshold){
		return 0;
	}
	return input;
}

double MyFreenectDevice::depth2mm(uint N){
	double mm = depth2mm_FORMULA;
	if(PRINT_COMMENTS)
			cout<<"distance (mm) = "<<mm<<endl;
	return mm;
}

double MyFreenectDevice::mm2Depth(double mm){
	double N = mm2Depth_FORMULA;
	if(N < 0)
		N = 1;
	if(PRINT_COMMENTS){
		cout<<"Depth (N) = "<<N<<endl;
		cout<<"distance (mm) = "<<mm<<endl;
	}
	return N;
}

// Currently only handles looking in the positive X and Postive Y direction.
// Needs to handle the negative X and Negative Y directions.

double MyFreenectDevice::updateDistanceThreshold(int x, int y, int dir){
	// 1 * (x || y) = 25cm
	int output;
	if(dir == POSTIVE_X){
		if(x>15)
			x = 15;

		output = (MAX_X/COORDINATE_SIZE -1) - (x);

	}else if(dir == NEGATIVE_X){
		if(x>15)
			x = 15;

		output = x;
	}else if(dir == POSTIVE_Y){
		if(y > 13)
			y = 13;
		output = (MAX_Y/COORDINATE_SIZE - 1) - (y);

	}else if(dir == NEGATIVE_Y){
		if(y > 13)
			y = 13;
		output = y;
	}
	if(PRINT_COMMENTS)
		cout << "Co-ordinates to the edge of the arena = "<< output <<endl;
	return mm2Depth(((double)output * COORDINATE_SIZE * MM_CONVERSION + EXTENSION_FACTOR));
}




