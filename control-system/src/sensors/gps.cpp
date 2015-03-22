/*
	gps.c

	Contains functions for dealing with the boat's GPS. To use the
	GPS it must first be initialised. The GPS uses TinyGPS to parse the GPS NMEA strings

	This code is released under the terms of the LGPLv3 licence.


        TODO:
          - A debug print function that prints the GPS NMEA
 */

#include "gps.h"

GPS Gps = GPS();

//////////////////////////////////////////////////////////////////////////
GPSPosition GPS::position()
{
	GPSPosition pos;
	tiny_gps.f_get_position(&pos.latitude, &pos.longitude);

	return pos;
}

//////////////////////////////////////////////////////////////////////////
GPSDateTime GPS::date_time()
{
	GPSDateTime date_time;
	byte hundredths;
	unsigned long fix_age;

	tiny_gps.crack_datetime(&date_time.year, &date_time.month, &date_time.day,
	&date_time.hour, &date_time.minute, &date_time.second, &hundredths, &fix_age);

	return date_time;
}

//////////////////////////////////////////////////////////////////////////
bool GPS::has_fix()
{
	long lat, lon;
	unsigned long fix_age;

	tiny_gps.get_position(&lat, &lon, &fix_age);
	if(fix_age == TinyGPS::GPS_INVALID_AGE) {
		debug_print("WARNING No GPS fix detected!", DEBUG_LEVEL_IMPORTANT);
		return false;
	} else if(fix_age > 5000) {
		debug_print("WARNING Possibly old GPS data!", DEBUG_LEVEL_MINOR);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void GPS::poll_data()
{
    char c;
	has_fix();

	delay(500);
	unsigned long timeout = millis() + 500;

	while(timeout > millis()) {
		delay(1);
		while(!Serial1.available());
		c = Serial1.read();
		Serial.print(c);
		tiny_gps.encode(c);
	}
	Serial.println();
}

//////////////////////////////////////////////////////////////////////////
void GPS::print_nmea()
{
        bool not_found = 1;
        char c;
        while(not_found) {
          if(Serial1.available()) {
             c = Serial1.read();
             Serial.print(c);

             if(c == '\n') {
               not_found = 0;
             }
             tiny_gps.encode(c);
          }
        }

}
