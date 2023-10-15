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


SunlightDurationRecorder::SunlightDurationRecorder(Sensor *sensorPtr):sensor(sensorPtr){};
SunlightDurationRecorder::~SunlightDurationRecorder(){
    delete sensor;
};

// Function to calculate the duration of sunlight in hours based on a digital value (0 or 1).
float SunlightDurationRecorder::getSunlightDurationInHours() {
	UWORD digitalValue = sensor->readDigitalValue();
	auto now = std::chrono::system_clock::now();
    if (!initialized_) {
        // First call to the function to initialize logging information
        start_time_ = now;
        duration_0_ = std::chrono::duration<int, std::ratio<1, 1>>(0);
        last_reset_time_ = now;
        initialized_ = true;
    } else {
        // Check to see if a new day has been reached, and if so, reset the duration record
        auto time_since_last_reset = now - last_reset_time_;
        if (time_since_last_reset >= std::chrono::hours(24)) {
            duration_0_ = std::chrono::duration<int, std::ratio<1, 1>>{0};
            last_reset_time_ = now;
        }
    }

    // Record the duration since the start_time_ according to the digital value (0 or 1).
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time_);
    if (digitalValue == 0) {
        duration_0_ += duration;
    }
    if (digitalValue == 1) {
        // If digitalValue is 1, reset the start_time_ to the current time.
        start_time_ = now;
    }

    // Get the total duration of sunlight in seconds.
    int light_duration = duration_0_.count();
    std::chrono::seconds Drt{light_duration};
    // Convert the duration to hours and return the result.
    float hours = std::chrono::duration_cast<std::chrono::hours>(Drt).count();
    std::cout << hours << " hours" << std::endl;
    return hours;
}



void Time::startRecording() {
        startTime = std::chrono::high_resolution_clock::now();
    }

void Time::stopRecording() {
    endTime = std::chrono::high_resolution_clock::now();
}

long long Time::getDuration() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    }
