/*
    sensors.h

    Stores the state the sensors were last in and provides functions for updating
    and getting the state of the sensors.

    This code is released under the terms of the LGPLv3 licence.
 */

#ifndef SENSORS_H
#define SENSORS_H

#include "sensors/compass.h"
#include "sensors/gps.h"
#include "sensors/wind_sensor.h"

class SensorMgr {
public:
    /***************************************************************************
     * Initialises any sensors that need to be initialised
     *
     **************************************************************************/
    void initialise();

    /***************************************************************************
     * Reads data from the externel sensors and updates the internal state of
     * the sensor data.
     *
     **************************************************************************/
    void read();

    /***************************************************************************
    * Returns the compass's bearing.
    *
    **************************************************************************/
    int bearing();

    /***************************************************************************
    * Returns the compass's pitch.
    *
    **************************************************************************/
    int pitch();

    /***************************************************************************
    * Returns the compass's roll.
    *
    **************************************************************************/
    int roll();

    float wind_speed();

    int wind_direction();

    /***************************************************************************
    * Returns the latest GPS position
    *
    **************************************************************************/
    GPSPosition position();

    /***************************************************************************
    * Returns the latest GPS time and date
    *
    **************************************************************************/
    GPSDateTime date_time();
private:
    unsigned int compass_bearing;
    unsigned int compass_pitch;
    unsigned int compass_roll;
    float wind_spd;
    unsigned int wind_dir;
    GPSPosition gps_position;
    GPSDateTime gps_date_time;
};

extern SensorMgr Sensors;

#endif
