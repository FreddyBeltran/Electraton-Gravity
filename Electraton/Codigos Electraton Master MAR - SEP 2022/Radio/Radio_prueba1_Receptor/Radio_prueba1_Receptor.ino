#include <SPI.h>
#include <nRF24L01.h> //Libreria para las antenas 
#include <RF24.h> //Libreria para las antenas



//Variable con la dirección del canal por donde se va a transmitir
const uint64_t direccion = 0xF0F0F0F0E1LL; // El valor de la variable define el canal que se va a utilizar

//creamos el objeto radio (NRF24L01)
RF24 radio(4, 5); // Puerto CE en pin 4, puerto CSN en pin 5;

//vector con los datos a recibir
float datos[3];

void setup()
{
  //inicializamos el puerto serie
  Serial.begin(9600); 
  
  //inicializamos el NRF24L01 
  radio.begin();
 
  //Abrimos el canal de Lectura
  radio.openReadingPipe(0, direccion);

  //Definimos la distancia de transmisión del PA, aquí solo se declara MAX o MIN dependiendo de la distancia (RF24_PA_MIN -> Mínimo) (RF24_PA_MAX -> Máximo)
  radio.setPALevel(RF24_PA_MIN);
  
  //empezamos a escuchar por el canal
  radio.startListening();
 
}
 
void loop()
{ 

 if ( radio.available() )
 {    
    //Leemos los datos y los guardamos en la variable datos[]
    radio.read(datos, sizeof(datos));
    
    //reportamos por el puerto serial los datos recibidos
    Serial.print("Dato0= " );
    Serial.print(datos[0]);
    Serial.print(" V, ");
    Serial.print("Dato1= " );
    Serial.print(datos[1]);
    Serial.print(" ms, ");
    Serial.print("Dato2= " );
    Serial.println(datos[2]);
 }
 else
 {
    Serial.println("No hay datos de radio disponibles");
 }
 delay(1000);
}