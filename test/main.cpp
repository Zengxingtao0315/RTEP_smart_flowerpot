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


//expression plants emotion or status
char EmojiSelector(float temperature, float humidity, int digitalValue, float light_duration ){
	char emojipath = 0;
	//When everything is fine
	if(temperature <= 38.0 && temperature >= 15.0 && digital == 0 && humidity >= 30.0 && humidity <= 50.0)
	{
		emojipath = "./pic/happy.bmp";		
	}//Too many hours of sunlight when the temperature is too high
	else if(temperature > 38.0  && digital == 0 && humidity >= 30.0 && humidity <= 50.0 && light_duration > 8.0){
		emojipath = "./pic/too_much_light.bmp";
	}//Insufficient sunshine hours when the temperature is too high
	else if(temperature > 38.0  && digital == 0 && humidity >= 30.0 && humidity <= 50.0 && light_duration <= 8.0){
		emojipath = "./pic/too_hot.bmp";
	}//When plants are not exposed to light and have insufficient hours of sunlight
	else if(temperature <= 38.0 && temperature >= 15.0  && digital == 1 && humidity >= 30.0 && humidity <= 50.0 && light_duration <= 8.0){
		emojipath = "./pic/lack_of_sunlight.bmp";
	}//Any time the temperature is too low
	else if(temperature < 15.0  && humidity >= 30.0 && humidity <= 50.0 ){
		emojipath = "./pic/cold.bmp";
	}//Any time the humidity is too low
	else if(humidity < 30.0 ){
		emojipath = "./pic/need_water.bmp";
	}//Any time the humidity is too high
	else if(humidity > 50.0 ){
		emojipath = "./pic/overwatered.bmp";
	}

	
	return emojipath;
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
    DEV.Delay_ms(500);
	
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
    DEV.Delay_ms(500);
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
	Sensor Sensor(DIGITALPIN, ANALOGPIN, DHTPIN);
	UWORD  digitalValue;
	//UWORD  analogValue;
	float temperature, humidity;
	float light_duration;
	SunlightDurationRecorder duration;
	string LightHours = "hours of light still needed ";
	char Emojipath = "";
	char Temp = "Temp:";
	char Hum = "Temp:";
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
		//Read the temperature and humidity of the DHT sensor after approximately one second
		DEV.Delay_ms(1000);
		dhtFLAG = Sensor.readDHTdata(&temperature, &humidity);
		if(dhtFLAG)
		{
			Temp = Temp + to_string(temperature);
			Paint.DrawString_EN(10, 16, &Temp, &Font12, BLACK, WHITE);
			Hum = Hum  + to_string(humidity);
			Paint.DrawString_EN(10, 28, &Hum, &Font12, BLACK, WHITE);
			DEV.Delay_ms(50);

		}
		
		//Digital reading of the light emitting diode, 1 for almost no light, 0 for light
		digitalValue = Sensor.readDigitalValue();
		//analogValue = Sensor.readAnalogValue();
		//Calculate the duration of the reading at 0, which is also the duration of daylight
		light_duration = duration.getSunlightDurationInHours(digitalValue);
		if(LightHours < 8.0){
			LightHours = to_string(8.0-light_duration) + "hours of light still needed " ;
		}
		else{
			LightHours = "not need more light";
		}
		
		Paint.DrawString_EN(10, 40, LightHours, &Font12, BLACK, WHITE);
		DEV.Delay_ms(50);
		//display of the plant emoji
		Emojipath = EmojiSelector(temperature, humidity,digitalValue, light_duration);
		Paint.GUI_ReadBmp_65K(&Emojipath, 32, 64);
		DEV.Delay_ms(50);
		
		OLED.Display(BlackImage);
		DEV.Delay_ms(50);
		
		OLED.Clear();
	}


    return 0;
}
