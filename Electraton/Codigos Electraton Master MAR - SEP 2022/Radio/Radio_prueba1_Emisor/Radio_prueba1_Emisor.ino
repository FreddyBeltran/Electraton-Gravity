#include <SPI.h>
#include <nRF24L01Nano.h> //Lib de la antena adaptada para el procesamiento del NANO
#include <RF24Nano.h> //Lib de la antena adaptada para el procesamiento del NANO

//Declaremos los pines CE y el CSN
#define CE_PIN 9 //Se declara el puerto CE en el pin 9 del NANO
#define CSN_PIN 10 //Se declara el puerto CSN en el pin 10 del NANO
 
//Variable con la dirección del canal por donde se va a transmitir
const uint64_t direccion = 0xF0F0F0F0E1LL; // El valor de la variable define el canal que se va a utilizar

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector con los datos a enviar
float datos[3];

void setup()
{
  //inicializamos el NRF24L01 
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600); 
  //Abrimos un canal de escritura
  radio.openWritingPipe(direccion);
 
}
 
void loop()
{ 
  //cargamos los datos en la variable datos[] ---> Estos son datos ejemplo, aquí utilzaríamos los datos de los sensores
  datos[0]=analogRead(0)* (5.0 / 1023.0);
  datos[1]=millis();
  datos[2]=3.14;
  
  //enviamos los datos
  bool ok = radio.write(datos, sizeof(datos));

  //reportamos por el puerto serial los datos enviados 
  if(ok) // Se declararán como enviados, hasta que los datos sean recibidos
  {
     Serial.print("Datos enviados: "); 
     Serial.print(datos[0]); 
     Serial.print(" , "); 
     Serial.print(datos[1]); 
     Serial.print(" , "); 
     Serial.println(datos[2]); 
  }
  else
  {
     Serial.println("no se ha podido enviar");
  }
  delay(5000);
}