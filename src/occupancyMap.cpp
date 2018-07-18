/*
 * occupancyMap.cpp
 *
 *  Created on: 03/02/2012
 *      Author: troy
 */

#include "occupancyMap.h"

void occupancyMap::printOccupancyMap(){
	for(int l = 0 ; l < 14; l++){
		for(int m = 0 ; m < 16; m++){
			cout<< elements[m+l*14].cost<< " ";
		}
		cout<<endl;
	}
}
