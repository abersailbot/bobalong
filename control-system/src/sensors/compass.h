/*
	compass.h

	Provides functions for interacting with Bobalong's HMC6343 electronic compass

	This code is released under the terms of the LGPLv3 licence.
 */

#ifndef COMPASS_H
#define COMPASS_H

struct CompassData {
	unsigned int bearing;
	unsigned int pitch;
	unsigned int roll;
};

/*******************************************************************************
 * Initialises the wire library and setups the compass.
 *
*******************************************************************************/
void HMC6343_initialise();

/*******************************************************************************
 * Reads the compass and returns any data read.
 *
*******************************************************************************/
CompassData HMC6343_read_data();

#endif
