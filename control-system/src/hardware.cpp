
/*
    hardware.h

    Contains m functions for interacting with simple hardware on the boat,
    this includes:
    - Rudder Servo
    - Sail Servo
    - Serial multiplexer

    This code is released under the terms of the LGPLv3 licence.
*/

#include "hardware.h"
#include "Servo.h"
#include "configure.h"

Servo rudder;
Servo sail;

////////////////////////////////////////////////////////////////////////////////
void initialise_servos()
{
    rudder.attach(PIN_RUDDER_DATA);
    sail.attach(PIN_SAIL_DATA);
}

////////////////////////////////////////////////////////////////////////////////
void set_multiplexer(uint8_t line)
{
    if(line & 1) {
        digitalWrite(MULTIPLEXER_IN1_PIN, HIGH);
    } else {
        digitalWrite(MULTIPLEXER_IN1_PIN, LOW);
    }

    if(line & 2) {
        digitalWrite(MULTIPLEXER_IN2_PIN, HIGH);
    } else {
        digitalWrite(MULTIPLEXER_IN2_PIN, LOW);
    }
}

//////////////////////////////////////////////////////////////////////////
void set_servo(uint8_t servo, uint8_t angle)
{
    if(servo == SERVO_SAIL) {
        sail.write(angle);
    } else {
        rudder.write(angle);
    }
}
