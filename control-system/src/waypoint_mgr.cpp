/*
	waypoint_mgr.cpp

	Maintains a list of waypoints and functions for manipulating them

	This code is released under the terms of the LGPLv3 licence.
 */

#include "waypoint_mgr.h"
#include "utils.h"

WaypointMgr Waypoints = WaypointMgr();

//////////////////////////////////////////////////////////////////////////
WaypointMgr::WaypointMgr()
{
	curr_waypoint = 0;
	waypoint_end = 0;
	bool finished = true;
}

//////////////////////////////////////////////////////////////////////////
GPSPosition WaypointMgr::current()
{
	return waypoints[curr_waypoint];
}

//////////////////////////////////////////////////////////////////////////
int WaypointMgr::current_index()
{
	return curr_waypoint;
}

//////////////////////////////////////////////////////////////////////////
unsigned int WaypointMgr::count()
{
	return waypoint_end;
}

//////////////////////////////////////////////////////////////////////////
void WaypointMgr::advance()
{
	if(curr_waypoint < waypoint_end) {
		curr_waypoint++;
	}
	else if (curr_waypoint == waypoint_end) {
		is_finished = true;
    }
}

//////////////////////////////////////////////////////////////////////////
bool WaypointMgr::finished()
{
	return is_finished;
}

//////////////////////////////////////////////////////////////////////////
void WaypointMgr::add_waypoint(float latitude, float longitude)
{
	if(waypoint_end >= WPMGR_MAX_WAYPOINTS) {
		debug_print("WARNING: More waypoints than space, some will be lost!", DEBUG_LEVEL_CRITICAL);
		delay(1000);
		return;
	}

	is_finished = false;
	waypoints[waypoint_end].latitude = latitude;
	waypoints[waypoint_end].longitude = longitude;

	waypoint_end++;

	debug_print("Waypoint added!", DEBUG_LEVEL_IMPORTANT);
}
