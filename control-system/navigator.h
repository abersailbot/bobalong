/*
	navigator.h

	Controls the high level navigation of the boat by working out what direction the 
	boat should be heading and at what angle the sail should be set to.

	This code is released under the terms of the LGPLv3 licence.
 */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

enum SAIL_MODE {
	NORMAL,
	TACK_LEFT,
	TACK_RIGHT
};

class Navigator {
public:
	/**
	 * Works out the desired heading of the boat, if the boat should tack or not. This is essentially the main sailing algorithm of the boat
	 *
	 */
	void navigate(int heading, int wind_dir);

	/**
	 * Returns the angle at which the sail should be set.
	 * 
	 * @param  wind_dir 			The direction of the wind.
	 * 
	 */
	int get_sail_angle(int wind_dir);

	/**
	 * Returns the angle at which the rudder should be set.
	 *
	 * @param boat_heading 		The current heading of the boat
	 * 
	 */
	int get_rudder_angle(int compass_heading);

	/**
	 * Returns the current sailing mode the boat is in
	 * @return [description]
	 */
	SAIL_MODE sail_mode();
private:
	int desired_heading;
	SAIL_MODE curr_mode;
	GPSPostiion tack_pos;
};

Navigator Pilot;
#endif