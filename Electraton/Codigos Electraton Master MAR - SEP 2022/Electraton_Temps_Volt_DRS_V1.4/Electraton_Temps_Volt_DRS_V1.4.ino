//Essential libraries for OLED and Servomotor
//Se remueve funcionalidad de drs
#include <U8g2lib.h>
#include <Adafruit_MLX90614.h>  //Melexis
#include "patros.h"

//Declaracion de MLX
Adafruit_MLX90614 termoDI = Adafruit_MLX90614();  // DI = Delantero Izquierdo
Adafruit_MLX90614 termoDD = Adafruit_MLX90614();  // DD = Delantero Derecho
Adafruit_MLX90614 termoTI = Adafruit_MLX90614();  // TI = Trasero Izquierdo
Adafruit_MLX90614 termoTD = Adafruit_MLX90614();  // TD = Trasero Derecho

//i2c address space
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

//Variables to use
int b_DRS = 9;                                                  //Button DRS
int s_Brake = 10;                                                 //Braking pedal
int pin_DRS = 6;                                                //PWM drs
bool state_DRS = false;                                         //State of DRS active or not
int B4State, B3State, B2State, B1State = 0;                     //Button states
double B1Value, B2Value, B3Value, B4Value = 0;                  //Batterry Values 1-4
double BATVAL = 0;                                              //Total Battery Value
float temp1 = 0, temp2 = 58.51, temp3 = 365.1, temp4 = 12.521;  //Test Temparature values
int bShowMode = 12;                                             //Boton de showmode
bool showState = false;                                         //Show State mode
int firstpress = 0;                                             //Count of first press
int newtime = 0;                                                //Time for the comparation of show mode
float tempDI = 0.0, tempDD = 0.0, tempTI = 0.0, tempTD = 0.0;   //Brakes Temperatures



void setup() {
  Serial.begin(57600);
  pinMode(b_DRS, INPUT);
  pinMode(s_Brake, INPUT);
  pinMode(pin_DRS, OUTPUT);
  pinMode(bShowMode, INPUT);
  termoDI.begin(0x5A);  //Melexi Delantero Izquierdo 0x5A
  termoDD.begin(0x5B);  //Melexi Delantero Derecho 0x5B
  termoTI.begin(0x5C);  //Melexi Trasero Izquierdo 0x5C
  termoTD.begin(0x5D);  //Melexi Trasero Derecho 0x5D
  u8g2.setBusClock(100000);
  u8g2.begin();
  delay(200);
}

void loop() {
  if (digitalRead(bShowMode) == 0) {
    firstpress = millis();
    while (digitalRead(bShowMode) == 0) {
      newtime = millis();
    }
    if (newtime - firstpress >= 3000) {
      showMode();
    }
  }
  else{
    //VB
    VB();
    //Temperatura
    temp();
    //    //DRS functionality
    /*if (digitalRead(b_DRS) == LOW && state_DRS == false) {
      while (digitalRead(b_DRS) == LOW) {
        //        DRS_ON();
      }

    } else if ((digitalRead(b_DRS) == LOW || digitalRead(s_Brake) == HIGH) && state_DRS == true) {
      while (digitalRead(b_DRS) == LOW || digitalRead(s_Brake) == HIGH) {
      //  DRS_OFF();
      }
    }*/
    infodisplay();
  }
}

void VB() {
  B1Value = analogRead(A0);
  B2Value = analogRead(A1);
  B3Value = analogRead(A2);
  B4Value = analogRead(A3);
  //BATVAL = ((B1Value + B2Value + B3Value + B4Value) / 4) / 1001.2 * 100;
  BATVAL = ((B2Value + B4Value) / 2) / 1001.2 * 100;
  B1State = map(B1Value, 0, 1002, 0, 5);
  B2State = map(B2Value, 0, 1002, 0, 5);
  B3State = map(B3Value, 0, 1002, 0, 5);
  B4State = map(B4Value, 0, 1002, 0, 5);
  delay(10);
}

void temp() {
  tempDI = termoDI.readObjectTempC();  //0x5A

  tempDD = termoDD.readObjectTempC();  //0x5B
  
  tempTI = termoTI.readObjectTempC();  //0x5C
  
  tempTD = termoTD.readObjectTempC();  //0x5D
  
}

/*void DRS_ON() {
  //delay(100);
  state_DRS = true;
  analogWrite(pin_DRS, 255); //checar si es con servo o piston
  VB();
  temp();
  infodisplay();
}

void DRS_OFF() {
  //delay(100);
  state_DRS = false;
  analogWrite(pin_DRS, 255);
  VB();
  temp();
  infodisplay();
}*/
//checar los valores de las baterias/fresnos para ver que es temp 1 - 4

void infodisplay() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_crox2cb_tn);
    //u8g2.setCursor(5, 26);
    u8g2.setCursor(50, 30);
    u8g2.print(temp1, 1);
    /*u8g2.setCursor(5, 43);
    u8g2.print(temp2, 1);
    u8g2.setCursor(70, 26);
    u8g2.print(temp3, 1);
    u8g2.setCursor(70, 43);
    u8g2.print(temp4, 1);*/

    /*if (state_DRS == true) {
      u8g2.setFont(u8g2_font_squeezed_b7_tr);
      u8g2.drawFrame(108, -1, 30, 12);
      u8g2.drawStr(111, 9, "DRS");
    }*/
    u8g2.setFont(u8g2_font_crox1hb_tf);
    u8g2.drawHLine(0, 51, 128);
    u8g2.setCursor(1, 63);
    u8g2.print(BATVAL, 1);
    u8g2.drawStr(27, 63, "%");
    u8g2.setFont(u8g2_font_battery19_tn);
    u8g2.setFontDirection(1);
    /*u8g2.setCursor(40, 54);
    u8g2.print(B1State);*/
    u8g2.setCursor(62, 54);
    u8g2.print(B2State);
    /*u8g2.setCursor(84, 54);
    u8g2.print(B3State);*/
    u8g2.setCursor(106, 54);
    u8g2.print(B4State);
    u8g2.setFontDirection(0);
  } while (u8g2.nextPage());
}

void showMode() {  //animation of the sponsors logos

  u8g2.firstPage();
  do {
    u8g2.drawXBMP(2, 0, troquemsa_width, troquemsa_height, troquemsa_bits);
  } while (u8g2.nextPage());
  delay(3000);
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(12, 0, moldem_width, moldem_height, moldem_bits);
  } while (u8g2.nextPage());
  delay(3000);
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP( 0, 0, richie_width, richie_height, richie_bits);
  } while (u8g2.nextPage());
  delay(3000);

  u8g2.firstPage();
  do {
    u8g2.drawXBMP(0, 0, vemo_width, vemo_height, vemo_bits);
  } while (u8g2.nextPage());
  delay(3000);
  //
}
