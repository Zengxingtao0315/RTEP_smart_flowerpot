/*****************************************************************************
* | File      	:   OLED_show.cpp
* | Author      :   xingtao Zeng
* | Function    :   Display of plant status information, humidity, temperature, light intensity
* | Info        :
******************************************************************************/
#include "test.h"
#include "OLED.h"
#include <iostream> // C++标准头文件，其中包含cout和endl的定义。

/*******************************************************************************
function:
            Check internet connection
*******************************************************************************/
class InternetConnectionChecker {
public:
    bool CheckInternetConnection() {
        struct addrinfo hints;
        struct addrinfo *res;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        int status = getaddrinfo("www.google.com", NULL, &hints, &res);
        if(status != 0) {
            return false;
        }
        freeaddrinfo(res);
        return true;
    }
};

int OLED_show()
{
    std::cout << "OLED showing" << std::endl;
    if (DEV_ModuleInit() != 0) {
        return -1;
    }

    if (USE_IIC) {
        std::cout << "Only USE_SPI, Please revise DEV_Config.h !!!" << std::endl;
        return -1;
    }

    std::cout << "OLED Init..." << std::endl;
	//
	OLED OLED;
	wiringPiSetup();
	
    OLED.Init();
    DEV_Delay_ms(500);
    // Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = (OLED_WIDTH * 2) * OLED_HEIGHT;
    if ((BlackImage = new UBYTE[Imagesize + 300]) == NULL) {
        std::cout << "Failed to apply for black memory..." << std::endl;
        return -1;
    }
    std::cout << "Paint_NewImage" << std::endl;
    Paint_NewImage(BlackImage, OLED_WIDTH, OLED_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    std::cout << "Drawing" << std::endl;
    //
    Paint_SelectImage(BlackImage);
    DEV_Delay_ms(500);
    Paint_Clear(BLACK);
    // initialise the whole display
    //GUI_ReadBmp_65K("./pic/OLED.bmp", 0, 0);
    // Show image on page
    //OLED_Display(BlackImage);
    //DEV_Delay_ms(500);
    //Paint_Clear(BLACK);
    Paint_DrawString_EN(5, 16, "Temperature:", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(5, 32, "Humidity:", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(5, 48, "Light intensity:", &Font16, WHITE, BLACK);
    
	OLED.Display(BlackImage);
    DEV_Delay_ms(500);
    while (1) {
        //Get local time
		Time time;
		PAINT_TIME local_time;
		time.GetLocalTime(&local_time);
        // display of time

        Paint_DrawTime(0, 0, local_time, &Font12, BLACK, TIME_COLOR);
        OLED.SetWindow_Display(BlackImage, 0, 0, 111, 15);

        //display of internet status
		InternetConnectionChecker checker;
		bool connected = checker.CheckInternetConnection();
        connected ? GUI_ReadBmp_65K("./pic/internet_up.bmp", 0, 0) : GUI_ReadBmp_65K("./pic/internet_down.bmp", 0, 0);
        OLED.SetWindow_Display(BlackImage, 111, 0, 127, 15);
        DEV_Delay_ms(500);
		
		//display of plant information
		Sensor SensorPin(DIGITALPIN, ANALOGPIN, DHTPIN);
		int digitalValue = sensor.readDigitalValue();
		float analogValue = sensor.readAnalogValue();
		float temperature, humidity;
		int dhtResult = sensor.readDHTdata(temperature, humidity);
		
		if (dhtResult == 0) {
			Paint_DrawNum(69, 16, temperature, &Font16, 4, WHITE, BLACK);
			OLED.SetWindow_Display(BlackImage,69, 16, 127, 31);
			
			Paint_DrawNum(69, 32, humidity, &Font16, 4, WHITE, BLACK);
			OLED.SetWindow_Display(BlackImage,69, 32, 127, 47);
		}
		else {
			cout << "Failed to read DHT data." << endl;
		}
		
	
		Paint_DrawNum(69, 48, analogValue, &Font16, 4, WHITE, BLACK);
		OLED.SetWindow_Display(BlackImage,69, 48, 127, 63);
		
		//display of the plant emoji
			
		
		
		
		OLED.Clear();
		
		
	}