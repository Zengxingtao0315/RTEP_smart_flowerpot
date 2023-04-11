#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <ctdint>
#include <iostream>
extern "C" {
	#include "test.h"
};
#include <ctime>
#include <time.h>

class Time {
public:
    void getLocalTime(PAINT_TIME *time)
};