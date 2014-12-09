/*
	compass.cpp

	Provides functions for interacting with Bobalong's HMC6343 electronic compass

	This code is released under the terms of the LGPLv3 licence.
 */

#include "compass.h"
#include "Arduino.h"
#include "Wire.h"
#include "utils.h"

#define HMC6343_ADDRESS 		0x19 // I2C device address
#define HMC6343_BEARING_REG 		0x50

#define I2C_TIMEOUT_MS 			1000

HMC6343 Compass = HMC6343();

//////////////////////////////////////////////////////////////////////////
void HMC6343::initialise(){
	Wire.begin();
	debug_print("I2C setup", DEBUG_LEVEL_IMPORTANT);
	delay(500);
}

//////////////////////////////////////////////////////////////////////////
int HMC6343::get_bearing(){
	return bearing;
}

//////////////////////////////////////////////////////////////////////////
int HMC6343::get_pitch(){
	return pitch;
}

//////////////////////////////////////////////////////////////////////////
int HMC6343::get_roll(){
	return roll;
}

//////////////////////////////////////////////////////////////////////////
bool HMC6343::poll_data(){
	byte high, low;
	unsigned long time_out;
 
	// Select the bearing register to be read
	Wire.beginTransmission(HMC6343_ADDRESS);  
	Wire.write(HMC6343_BEARING_REG);
	Wire.endTransmission();

	// requests 6 bytes which will include bearing, pitch and roll
	Wire.requestFrom(HMC6343_ADDRESS, 6, true);

	 // wait for the data or time out
	time_out = millis() + I2C_TIMEOUT_MS;
	while(Wire.available() < 6) {
		if(millis() > time_out) {
			debug_print("Compass timed out!", DEBUG_LEVEL_CRITICAL);
			return false;
		}
	}

	bearing = ((Wire.read() << 8)+ Wire.read());
	pitch = ((Wire.read() << 8)+ Wire.read());
	roll = ((Wire.read() << 8)+ Wire.read());

	return true;
}
