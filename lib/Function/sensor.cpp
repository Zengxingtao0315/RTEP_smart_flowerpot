/*****************************************************************************
* | File      	:   sensor.cpp
* | Author      :   xingtao Zeng
* | Function    :   Create function classes to read sensors for temperature, humidity and light intensity
* | Info        :
******************************************************************************/

#include "sensor.hpp"

using namespace std;
UBYTE dht_data[5];

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
bool Sensor::readDHTdata(float* temperature, float* humidity) {
    
	if ( wiringPiSetup() == -1 )
    exit( 1 );

	UBYTE laststate = HIGH;
    UBYTE counter = 0;
    UBYTE j = 0, i;
    float f;
	
	
    dht_data.data[0] = dht_data.data[1] = dht_data.data[2] = dht_data.data[3] = dht_data.data[4] = 0;
    // pull pin down for 18 milliseconds
	pinMode(dhtPin, OUTPUT);
    digitalWrite(dhtPin, LOW);
    delay(18);
	// then pull it up for 40 microseconds 
    digitalWrite(dhtPin, HIGH);
    delayMicroseconds(40);
	// prepare to read the pin 
    pinMode(dhtPin, INPUT);
	// detect change and read data 
    for (i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while (digitalRead(dhtPin) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(dhtPin);
        if (counter == 255) break;
		// ignore first 3 transitions
        if ((i >= 4) && (i % 2 == 0)) {
		// shove each bit into the storage bytes 
            dht_data.data[j / 8] <<= 1;
            if (counter > 16)
                dht_data.data[j / 8] |= 1;
            j++;
        }
    }
	
	/*
   * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
   * print it out if data is good
   */
   
    if ((j >= 40) && (dht_data.data[4] == ((dht_data.data[0] + dht_data.data[1] + dht_data.data[2] + dht_data.data[3]) & 0xFF))) {
        //f = dht_data.data[2] * 9.0 / 5.0 + 32;
		
		
		*temperature = dht_data.data[0] + (float)dht_data.data[1] / 100.0;
        *humidity = dht_data.data[2] + (float)dht_data.data[3] / 100.0;
		
		std::cout << "Temperature: " << *temperature << std::endl;
        std::cout << "Humidity: " << *humidity << std::endl;
        return 0;  
    }
    else {
        std::cout << "Data not good, skip" << std::endl;
        return -1; 
    }
}

