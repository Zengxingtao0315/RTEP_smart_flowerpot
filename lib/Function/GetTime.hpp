#ifndef _GETTIME_HPP_
#define _GETTIME_HPP_

#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <ctdint>
#include <iostream>
extern "C" {
	#include <time.h>
};
#include <ctime>


class Time {
public:
    void getLocalTime(PAINT_TIME *time)
};