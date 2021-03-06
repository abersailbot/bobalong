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

/// ///////////////////////////////////////////////////////////////////////
///
/// Pin Definitions
///
//////////////////////////////////////////////////////////////////////////


#define PIN_GPS_ENABLE	   		4  	// the enable must be high for the GPS to work,
                              		// if its low then the gps is in sleep
                              		// mode(Double check this).
// multiplexer
#define MULTIPLEXER_D1_PIN		9
#define MULTIPLEXER_D2_PIN		8
#define MULTIPLEXER_SERIAL1		1
#define MULTIPLEXER_GPS			2
#define MULTIPLEXER_ROWIND		4

// Servo
#define PIN_RUDDER_DATA	   	11
#define PIN_SAIL_DATA	   	10

// PI Controller
#define P_VAL				1.00
#define I_VAL				0.01
#define NORMAL_RUDDER_POS	90

#endif
