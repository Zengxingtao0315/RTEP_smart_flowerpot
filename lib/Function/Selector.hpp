#ifndef _SELECTOR_HPP_
#define _SELECTOR_HPP_

#include <iostream>
#include <cstring>
#include <string>
#include "checkInternet.hpp"
#include "sensor.hpp"
#include "GetTime.hpp"
#include "../Config/DEV_Config.hpp"

class Selector {
public:
    const char* EmojiSelector(double temperature, double humidity);
    const char* BmpSelector();
	const char* lightSelector();
	
private:
	char* bmp_path = "./pic/internet_down.bmp";
	bool light_flag = 0;
};


#endif