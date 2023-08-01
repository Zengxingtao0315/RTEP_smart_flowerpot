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


class Time {
public:
    PAINT_TIME getLocalTime();
};

#endif