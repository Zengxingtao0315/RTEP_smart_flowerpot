/*****************************************************************************
* | File      	:   GetTime.cpp
* | Author      :   xingtao Zeng
* | Function    :   Get local time
* | Info        :
******************************************************************************/
extern "C" {
	#include "test.h"
};
#include "GetTime.h"


/*******************************************************************************
function:
            Get local time
*******************************************************************************/
void Time::GetLocalTime(PAINT_TIME *time) {
    time_t now;
    struct tm *local;

    time(&now);
    local = localtime(&now);

    // 提取需要的时间信息
    time->Hour = local->tm_hour;
    time->Min = local->tm_min;
	time->Sec = local->tm_sec;
}
