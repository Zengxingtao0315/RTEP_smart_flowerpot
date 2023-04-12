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
    Paint.DrawString_EN(5, 16, "Temperature:", &Font16, WHITE, BLACK);
    Paint.DrawString_EN(5, 32, "Humidity:", &Font16, WHITE, BLACK);
    Paint.DrawString_EN(5, 48, "Light intensity:", &Font16, WHITE, BLACK);
    
	OLED.Display(BlackImage);
    DEV.Delay_ms(500);
	/*******
    while (1) {
        //Get local time
		Time time;
		PAINT_TIME local_time;
		local_time = time.GetLocalTime();
        // display of time

        Paint.DrawTime(0, 0, local_time, &Font12, BLACK, TIME_COLOR);
        OLED.SetWindow_Display(BlackImage, 0, 0, 111, 15);

        //display of internet status
		InternetConnectionChecker checker;
		bool connected = checker.CheckInternetConnection();
        connected ? Paint.GUI_ReadBmp_65K("./pic/internet_up.bmp", 111, 0) : Paint.GUI_ReadBmp_65K("./pic/internet_down.bmp", 111, 0);
        OLED.SetWindow_Display(BlackImage, 111, 0, 127, 15);
        DEV_Delay_ms(500);
		
		//display of plant information
		Sensor Sensor(DIGITALPIN, ANALOGPIN, DHTPIN);
		int digitalValue = sensor.readDigitalValue();
		float analogValue = sensor.readAnalogValue();
		float temperature, humidity;
		int dhtResult = sensor.readDHTdata(temperature, humidity);
		
		if (dhtResult == 0) {
			Paint.DrawNum(69, 16, temperature, &Font16, 4, WHITE, BLACK);
			OLED.SetWindow_Display(BlackImage,69, 16, 127, 31);
			
			Paint.DrawNum(69, 32, humidity, &Font16, 4, WHITE, BLACK);
			OLED.SetWindow_Display(BlackImage,69, 32, 127, 47);
		}
		else {
			cout << "Failed to read DHT data." << endl;
		}
		
	
		Paint.DrawNum(69, 48, analogValue, &Font16, 4, WHITE, BLACK);
		OLED.SetWindow_Display(BlackImage,69, 48, 127, 63);
		
		//display of the plant emoji
		Paint.GUI_ReadBmp_65K("./pic/happy.bmp", 32, 64);
		OLED.SetWindow_Display(BlackImage,32, 64, 96, 127);
		
		
		
		OLED.Clear();
	}
	*****************************************************/

    return 0;
}
