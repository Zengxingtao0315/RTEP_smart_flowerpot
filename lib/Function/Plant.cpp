#include "Plant.hpp"
#include <iostream>
using namespace std;



    Plant::Plant(struct range tRg,struct range hRg)
        : temperatureRange(tRg), humidityRange(hRg), curr_temp(0.0), curr_humi(0.0) {}
    
    
    double Plant::get_temperature() {
        return curr_temp;
    }

    double Plant:: get_humidity() {
        return curr_humi;
    }


    void Plant:: set_temperature(double temp) {
        curr_temp = temp;
    }

    void Plant:: set_humidity(double humi) {
        curr_humi = humi;
    }


    range Plant:: getHumidityRange()
    {
        return humidityRange;
    }

    range Plant::getTemperatuereRange(){
        return temperatureRange;
    }
    
    
    
 
    
