#include <Arduino.h>
#include "DHT.h"
#include "SoftwareSerial.h"
#include "Adafruit_Sensor.h"

#define DHT_PIN     2
#define DHT_TYPE    DHT11
#define DHT_TAG     0x73

typedef struct {
    uint8_t tag;
    uint8_t re_tag;
    uint8_t temperature;
    uint8_t humidity;
    uint8_t heatIndex;
    uint16_t checksum;
} DHTData;

DHTData data = { DHT_TAG, DHT_TAG, 0, 0, 0, 0 };

DHT dht(DHT_PIN, DHT_TYPE);
SoftwareSerial interface(10, 11); // RX, TX

void setup() {
    interface.begin(115200);
    dht.begin();
}

void loop() {
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    if (!isnan(data.temperature) && !isnan(data.humidity)) 
        data.heatIndex = dht.computeHeatIndex(data.temperature, data.humidity, false);
    else
    {
        data.temperature = 0;
        data.humidity = 0;
        data.heatIndex = 0;
    }

    data.checksum = 0;

    for (size_t i = 0; i < sizeof(DHTData); i++) 
        data.checksum += ((byte*)&data)[i];

    for (size_t i = 0; i < sizeof(DHTData); i++) 
        interface.write(((byte*)&data)[i]);
    
    delay(250);
}