#ifndef _PLANT_HPP_
#define _PLANT_HPP_

#include <iostream>

using namespace std;

typedef struct range{
    float max;
    float min;
};


class Plant {
public:
    Plant( range tRg, range hRg);

    double get_temperature(); 
    double get_humidity(); 
    void set_temperature(double temp); 

    void set_humidity(double humi) ;
    range getHumidityRange();
    range getTemperatuereRange();

private:
    double curr_temp;
    double curr_humi;

    range temperatureRange;
    range humidityRange;
};
#endif