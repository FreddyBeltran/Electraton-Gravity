#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 2); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[4];
    radio.read(&text, sizeof(text));
    int hum = text[0];
    int temp = text[1];
    int ID = text[2];
    int rx = text[3];
    Serial.print("hum: ");
    Serial.println(hum);
    Serial.print("temp: ");
    Serial.println(temp);
    Serial.print("ID: ");
    Serial.println(ID);
    Serial.print("random: ");
    Serial.println(rx);
    Serial.println("");
  }
   
}
