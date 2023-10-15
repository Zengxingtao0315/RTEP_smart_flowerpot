#include <iostream>

using namespace std;

typedef struct range{
    float max;
    float min;
};


class Plant {
public:
    Plant( range tRg, range hRg, range lrRg);

    double get_temperature(); 
    double get_humidity(); 
    float get_light_ratio();
    void set_temperature(double temp); 

    void set_humidity(double humi) ;
    void set_light_ratio(float ratio);
    range getHumidityRange();
    range getTemperatuereRange();
    range getLightratiorange();

private:
    double curr_temp;
    double curr_humi;
    double curr_lightratio;

    range temperatureRange;
    range humidityRange;
    range LightRatiorange;
};
