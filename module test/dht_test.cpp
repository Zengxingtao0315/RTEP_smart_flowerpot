#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS	85
#define DHTPIN		7
int dht11_dat[5] = { 0, 0, 0, 0, 0 };
 
void read_dht11_dat()
{
	
	uint8_t cnt = 7;
	uint8_t idx = 0;
	float	f; 
 
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( 20 );
	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( 40 );
	pinMode( DHTPIN, INPUT );
	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(DHTPIN) == LOW)
		if (loopCnt-- == 0) return -2;


	loopCnt = 10000;
	while(digitalRead(DHTPIN) == HIGH)
		if (loopCnt-- == 0) return -2;
	
	
	
	
	for ( i = 0; i < 40; i++ )
	{
		loopCnt = 10000;
		while(digitalRead(DHTPIN) == LOW)
			if (loopCnt-- == 0) return -2;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(DHTPIN) == HIGH)
			if (loopCnt-- == 0) return -2;

		if ((micros() - t) > 40) dht11_dat[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}
 
	if  (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) 
	{
		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
	}else  {
		printf( "Data not good, skip\n" );
	}
}
 
int main( void )
{
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
 
	if ( wiringPiSetup() == -1 )
		exit( 1 );
 
	while ( 1 )
	{
		read_dht11_dat();
		delay( 1000 ); 
	}
 
	return(0);
}