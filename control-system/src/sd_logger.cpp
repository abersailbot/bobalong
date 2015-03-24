////////////////////////////////////////////////////////////////////////////////
///
/// File: logger.cpp
///
/// This class provides functions for logging data.
///
/// This code is released under the terms of the LGPLv3 licence.
///
////////////////////////////////////////////////////////////////////////////////

#include "sd_logger.hpp"
#include "sensors.h"
#include "hardware.h"
#include "waypoint_mgr.h"

////////////////////////////////////////////////////////////////////////////////
void SDLogger::appendLog()
{
    set_multiplexer(MULTIPLEXER_SERIAL1);

    GPSDateTime time = Sensors.date_time();

    // timestamp
    Serial1.print(time.day); Serial1.print("/");
    Serial1.print(time.month); Serial1.print("/");
    Serial1.print(time.year);

    Serial1.print(time.hour); Serial1.print(":");
    Serial1.print(time.minute); Serial1.print(":");
    Serial1.print(time.second); Serial1.print(" ");

    logCompass();
    logGPS();
    logWind();
    logWaypoint();

    Serial1.println();
}

////////////////////////////////////////////////////////////////////////////////
void SDLogger::logCompass()
{
    Serial1.print(" Heading: ");
    Serial1.print(Sensors.bearing());
    Serial1.print(" Pitch: ");
    Serial1.print(Sensors.pitch());
    Serial1.print(" Roll: ");
    Serial1.print(Sensors.roll());
}

////////////////////////////////////////////////////////////////////////////////
void SDLogger::logGPS()
{
    GPSPosition pos = Sensors.position();

    Serial1.print(" Latitude: "); Serial1.print(pos.latitude, 6); Serial1.print(" Longitude: "); Serial1.print(pos.longitude, 6);
}

////////////////////////////////////////////////////////////////////////////////
void SDLogger::logWaypoint()
{
    GPSPosition wp = Waypoints.current();
    GPSPosition pos = Sensors.position();

    float dist = TinyGPS::distance_between(pos.latitude, pos.longitude, wp.latitude, wp.longitude);

    Serial1.print(" Waypoint: "); Serial1.print(Waypoints.current_index());
    Serial1.print(" WpDistance: "); Serial1.print(dist);
}

////////////////////////////////////////////////////////////////////////////////
void SDLogger::logWind() {
    Serial1.print(" WindSpd: ");
    Serial1.print(Sensors.wind_speed());

    Serial1.print(" WindDir: ");
    Serial1.print(Sensors.wind_direction());
}
