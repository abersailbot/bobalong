#include <Wire.h>
#include <Servo.h>
#include "utils.h"
#include "sensors.h"
#include "hardware.h"
#include "waypoint_mgr.h"
#include "sd_logger.hpp"
#include "boat_state.hpp"

#define SAIL_LEFT	135
#define SAIL_RIGHT	45

int desired_heading;
SDLogger sdLogger;

BoatState lastState;

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

	Waypoints.add_waypoint(52.400246, -3.869921);
	Waypoints.add_waypoint(52.400825, -3.870545);
	Waypoints.add_waypoint(52.400816, -3.869552);
	Waypoints.add_waypoint(52.399860, -3.869579);

	Serial.println("Initialised");
}


////////////////////////////////////////////////////////////////////////////////
void loop() {
	static int rudder_pos = 90;

	Sensors.read();

	// log sensor data
	startLog();
	log_compass();
	log_gps();
	log_wind();
	endLog();

	// log to the sd card
	sdLogger.appendLog();

	// advance to the next waypoint if we are there
	if(at_waypoint()) {
		Waypoints.advance();
	}

	desired_heading = clamp_angle(desiredHeading());
	Serial.print("Desired: "); Serial.println(desired_heading);

	// set rudder
	rudder_pos = get_rudder_angle(Sensors.bearing());
	Serial.print("Rudder pos: "); Serial.println(rudder_pos);
	set_servo(SERVO_RUDDER, rudder_pos);

	setSail();
}

////////////////////////////////////////////////////////////////////////////////
bool at_waypoint()
{
	GPSPosition curr_pos = Sensors.position();
	GPSPosition wp = Waypoints.current();

	float dist = TinyGPS::distance_between(curr_pos.latitude, curr_pos.longitude, wp.latitude, wp.longitude);

	if( dist < 10) {
		Serial.println("Hit Waypoint");
		return true;
	} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////
int desiredHeading()
{
	GPSPosition curr_pos = Sensors.position();
	GPSPosition wp = Waypoints.current();

	return TinyGPS::course_to(curr_pos.latitude, curr_pos.longitude, wp.latitude, wp.longitude);
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
	lastState.pGain = pCorrection;


	//  I
	errorSum += error;
	iCorrection = I_VAL * errorSum;
	lastState.iGain = iCorrection;
	lastState.errorSum = errorSum;

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
void setSail()
{
	if(Sensors.wind_direction() > 180) {
		set_servo(SERVO_SAIL, SAIL_LEFT);
	} else {
		set_servo(SERVO_SAIL, SAIL_RIGHT);
	}
}


////////////////////////////////////////////////////////////////////////////////
// Logging to the serial monitor
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void log_compass() {
	Serial.print(" Heading: ");
	Serial.print(Sensors.bearing());
	Serial.print(" Pitch: ");
	Serial.print(Sensors.pitch());
	Serial.print(" Roll: ");
	Serial.print(Sensors.roll());
}

////////////////////////////////////////////////////////////////////////////////
void log_gps() {
	GPSPosition pos = Sensors.position();
	Serial.print(" Latitude: "); Serial.print(pos.latitude, 6); Serial.print(" Longitude: "); Serial.print(pos.longitude, 6);

	GPSPosition wp = Waypoints.current();
	float dist = TinyGPS::distance_between(wp.latitude, wp.longitude, pos.latitude, pos.longitude);

	Serial.print(" WpDistance: "); Serial.print(dist);
	Serial.print(" Waypoint: "); Serial.print(Waypoints.current_index());
}

////////////////////////////////////////////////////////////////////////////////
void log_wind() {
	Serial.print(" WindSpd: "); Serial.print(Sensors.wind_speed()); Serial.print(" WindDir: "); Serial.print(Sensors.wind_direction());
}

////////////////////////////////////////////////////////////////////////////////
void startLog()
{
	set_multiplexer(MULTIPLEXER_SERIAL1);

	GPSDateTime time = Sensors.date_time();

	// timestamp
	Serial.print(time.day); Serial.print("/");
	Serial.print(time.month); Serial.print("/");
	Serial.println(time.year);

	Serial.print(time.hour); Serial.print(":");
	Serial.print(time.minute); Serial.print(":");
	Serial.print(time.second); Serial.print(" ");

}

////////////////////////////////////////////////////////////////////////////////
void endLog()
{
	Serial.println();
}
