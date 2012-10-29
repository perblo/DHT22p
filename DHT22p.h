// 
//    FILE: dht.h
// VERSION: 0.1.03
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//
//     URL: http://arduino.cc/playground/Main/DHTLib
//
// HISTORY:
// see dht.cpp file
// 

#ifndef dht22p_h
#define dht22p_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>s
#endif



typedef enum
{
  DHT_ERROR_NONE = 0,
  DHT_ERROR_CHECKSUM,
  DHT_ERROR_TIMEOUT,
} DHT22p_ERROR_t;



class DHT22p
{
public:
	DHT22p(uint8_t pin);
	DHT22p_ERROR_t readData();
	int humidity;  // value x 100, two decimal places
	int temperature;  // värdet *100

private:
	uint8_t _pin;
	uint8_t bits[5];  // buffer to receive data
	DHT22p_ERROR_t read(uint8_t pin);
};
#endif
//
// END OF FILE
//