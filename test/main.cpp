extern "C" {

	#include <sys/socket.h>
	#include <netdb.h>
	#include <time.h>
}
#include "DEV_Config.hpp"
#include "Debug.hpp"
#include "GUI_Paint.hpp"
#include "sensor.hpp"
#include "GetTime.hpp"
#include "OLED.hpp"
#include "checkInternet.hpp"

#include <ctime> 
#include <cstdlib> 
#include <cmath>
#include <csignal> 
#include <cstring> 
#include <ctime>


#include <wiringPi.h>

#include <iostream> // C++标准头文件，其中包含cout和endl的定义。

#define OLED_WIDTH 128
#define OLED_HEIGHT 128

using namespace std;
extern DEV DEV;

void Handler(int signo)
{
	// System Exit
	std::cout << "Handler: exit" << std::endl;
	DEV.ModuleExit();

	std::exit(0);
}

int main()
{
	std::signal(SIGINT, Handler);
    
	std::cout << "OLED showing" << std::endl;
    if (DEV.ModuleInit() != 0) {
        return -1;
    }

    if (0) {
        std::cout << "Only USE_SPI, Please revise DEV_Config.h !!!" << std::endl;
        return -1;
    }

    std::cout << "OLED Init..." << std::endl;
	//
	OLED OLED;
	wiringPiSetup();
	
    OLED.Init();
    DEV.Delay_ms(500);
    // Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = (OLED_WIDTH * 2) * OLED_HEIGHT;
    if ((BlackImage = new UBYTE[Imagesize + 300]) == NULL) {
        std::cout << "Failed to apply for black memory..." << std::endl;
        return -1;
    }
    std::cout << "Paint_NewImage" << std::endl;
	Paint Paint;
    Paint.NewImage(BlackImage, OLED_WIDTH, OLED_HEIGHT, 0, BLACK);
    Paint.SetScale(65);
    std::cout << "Drawing" << std::endl;
    //
    Paint.SelectImage(BlackImage);
    DEV.Delay_ms(500);
    Paint.Clear(BLACK);
    // initialise the whole display
    //GUI_ReadBmp_65K("./pic/OLED.bmp", 0, 0);
    // Show image on page
    //OLED_Display(BlackImage);
    //DEV_Delay_ms(500);
    //Paint_Clear(BLACK);
    Paint.DrawString_EN(10, 16, "Temp:", &Font12, BLACK, WHITE);
    Paint.DrawString_EN(10, 28, "Hum:", &Font12, BLACK, WHITE);
    Paint.DrawString_EN(10, 40, "Lux:", &Font16, BLACK, WHITE);
    

	
	Time time;
	PAINT_TIME local_time;
	InternetConnectionChecker checker;
	bool connected;
	Sensor Sensor(DIGITALPIN, ANALOGPIN, DHTPIN);
	UWORD  digitalValue;
	UWORD  analogValue;
	float temperature, humidity;
	Duration light_duration;
    while (1) {
        //Get local time
		local_time = time.getLocalTime();
        // display of time

        Paint.DrawTime(10, 0, &local_time, &Font12, BLACK, TIME_COLOR);
		DEV.Delay_ms(50);
		
        //display of internet status
		connected = checker.CheckInternetConnection();
        connected ? Paint.GUI_ReadBmp_65K("./pic/internet_up.bmp", 65, 0) : Paint.GUI_ReadBmp_65K("./pic/internet_down.bmp", 65, 0);
        DEV.Delay_ms(50);
		
		//display of plant information
		
		digitalValue = Sensor.readDigitalValue();
		analogValue = Sensor.readAnalogValue();
		if(digitalValue != 0){
			if(light_duration.hour < 8){
				calculateDaylightDuration(int digitalValue, Duration *light_duration);
				std::cout << "Need more light" << std::endl;
				
			}
			
		}
		DEV.Delay_ms(1000);
		if(Sensor.readDHTdata(&temperature, &humidity))
		{
			
			Paint.DrawNum(49, 16, temperature, &Font12, 4, WHITE, BLACK);
			DEV.Delay_ms(50);
			Paint.DrawNum(49, 28, humidity, &Font12, 4, WHITE, BLACK);

		}
		
	
		Paint.DrawNum(49, 40, analogValue, &Font12, 4, WHITE, BLACK);

		DEV.Delay_ms(50);
		//display of the plant emoji
		Paint.GUI_ReadBmp_65K("./pic/happy.bmp", 32, 64);

		DEV.Delay_ms(50);
		
		OLED.Display(BlackImage);
		DEV.Delay_ms(50);
		
		OLED.Clear();
	}


    return 0;
}
