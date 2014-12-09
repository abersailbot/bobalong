/*
	navigator.h

	Controls the high level navigation of the boat by working out what direction the
	boat should be heading and at what angle the sail should be set to.

	This code is released under the terms of the LGPLv3 licence.
 */


  /**********************************************/
  /*   CHANGE DESIRED HEADING RANGE -180 t0 180 */
  /**********************************************/

#include "gps.h"

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

enum SAIL_MODE {
	NORMAL,
	TACK_LEFT,
	TACK_RIGHT
};

class Navigator {
public:
	Navigator();

	/**********************************************************************************
	 * Returns the current sailing mode the boat is in.

	 *********************************************************************************/
	SAIL_MODE sail_mode();

	/**********************************************************************************
	 * Performs the high level navigation of the boat. Works out:
	 *		- The desired heading of the boat
	 *		- Whether the boat should tack
	 *		- Whether the boat has arrived at its current waypoint target
	 *
	 * @param heading 				The current heading of the boat
	 * @param wind_dir 				The current direction of the wind.
	 *
	 *********************************************************************************/
	void navigate(int heading, int wind_dir);

	/**********************************************************************************
	 * Returns the angle at which the sail should be set.
	 *
	 * @param  wind_dir 			The direction of the wind relative to the boat
	 *
	 *********************************************************************************/
	int get_sail_angle(int wind_dir);

	/**********************************************************************************
	 * Returns the angle at which the rudder should be set. This is worked out by
	 * getting the difference between the boat's current heading and the desired
	 * heading, and then applying a PI function for a smooth turn.
	 *
	 * @param 						The current boat heading
	 *
	 *********************************************************************************/
	int get_rudder_angle(int heading);
private:
	/**********************************************************************************
	 * Determines if we should tack or not.
	 *
	 *********************************************************************************/
	bool should_tack();

	int desired_heading;
	int last_boat_heading;
	int relative_wind;
	SAIL_MODE curr_mode;
	GPSPosition tack_pos;
};

extern Navigator Pilot;
#endif
