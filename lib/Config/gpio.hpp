/*****************************************************************************

This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/tree/master/RaspberryPi/user_dev/c/lib/Config
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
******************************************************************************/
#ifndef __SYSFS_GPIO_HPP_
#define __SYSFS_GPIO_HPP_

#include <cstdio>
#include "Debug.hpp"
#define GPIO_IN  0
#define GPIO_OUT 1

#define GPIO_LOW  0
#define GPIO_HIGH 1

#define NUM_MAXBUF  4
#define DIR_MAXSIZ  60


class GPIO{
public:
	int GPIO_Export(int Pin);
	int GPIO_Direction(int Pin, int Dir);
	int GPIO_Write(int Pin, int value);
};
#endif