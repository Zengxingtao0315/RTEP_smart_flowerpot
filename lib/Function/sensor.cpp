/*****************************************************************************
* | File      	:   sensor.cpp
* | Author      :   xingtao Zeng
* | Function    :   Create function classes to read sensors for temperature, humidity and light intensity
* | Info        :   WENBO YANG modified the function of readDHTdata
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
DHTdata Sensor::readDHTdata() {
    
	DHTdata dht;

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
    delayMicroseconds(18055);
	// then pull it up for 40 microseconds 
    digitalWrite(dhtPin, HIGH);
    delayMicroseconds(40);
	// prepare to read the pin 
    pinMode(dhtPin, INPUT);
	
	
	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while (digitalRead(dhtPin) == LOW && loopCnt > 0) {
        loopCnt--;
    }
    if (loopCnt == 0) {
		std::cout>>"dht read timeout">>std::endl;

        return -2;

		
    }

    loopCnt = 10000;
    while (digitalRead(dhtPin) == HIGH && loopCnt > 0) {
        loopCnt--;
    }
    if (loopCnt == 0) {
		std::cout>>"dht read timeout">>std::endl;

        return -2;

    }
	
	for ( i = 0; i < 40; i++ )
	{
		loopCnt = 10000;
		while(digitalRead(dhtPin) == LOW)
			if (loopCnt-- == 0) {
				std::cout>>"dht read timeout">>std::endl;

				return -2;

			}
		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(dhtPin) == HIGH)
			if (loopCnt-- == 0) {
				std::cout>>"dht read timeout">>std::endl;

				return -2;

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

		DHTdata.humidity = dht_data[0] + dht_data[1] * 0.1;
		DHTdata.temperature = dht_data[2] + dht_data[3] * 0.1;

		std::cout << "Humidity = " << dht_data[0] << "." << dht_data[1] << " % "
          << "Temperature = " << dht_data[2] << "." << dht_data[3] << " C ("
          << f << " F)" << std::endl;
		  return DHTdata;

	}else  {
		printf( "Data not good, skip\n" );
		return 0;
	}

}
