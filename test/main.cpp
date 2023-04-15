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
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <thread>

#define OLED_WIDTH 128
#define OLED_HEIGHT 128

using namespace std;
//extern DEV DEV;
extern Timer Timer;

//expression plants emotion or status
const char* EmojiSelector(float temperature, float humidity, int digital, float light_duration ){
	//When everything is fine
	if(temperature <= 38.0 && temperature >= 15.0 && digital == 0 && humidity >= 30.0 && humidity <= 50.0)
	{
		return "./pic/happy.bmp";		
	}//Too many hours of sunlight when the temperature is too high
	else if(temperature > 38.0  && digital == 0 && humidity >= 30.0 && humidity <= 50.0 && light_duration > 8.0){
		return "./pic/too_much_light.bmp";
	}//Insufficient sunshine hours when the temperature is too high
	else if(temperature > 38.0  && digital == 0 && humidity >= 30.0 && humidity <= 50.0 && light_duration <= 8.0){
		return "./pic/too_hot.bmp";
	}//When plants are not exposed to light and have insufficient hours of sunlight
	else if(temperature <= 38.0 && temperature >= 15.0  && digital == 1 && humidity >= 30.0 && humidity <= 50.0 && light_duration <= 8.0){
		return "./pic/lack_of_sunlight.bmp";
	}//Any time the temperature is too low
	else if(temperature < 15.0  && humidity >= 30.0 && humidity <= 50.0 ){
		return "./pic/cold.bmp";
	}//Any time the humidity is too low
	else if(humidity < 30.0 ){
		return "./pic/need_water.bmp";
	}//Any time the humidity is too high
	else if(humidity > 50.0 ){
		return "./pic/overwatered.bmp";
	}

	
	
}






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
	//OLED Init...
    std::cout << "OLED Init..." << std::endl;
	
	wiringPiSetup();
	Paint Paint;
	OLED OLED;
 
    OLED.Init();
    //DEV.Delay_ms(50);

	
    // Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = (OLED_WIDTH * 2) * OLED_HEIGHT;
    if ((BlackImage = new UBYTE[Imagesize + 300]) == NULL) {
        std::cout << "Failed to apply for black memory..." << std::endl;
        return -1;
    }
    std::cout << "Paint_NewImage" << std::endl;
	
    Paint.NewImage(BlackImage, OLED_WIDTH, OLED_HEIGHT, 0, BLACK);
    Paint.SetScale(65);
    std::cout << "Drawing" << std::endl;
    
	//Select Image
    Paint.SelectImage(BlackImage);
    DEV.Delay_ms(50);
    Paint.Clear(BLACK);
	
    // initialise the whole display
    //GUI_ReadBmp_65K("./pic/OLED.bmp", 0, 0);
    // Show image on page
    //OLED_Display(BlackImage);
    //DEV_Delay_ms(500);
    //Paint_Clear(BLACK);

	// init time funciton, get local time.
	Time time;
	PAINT_TIME local_time;
	
	//init InternetConnectionChecker
	InternetConnectionChecker checker;
	bool connected; //Determining network connection status
	
	//init Sensors pin
	Sensor Sensor(DIGITALPIN,  DHTPIN);
	UWORD  digitalValue;
	//UWORD  analogValue;
	double temperature, humidity;
	float light_duration;
	SunlightDurationRecorder duration;
	
    while (1) {
        //Get local time
		local_time = time.getLocalTime();
        // display of time

        Paint.DrawTime(10, 0, &local_time, &Font12, BLACK, TIME_COLOR);
		//DEV.Delay_ms(500);
		
        //display of internet status
		connected = checker.CheckInternetConnection();
        connected ? Paint.GUI_ReadBmp_65K("./pic/internet_up.bmp", 100, 0) : Paint.GUI_ReadBmp_65K("./pic/internet_down.bmp", 100, 0);
        DEV.Delay_ms(500);
		
		//display of plant information
		//Read the temperature and humidity of the DHT sensor after approximately one second
		DEV.Delay_ms(1000);
		dhtSTAT dhtFLAG = Sensor.readDHTdata(&temperature, &humidity);
		if(dhtFLAG == TIMEOUT) Debug("DHT11 module timeout");
		if(dhtFLAG == SUCCESS)
		{
			
			Paint.DrawString_EN(10, 20, "Temp(C):", &Font12, BLACK, WHITE);
			Paint.DrawNum(59, 20, temperature, &Font12, 4, BLACK, WHITE);
			Paint.DrawString_EN(10, 32, "Hum(%):", &Font12, BLACK, WHITE);
			Paint.DrawNum(59, 32, humidity, &Font12, 4, BLACK, WHITE);
			DEV.Delay_ms(500);

		}
		
		//Digital reading of the light emitting diode, 1 for almost no light, 0 for light
		digitalValue = Sensor.readDigitalValue();
		if (digitalValue){
			Paint.DrawString_EN(10, 44, "light", &Font12, BLACK, WHITE);
			std::cout<<"light"<<std::endl;
		}else{
			std::cout<<"dark""<<std::endl;
			Paint.DrawString_EN(10, 44, "Dark", &Font12, BLACK, WHITE);
		}
		DEV.Delay_ms(1000);
		//analogValue = Sensor.readAnalogValue();
		//Calculate the duration of the reading at 0, which is also the duration of daylight
		light_duration = duration.getSunlightDurationInHours(digitalValue);
		/**********************************
		if(light_duration < 8.0){
			Paint.DrawString_EN(10, 40, "need more light", &Font12, BLACK, WHITE);
		}
		else{
			Paint.DrawString_EN(10, 40, "light sufficient", &Font12, BLACK, WHITE);
		}
		**********************************/
		
		//display of the plant emoji
		Paint.GUI_ReadBmp_65K(EmojiSelector(temperature, humidity,digitalValue, light_duration), 32, 64);
		DEV.Delay_ms(1000);
		
		OLED.Display(BlackImage);
		DEV.Delay_ms(1000);
		
		OLED.Clear();
	}


    return 0;
}
