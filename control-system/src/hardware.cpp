
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

Servo rudder_servo;
Servo sail_servo;

////////////////////////////////////////////////////////////////////////////////
void initialise_servos()
{
    rudder_servo.attach(PIN_RUDDER_DATA);
    sail_servo.attach(PIN_SAIL_DATA);
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
        sail_servo.write(angle);
    } else {
        rudder_servo.write(angle);
    }
}
