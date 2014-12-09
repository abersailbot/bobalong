#include <Wire.h>
#include <SoftwareSerial.h>

#include "utils.h"
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

        // gps coordinates
        GPSPosition wps[3];
        wp[0].latitude = 0;
        wp[0].longitude = 0;
        
        wp[1].latitude = 0;
        wp[1].longitude = 0;
        
        wp[2].latitude = 0;
        wp[2].longitude = 0;
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
        Gps.poll_data();
	Serial.println("Testing Gps");
	if(!Gps.has_fix()){
          Serial.println("GPS waiting for fix");
	  for(int i = 0; !Gps.has_fix(); i++){
		Serial.println(i);
		Gps.poll_data();
                delay(1000);
          }
	}
	Serial.println("\nFix found!");
	GPSPosition pos = Gps.position();
	Serial.print("position: "); Serial.print(pos.latitude); Serial.print(" "); Serial.println(pos.longitude);
	GPSDateTime time = Gps.date_time();
	Serial.print("time: ");
		Serial.print(time.hours()); Serial.print("h ");
		Serial.print(time.minutes()); Serial.print("m ");
		Serial.print(time.seconds()); Serial.print("s ");

		Serial.print(time.day()); Serial.print("/");
		Serial.print(time.month()); Serial.print("/");
		Serial.println(time.year()); Serial.print("\n");


}

void waypoint_test() {
      
}

/////////////////////////////////////////////////////////////////
void rowind_test() {
	// todo
}

/////////////////////////////////////////////////////////////////
void loop() {
      delay(5000);
	#ifdef TEST_SENSORS
		gps_test();
 
                compass_test();
                
                
		rowind_test();
	#endif
}
