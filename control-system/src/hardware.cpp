
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
        Serial.println("Serial changed to Serial 1");
        digitalWrite(MULTIPLEXER_D1_PIN, LOW);
        digitalWrite(MULTIPLEXER_D2_PIN, LOW);
    } else if(line & 2) {
        Serial.println("Serial changed to GPS");
        digitalWrite(MULTIPLEXER_D1_PIN, HIGH);
        digitalWrite(MULTIPLEXER_D2_PIN, LOW);
    } else if(line & 4){
        Serial.println("Serial changed to rowind");
        digitalWrite(MULTIPLEXER_D1_PIN, LOW);
        digitalWrite(MULTIPLEXER_D2_PIN, HIGH);
    } else {
        Serial.println("Serial changed to Serial 4");
        digitalWrite(MULTIPLEXER_D1_PIN, HIGH);
        digitalWrite(MULTIPLEXER_D2_PIN, HIGH);
    }
    delay(50);
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
