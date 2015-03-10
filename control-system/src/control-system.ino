#include <Wire.h>
#include <Servo.h>
#include "utils.h"
#include "sensors.h"
#include "hardware.h"

Servo rudder;

void setup() {
	delay(5000);
	Serial.begin(9600);
	Serial.println("Starting up");

	Sensors.initialise();
}

////////////////////////////////////////////////////////////////////////////////
void print_compass() {
	Serial.print("Heading: ");
	Serial.print(Sensors.bearing());
	Serial.print("Pitch: ");
	Serial.print(Sensors.pitch());
	Serial.print("Roll: ");
	Serial.print(Sensors.roll());
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
	Serial.print("Wind speed: "); Serial.print(Sensors.wind_speed()); Serial.print("direction: "); Serial.println(Sensors.wind_direction());
}


////////////////////////////////////////////////////////////////////////////////
void loop() {
	Sensors.read();

	print_compass();
	print_gps();
	print_wind();

	set_servo(SERVO_SAIL, 140);
	set_servo(SERVO_RUDDER, 30);

	delay(1500);
}
