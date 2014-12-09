/*
	utils.c

	Provides some general utility functions

	This code is released under the terms of the LGPLv3 licence.
 */

#include "utils.h"
#include "configure.h"

//////////////////////////////////////////////////////////////////////////
void debug_print(char* msg, int debug_level)
{
	if(debug_level <= DEBUG_LEVEL) {
		Serial.print("[Debug ");
    	Serial.print(debug_level);
    	Serial.print("] ");
		Serial.println(msg);
	}
}

//////////////////////////////////////////////////////////////////////////
int get_heading_diff(int heading0, int heading1)
{
	int heading_diff = heading0 - heading1;

	if(heading_diff < -180) {
		return 360 + heading_diff;
	}
	
	if(heading_diff > 180) {
		return 0 - (360 - heading_diff);
	}

	return heading_diff;
}

//////////////////////////////////////////////////////////////////////////
int clamp_angle(int angle)
{
	if(angle > 359) {
		return clamp_angle(angle - 359);
	} else if(angle < 0) {
		return clamp_angle(359 - angle);
	} else {
		return angle;
	}
}