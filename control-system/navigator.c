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
#define SAIL_LEFT					45
#define SAIL_RIGHT					135

//////////////////////////////////////////////////////////////////////////
Navigator::Navigator()
{
	curr_mode = SAIL_MODE::NORMAL;
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
		Logic isn't complete
	*/

	switch(curr_mode) {
		case SAIL_MODE::NORMAL:
			// determine if we need to tack
			if(should_tack()) {
				curr_mode = SAIL_MODE::TACK_LEFT;
				tack_pos = Gps.position();
			}

			// have we reached our waypoint
			GPSPosition curr_pos = Gps.position();
			GPSPosition wp = Waypoints.current();
			if(TinyGPS::distance_between(wp.latitude, wp.longitude, curr_pos.latitude, curr_pos.longitude) < MAX_WAYPOINT_DISTANCE) {
				Waypoints.advance();
			}

			break;
		case SAIL_MODE::TACK_LEFT:
			// can we return to normal sailing
			if(!should_tack()) {
				curr_mode = SAIL_MODE::NORMAL;
				break;
			}
			// do we need to switch tack
			GPSPosition curr_pos = Gps.position();
			if(TinyGPS::distance_between(tack_pos.latitude, tack_pos.longitude, curr_pos.latitude, curr_pos.longitude) > TACK_DISTANCE) {
				tack_pos = curr_pos;
				curr_mode == SAIL_MODE::TACK_RIGHT;
			} else {
				desired_heading += TACK_AMOUNT;
			}
			break;
		case SAIL_MODE::TACK_RIGHT:
			// can we return to normal sailing
			if(!should_tack()) {
				curr_mode = SAIL_MODE::NORMAL;
			}

			// do we need to switch tack
			GPSPosition curr_pos = Gps.position();
			if(TinyGPS::distance_between(tack_pos.latitude, tack_pos.longitude, curr_pos.latitude, curr_pos.longitude) > TACK_DISTANCE) {
				tack_pos = curr_pos;
				curr_mode == SAIL_MODE::TACK_LEFT;
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

}

//////////////////////////////////////////////////////////////////////////
bool Navigator::should_tack(int heading, int wind_dir)
{
	if(relative_wind >= TACK_AMOUNT) {
		return true;
	}
	return false;
}