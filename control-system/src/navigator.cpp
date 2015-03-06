/*
	navigator.c

	Controls the high level navigation of the boat by working out what direction the
	boat should be heading and at what angle the sail should be set to.

	This code is released under the terms of the LGPLv3 licence.
 */

#include "navigator.h"
#include "waypoint_mgr.h"
#include "sensors/TinyGPS.h"
#include "utils.h"1


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
	desired_heading = 166;
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

	// check waypoints

	GPSPosition curr_pos = Gps.position();
	GPSPosition wp = Waypoints.current();
	float dist = TinyGPS::distance_between(wp.latitude, wp.longitude, curr_pos.latitude, curr_pos.longitude);
	if( dist < 10) {
		Waypoints.advance();
		Serial.println("Hit waypoint, moving onto next");
	} else {
		Serial.print("Distance : ");
		Serial.println(dist);
	}

	desired_heading = TinyGPS::course_to(wp.latitude, wp.longitude, curr_pos.latitude, curr_pos.longitude);

	// set rudder
	int rudder_pos = get_rudder_angle(heading);
	if(rudder_pos < 85) {
		Serial.println("Turn left!");
	} else if(rudder_pos > 95) {
		Serial.println("Turn right!");
	}
	Serial.print("Rudder pos: "); Serial.println(rudder_pos);

	//rudder.write(rudder_pos);

	// set wing sail
	//int sail_pos = get_sail_angle(wind_dir);
	//sail.write(sail_pos);
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


	float pCorrection;
	float iCorrection;
	float error;
	static float errorSum;
	int rudderAngle;


	// The desired heading is now relative to zero
	float desired_headingCP = desired_heading - heading;

	// Finding out if the value should be negative or positive and how big
	if (desired_headingCP > 180){
		desired_headingCP -= 360;
	}
	else if (desired_headingCP < -180){
		desired_headingCP += 360;
	}


	// A positive number means that the boat will turn left.
	// A negative number is right

	//  P
	error = desired_headingCP * (-1);
	pCorrection = P_VAL * error;


	//  I
	errorSum += error;
	iCorrection = I_VAL * errorSum;

	errorSum *= 0.95;

	rudderAngle = NORMAL_RUDDER_POS + round(pCorrection + iCorrection);

	// Making sure the rudder does not go over 45 degrees from normal position (90 deg(?))
	if (rudderAngle > (NORMAL_RUDDER_POS + 45)){
		rudderAngle = NORMAL_RUDDER_POS + 45;
	}
	else if (rudderAngle < (NORMAL_RUDDER_POS - 45)){
		rudderAngle = NORMAL_RUDDER_POS - 45;
	}

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
