// Arduino Libraries
#include <Wire.h>
#include <SoftwareSerial.h>
/////////////////////////////////////////////////////////////////
#define TEST_COMPASS


/////////////////////////////////////////////////////////////////
// Boat libraries
#include "pin_defs.h"

#ifdef TEST_COMPASS
	#include "compass.h"
#endif#

#ifdef TEST_ROWIND
	#include "wind_sensor.h"
#endif

#ifdef TEST_GPS
	#include "gps.h"
#endif

void setup() {
	Serial.begin(9600);
	Serial.println("Starting up");
	
#ifdef TEST_COMPASS
	Compass.initialise();
#endif

#ifdef TEST_GPS
	
#endif

#ifdef TEST_ROWIND
#endif
}


void compass_test() {
#ifdef TEST_COMPASS
	Serial.println("Reading compass");
	Compass.poll_data();
	Serial.print("Heading: ");
	Serial.println(Compass.get_bearing());	
#endif
}

void gps_test() {
	
}

void loop() {
	compass_test();
}
