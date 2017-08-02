/*
 * ChipELearning
 * Programme pour Teensy 3.2
 * Martineau Sébastien
 * Instagram : @sebmar89
 * 10/07/2017   V1.0
 */
// Standard
#include <SPI.h>
//#include <Wire.h>

//I2C
#include <I2CTrame.h>
I2CTrame I2C_trame = I2CTrame();

//maestro 
#include <PololuMaestro.h>
#include <SoftwareSerial.h>
SoftwareSerial maestroSerial(9, 10);
MicroMaestro maestro(maestroSerial);

//acc/gyro
#include <LSM303.h>
LSM303 compass;

//touch
#include <CapacitiveSensor.h>
CapacitiveSensor TouchSensor = CapacitiveSensor(A2,A3);   // 1M   ,  1K

// EEPROM
#include <EEPROM.h>
unsigned int addr = 0;

// Display I2C
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 3
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  16 


// Remote control
#include <IRremote.h>
#define FLECHEH 0xFDA05F
#define FLECHEB 0xFDB04F
#define FLECHEG 0xFD10EF
#define FLECHED 0xFD50AF
#define TCH1 0xFD08F7
#define TCH2 0xFD8877
#define TCH3 0xFD48B7
#define TCH4 0xFD28D7
#define TCH5 0xFDA857
#define TCH6 0xFD6897
#define TCH7 0xFD18E7
#define TCH8 0xFD9867
#define TCH9 0xFD58A7
#define TCH0 0xFD30CF
#define VOLP 0xFD40BF
#define VOLM 0xFD00FF
#define PAUSE 0xFD807F
#define SETUP 0xFD20DF
#define MODE 0xFD609F
#define ENTER 0xFD906F
#define RETOUR 0xFD708F
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;


// Analog
int ServoM1 = A0,ServoM2 = A1,ServoM3 = A6,ServoM4 = A7,ServoM5= A8,ServoM6 = A9;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

//led
#define led 13


//variable
#define _TPS 2000  // delay before screensaver
int tps =0;
int Vitesse = 500;
int INITSM0 = 6176 ,INITSM1 = 5864,INITSM2 = 6176,INITSM3 = 5864,INITSM4 = 5720,INITSM5 = 5480;
bool State_Pause = false;
bool state_remote = false;
bool state_touch = false;
bool Phase_init = false;
byte inc_nbr_position = 0;
int val_int;  
byte val_byte[11][7];
byte remote_touch;
byte pos_byte =0;
int pos_int = 0;
int data_acc_y =0;
byte V_Nominale = 15;

void setup()
{
//servomotor
  maestroSerial.begin(115200);
// led
  pinMode(led, OUTPUT);
  digitalWrite(led,HIGH);
// I2C
  I2C_trame.begin(); 
//analog  
  analogReference(DEFAULT) ;
// Remote control
  irrecv.enableIRIn(); // Start the receiver
// touch sensor
  TouchSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
  delay(200);
//acc/gyro
  compass.init();
  compass.enableDefault();
//Display 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display(); // show splashscreen
  delay(2000);

// init
  display.drawSinus();
  maestro.setTarget(0, INITSM0); //6000->1500     (/4)
  maestro.setTarget(1, INITSM1);
  maestro.setTarget(2, INITSM2);
  maestro.setTarget(3, INITSM3);
  maestro.setTarget(4, INITSM4);
  maestro.setTarget(5, INITSM5);
  display.drawSinus();
  display.clearDisplay();   // clears the screen and buffer
  tps = millis();

  maestro.setSpeed(0, V_Nominale); maestro.setSpeed(1, V_Nominale); maestro.setSpeed(3, V_Nominale); maestro.setSpeed(4, V_Nominale); maestro.setSpeed(5, V_Nominale); maestro.setSpeed(6, V_Nominale); 
}

