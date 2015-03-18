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
#include "../configure.h"

struct GPSPosition {
	float latitude;
	float longitude;
};

class GPSDateTime {
public:
	int year;
	byte month;
	byte day;
	byte hour;
	byte minute;
	byte second;
};

class GPS {
public:

	/**********************************************************************************
	 * Returns the latest GPS position
	 *
	 *********************************************************************************/
	GPSPosition position();

	/**********************************************************************************
	 * Returns the latest GPS time and date
	 *
	 *********************************************************************************/
	GPSDateTime date_time();

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

    void print_nmea();
private:
	TinyGPS tiny_gps;
};

extern GPS Gps;

#endif
