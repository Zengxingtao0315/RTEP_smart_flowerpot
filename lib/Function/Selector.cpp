#include "Selector.hpp"



const char* Selector::EmojiSelector(double temperature, double humidity){
	
	using namespace std;
	//When everything is fine
	
	float light_duration = LightRecorder->getSunlightDurationInHours();
	
	UWORD digital = sensorPtr->readDigitalValue();

	if(temperature <= tempRange.max && temperature >= tempRange.min && digital == 0 && humidity >= humiRange.min && humidity <= humiRange.max)
	{
		return "./pic/happy.bmp";		
	}//Too many hours of sunlight when the temperature is too high
	else if(temperature > tempRange.max  && digital == 0 && humidity >= humiRange.min && humidity <= humiRange.max && light_duration > 8.0){
		return "./pic/too_much_light.bmp";
	}//Insufficient sunshine hours when the temperature is too high
	else if(temperature > tempRange.max  && digital == 0 && humidity >= humiRange.min && humidity <= humiRange.max && light_duration <= 8.0){
		return "./pic/too_hot.bmp";
	}//When plants are not exposed to light and have insufficient hours of sunlight
	else if(temperature <= tempRange.max && temperature >= tempRange.min  && digital == 1 && humidity >= humiRange.min && humidity <= humiRange.max && light_duration <= 8.0){
		return "./pic/lack_of_sunlight.bmp";
	}//Any time the temperature is too low
	else if(temperature < tempRange.min  && humidity >= humiRange.min && humidity <= humiRange.max ){
		return "./pic/cold.bmp";
	}//Any time the humidity is too low
	else if(humidity < humiRange.min ){
		return "./pic/need_water.bmp";
	}//Any time the humidity is too high
	else if(humidity > humiRange.max ){
		return "./pic/overwatered.bmp";
	}	
}


const char* Selector::BmpSelector(){
	
	InternetConnectionChecker checker;
	checker.CheckInternetConnection()? bmp_path = "./pic/internet_up.bmp" : bmp_path = "./pic/internet_down.bmp";
	return bmp_path;
	
}


const char* Selector::lightSelector(){
	char* Lstr = "bright";
	light_flag = sensorPtr->readDigitalValue();
	if (light_flag == 0){
		Lstr = "bright";
		std::cout<<"bright"<<std::endl;
	}else{
		Lstr = "dark";
		std::cout<<"dark"<<std::endl;
	}
	return Lstr;
	
	
}

void Selector::setTemperatureRange(range tRg)
{
	tempRange = tRg;
}


void Selector::setHumidityRange(range hRg)
{
	humiRange = hRg;
}



Selector::Selector(Sensor * sensor, range tRg, range hRg): sensorPtr(sensor), tempRange(tRg), humiRange(hRg){	

}
Selector::~Selector(){
	delete sensorPtr;

}