void loop() {
  
  if(millis() > (tps + _TPS)) {display.drawSinus();}
            
  if (irrecv.decode(&results)) {
    if(results.value != 0xFFFFFFFF){
            display.clearDisplay();   // clears the screen and buffer
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(10,8);
            display.print(results.value, HEX);
            display.display(); // show 
      
            switch(results.value){
              case FLECHEH : // avance
                  State_Pause = false;
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2+1000);       maestro.setTarget(3, INITSM3+1000); maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2+1000);       maestro.setTarget(3, INITSM3+1000); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2+1000);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3-1000); maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                 
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2+1000);       maestro.setTarget(3, INITSM3+1000); maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2+1000);       maestro.setTarget(3, INITSM3+1000); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2+1000);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3-1000); maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                  
                  tps = millis();
              break;
              case FLECHEB : // recule
                  State_Pause = false;
                  
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1000);       maestro.setTarget(3, INITSM3-1000); maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1000);       maestro.setTarget(3, INITSM3-1000); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1000);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3+1000); maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                 
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1000);       maestro.setTarget(3, INITSM3-1000); maestro.setTarget(4, INITSM4-800); maestro.setTarget(5, INITSM5-800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1000);       maestro.setTarget(3, INITSM3-1000); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1000);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//lever
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3+1000); maestro.setTarget(4, INITSM4+800); maestro.setTarget(5, INITSM5+800);     delay(Vitesse);//tourner
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);  maestro.setTarget(3, INITSM3); maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);//baisser
                  
                  tps = millis();
              break;
              case FLECHEG : // tourne gauche
                  State_Pause = false;
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4+800);      maestro.setTarget(5, INITSM5+800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1200);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4+800);      maestro.setTarget(5, INITSM5+800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1200);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2-1200);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800);      maestro.setTarget(5, INITSM5-800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800);      maestro.setTarget(5, INITSM5-800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  tps = millis();
              break;
              case FLECHED : // tourne droite
                  State_Pause = false;
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800);      maestro.setTarget(5, INITSM5-800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3+1200);      maestro.setTarget(4, INITSM4-800);      maestro.setTarget(5, INITSM5-800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3+1200);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3+1200);      maestro.setTarget(4, INITSM4+800);      maestro.setTarget(5, INITSM5+800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4+800);      maestro.setTarget(5, INITSM5+800);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  tps = millis();
              break;
              case MODE : // balancing
                  State_Pause = false;
                  state_remote = false;
                  state_touch = false;
                  Phase_init = false;
                  compass.read();
                  data_acc_y = compass.a.y;
                  display.clearDisplay();   // clears the screen and buffer
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(10,8);
                  display.print("Balancing");
                  display.setCursor(10,16);
                  display.print(data_acc_y);
                  display.display(); // show 
                  maestro.setSpeed(5, 5); 
                  maestro.setSpeed(4, 10);
                  maestro.setTarget(0, INITSM0);      maestro.setTarget(1, INITSM1);      maestro.setTarget(2, INITSM2);      maestro.setTarget(3, INITSM3);  maestro.setTarget(4, INITSM4-2000);  maestro.setTarget(5, INITSM5-4000);     delay(1000);
                  maestro.setTarget(0, INITSM0);      maestro.setTarget(1, INITSM1);      maestro.setTarget(2, INITSM2);      maestro.setTarget(3, INITSM3);  maestro.setTarget(4, INITSM4);  maestro.setTarget(5, INITSM5-3000);     delay(200);
                  
                  maestro.setSpeed(5, 4); 
                  do{
                     if (irrecv.decode(&results)) {
                        if(results.value != 0xFFFFFFFF &&  results.value == RETOUR ){
                            state_remote = true;
                        }
                     }
                     irrecv.resume(); // Receive the next value
                     if((TouchSensor.capacitiveSensor(50)/10) > 200){
                            state_touch = true;
                     }
                     compass.read();
                     data_acc_y = compass.a.y;
                     display.clearDisplay();   // clears the screen and buffer
                     display.setTextSize(1);
                     display.setTextColor(WHITE);
                     display.setCursor(10,8);
                     display.print("Balancing");
                     display.setCursor(10,16);
                     display.print( data_acc_y);
                     display.display(); // show 
                     
                        maestro.setTarget(5, INITSM5 + ((data_acc_y)/4) - 3000); 
                        maestro.setTarget(4, INITSM4 - ((data_acc_y)/8)); 
                        maestro.setTarget(1, INITSM1 - ((data_acc_y)/8)); 
                        maestro.setTarget(0, INITSM0 - ((data_acc_y)/8)); 
                     
                     delay(200);
                  }while(!state_remote && !state_touch);
                  maestro.setSpeed(0, V_Nominale); maestro.setSpeed(1, V_Nominale); maestro.setSpeed(3, V_Nominale); maestro.setSpeed(4, V_Nominale); maestro.setSpeed(5, V_Nominale); maestro.setSpeed(6, V_Nominale); 
                  maestro.setTarget(0, INITSM0);      maestro.setTarget(1, INITSM1);      maestro.setTarget(2, INITSM2);      maestro.setTarget(3, INITSM3);maestro.setTarget(4, INITSM4);maestro.setTarget(5, INITSM5);     delay(Vitesse);
                  tps = millis();
              break;
              case TCH0 :
                  State_Pause = false;
                  display.clearDisplay();   // clears the screen and buffer
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(10,8);
                  display.print("Position init");
                  display.display(); // show 
                  maestro.setTarget(0, INITSM0);      maestro.setTarget(1, INITSM1);      maestro.setTarget(2, INITSM2);      maestro.setTarget(3, INITSM3);maestro.setTarget(4, INITSM4);maestro.setTarget(5, INITSM5);     delay(Vitesse);
                  tps = millis();
              break;
              case TCH1 : // mvt eeprom 1
                  State_Pause = false;
                  maestro.setTarget(0, INITSM0);      maestro.setTarget(1, INITSM1);      maestro.setTarget(2, INITSM2);      maestro.setTarget(3, INITSM3);  maestro.setTarget(4, INITSM4);  maestro.setTarget(5, INITSM5);     delay(Vitesse);
                  maestro.setTarget(0, INITSM0-1000); maestro.setTarget(1, INITSM1-1000); maestro.setTarget(2, INITSM2-1000); maestro.setTarget(3, INITSM3);  maestro.setTarget(4, INITSM4);  maestro.setTarget(5, INITSM5);     delay(Vitesse);
                  maestro.setTarget(0, INITSM0);      maestro.setTarget(1, INITSM1);      maestro.setTarget(2, INITSM2);      maestro.setTarget(3, INITSM3);  maestro.setTarget(4, INITSM4);  maestro.setTarget(5, INITSM5);     delay(Vitesse);
                  maestro.setTarget(0, INITSM0-1000); maestro.setTarget(1, INITSM1-1000); maestro.setTarget(2, INITSM2-1000); maestro.setTarget(3, INITSM3);  maestro.setTarget(4, INITSM4);  maestro.setTarget(5, INITSM5);     delay(Vitesse);
                  maestro.setTarget(0, INITSM0);      maestro.setTarget(1, INITSM1);      maestro.setTarget(2, INITSM2);      maestro.setTarget(3, INITSM3);  maestro.setTarget(4, INITSM4);  maestro.setTarget(5, INITSM5);     delay(Vitesse);
                  tps = millis();
              break;
              case TCH2 : // mvt eeprom 2
                  State_Pause = false;
                  maestro.setTarget(0, INITSM0+1000);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0-1000);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0+1000);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0-1000);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0+1000);  maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  maestro.setTarget(0, INITSM0);       maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);       maestro.setTarget(3, INITSM3);      maestro.setTarget(4, INITSM4-800);      maestro.setTarget(5, INITSM5);          delay(Vitesse);
                  
                  tps = millis();
              break;
              case TCH3 : // mvt eeprom 3
                  State_Pause = false;
                  tps = millis();
              break;
              case TCH4 : // mvt ram 4
                  State_Pause = false;
                   for(int j=1; j<11 ; j++){
                         if((EEPROM.read((4*10*7)+(j*7)))==0xFF){
                            for(int k=1; k<7 ; k++){
                                  if(k==1 || k==2){
                                    pos_byte = EEPROM.read((4*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                               
                                  }else{
                                    pos_byte = EEPROM.read((4*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                               
                                  }
                            }
                            j=11;  
                            break; 
                         }else{
                             for(int k=1; k<7 ; k++){ 
                                  if(k==1 || k==2){
                                    pos_byte = EEPROM.read((4*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                               
                                  }else{
                                    pos_byte = EEPROM.read((4*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                               
                                  }                             
                             }
                         }
                       delay(Vitesse);
                   }
                  tps = millis();
              break;
              case TCH5 : // mvt ram 5
                  State_Pause = false;
                   for(int j=1; j<11 ; j++){
                         if((EEPROM.read((5*10*7)+(j*7)))==0xFF){
                            for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                    pos_byte = EEPROM.read((5*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                           
                                  }else{
                                    pos_byte = EEPROM.read((5*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                           
                                  }              
                               }
                            j=11;  
                            break; 
                         }else{
                             for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                    pos_byte = EEPROM.read((5*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                           
                                  }else{
                                    pos_byte = EEPROM.read((5*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                           
                                  }                              
                             }
                         }
                       delay(Vitesse);
                   }
                  tps = millis();
              break;
              case TCH6 : // mvt ram 6
                  State_Pause = false;
                   for(int j=1; j<11 ; j++){
                       if((EEPROM.read((6*10*7)+(j*7)))==0xFF){
                            for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                    pos_byte = EEPROM.read((6*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                           
                                  }else{
                                   pos_byte = EEPROM.read((6*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                             
                                  }                           
                               }
                            j=11;  
                            break; 
                         }else{
                             for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                   pos_byte = EEPROM.read((6*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                
                                  }else{
                                   pos_byte = EEPROM.read((6*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                             
                                  }
                                                             
                             }
                         }
                       delay(Vitesse);
                   }
                  tps = millis();
              break;
              case TCH7 : // mvt ram 7
                  State_Pause = false;
                   for(int j=1; j<11 ; j++){
                       if((EEPROM.read((7*10*7)+(j*7)))==0xFF){
                            for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                   pos_byte = EEPROM.read((7*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                 
                                  }else{
                                   pos_byte = EEPROM.read((7*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                                
                                  }                            
                               }
                            j=11;  
                            break; 
                         }else{
                             for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                  pos_byte = EEPROM.read((7*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                    
                                  }else{
                                   pos_byte = EEPROM.read((7*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                                 
                                  }                      
                             }
                         }
                       delay(Vitesse);
                   }
                  tps = millis();
              break;
              case TCH8 : // mvt ram 8
                  State_Pause = false;
                   for(int j=1; j<11 ; j++){
                       if((EEPROM.read((8*10*7)+(j*7)))==0xFF){
                            for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                  pos_byte = EEPROM.read((8*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                     
                                  }else{
                                   pos_byte = EEPROM.read((8*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                                  
                                  }                               
                               }
                            j=11;  
                            break; 
                         }else{
                             for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                  pos_byte = EEPROM.read((8*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                
                                  }else{
                                   pos_byte = EEPROM.read((8*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                                 
                                  }                            
                             }
                         }
                       delay(Vitesse);
                   }
                  tps = millis();
              break;
              case TCH9 : // mvt ram 9
                  State_Pause = false;
                   for(int j=1; j<11 ; j++){
                       if((EEPROM.read((9*10*7)+(j*7)))==0xFF){
                            for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                  pos_byte = EEPROM.read((9*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                                     
                                  }else{
                                   pos_byte = EEPROM.read((9*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                                  
                                  }                         
                               }
                            j=11;  
                            break; 
                         }else{
                             for(int k=1; k<7 ; k++){
                               if(k==1 || k==2){
                                  pos_byte = EEPROM.read((9*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 4000, 8000);   maestro.setTarget((k-1), pos_int);                        
                                  }else{
                                   pos_byte = EEPROM.read((9*10*7)+(j*7)+k);  pos_int = map(pos_byte, 0, 180, 2000, 10000);   maestro.setTarget((k-1), pos_int);                                                   
                                  }                          
                             }
                         }
                       delay(Vitesse);
                   }
                  tps = millis();
              break;
              case VOLP : // speed +
                  Vitesse -= 50;
                  if(Vitesse<200) {Vitesse = 200;}
                  display.clearDisplay();   // clears the screen and buffer
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(10,0);
                  display.print("Delay : ");
                  display.setCursor(10,8);
                  display.print(Vitesse);
                  display.display(); // show 
                  delay(200);
                  tps = millis();
              break;
              case VOLM : // speed -
                  Vitesse += 50;
                  if(Vitesse>1000) {Vitesse = 1000;}
                  display.clearDisplay();   // clears the screen and buffer
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(10,0);
                  display.print("Delay : ");
                  display.setCursor(10,8);
                  display.print(Vitesse);
                  display.display(); // show 
                  delay(200);
                  tps = millis();
              break;
              case PAUSE :  // detorque servomoteur - position init
                  if(State_Pause == false){ maestro.setTarget(0, 0);maestro.setTarget(1, 0); maestro.setTarget(2, 0);maestro.setTarget(3, 0);maestro.setTarget(4, 0);maestro.setTarget(5, 0); State_Pause = true; }
                  else {
                    pos_int = analogRead(ServoM1); pos_int = map(pos_int, 140, 740, 4000, 8000); maestro.setTarget(0, pos_int);
                    pos_int = analogRead(ServoM2); pos_int = map(pos_int, 140, 740, 4000, 8000); maestro.setTarget(1, pos_int); 
                    pos_int = analogRead(ServoM3); pos_int = map(pos_int, 140, 740, 2000, 10000); maestro.setTarget(2, pos_int);
                    pos_int = analogRead(ServoM4); pos_int = map(pos_int, 140, 740, 2000, 10000); maestro.setTarget(3, pos_int);
                    pos_int = analogRead(ServoM5); pos_int = map(pos_int, 140, 740, 2000, 10000); maestro.setTarget(4, pos_int);
                    pos_int = analogRead(ServoM6); pos_int = map(pos_int, 140, 740, 2000, 10000); maestro.setTarget(5, pos_int);
                    State_Pause = false;}
                  tps = millis();
              break;
              case SETUP :  // config mvt
              
                  display.clearDisplay();   // clears the screen and buffer
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(10,8);
                  display.print("Setup mvt");
                  display.setCursor(10,16);
                  display.print("start position");
                  display.setCursor(10,24);
                  display.print("touch button");
                  display.display(); // show 
                  state_remote = false;
                  state_touch = false;
                  inc_nbr_position = 0;
                  maestro.setTarget(0, 0);maestro.setTarget(1, 0); maestro.setTarget(2, 0);maestro.setTarget(3, 0);maestro.setTarget(4, 0);maestro.setTarget(5, 0);
                  do{
                     if (irrecv.decode(&results)) {
                        if( results.value != 0xFFFFFFFF && ( results.value == ENTER || results.value == RETOUR || inc_nbr_position>10 )){
                            state_remote = true;
                        }
                     }
                    if((TouchSensor.capacitiveSensor(50)/10) > 200){
                            state_touch = true;
                    }
                    if(state_touch){
                        state_touch = false;
                        inc_nbr_position ++;  
                        //save position 
                        val_byte[inc_nbr_position][0] = 0x00;
                        val_int = analogRead(ServoM1);  val_byte[inc_nbr_position][1] = map(val_int, 140, 740, 0, 180); // EEPROM.write((inc_nbr_position*6)+1, val_byte);
                        val_int = analogRead(ServoM2);  val_byte[inc_nbr_position][2] = map(val_int, 140, 740, 0, 180); 
                        val_int = analogRead(ServoM3);  val_byte[inc_nbr_position][3] = map(val_int, 140, 740, 0, 180); 
                        val_int = analogRead(ServoM4);  val_byte[inc_nbr_position][4] = map(val_int, 140, 740, 0, 180); 
                        val_int = analogRead(ServoM5);  val_byte[inc_nbr_position][5] = map(val_int, 140, 740, 0, 180); 
                        val_int = analogRead(ServoM6);  val_byte[inc_nbr_position][6] = map(val_int, 140, 740, 0, 180); 
                        display.clearDisplay();   // clears the screen and buffer
                        display.setTextSize(1);
                        display.setTextColor(WHITE);
                        display.setCursor(10,8);
                        display.print("Setup mvt");
                        display.setCursor(10,16);
                        display.print("position n ");
                        display.print(inc_nbr_position);
                        display.print(" /10");
                        display.setCursor(10,24);
                        display.print("touch button");
                        display.display(); // show        
                        delay(1000);    
                    }
                     irrecv.resume(); // Receive the next value
                     delay(200);
                  }while(!state_remote && inc_nbr_position <10);
                  val_byte[inc_nbr_position][0] = 0xFF;
                     // save position -> n° button
                  display.clearDisplay();   // clears the screen and buffer
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(10,8);
                  display.print("Setup mvt");
                  display.setCursor(10,16);
                  display.print("nombre position : ");
                  display.print(inc_nbr_position);
                  display.setCursor(10,24);
                  display.print("n  button ?  4-9");
                  display.display(); // show 
                  state_remote = false;
                  state_touch = false;
                  remote_touch = 0;
                  maestro.setTarget(0, INITSM0);maestro.setTarget(1, INITSM1); maestro.setTarget(2, INITSM2);maestro.setTarget(3, INITSM3);maestro.setTarget(4,INITSM4);maestro.setTarget(5, INITSM5); 
                  do{
                     if (irrecv.decode(&results)) {
                        if(results.value != 0xFFFFFFFF && ( results.value == TCH4 || results.value == TCH5 || results.value == TCH6 || results.value == TCH7 || results.value == TCH8 || results.value == TCH9)){
                            state_remote = true;
                            if(results.value == TCH4) {remote_touch = 4;} if(results.value == TCH5) {remote_touch = 5;}if(results.value == TCH6) {remote_touch = 6;}if(results.value == TCH7) {remote_touch = 7;}if(results.value == TCH8) {remote_touch = 8;}if(results.value == TCH9) {remote_touch = 9;}
                            for(int j=1; j<11 ; j++){
                              for(int k=0; k<7 ; k++){
                                EEPROM.write((remote_touch*10*7)+(j*7)+k, val_byte[j][k]); // write(addr, val);
                              }
                            }
                        }
                     }
                     irrecv.resume(); // Receive the next value
                     delay(300);
                  }while(!state_remote);
                  state_remote = false;
                  tps = millis();
              break;
              default :
              break;
              
            }
            
    }
    irrecv.resume(); // Receive the next value
  }   
}

/* display.drawSinus();
  
  maestro.setTarget(0, 0);

  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,1);
  sensorValue = analogRead(sensorPin); 
  display.print(sensorValue);
     
  if (irrecv.decode(&results)) {
    if(results.value != 0xFFFFFFFF){
      display.setCursor(10,8);
      display.print(results.value, HEX);
    }
    irrecv.resume(); // Receive the next value
  }
  
   
  display.setCursor(10,16);
  display.print( (short)(TouchSensor.capacitiveSensor(50)/10));

  compass.read();
  display.setCursor(10,24);
  display.print( compass.a.x);

  
  display.display();
  delay(100);*/
