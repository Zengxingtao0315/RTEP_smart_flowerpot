#ifndef _GETTIME_HPP_
#define _GETTIME_HPP_

#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <chrono>
extern "C" {
	#include <time.h>
};
#include <ctime>
#include "../GUI/GUI_Paint.hpp"
#include "Sensor.hpp" 


class Time {
public:
    PAINT_TIME getLocalTime();
    void startRecording();

    void stopRecording();
    long long getDuration();
    Time();
    ~Time();

private:
    auto starTime;
    auto endTime;
    PAINT_TIME time;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
};

class SunlightDurationRecorder {
public:
	float getSunlightDurationInHours();
    SunlightDurationRecorder(Sensor *sensor);
    ~SunlightDurationRecorder();
private:
    Sensor * sensor = new Sensor(DIGITALPIN, DHTPIN);
    bool initialized_{false};
    std::chrono::system_clock::time_point start_time_;
    std::chrono::duration<int, std::ratio<1, 1>> duration_0_{0};
    std::chrono::system_clock::time_point last_reset_time_;
};

#endif