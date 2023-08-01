#include "Selector.hpp"



const char* Selector::EmojiSelector(double temperature, double humidity, int digital, float light_duration ){
	using namespace std;
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


char* Selector::BmpSelector(){
	
	InternetConnectionChecker checker;
	checker.CheckInternetConnection()? bmp_path = "./pic/internet_up.bmp" : bmp_path = "./pic/internet_down.bmp";
	return bmp_path;
	
}


std::string Selector::lightSelector(){
	std::string Lstr = "bright";
	Sensor Sensor(DIGITALPIN,  DHTPIN);
	light_flag = Sensor.readDigitalValue();
	if (light_flag == 0){
		Lstr = "bright";
		std::cout<<"bright"<<std::endl;
	}else{
		Lstr = "dark";
		std::cout<<"dark"<<std::endl;
	}
	return Lstr;
	
	
}