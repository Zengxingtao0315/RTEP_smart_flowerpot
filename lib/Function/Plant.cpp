#include "Plant.hpp"
#include <iostream>
using namespace std;



    Plant::Plant(struct range tRg,struct range hRg, range lrRg)
        : temperatureRange(tRg), humidityRange(hRg), LightRatiorange(lrRg), curr_temp(0.0), curr_humi(0.0), curr_lightratio(0.0) {}
    
    
    double Plant::get_temperature() {
        return curr_temp;
    }

    double Plant:: get_humidity() {
        return curr_humi;
    }

    float Plant:: get_light_ratio() {
        return curr_lightratio;
    }

    void Plant:: set_temperature(double temp) {
        curr_temp = temp;
    }

    void Plant:: set_humidity(double humi) {
        curr_humi = humi;
    }

    void Plant:: set_light_ratio(float ratio) {
        curr_lightratio = ratio;
    }

    range Plant:: getHumidityRange()
    {
        return humidityRange;
    }

    range Plant::getTemperatuereRange(){
        return temperatureRange;
    }
    
    
    range Plant::getLightratiorange(){
        return LightRatiorange;
    }
    
 
    
/***
int main()
{
    double humi = 4.0;
    Plant xianrenzhang(0.3,2.0,0.3);
    xianrenzhang.show();
    xianrenzhang.set_humidity(humi);
    double humi_1 = xianrenzhang.get_humidity();
    cout << humi_1 << endl;
    return 0;

}
******/