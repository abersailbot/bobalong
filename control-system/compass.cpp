/*
	compass.cpp

	Provides functions for interacting with Bobalong's HMC6343 electronic compass

	This code is released under the terms of the LGPLv3 licence.
 */

#include "compass.h"
#include "Arduino.h"
#include "Wire.h"
#include "utils.h"

#define HMC6343_ADDRESS 			0x19 // I2C device address
#define HMC6343_BEARING_REG 		0x50

#define I2C_TIMEOUT_MS 				1000

//////////////////////////////////////////////////////////////////////////
void Compass::initialise()
{
	Wire.begin();
}

//////////////////////////////////////////////////////////////////////////
int Compass::bearing()
{
	return bearing;
}

//////////////////////////////////////////////////////////////////////////
int Compass::pitch()
{
	return pitch;
}

//////////////////////////////////////////////////////////////////////////
int Compass::roll()
{
	return roll;
}

//////////////////////////////////////////////////////////////////////////
bool Compass::poll_data()
{
	byte high, low;
	unsigned long timeout;
        
	// Select the bearing register to be read
	Wire.beginTransmission(HMC6343_ADDRESS);  
	Wire.write(reg);
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