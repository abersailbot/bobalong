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
	year += year > 80 ? 1900 : 2000;
	return year;
}

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
	tiny_gps.get_datetime(&date_time.date, &date_time.time);
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
	while(Serial1.available()) {
        c = Serial1.read();
		Serial.print(c);
		if(tiny_gps.encode(c)){
        	Serial.println("encoded");
    	}

	}
	Serial.println();
}

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
