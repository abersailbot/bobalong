#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#include "utils.h"
#include "compass.h"
#include "wind_sensor.h"
#include "gps.h"
#include "waypoint_mgr.h"
#include "navigator.h"

Servo rudder;

void setup() {
	delay(5000);
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
    wps[0].latitude = 52.415603;
    wps[0].longitude = -4.065145;

    wps[1].latitude = 52.415454;
    wps[1].longitude = -4.065937;

    wps[2].latitude = 52.416385;
    wps[2].longitude = -4.066550;

    Waypoints.add_waypoints(wps, 3);
	rudder.attach(PIN_RUDDER_DATA);
}
////////////////////////////////////////////////////////////////////////////////
// MOVE INTO TESTING MODULE

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
  Gps.print_nmea();
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
      GPSPosition curr_pos = Gps.position();
      GPSPosition wp = Waypoints.current();
      float dist = TinyGPS::distance_between(wp.latitude, wp.longitude, curr_pos.latitude, curr_pos.longitude);
      if( dist < 10) {
          Waypoints.advance();
          Serial.println("Hit waypoint, moving onto next");
      } else {
          Serial.print("Distance : ");
          Serial.println(dist);
      }
}

/////////////////////////////////////////////////////////////////
void rowind_test() {
	WindSensor.poll_data();

	Serial.print("Wind speed: "); Serial.print(WindSensor.get_speed()); Serial.print("direction: "); Serial.println(WindSensor.get_direction());
}

/////////////////////////////////////////////////////////////////
void rudder_test() {
	int bearing = Compass.get_bearing();
	int rudder_pos = Pilot.get_rudder_angle(bearing);
	rudder.write(rudder_pos);
	Serial.print("Rudder Pos: "); Serial.println(rudder_pos);

}


////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void loop() {
	#ifdef TEST_SENSORS
		//gps_test();
		//Serial.println("test");
    compass_test();
		//rowind_test();
		rudder_test();
		//waypoint_test();

		/*if(Gps.has_fix()) {
			Pilot.navigate(bearing, 0);
		}*/

		//rowind_test();
		delay(50);
	#endif
}
