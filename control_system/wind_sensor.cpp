/*
	wind_sensor.cpp

	Provides functions for interacting with Bobalong's Rowind wind sensor. The
	wind sensor uses a software serial to communicate with the device.

	This code is released under the terms of the LGPLv3 licence.
*/

#include "wind_sensor.h"
#include "Arduino.h"
#include "configure.h"
#include <SoftwareSerial.h>

#define DEBUG_ROWIND

Rowind WindSensor = Rowind();

//////////////////////////////////////////////////////////////////////////
Rowind::Rowind()
	:rowind_serial(PIN_ROWIND_RX, PIN_ROWIND_TX)
{

}

int Rowind::get_speed() {
	return speed;
}

int Rowind::get_direction() {
	return direction;
}

//////////////////////////////////////////////////////////////////////////
void Rowind::initialise()
{
	rowind_serial.begin(4800);
}

//////////////////////////////////////////////////////////////////////////
void Rowind::poll_data()
{
	rowind_serial.listen();
	char* line = get_nmea();
	// Make sure nothing went wrong and that we have a valid line
	if(line == 0) {
		debug_print("Error getting Rowind data!", DEBUG_LEVEL_CRITICAL);
	}

	// Parse line
	char *s = line;
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

//////////////////////////////////////////////////////////////////////////
char* Rowind::get_nmea()
{
	char nmea_buffer[50];
	bool got_data = false;
	unsigned long timeout = millis() + 5000;

	#ifdef DEBUG_ROWIND
	Serial.println("Reading RoWind serial...");
	delay(1000);
	#endif

	// Keep going until we either get the correct data or timeout after
	// 5 seconds
	while(!got_data & millis() <= timeout) {
		// Gives the rowind enough time to send a char
		delay(3);
		char c = rowind_serial.read();

		#ifdef DEBUG_ROWIND
		Serial.print(c);
		#endif

		// Check if the char is the start of a nmea string
		if(c == '$') {
			#ifdef DEBUG_ROWIND
			// Starts a new line on the debug terminal, for better viewing
			Serial.println();
			#endif

			// Once we know we are at the start of a nmea string we read
			// until we hit the end of the string and store it in a buffer
			int i = 0;
			while(c != '\n' & i < 50) {
				nmea_buffer[i] = c;

				#ifdef DEBUG_ROWIND
				Serial.print(c);
				#endif

				c = rowind_serial.read();
				i++;
			}

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

	return nmea_buffer;
}
