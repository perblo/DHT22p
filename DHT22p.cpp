//
//    FILE: DHT22p.cpp
// VERSION: 0.1.04
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//
// DATASHEET: 
//
// HISTORY:
// 0.1.04 fork and dht11 support removed
// 0.1.03 added error values for temp and humidity when read failed
// 0.1.02 added error codes
// 0.1.01 added support for Arduino 1.0, fixed typos (31/12/2011)
// 0.1.0 by Rob Tillaart (01/04/2011)
// inspired by DHT11 library
//

#include "DHT22p.h"

#define TIMEOUT 10000

/////////////////////////////////////////////////////
//
// PUBLIC
//

DHT22p::DHT22p(uint8_t pin)
{
    _pin = pin;

}

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
DHT22p_ERROR_t DHT22p::readData()
{
	uint8_t pin =_pin;
	// READ VALUES
	DHT22p_ERROR_t rv = read(pin);
	if (rv != 0) 
    {
		humidity    = -999;    // invalid value, or is NaN prefered?
		temperature = -999;  // invalid value
		return rv;
	}

	// TEST CHECKSUM
	uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
	if (bits[4] != sum) return DHT_ERROR_CHECKSUM;

	// CONVERT AND STORE
	humidity    = int(word(bits[0], bits[1])) * 10;

	int sign = 1;
	if (bits[2] & 0x80) // negative temperature
	{
		bits[2] = bits[2] & 0x7F;
		sign = -1;
	}
	temperature = sign * int(word(bits[2], bits[3])) * 10;

	

	return DHT_ERROR_NONE;
}

/////////////////////////////////////////////////////
//
// PRIVATE
//

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_TIMEOUT
DHT22p_ERROR_t DHT22p::read(uint8_t pin)
{
	// INIT BUFFERVAR TO RECEIVE DATA
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(20);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	// GET ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = TIMEOUT;
	while(digitalRead(pin) == LOW)
		if (loopCnt-- == 0) return DHT_ERROR_TIMEOUT;

	loopCnt = TIMEOUT;
	while(digitalRead(pin) == HIGH)
		if (loopCnt-- == 0) return DHT_ERROR_TIMEOUT;

	// READ THE OUTPUT - 40 BITS => 5 BYTES
	for (int i=0; i<40; i++)
	{
		loopCnt = TIMEOUT;
		while(digitalRead(pin) == LOW)
			if (loopCnt-- == 0) return DHT_ERROR_TIMEOUT;

		unsigned long t = micros();

		loopCnt = TIMEOUT;
		while(digitalRead(pin) == HIGH)
			if (loopCnt-- == 0) return DHT_ERROR_TIMEOUT;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;   
			idx++;      
		}
		else cnt--;
	}

	return DHT_ERROR_NONE;
}
//
// END OF FILE
//