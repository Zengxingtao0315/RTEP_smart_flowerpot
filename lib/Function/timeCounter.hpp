#ifndef _TIMECOUNTER_HPP_
#define _TIMECOUNTER_HPP_

#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
extern "C" {
	#include <time.h>
};
#include <ctime>
#include "../GUI/GUI_Paint.hpp"


struct {
    int sec = 0;
    int min = 0;
    int hour = 0;
}Duration;

#endif