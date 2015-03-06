/*
	wind_sensor.h

	Provides functions for interacting with Bobalong's Rowind wind sensor. The
	wind sensor uses a software serial to communicate with the device.

	This code is released under the terms of the LGPLv3 licence.
 */

#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

class Rowind {
public:
	float get_speed();

	int get_direction();

	/**********************************************************************************
	 * Listens to the Rowind's serial line and tries to poll it for the wind speed
	 * and direction.
	 *
	*********************************************************************************/
	void poll_data();
private:
	/**********************************************************************************
	* Returns a Rowind's NMEA string.
	*
	* @return 						A char array of 30 characters, in the format:
	*                 				$IIMWV,315.0,R,000.00,N,A
	*
	*********************************************************************************/
	char* get_nmea();

	float speed;
	int direction;
};

extern Rowind WindSensor;

#endif
