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
void WaypointMgr::add_waypoints(GPSPosition* waypoint, unsigned int num)
{
	if(num == 0) {
		debug_print("WARNING: No waypoints in the list to add!", DEBUG_LEVEL_CRITICAL);
		is_finished = true;
		return;
	}
	else if(num >= WPMGR_MAX_WAYPOINTS) {
		debug_print("WARNING: More waypoints than space, some will be lost!", DEBUG_LEVEL_CRITICAL);
		num = WPMGR_MAX_WAYPOINTS;
	}

	is_finished = false;
	waypoint_end = num - 1;

	for(unsigned int i = 0; i < num; i++) {
		waypoints[i] = waypoint[i];
	}

	debug_print("Waypoints added!", DEBUG_LEVEL_IMPORTANT);
}
