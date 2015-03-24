////////////////////////////////////////////////////////////////////////////////
///
/// File: logger.hpp
///
/// This class provides functions for logging data.
///
/// This code is released under the terms of the LGPLv3 licence.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SD_LOGGER_H
#define SD_LOGGER_H

class SDLogger {
public:
    void appendLog();
private:
    void logCompass();
    void logGPS();
    void logWaypoint();
    void logWind();
};

#endif
