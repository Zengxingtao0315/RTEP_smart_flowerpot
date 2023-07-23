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
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			// ensure thread safe
			std::unique_lock<std::mutex> lock(dataMutex);
			temperature = data.temperature;
			humidity = data.humidity;


		
		// Notify threads waiting on condition variables that new data is available
        dataCondVar.notify_all();

        // Wait a while before doing the next read
        
		}
	}
	
double Sensor::getTemperature() {
    std::unique_lock<std::mutex> lock(dataMutex);
    // Wait for new data to arrive, or time out
    dataCondVar.wait_for(lock, std::chrono::milliseconds(200), [this] {
        return temperature != lastTemperature;
    });
    // Update the latest temperature values
    lastTemperature = temperature;
    return temperature;
}

double Sensor::getHumidity() {
    std::unique_lock<std::mutex> lock(dataMutex);
    //  Wait for new data to arrive, or time out
    dataCondVar.wait_for(lock, std::chrono::milliseconds(200), [this] {
        return humidity != lastHumidity;
    });
    // update latest temperature
    lastHumidity = humidity;
    return humidity;
}

Sensor::Sensor(int digitalPin, int dhtPin)
    std::digitalPin(digitalPin), dhtPin(dhtPin), lastTemperature(0.0), lastHumidity(0.0) {
    if (wiringPiSetup() == -1) {
        std::cerr << "Error initializing wiringPi" << std::endl;
        exit(1);
    }
    dhtThread = std::thread(&Sensor::readDHTdataLoop, this);
}
Sensor::~Sensor(){
        
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

    DHTdata data;

	int dht11_dat [5] = { 0, 0, 0, 0, 0 };
	uint8_t cnt = 7;
	uint8_t idx = 0;
	float	f; 
 int i ;
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( 19 );
	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( 40 );
	pinMode( DHTPIN, INPUT );
	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(DHTPIN) == LOW)
		if (loopCnt-- == 0) ;


	loopCnt = 10000;
	while(digitalRead(DHTPIN) == HIGH)
		if (loopCnt-- == 0) ;
	
	
	
	
	for ( i = 0; i < 40; i++ )
	{
		loopCnt = 10000;
		while(digitalRead(DHTPIN) == LOW)
			if (loopCnt-- == 0) ;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(DHTPIN) == HIGH)
			if (loopCnt-- == 0) ;

		if ((micros() - t) > 40) dht11_dat[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}
	
	if  (dht11_dat[4] == (  (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) 
	{
		data.humidity = dht11_dat[0];
		data.temperature = dht11_dat[2] + dht11_dat[3] * 0.1;
	
		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
		return data;
	}else  {
		printf( "Data not good, skip\n" );
		
	}

}
