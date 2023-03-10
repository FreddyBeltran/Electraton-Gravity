/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/


// Hola este es mi cambio 

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DHT.h"

const int DHTPIN = 2;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int humidity, temperature;
const int tempSensID = 0;  //

RF24 radio(9, 8);  // CE, CSN

const byte address[6] = "00001";

int data[4];

void setup() {
  dht.begin();
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from sensor!"));
    return;
  }


  Serial.println("-------------- Data sent --------------");

  data[0] = humidity;
  Serial.print("Humedad: ");
  Serial.println(humidity);
  char hum = data[0];

  data[1] = temperature;
    Serial.print("Temperatura: ");
  Serial.println(temperature);
  char temp = data[1];

  data[2] = tempSensID;
  Serial.print("ID: ");
  Serial.println(tempSensID);
  char ID = data[2];

  data[3] = 4;
  Serial.print("Num Random: ");
  Serial.println(4);
  char x = data[3];

  // String s = String(data);

  const char text[] = {hum, temp, ID, x};
  //int newhum = text[0];
  //Serial.println(newhum);
  radio.write(&text, sizeof(text));
  //radio.write(&data, sizeof(data));
  delay(1000);
}