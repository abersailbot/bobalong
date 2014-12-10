/*
	configure.h

	Contains all the configuration for Bobalong, such as pin setting,
	and debug level.

	This code is released under the terms of the LGPLv3 licence.
 */

#include "utils.h"

#ifndef CONFIGURE_H
#define CONFIGURE_H

// The current debug level of the boat
#define DEBUG_LEVEL 			DEBUG_LEVEL_MINOR

#define TEST_SENSORS

//////////////////////////////////////////////////////////////////////////
///
/// Pin Definitions
///
//////////////////////////////////////////////////////////////////////////


// GPS
#define PIN_GPS_RX        8
#define PIN_GPS_TX         9
#define PIN_GPS_ENABLE	   4 	// the enable must be high for the GPS to work,
                              // if its low then the gps is in sleep
                              // mode(Double check this).

// Rowind
#define PIN_ROWIND_RX	     10
#define PIN_ROWIND_TX      11

// Servo
#define PIN_RUDDER_DATA	   5
#define PIN_SAIL_DATA	   6

// PI Controller
#define P_VAL				1.00;
#define I_VAL				0.00;
#define NORMAL_RUDDER_POS	90;

#endif
