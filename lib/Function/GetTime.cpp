/*****************************************************************************
* | File      	:   GetTime.cpp
* | Author      :   xingtao Zeng
* | Function    :   Get local time
* | Info        :
******************************************************************************/

#include "GetTime.hpp"

/*******************************************************************************
function:
            Get local time
*******************************************************************************/
PAINT_TIME Time::getLocalTime() {
    PAINT_TIME time;
	std::time_t now;
    std::tm *local;

    std::time(&now);
    local = std::localtime(&now);

    // 提取需要的时间信息
    time.Hour = local->tm_hour;
    time.Min = local->tm_min;
	time.Sec = local->tm_sec;
	return time;
}
