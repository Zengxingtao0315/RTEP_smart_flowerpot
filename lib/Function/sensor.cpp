/*****************************************************************************
* | File      	:   sensor.cpp
* | Author      :   xingtao Zeng
* | Function    :   Create function classes to read sensors for temperature, humidity and light intensity
* | Info        :   WENBO YANG modified the function of readDHTdata
******************************************************************************/

#include "sensor.hpp"

using namespace std;
void Sensor::readDHTdataLoop() {
		while (true) {
			

			// 确保线程安全
			std::unique_lock<std::mutex> lock(dataMutex);
			DHTdata data = readDHTdata();

			temperature = data.temperature;
			humidity = data.humidity;
		
		// 通知等待在条件变量上的线程，有新的数据可用
        dataCondVar.notify_all();

        // 等待一段时间再进行下一次读取
        std::this_thread::sleep_for(2s);
		}
	}
	
double Sensor::getTemperature() {
    std::unique_lock<std::mutex> lock(dataMutex);
    // 等待新数据的到来，或者超时
    dataCondVar.wait_for(lock, std::chrono::milliseconds(2000), [this] {
        return temperature != lastTemperature;
    });
    // 更新最新的温度值
    lastTemperature = temperature;
    return temperature;
}

double Sensor::getHumidity() {
    std::unique_lock<std::mutex> lock(dataMutex);
    // 等待新数据的到来，或者超时
    dataCondVar.wait_for(lock, std::chrono::milliseconds(2000), [this] {
        return humidity != lastHumidity;
    });
    // 更新最新的湿度值
    lastHumidity = humidity;
    return humidity;
}

Sensor::Sensor(int digitalPin, int dhtPin)
    : digitalPin(digitalPin), dhtPin(dhtPin), lastTemperature(0.0), lastHumidity(0.0) {
    if (wiringPiSetup() == -1) {
        std::cerr << "Error initializing wiringPi" << std::endl;
        exit(1);
    }
    dhtThread = std::thread(&Sensor::readDHTdataLoop, this);
}
Sensor::~Sensor(){
        // 等待线程结束
        if (dhtThread.joinable()) {
            dhtThread.join();
        }
    }

	// get light digital value
UWORD Sensor::readDigitalValue() {
        UWORD value = digitalRead(digitalPin);
        return value;
    }
	
DHTdata Sensor::readDHTdata() {
    
	double temperature ;
    double humidity ;


	UBYTE dht_data[5];
	UBYTE cnt = 0;
	UBYTE idx = 0;
	int i ;
	
	for(i=0;i<5;i++)
        dht_data[i]=0;
	
    // pull pin down for 20 milliseconds
	pinMode(dhtPin, OUTPUT);
    digitalWrite(dhtPin, LOW);
	std::this_thread::sleep_for(23ms);
	// then pull it up for 40 microseconds 
    digitalWrite(dhtPin, HIGH);
    std::this_thread::sleep_for(std::chrono::microseconds(41));
	// prepare to read the pin 
    pinMode(dhtPin, INPUT);
	pullUpDnControl(dhtPin, PUD_UP);

	
	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while (digitalRead(dhtPin) == LOW && loopCnt > 0) {
        loopCnt--;
    }
    if (loopCnt == 0) {
		std::cout<<"1dht read timeout"<<std::endl;

        

		
    }

    loopCnt = 10000;
    while (digitalRead(dhtPin) == HIGH && loopCnt > 0) {
        loopCnt--;
    }
    if (loopCnt == 0) {
		std::cout<<"2dht read timeout"<<std::endl;

        

    }
	
	for ( i = 0; i < 40; i++ )
	{
		loopCnt = 10000;
		while (digitalRead(dhtPin) == LOW && loopCnt > 0) {
			loopCnt--;
		}
		if (loopCnt == 0) {
			std::cout<<"3dht read timeout"<<std::endl;
		}
		unsigned long t = micros();

		loopCnt = 10000;
		while (digitalRead(dhtPin) == HIGH && loopCnt > 0) {
			loopCnt--;
		}
		if (loopCnt == 0) {
			std::cout<<"3dht read timeout"<<std::endl;
		}
		if ((micros() - t) > 40) dht_data[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}
 
	if  (dht_data[4] == ( (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF) ) 
	{
		f = dht_data[2] * 9. / 5. + 32;

		humidity = dht_data[0] + dht_data[1] * 0.1;
		temperature = dht_data[2] + dht_data[3] * 0.1;

		return DHTdata{temperature, humidity};
	}else  {
		printf( "Data not good, skip\n" );
		
	}

}
