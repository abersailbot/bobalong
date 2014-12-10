/*
	waypoint_mgr.h 

	Maintains a list of waypoints and functions for manipulating them

	This code is released under the terms of the LGPLv3 licence.
 */


#include "gps.h"

#ifndef WAYPOINT_MGR_H
#define WAYPOINT_MGR_H

#define WPMGR_MAX_WAYPOINTS		10

class WaypointMgr {
public:
	WaypointMgr();

	/**********************************************************************************
	 * Returns the current waypoint we are on.
	 * 
	 *********************************************************************************/
	GPSPosition current();

	/**********************************************************************************
	 * Returns the index of the waypoint we are on.
	 * 
	 *********************************************************************************/
	int current_index();

	/**********************************************************************************
	 * Returns the number of waypoints stored.
	 * 
	 *********************************************************************************/
	unsigned int count();

	/**********************************************************************************
	 * Advances to the next waypoint, if we are already on the last
	 * waypoint then we stay here and when finished() is called, true
	 * will be returned
	 * 
	 *********************************************************************************/
	void advance();

	/**********************************************************************************
	 * Returns true when advance is called on the last waypoint.
	 * 
	 *********************************************************************************/
	bool finished();

	/**********************************************************************************
	 * Adds a new set of waypoints to the waypoint manager.
	 * NOTE: Writes over the existing waypoints.
	 * 
	 * @param waypoint 			Array of new waypoints
	 * @param num      			Number of waypoints
	 *********************************************************************************/	
	void add_waypoints(GPSPosition* waypoint, unsigned int num);
private:
	GPSPosition waypoints[WPMGR_MAX_WAYPOINTS];  // All the waypoints
	unsigned int curr_waypoint; // The current waypoint we are on
	unsigned int waypoint_end; // The index of the last waypoint in the array
	bool is_finished; // Whether we have finished all the waypoints
};

extern WaypointMgr Waypoints;

#endif
