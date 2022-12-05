#include <U8g2lib.h>
#include <FreqCount.h>
#include "carro.h"

//Characteristics of OLED
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

//Variables to use
float diameter = 0.24;    //Wheel diameter in meters
float ms = 0;             //Velocity m/s
float velocidad = 0;      //Velocity kmh
int b_OT = 9;             //OT button pin
float initialTime_OT = 0;      //OT button state
float timer_OT = 0;       //OT on timer
bool state_OT = false;    //OT state
int availability_OT = 0;  //OT availability  //implementar la disponibilidad de bateria para no drenar las celdas
float countdown = 0;      //OT time remaining
int LM3914 = 0;           //PWM value for LEDs
int leds = 6;             //LEDs pin
int bShowMode = 12;       //Button showmode
int brake = 10;           //Brake input
bool showState = false;   //Show State mode
int firstpress = 0;       //Count of first press
int newtime = 0;          //New time
//
int radio = 56;
int altura = 92;
int centrox = 64;
float angulo = 0;
int posx = 0;
int posy = 0;
float angesq = 0;
int orig1x = 0, orig1y = 0, orig2x = 0, orig2y = 0;
//

void setup() {
  Serial.begin(57600);
  pinMode(bShowMode, INPUT);
  pinMode(brake, INPUT);
  pinMode(b_OT, INPUT);
  pinMode(leds, OUTPUT);  //wait for the OLED to power up
  u8g2.begin();
  FreqCount.begin(1000);
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
  else {
    if (FreqCount.available()) {
      velocity();
    }
    //Sección botones
    if (digitalRead(brake)) {
      if (state_OT == true) {
        digitalWrite(leds, 0); //checar que valor prende los leds 0/1 O NO VA
        state_OT = false;
      }
    } else if (digitalRead(b_OT) == 0 && state_OT == false) {
      while (digitalRead(b_OT) == 0) {
      }  //No empieza hasta que se libera y no se alterna
      state_OT = true;
      initialTime_OT = millis();
      func_OT();
    } else if (digitalRead(b_OT) == 0 && state_OT == true) {
      while (digitalRead(b_OT) == 0) {
      }  //No empieza hasta que se libera y no se alterna
      digitalWrite(leds, 0); // checar que valor prende los leds 0/1 O NO VA
      state_OT = false;
    }
  infodisplay();}
}

void velocity() { 
  unsigned long count = (FreqCount.read());
  ms = (PI * diameter * count);
  velocidad = ms * 3.6;
  angulo = ((velocidad / 60) * 1.047 - 1.047);
  angesq = (angulo / 1.047) * PI / 2;
  posx = (radio - 4) * sin(angulo) + 64;
  posy = -(radio - 4) * cos(angulo) + 92;
  orig1x = 3 * cos(angesq - PI) + 64;
  orig1y = 3 * sin(angesq - PI) + 60;
  orig2x = 3 * cos(angesq) + 64;
  orig2y = 3 * sin(angesq) + 60;
}

void func_OT() {
  countdown = ((30000 - timer_OT) / 1000);
  LM3914 = ((countdown + 5) * 3.1);
  analogWrite(leds, LM3914);
  timer_OT = (millis()) - (initialTime_OT);
  if (timer_OT >= 30000) {
    state_OT = false;
    analogWrite(leds, 0);
  }
}

void infodisplay() {  //sensors information display
  u8g2.firstPage();
  do {
    //u8g2.setFont(u8g2_font_luRS24_tr);
    u8g2.setFont(u8g2_font_logisoso32_tn);
    u8g2.setCursor(4, 34);
    u8g2.print(velocidad, 2);
    u8g2.drawCircle(centrox, altura, radio, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
    u8g2.drawCircle(centrox, 92, 54, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
    //u8g2.drawHLine(60,63,4);
    u8g2.drawLine(orig1x, orig1y, posx, posy);
    u8g2.drawLine(orig2x, orig2y, posx, posy);
    //u8g2.drawTriangle(,,3*cos(angesq)+64,3*sin(angesq)+60,posx,posy);
    u8g2.drawDisc(64, 64, 7, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);

    if (state_OT == true) {
      u8g2.setFont(u8g2_font_t0_11_me);
      u8g2.setCursor(4, 50);
      u8g2.print("OT:");
      u8g2.setCursor(4, 60);
      u8g2.print(countdown, 0);
    }
  } while (u8g2.nextPage());
}

void showMode() {  //Animation of the teams LOGO
  for (int pos = -70; pos < 160; pos++) {
    u8g2.firstPage();
    u8g2.setBitmapMode(1);
    do {
      if (pos == 21) {
        u8g2.drawXBMP(22, 0, shield_width, shield_height, shield_bits);
        u8g2.drawXBMP(pos, 0, carStop_width, carStop_height, carStop_bits);
      } else {
        u8g2.drawXBMP(22, 0, shield_width, shield_height, shield_bits);
        if (pos % 2) {
          u8g2.drawXBMP(pos, 0, car1_width, car1_height, car1_bits);
        } else {
          u8g2.drawXBMP(pos, 0, car2_width, car2_height, car2_bits);
        }
      }
    } 
    while (u8g2.nextPage());
    if (pos == 21) {
      delay(3000);
    }
  }
}
