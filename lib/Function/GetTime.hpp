#ifndef _GETTIME_HPP_
#define _GETTIME_HPP_

#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
extern "C" {
	#include <time.h>
};
#include <ctime>


class Time {
public:
    PAINT_TIME getLocalTime(PAINT_TIME time);
};