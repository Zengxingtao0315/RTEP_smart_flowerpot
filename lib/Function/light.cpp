/*****************************************************************************
* | File      	:   light.c
* | Author      :   xingtao Zeng
* | Function    :   Drive light module 
* | Info        :
******************************************************************************/

#include "light.h"
#include "test.h"

class Sensor {
private:
    int digitalPin;
    int analogPin;
	int dhtPin;

public:
    SensorPin(int digitalPin, int analogPin, int dhtPin) {
        this->digitalPin = digitalPin;
        this->analogPin = analogPin;
		this->dhtPin = dhtpin;
    }
	
	// get light digital value
    int readDigitalValue() {
        int value = digitalRead(digitalPin);
        return value;
    }
	
	// get light analogue value
    int readAnalogValue() {
        int value = analogRead(analogPin);
        return value;
    }
	
	// get dht temperature and humidity
	int read_dht_data(DHTData *dht_data) {
    
	if ( wiringPiSetup() == -1 )
    exit( 1 );

	UBYTE laststate = HIGH;
    UBYTE counter = 0;
    UBYTE j = 0, i;
    float f;
	
    dht_data.humidity_int = dht_data.humidity_dec = dht_data->temperature_int = dht_data.temperature_dec = 0;
	
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
        f = dht_data.data[2] * 9.0 / 5.0 + 32;
        dht_data.humidity_int = dht_data.data[0];
        dht_data.humidity_dec = dht_data.data[1];
        dht_data.temperature_int = dht_data.data[2] / 10;
        dht_data.temperature_dec = dht_data.data[2] % 10;
        return 0;  
    }
    else {
        std::cout << "Data not good, skip" << std::endl;
        return -1; 
    }
}
};
