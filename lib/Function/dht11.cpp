/*****************************************************************************
* | File      	:   dht11.c
* | Author      :   xingtao Zeng
* | Function    :   Read humidity value and temperature value
* | Date        :   2023-04-05
* | Info        :	Use DHT11 sensor to get humidity and temperature
******************************************************************************/ 
#include "test.h"
#include "dht11.h"
 
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
	pinMode(DHT_PIN, OUTPUT);
    digitalWrite(DHT_PIN, LOW);
    delay(18);
	// then pull it up for 40 microseconds 
    digitalWrite(DHT_PIN, HIGH);
    delayMicroseconds(40);
	// prepare to read the pin 
    pinMode(DHTPIN, INPUT);
	// detect change and read data 
    for (i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while (digitalRead(DHTPIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(DHTPIN);
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

