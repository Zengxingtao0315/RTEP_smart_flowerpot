/*****************************************************************************
* | File      	:   sensor.cpp
* | Author      :   xingtao Zeng
* | Function    :   Create function classes to read sensors for temperature, humidity and light intensity
* | Info        :
******************************************************************************/

#include "sensor.hpp"

using namespace std;


int waitForPinStatus(int status, int timeout)
{
    int t = 0;
    while (digitalRead(DHTPIN) != status) {
        if (t >= timeout) {
            return -1;
        }
        delayMicroseconds(1);
        t++;
    }
    return 0;
}



Sensor::Sensor(int digitalPin, int dhtPin) {
        this->digitalPin = DIGITALPIN;
        //this->analogPin = ANALOGPIN;
		this->dhtPin = DHTPIN;
    }
	
	// get light digital value
UWORD Sensor::readDigitalValue() {
        UWORD value = digitalRead(digitalPin);
        return value;
    }
	
	// get light analogue value
	/*************************
UWORD Sensor::readAnalogValue() {
        UWORD value = analogRead(analogPin);
        return value;
    }
	*************************/
	// get dht temperature and humidity
bool Sensor::readDHTdata(double* temperature, double* humidity) {
    
	UBYTE dht_data[5];
	UBYTE crc = 0;
	if ( wiringPiSetup() == -1 )
    exit( 1 );

	UBYTE laststate = HIGH;
    UBYTE counter = 0;
    UBYTE j = 0, i;

	
	for(i=0;i<5;i++)
        dht_data[i]=0;
	
    // pull pin down for 20 milliseconds
	pinMode(dhtPin, OUTPUT);
    digitalWrite(dhtPin, LOW);
    delay(20);
	// then pull it up for 40 microseconds 
    digitalWrite(dhtPin, HIGH);
    delayMicroseconds(30);
	// prepare to read the pin 
    pinMode(dhtPin, INPUT);
	pullUpDnControl(dhtPin, PUD_UP);
	
	if (waitForPinStatus(LOW, 5000) < 0) {
        return -1;
    }
	
	// detect change and read data 
    for (int i = 0; i < 40; i++) {
        if (waitForPinStatus(HIGH, 100) < 0) {
            return -2;
        }

        delayMicroseconds(30);

        if (digitalRead(dhtPin)) {
            dht_data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }
	
	/*
   * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
   * print it out if data is good
   */
	crc = dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3];
    if (crc != dht_data[4]) {
        std::cout << "Data not good, skip" << std::endl;
		return -3;
    }
    else {
        //f = dht_data.data[2] * 9.0 / 5.0 + 32;
		
		*temperature = dht_data[0] + (double)dht_data[1] *0.1;
        *humidity = dht_data[2] + (double)dht_data[3] *0.1;
		
		std::cout << "Temperature: " << *temperature << std::endl;
        std::cout << "Humidity: " << *humidity << std::endl;
        return 0;  
    }

}

