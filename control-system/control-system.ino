// Arduino Libraries
#include <Wire.h>
#include <SoftwareSerial.h>

/////////////////////////////////////////////////////////////////
// Boat libraries
#include "compass.h"
#include "wind_sensor.h"
#include "gps.h"

void setup() {
	Serial.begin(9600);
	Serial.println("Starting up");

	debug_print("Initialising Compass!", DEBUG_LEVEL_CRITICAL);
	delay(200);
	Compass.initialise();

	debug_print("Initialising GPS!", DEBUG_LEVEL_CRITICAL);
	delay(200);
	Gps.initialise();

	debug_print("Initialising Rowind!", DEBUG_LEVEL_CRITICAL);
	delay(200);
	WindSensor.initialise();
}

/////////////////////////////////////////////////////////////////
void compass_test() {
	Serial.println("Reading compass");
	Compass.poll_data();
	Serial.print("Heading: ");
	Serial.println(Compass.get_bearing());
}

/////////////////////////////////////////////////////////////////
void gps_test() {
	// todo
}

/////////////////////////////////////////////////////////////////
void rowind_test() {
	// todo
}

/////////////////////////////////////////////////////////////////
void loop() {
	#ifdef TEST_SENSORS
		compass_test();
		gps_test();
		rowind_test();
	#endif
}
