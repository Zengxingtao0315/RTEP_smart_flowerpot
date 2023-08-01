extern "C" {

	#include <sys/socket.h>
	#include <netdb.h>
	#include <time.h>
}
#include "../lib/Config/DEV_Config.hpp"
#include "../lib/Config/Debug.hpp"
#include "../lib/GUI/GUI_Paint.hpp"
#include "../lib/Function/sensor.hpp"
#include "../lib/Function/Selector.hpp"
#include "../lib/Function/GetTime.hpp"
#include "../lib/OLED/OLED.hpp"
#include "../lib/Function/checkInternet.hpp"

#include <ctime> 
#include <cstdlib> 
#include <cmath>
#include <csignal> 
#include <cstring> 
#include <ctime>
#include <iostream>
#include <boost/asio.hpp>
#include <memory>

#include <wiringPi.h>

#include <iostream> 

#include <string>
#include <thread>


#define OLED_WIDTH 128
#define OLED_HEIGHT 128


using namespace boost::asio;
using ip::tcp;
using namespace std;
extern DEV DEV;
Sensor Sensor(DIGITALPIN,  DHTPIN);





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
    if (DEV.ModuleInit() != 0 || USE_IIC) {
        std::cerr << "Failed to initialize components." << std::endl;
        return -1;
    }
	
	//OLED Init...
    std::cout << "OLED Init..." << std::endl;
	
	wiringPiSetup();
	Paint Paint;
	OLED OLED;
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
	
    Paint.NewImage(BlackImage, OLED_WIDTH, OLED_HEIGHT, BLACK);
    std::cout << "Drawing" << std::endl;
    
	//Select Image
    Paint.SelectImage(BlackImage);

	DEV.Delay_ms(500);

    Paint.Clear(BLACK);


	// init time funciton, get local time.
	Time time;
	PAINT_TIME local_time;
	//init Selector
	Selector selector;
	//init Sensors pin
	
	UWORD  digitalValue;
	//UWORD  analogValue;
	double temp ;
	double hum ;

	
    while (1) {
		std::cout<<"painting the first page!"<<std::endl;
		//display of internet status
		Paint.GUI_ReadBmp(selector.BmpSelector(), 100, 0);
		Paint.DrawTime(5, 0, &local_time, &Font12, BLACK, TIME_COLOR);
		//Humidity and Temperature
		
		temp = Sensor.getTemperature();
        hum = Sensor.getHumidity();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		Paint.DrawString(5, 13, "Humi(%)", &Font12, BLACK, WHITE);
		Paint.DrawNum(60, 13,hum, &Font12, 1,  WHITE, BLACK);
		Paint.DrawString(5, 25, "Temp(C)", &Font12, BLACK, WHITE);
		Paint.DrawNum(60, 25, temp, &Font12, 1,  WHITE, BLACK);		
		Paint.DrawString(5, 37, selector.lightSelector(), &Font12, BLACK, WHITE);
		OLED.Display(BlackImage);
		Paint.Clear(BLACK);	

		//display of the plant emoji
		std::cout<<"painting the emoji page!"<<std::endl;
		Paint.GUI_ReadBmp(selector.EmojiSelector(temp,hum), 32, 64);
		
		OLED.Display(BlackImage);
		DEV.Delay_ms(10000);
		Paint.Clear(BLACK);	
		DEV.Delay_ms(10000);
		
		OLED.Clear();

	}

    return 0;
}
