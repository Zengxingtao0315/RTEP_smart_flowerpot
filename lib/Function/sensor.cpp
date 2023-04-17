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
			DHTdata data = readDHTdata();

			// 确保线程安全
			std::unique_lock<std::mutex> lock(dataMutex);


			temperature = data.temperature;
			humidity = data.humidity;
		
		// 通知等待在条件变量上的线程，有新的数据可用
        dataCondVar.notify_all();

        // 等待一段时间再进行下一次读取
        dataCondVar.wait_for(lock, std::chrono::milliseconds(2000));
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

Sensor::Sensor(int digitalPin, int dhtPin) {
        this->digitalPin = DIGITALPIN;
        //this->analogPin = ANALOGPIN;
		this->dhtPin = DHTPIN;
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
	float	f;
	if ( wiringPiSetup() == -1 )
    exit( 1 );
	
	for(i=0;i<5;i++)
        dht_data[i]=0;
	
    // pull pin down for 20 milliseconds
	pinMode(dhtPin, OUTPUT);
    digitalWrite(dhtPin, LOW);
    delayMicroseconds(19055);
	// then pull it up for 40 microseconds 
    digitalWrite(dhtPin, HIGH);
    delayMicroseconds(40);
	// prepare to read the pin 
    pinMode(dhtPin, INPUT);
	pullUpDnControl(dhtPin, PUD_UP);

	
	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while (digitalRead(dhtPin) == LOW && loopCnt > 0) {
        loopCnt--;
    }
    if (loopCnt == 0) {
		std::cout<<"dht read timeout"<<std::endl;

        

		
    }

    loopCnt = 10000;
    while (digitalRead(dhtPin) == HIGH && loopCnt > 0) {
        loopCnt--;
    }
    if (loopCnt == 0) {
		std::cout<<"dht read timeout"<<std::endl;

        

    }
	
	for ( i = 0; i < 40; i++ )
	{
		loopCnt = 10000;
		while(digitalRead(dhtPin) == LOW)
			if (loopCnt-- == 0) {
				std::cout<<"dht read timeout"<<std::endl;

			}
		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(dhtPin) == HIGH)
			if (loopCnt-- == 0) {
				std::cout<<"dht read timeout"<<std::endl;

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
