// 
//   FILE:  DHT22p_test.pde
// PURPOSE: DHT library test sketch for Arduino
//

#include <DHT22p.h>

DHT22p dht;


#define DHT22_PIN 7

void setup()
{
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop()
{
  // READ DATA
  Serial.print("DHT22, \t");
  int chk = dht.readData(DHT22_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  Serial.print(dht.humidity, 1);
  Serial.print(",\t");
  Serial.println(dht.temperature, 1);

  delay(1000);

  
}
//
// END OF FILE
//