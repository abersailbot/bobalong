/*
    hardware.h

    Contains m functions for interacting with simple hardware on the boat,
    this includes:
        - Rudder Servo
        - Sail Servo
        - Serial multiplexer

    This code is released under the terms of the LGPLv3 licence.
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

#define SERVO_SAIL      0
#define SERVO_RUDDER    1

/*******************************************************************************
 * Initialises servos
 *
 ******************************************************************************/
void initialise_servos();

/*******************************************************************************
 * Sets the current serial line on the multiplexer.
 *
 * @param line          The serial line to select, 1-4
 *
 ******************************************************************************/
void set_multiplexer(uint8_t line);

/*******************************************************************************
 * Sets a servo to a given angle
 *
 * @param servo         The servo to set, either SERVO_SAIL or SERVO_RUDDER
 * @param angle         The angle to set it to
 *
 ******************************************************************************/
void set_servo(uint8_t servo, uint8_t angle);

#endif
