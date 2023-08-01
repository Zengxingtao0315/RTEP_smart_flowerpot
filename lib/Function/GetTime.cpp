/*****************************************************************************
* | File      	:   GetTime.cpp
* | Author      :   xingtao Zeng
* | Function    :   Get local time
* | Info        :
******************************************************************************/

#include "GetTime.hpp"

/*******************************************************************************
Function:
    Get local time
*******************************************************************************/
// Function to get the current local time and return it in the form of a PAINT_TIME struct.
PAINT_TIME Time::getLocalTime() {
    PAINT_TIME time;
    std::time_t now;
    std::tm *local;

    // Get the current time in seconds since the epoch.
    std::time(&now);
    // Convert the current time to the local time (considering time zone and daylight saving).
    local = std::localtime(&now);

    // Extract the required time information (hour, minute, and second) from the local time and store it in the PAINT_TIME struct.
    time.Hour = local->tm_hour;
    time.Min = local->tm_min;
    time.Sec = local->tm_sec;
    return time;
}

