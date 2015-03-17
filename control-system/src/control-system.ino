#include <Wire.h>
#include <Servo.h>
#include "utils.h"
#include "sensors.h"
#include "hardware.h"
#include "waypoint_mgr.h"

Servo rudder;

int desired_heading;

void setup() {
	delay(5000);
	Serial.begin(9600);
	Serial1.begin(4800);
	Serial.println("Starting up");

	pinMode(MULTIPLEXER_D1_PIN, OUTPUT);
	pinMode(MULTIPLEXER_D2_PIN, OUTPUT);

	digitalWrite(MULTIPLEXER_D1_PIN, LOW);
	digitalWrite(MULTIPLEXER_D2_PIN, LOW);

	Sensors.initialise(SENSOR_COMPASS | SENSOR_GPS | SENSOR_ROWIND);

	initialise_servos();

	// add waypoints


	Serial.println("Initialised");
}


////////////////////////////////////////////////////////////////////////////////
void loop() {
	Sensors.read();

	// print sensor data
	print_compass();
	print_gps();
	print_wind();

	// test servos
	set_servo(SERVO_SAIL, 140);
	set_servo(SERVO_RUDDER, 30);

	// advance to the next waypoint if we are there
	GPSPosition curr_pos = Sensors.position();
	GPSPosition wp = Waypoints.current();

	float dist = TinyGPS::distance_between(wp.latitude, wp.longitude, curr_pos.latitude, curr_pos.longitude);

	if( dist < 10) {
		Waypoints.advance();
		Serial.println("Hit waypoint, moving onto next");
	} else {
		Serial.print("Distance : ");
		Serial.println(dist);
	}

	desired_heading = TinyGPS::course_to(wp.latitude, wp.longitude, curr_pos.latitude, curr_pos.longitude);

	// set rudder
	int rudder_pos = get_rudder_angle(Sensors.bearing());
	if(rudder_pos < 85) {
		Serial.println("Turn left!");
	} else if(rudder_pos > 95) {
		Serial.println("Turn right!");
	}
	Serial.print("Rudder pos: "); Serial.println(rudder_pos);
}

//////////////////////////////////////////////////////////////////////////
int get_rudder_angle(int heading)
{

	// PI Controller


	float pCorrection;
	float iCorrection;
	float error;
	static float errorSum;
	int rudderAngle;


	// The desired heading is now relative to zero
	float desired_headingCP = desired_heading - heading;

	// Finding out if the value should be negative or positive and how big
	if (desired_headingCP > 180){
		desired_headingCP -= 360;
	}
	else if (desired_headingCP < -180){
		desired_headingCP += 360;
	}


	// A positive number means that the boat will turn left.
	// A negative number is right

	//  P
	error = desired_headingCP * (-1);
	pCorrection = P_VAL * error;


	//  I
	errorSum += error;
	iCorrection = I_VAL * errorSum;

	errorSum *= 0.95;

	rudderAngle = NORMAL_RUDDER_POS + round(pCorrection + iCorrection);

	// Making sure the rudder does not go over 45 degrees from normal position (90 deg(?))
	if (rudderAngle > (NORMAL_RUDDER_POS + 45)){
		rudderAngle = NORMAL_RUDDER_POS + 45;
	}
	else if (rudderAngle < (NORMAL_RUDDER_POS - 45)){
		rudderAngle = NORMAL_RUDDER_POS - 45;
	}

	return rudderAngle;
}

////////////////////////////////////////////////////////////////////////////////
void print_compass() {
	Serial.print("Heading: ");
	Serial.print(Sensors.bearing());
	Serial.print(" Pitch: ");
	Serial.print(Sensors.pitch());
	Serial.print(" Roll: ");
	Serial.println(Sensors.roll());

}

////////////////////////////////////////////////////////////////////////////////
void print_gps() {
	GPSPosition pos = Sensors.position();
	Serial.print("position: "); Serial.print(pos.latitude); Serial.print(" "); Serial.println(pos.longitude);
	GPSDateTime time = Sensors.date_time();
	Serial.print("time: ");
	Serial.print(time.hours()); Serial.print("h ");
	Serial.print(time.minutes()); Serial.print("m ");
	Serial.print(time.seconds()); Serial.print("s ");

	Serial.print(time.day()); Serial.print("/");
	Serial.print(time.month()); Serial.print("/");
	Serial.println(time.year()); Serial.print("\n");
}


////////////////////////////////////////////////////////////////////////////////
void print_wind() {
	Serial.print("Wind speed: "); Serial.print(Sensors.wind_speed()); Serial.print(" Direction: "); Serial.println(Sensors.wind_direction());
}
