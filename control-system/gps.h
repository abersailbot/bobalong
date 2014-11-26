/*
	gps.h 

	Contains functions for dealing with the boat's GPS. To use the 
	GPS it must first be initialised. The GPS uses TinyGPS to parse the GPS NMEA strings

	This code is released under the terms of the LGPLv3 licence.
 */

#ifndef GPS_H
#define GPS_H

#include "TinyGPS.h"
#include <SoftwareSerial.h>
#include "Arduino.h"

struct GPSPosition {
	long latitude;
	long longitude;
};

class GPSDateTime {
public:
	int hours();
	int minutes();
	int seconds();
	int day();
	int month();
	int year();
	unsigned long time;
	unsigned long date;
};

class GPS {
public:
	/**********************************************************************************
	 * Initialises a GPS that is connected to a specific rx and tx pin
	 * 
	 *********************************************************************************/
	void initialise(int rx, int tx);

	/**********************************************************************************
	 * Returns the latest GPS position
	 * 
	 *********************************************************************************/
	GPSPosition position();

	/**********************************************************************************
	 * Returns the latest GPS time and date
	 * 
	 *********************************************************************************/
	GPSDateTime data_time();

	/**********************************************************************************
	 * Returns true if the GPS has a fix
	 * 
	 *********************************************************************************/
	bool has_fix();

	/**********************************************************************************
	 * Polls the GPS for data and feeds that data to TinyGPS which converts
	 * into something usable
	 * 
	 *********************************************************************************/
	void poll_data();
private:
	SoftwareSerial gps_serial(0, 0);
	TinyGPS tiny_gps;
};

GPS Gps;

#endif
