/*
	wind_sensor.h

	Provides functions for interacting with Bobalong's Rowind wind sensor. The
	wind sensor uses a software serial to communicate with the device.

	This code is released under the terms of the LGPLv3 licence.
 */

#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

class WindSensor {
public:
	/**********************************************************************************
	 * Initialises the wind sensor. Creates and opens a software serial to the rowind.
	 * 
	 *********************************************************************************/
	void initialise(int rx, int tx);

	/**********************************************************************************
	 * Returns the last polled wind direction. Not relative to the boat's heading
	 * 
	 *********************************************************************************/
	int wind_direction();

	/**********************************************************************************
	 * Returns the last polled wind speed.
	 * 
	 *********************************************************************************/
	double wind_speed();

	/**********************************************************************************
	 * Poll's the rowind's serial line and converts the nmea strings into usable data
	 * 
	 *********************************************************************************/
	bool poll_data();
private:
	/**********************************************************************************
	 * Returns a Rowind's NMEA string.
	 * 
	 * @return 						A char array of 30 characters, in the format: $IIMWV,315.0,R,000.00,N,A
	 * 
	 *********************************************************************************/
	char* get_nmea()

	double speed;
	int direction;
	SoftwareSerial wind_serial(0, 0);
};

WindSensor Rowind;

#endif
