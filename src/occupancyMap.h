/*
 * occupancyMap.h
 *
 *  Created on: 03/02/2012
 *      Author: troy
 */

#ifndef OCCUPANCYMAP_H_
#define OCCUPANCYMAP_H_

#include <iostream>

using namespace std;

struct mapBlocks{
	char x;
	char y;
	unsigned short cost;
	bool dirty;
};

class occupancyMap{

	public:
		occupancyMap(){
			for(int l = 0 ; l < 14; l++){
				for(int m = 0 ; m < 16; m++){
					elements[m+l*14].x = l;
					elements[m+l*14].y = m;
				}
			}
			for(int i=0; i < 14; i++){
				for(int j = 0; j < 16; j++)
					elements[j+14*i].cost = 3;
			}

			for(int k = 0 ; k < (16*14); k++)
				elements[k].dirty = false;

		}
		mapBlocks elements[16*14 + 1];

		void printOccupancyMap();
};


#endif /* OCCUPANCYMAP_H_ */
