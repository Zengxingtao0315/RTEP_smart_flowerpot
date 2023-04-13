#include timeCounter.hpp

using namespace std;
Duration calculateDaylightDuration(int digitalValue, Duration *light_duration) {
    
	Time time;
	PAINT_TIME start_time;
	PAINT_TIME end_time;
	start_time = time.getLocalTime();
	Duration duration ;
	

	Duration light_duration;
	//6:10:20
	if (start_time.Hour >= 6){
		if (digitalValue == 1) {
			// 累计秒数
			if(*light_duration.sec >= 60){
				*light_duration.sec = 0;
				*light_duration.min += 1;
			}
			*light_duration.sec += duration.sec;
			// 累计分钟数
			if(*light_duration.min >= 60){
				*light_duration.min = 0;
				*light_duration.hour += 1;
			}
			*light_duration.min += duration.min;
			// 累计小时数（已经过去的小时数+当前小时的光照时间）
			if(*light_duration.hour >= 24){
				*light_duration.hour = 0;
			}
			*light_duration.hour += duration.hour ;
		}	
		else{
			
		}

	}
	//6:10:23
	end_time = time.getLocalTime();
	duration.hour = start_time.Hour - end_time.Hour;
	duration.min = start_time.Min - end_time.Min;
	duration.sec = start_time.Sec - end_time.Sec;
    // 返回光照时长
    return light_duration;
}
