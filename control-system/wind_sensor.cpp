/*
	wind_sensor.cpp

	Provides functions for interacting with Bobalong's Rowind wind sensor. The
	wind sensor uses a software serial to communicate with the device.

	This code is released under the terms of the LGPLv3 licence.
 */

#include "wind_sensor.h"
#include "Arduino.h"
#include "utils.h"
#include <SoftwareSerial.h>

#define DEBUG_ROWIND

//////////////////////////////////////////////////////////////////////////
void WindSensor::initialise(int rx, int tx)
{
	wind_serial = SoftwareSerial(rx, tx);
	wind_serial.begin(4800);
}

//////////////////////////////////////////////////////////////////////////
int WindSensor::wind_direction()
{
	return direction;
}

//////////////////////////////////////////////////////////////////////////
double WindSensor::wind_speed()
{
	return speed;
}

bool WindSensor::poll_data()
{
	wind_serial.listen();
  	char* line = get_nmea();
  	// Make sure nothing went wrong and that we have a valid line
  	if(line == 0) {
  		return false;
  	}

	// Parse line
	char *s = line;
	char *str;
	int i = 0;

	//  Splits the string into tokens whic hare seperated by ','. Returns null when at the end
	while ((str = strtok_r( s, ",", &s )) != NULL ) {
		// Second token contains the wind direction
		if ( i == 1 ) {
			wind_data.direction = atof( str );

		// fourth token contains wind speed
		} else if ( i == 3 ) {
			wind_data.speed = atof( str );
		}
		i++;
	}

	return true;	
}

//////////////////////////////////////////////////////////////////////////
char* WindSensor::get_nmea()
{
	char nmea_buffer[50];
	bool got_data = false;
	unsigned long timeout = millis() + 5000;

	debug_serial("Reading Rowind serial", DEBUG_LEVEL_MINOR);

	// Keep going until we either get the correct data or timeout after
	// 5 seconds
	while(!got_data & millis() <= timeout) {
		// Gives the rowind enough time to send a char
		delay(3);
		char c = wind_serial.read();

		debug_serial(c, DEBUG_LEVEL_MINOR);

		// Check if the char is the start of a nmea string
		if(c == '$') {
			debug_serial("\n", DEBUG_LEVEL_MINOR);

			// Once we know we are at the start of a nmea string we read
			// until we hit the end of the string and store it in a buffer
			int i = 0;
			while(c != '\n' & i < 50) {
				nmea_buffer[i] = c;

				debug_serial(c, DEBUG_LEVEL_MINOR);

				c = wind_ss.read();
				i++;
			}

			// Now check if we have a $IIMWV nmea string
			if(nmea_buffer[1] == 'I') {
				got_data = true;
				debug_serial("Found $IIMWV", DEBUG_LEVEL_MINOR);
			}
		}
	}

    if(!got_data) {
    	debug_serial("Rowind timeout!", DEBUG_LEVEL_CRITICAL);
        return 0;
    }
        
	return nmea_buffer;
}
