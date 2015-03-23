/*
    sensors.cpp

    Stores the state the sensors were last in and provides functions for updating
    and getting the state of the sensors.

    This code is released under the terms of the LGPLv3 licence.
*/

#include "sensors.h"
#include "hardware.h"
#include "configure.h"

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
        wind_spd = WindSensor.get_speed();
        wind_dir = WindSensor.get_direction();
    }

    if(active_sensors & SENSOR_GPS) {
        Serial.println("Reading gps");
        // update gps
        set_multiplexer(MULTIPLEXER_GPS);
        Gps.poll_data();
        gps_position = Gps.position();
        gps_date_time = Gps.date_time();
        //Gps.print_nmea();
    }

    if(active_sensors & SENSOR_COMPASS) {
        Serial.println("Reading compass");
        // update compass data
        CompassData compass = HMC6343_read_data();
        // compass is positioned 98 degrees off the front of the boat
        compass_bearing = clamp_angle(compass.bearing - 98);
        compass_pitch = compass.pitch;
        compass_roll = compass.roll;
    }
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::bearing()
{
    return compass_bearing;
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::pitch()
{
    return compass_pitch;
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::roll()
{
    return compass_roll;
}

////////////////////////////////////////////////////////////////////////////////
float SensorMgr::wind_speed()
{
    return wind_spd;
}

////////////////////////////////////////////////////////////////////////////////
int SensorMgr::wind_direction()
{
    return wind_dir;
}

////////////////////////////////////////////////////////////////////////////////
GPSPosition SensorMgr::position()
{
    return gps_position;
}

////////////////////////////////////////////////////////////////////////////////
GPSDateTime SensorMgr::date_time()
{
    return gps_date_time;
}
