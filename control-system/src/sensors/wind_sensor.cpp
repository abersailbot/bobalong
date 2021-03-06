/*
	wind_sensor.cpp

	Provides functions for interacting with Bobalong's Rowind wind sensor. The
	wind sensor uses a software serial to communicate with the device.

	This code is released under the terms of the LGPLv3 licence.
*/

#include "wind_sensor.h"
#include "Arduino.h"
#include "../configure.h"

#define DEBUG_ROWIND

Rowind WindSensor = Rowind();
char nmea_buffer[50];

float Rowind::get_speed() {
	return speed;
}

int Rowind::get_direction() {
	return direction;
}

//////////////////////////////////////////////////////////////////////////
void Rowind::poll_data()
{
	if(!get_nmea()) {
		debug_print("Error getting Rowind data!", DEBUG_LEVEL_CRITICAL);
	} else {
		// Parse line
		char *s = nmea_buffer;
		char *str;
		int i = 0;

		//  Splits the string into tokens whic hare seperated by ','. Returns null when at the end
		while ((str = strtok_r( s, ",", &s )) != NULL ) {
			// Second token contains the wind direction
			if ( i == 1 ) {
				direction = atof( str );

				// fourth token contains wind speed
			} else if ( i == 3 ) {
				speed = atof( str );
			}
			i++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
bool Rowind::get_nmea()
{
	memset(nmea_buffer, 0, 50);
	bool got_data = false;
	unsigned long timeout = millis() + 10000;

	#ifdef DEBUG_ROWIND
	Serial.println("Reading RoWind serial...");
	delay(1000);
	#endif

	// Keep going until we either get the correct data or timeout after
	// 10 seconds
	while(!got_data && (millis() <= timeout)) {
		// Gives the rowind enough time to send a char
		//delay(3);

		while(!Serial1.available());
		char c = Serial1.read();

		//#ifdef DEBUG_ROWIND
		//Serial.print(c);
		//#endif

		// Check if the char is the start of a nmea string
		if(c == '$') {
			#ifdef DEBUG_ROWIND
			// Starts a new line on the debug terminal, for better viewing
			//Serial.println();
			#endif

			// Once we know we are at the start of a nmea string we read
			// until we hit the end of the string and store it in a buffer
			int i = 0;
			while(c != '\n' & i < 49) {
				nmea_buffer[i] = c;

				#ifdef DEBUG_ROWIND
				//Serial.print(c);
				#endif
				while(!Serial1.available());
				c = Serial1.read();
				i++;
			//	delay(5);
			}
			nmea_buffer[i] = 0;
//Serial.println(nmea_buffer);
			// Now check if we have a $IIMWV nmea string
			if(nmea_buffer[1] == 'I') {
				got_data = true;
				#ifdef DEBUG_ROWIND
				Serial.println("Found $IIMWV");
				#endif
			}
		}
	}

	if(!got_data) {
		debug_print("Rowind timed out, bad connection or no data!", DEBUG_LEVEL_CRITICAL);
		return 0;
	}

	return got_data;
}
