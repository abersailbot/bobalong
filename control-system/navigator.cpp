/*
	navigator.c

	Controls the high level navigation of the boat by working out what direction the 
	boat should be heading and at what angle the sail should be set to.

	This code is released under the terms of the LGPLv3 licence.
 */

#include "navigator.h"
#include "waypoint_mgr.h"
#include "TinyGPS.h"
#include "utils.h"
#include "gps.h"


#define TACK_AMOUNT 				45
#define MAX_WAYPOINT_DISTANCE		10
#define TACK_DISTANCE				20
#define SAIL_LEFT					45
#define SAIL_RIGHT					135

Navigator Pilot = Navigator();

//////////////////////////////////////////////////////////////////////////
Navigator::Navigator()
	:curr_mode(NORMAL)
{
	
}

//////////////////////////////////////////////////////////////////////////
SAIL_MODE Navigator::sail_mode()
{
	return curr_mode;
}

//////////////////////////////////////////////////////////////////////////
void Navigator::navigate(int heading, int wind_dir)
{
	last_boat_heading = heading;
	relative_wind = get_heading_diff(heading, wind_dir);

	/*
	
		in need of a rewrite!

	*/	

	// arduino throws a fit if we try to declare variables inside switches
	GPSPosition curr_pos = Gps.position();
	GPSPosition wp;

	switch(curr_mode) {
		case NORMAL:
			// determine if we need to tack
			if(should_tack()) {
				curr_mode = TACK_LEFT;
				tack_pos = Gps.position();
			}

			// have we reached our waypoint
			wp = Waypoints.current();
			if(TinyGPS::distance_between(wp.latitude, wp.longitude, curr_pos.latitude, curr_pos.longitude) < MAX_WAYPOINT_DISTANCE) {
				Waypoints.advance();
			}
			break;
		case TACK_LEFT:
			// can we return to normal sailing
			if(!should_tack()) {
				curr_mode = NORMAL;
				break;
			}
			// do we need to switch tack
			if(TinyGPS::distance_between(tack_pos.latitude, tack_pos.longitude, curr_pos.latitude, curr_pos.longitude) > TACK_DISTANCE) {
				tack_pos = curr_pos;
				curr_mode == TACK_RIGHT;
			} else {
				desired_heading += TACK_AMOUNT;
			}
			break;
		case TACK_RIGHT:
			// can we return to normal sailing
			if(!should_tack()) {
				curr_mode = NORMAL;
			}

			// do we need to switch tack
			if(TinyGPS::distance_between(tack_pos.latitude, tack_pos.longitude, curr_pos.latitude, curr_pos.longitude) > TACK_DISTANCE) {
				tack_pos = curr_pos;
				curr_mode == TACK_LEFT;
			} else {
				desired_heading -= TACK_AMOUNT;
			}
			break;
		}
}

//////////////////////////////////////////////////////////////////////////
int Navigator::get_sail_angle(int relative_wind)
{
	if(relative_wind < 180) {
		return SAIL_LEFT;
	} else {
		return SAIL_RIGHT;
	}
}

//////////////////////////////////////////////////////////////////////////
int Navigator::get_rudder_angle(int heading)
{
	
	// PI Controller

	const float pVal = 1.00;
	const float iVal = 0.00;
	float pCorrection = 0.00;
	float iCorrection = 0.00;
	float error = 0.00;
	static float errorSum = 0.00;
	int rudderAngle = 0;
	int normalRudderPos = 90;


	// The desired heading is now relative to zero
	desired_heading -= heading;

	// Finding out if the value should be negative or positive and how big
	if (desired_heading > 180){
		desired_heading -= 360;
	}
	else if (desired_heading < -180){
		desired_heading += 360;
	}

	desired_heading * -1;

	// A positive number means that the boat will turn left.
	// A negative number is right

	//  P
	error = desired_heading;
	pCorrection = pVal * error;


	//  I
	errorSum += error;
	iCorrection = iVal * errorSum;

	//errorSum * 0.95;

	rudderAngle = normalRudderPos + (int)(pCorrection + iCorrection);

	// Making sure the rudder does not go over 45 degrees from normal position (90 deg(?))
	if (rudderAngle > (normalRudderPos + 45)){
		rudderAngle = normalRudderPos + 45;
	}
	else if (rudderAngle < (normalRudderPos - 45)){
		rudderAngle = normalRudderPos - 45;
	}

	// todo
	return rudderAngle;
}

//////////////////////////////////////////////////////////////////////////
bool Navigator::should_tack()
{
	if(relative_wind >= TACK_AMOUNT) {
		return true;
	}
	return false;
}
