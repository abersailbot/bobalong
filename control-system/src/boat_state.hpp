////////////////////////////////////////////////////////////////////////////////
///
/// File: boat_state.hpp
///
/// This class provides a data struct which contains the current boat's state.
///
/// This code is released under the terms of the LGPLv3 licence.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef BOAT_STATE_H
#define BOAT_STATE_H

struct BoatState {
    unsigned int compassBearing;
    unsigned int compassPitch;
    unsigned int compassRoll;
    float windSpd;
    unsigned int windDir;
    GPSPosition gpsPosition;
    GPSDateTime gpsDateTime;
    unsigned int waypoint;
    float pGain;
    float iGain;
    float errorSum;
};

extern BoatState lastState;

#endif
