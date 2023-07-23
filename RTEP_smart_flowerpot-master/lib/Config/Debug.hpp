/*****************************************************************************
* | File      	:	Debug.h
* | Author      :   Xingtao Zeng
* | Function    :	debug with printf
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts

******************************************************************************/
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

#if DEBUG
	#define Debug(info, ...) std::cout << "Debug: " << info << __VA_ARGS__ << std::endl
#else
	#define Debug(info, ...)
#endif

#endif


