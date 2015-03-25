/*
    sensors.cpp

    Stores the state the sensors were last in and provides functions for updating
    and getting the state of the sensors.

    This code is released under the terms of the LGPLv3 licence.
*/

#include "sensors.h"
#include "hardware.h"
#include "configure.h"
#include "boat_state.hpp"

SensorMgr Sensors = SensorMgr();


////////////////////////////////////////////////////////////////////////////////
void SensorMgr::initialise(unsigned char sensors)
{
    active_sensors = sensors;

    if(active_sensors & SENSOR_COMPASS) {
        HMC6343_initialise();
    }
}

////////////////////////////////////////////////////////////////////////////////
void SensorMgr::read()
{
    Serial.println("Reading sensors");

    // DO NOT CHANGE THE ORDER!!! COMPASS BREAKS if it's read after rowind, NO ONE KNOWS

    if(active_sensors & SENSOR_ROWIND) {
        //Serial.println("Reading rowind");
        // update rowind
        set_multiplexer(MULTIPLEXER_ROWIND);
        WindSensor.poll_data();
        lastState.windSpd = WindSensor.get_speed();
        lastState.windDir = WindSensor.get_direction();
    }

    if(active_sensors & SENSOR_GPS) {
        Serial.println("Reading gps");
        // update gps
        set_multiplexer(MULTIPLEXER_GPS);
        Gps.poll_data();
        lastState.gpsPosition = Gps.position();
        lastState.gpsDateTime = Gps.date_time();
        //Gps.print_nmea();
    }

    if(active_sensors & SENSOR_COMPASS) {
        Serial.println("Reading compass");
        // update compass data
        CompassData compass = HMC6343_read_data();
        // compass is positioned 98 degrees off the front of the boat
        lastState.compassBearing = clamp_angle(compass.bearing +273);
        lastState.compassPitch = compass.pitch;
        lastState.compassRoll = compass.roll;
    }
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::bearing()
{
    return lastState.compassBearing;
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::pitch()
{
    return lastState.compassPitch;
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::roll()
{
    return lastState.compassRoll;
}

////////////////////////////////////////////////////////////////////////////////
float SensorMgr::wind_speed()
{
    return lastState.windSpd;
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::wind_direction()
{
    return lastState.windDir;
}

////////////////////////////////////////////////////////////////////////////////
GPSPosition SensorMgr::position()
{
    return lastState.gpsPosition;
}

////////////////////////////////////////////////////////////////////////////////
GPSDateTime SensorMgr::date_time()
{
    return lastState.gpsDateTime;
}
