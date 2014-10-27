/*
	waypoint_mgr.h 

	Contains a list of all the waypoints the boat will attempt to travel to.

	This code is released under the terms of the LGPLv3 licence.
 */

#pragma once

#include "gps.h"

#define NUM_WAYPOINTS		15

class WaypointManager {
public:
	/**********************************************************************************
	 * Returns the number of waypoints left.
	 * 
	 **********************************************************************************/
	int waypoints_left();

	/**********************************************************************************
	 * Returns the current waypoint.
	 * 
	 *********************************************************************************/
	GPSPosition get_waypoint();

	/**********************************************************************************
	 * Advances to the next waypoint in the list
	 * 
	 *********************************************************************************/
	void advance_waypoint();

	/**********************************************************************************
	 * Pushes a waypoint into the manager's queue.
	 * 
	 *********************************************************************************/
	void push_waypoint(GPSPosition gps_pos);
private:
	int curr_index;
	GPSPosition queue[NUM_WAYPOINTS];
};

WaypointManager WaypointMgr;