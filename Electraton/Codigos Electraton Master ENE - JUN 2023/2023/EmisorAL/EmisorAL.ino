#include <RH_NRF24.h>
#include "DHT.h"
#include <EEPROM.h>

RH_NRF24 nrf24(8,9); // Definimos la radio

const int DHTPIN = 2;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int deviceID = EEPROM.read(0);
int humidity, temperature;

void setup() {
  Serial.begin(115200);
  dht.begin();

  while(!Serial) // To wait for the serial port to get started (Arduino Leonardo)
  ;
  
  if (!nrf24.init())
  {
    Serial.println("init failed");
  }
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(3))
  {
    Serial.println("setChannel Failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
  { 
    Serial.println("setRF failed");
  }
  Serial.println("Transmitter started");

}

void loop() {

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println(F("Failed to read from sensor!"));
    return;
  }

  Serial.println("Sending to getaway");
  uint8_t data[4];

  data[0] = humidity;
  data[1] = temperature;
  data[2] = deviceID;

  Serial.println("------------- Measurments -------------");
  Serial.print("Humidity: ");
  Serial.print(data[0]);

  Serial.print(", Temperature: ");
  Serial.print(data[1]);

  Serial.print(", ID: ");
  Serial.print(data[2]);
  Serial.println();

  // Send to receptor via nrf24
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();

  // Wait for a reply
  uint8_t buf(RH_NRF24_MAX_MESSAGE_LEN);
  uint8_t len = sizeof(buf);

  if (nrf24.waitAvailableTimeout(1000))
  {
    // We should have a reply now
    if (nrf24.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply.");
  }
  delay(5000);
}







