/*
 * stateMachine.h
 *
 *  Created on: 02/02/2012
 *      Author: troy
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "kinect_device.h"
#include "objectHunter.h"
#include "occupancyMap.h"

#define     PRINT_TEST			true

#define 	PRINT_COMMENTS  		false
#define 	CONFIRM_CURRENT_FRAMES 	false
#define		ERROR					-1
#define		NEGATIVE				-1
#define		POSITIVE				1
#define		MAX_KINECT_TILT_ANGLE	30

bool state_mach(char* state,MyFreenectDevice& device,objectHunter *objects );
int charToint(char in);

#endif /* STATEMACHINE_H_ */
