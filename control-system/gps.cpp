/*
	gps.c

	Contains functions for dealing with the boat's GPS. To use the 
	GPS it must first be initialised. The GPS uses TinyGPS to parse the GPS NMEA strings

	This code is released under the terms of the LGPLv3 licence.
 */

#include "gps.h"

//////////////////////////////////////////////////////////////////////////
int GPSDateTime::hours()
{
	return time / 1000000;
}

//////////////////////////////////////////////////////////////////////////
int GPSDateTime::minutes()
{
	return (time / 10000) % 100 ;
}

//////////////////////////////////////////////////////////////////////////
int GPSDateTime::seconds()
{
	return (time / 100) % 100;
}

//////////////////////////////////////////////////////////////////////////
int GPSDateTime::day()
{
	return date / 10000;
}

//////////////////////////////////////////////////////////////////////////
int GPSDateTime::month()
{
	return (date / 100) % 100;
}

//////////////////////////////////////////////////////////////////////////
int GPSDateTime::year()
{
	byte year = date % 100;
    year += *year > 80 ? 1900 : 2000;
    return year;
}

//////////////////////////////////////////////////////////////////////////
void GPS::Initialise(int rx, int tx)
{
	gps_serial = SoftwareSerial(rx, tx);
	gps_serial.begin(4800);
	debug_print("GPS setup", DEBUG_LEVEL_IMPORTANT);
	delay(500);
}

//////////////////////////////////////////////////////////////////////////
GPSPosition GPS::position()
{
	GPSPosition pos;
	tiny_gps.get_position(&pos.lat, &pos.lon);
	return pos;
}

//////////////////////////////////////////////////////////////////////////
GPSDateTime GPS::date_time()
{
	GPSDateTime date_time;
	tiny_gps.get_datatime(&gps_date, &gps_time);
	return date_time;
}

//////////////////////////////////////////////////////////////////////////
bool GPS::has_fix()
{
	long lat, lon;
	unsigned long fix_age;

	tiny_gps.get_position(&lat, &lon, &fix_age);
	if(fix_age == TinyGPS::GPS_INVALID_AGE) {
		debug_print("WAARNING No GPS fix detected!", DEBUG_LEVEL_IMPORTANT);
		return false;
	} else if(fix_age > 5000) {
		debug_print("WARNING Possibly old GPS data!", DEBUG_LEVEL_MINOR);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void GPS::poll_date()
{
	serial.listen();

	while(serial.available()) {
		tiny_gps.encode(serial.read());
	}
}