#include <RH_NRF24.h>
#include <EEPROM.h>

RH_NRF24 nrf24(2, 4);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Receiver Started, ID: ");
  nrf24.init();
  nrf24.setChannel(3);
  nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm);
}

void loop() 
{
  if (nrf24.available())
  {
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
      //Send a reply
      uint8_t sdata[] = "Data Received.";
      nrf24.send(sdata, sizeof(sdata));
      nrf24.waitPacketSent();

      int humidity = buf[0];
      int temperature = buf[1];
      int deviceID = buf[2];

      Serial.println("--- Data Received ---");
      
      Serial.print("Device ID: ");
      Serial.print(deviceID);
      Serial.print(", Temperature: ");
      Serial.print(temperature);
      Serial.print(", Humidity: ");
      Serial.print(humidity);

      delay(1000);
    }
  }
  else 
  {

  }
}