#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_Simple_AHRS.h>
#include <LiquidCrystal_I2C.h>
#include <BnrOneA.h>
#include <EEPROM.h>

BnrOneA one;

LiquidCrystal_I2C lcd(0x3f,2,1,0,4,5,6,7,3,POSITIVE);

SoftwareSerial HC05(3, 2); // TX | RX

float roll, pitch, heading, x, y, z;
unsigned long prev_time = 0;
const long _delay = 250;
char c = ' ';
int inibe1=0;
int inibe2=0;
int inibe3=0;
int inibe4=0;
int inibe5=0;
int inibe6=0;
int inibe7=0;
int inibe8=0;
int inibe9=0;

Adafruit_9DOF dof = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_Simple_AHRS ahrs(&accel, &mag);

void getIMUdata(void);

void printLCD(void);

//******************************************************************

void setup()
{
  accel.begin();
  delay(100);
  mag.begin();
  delay(100);
  Wire.begin();
  delay(100);
  Serial.begin(9600);
  HC05.begin(9600);
  lcd.begin(16,2);
}

void loop()
{
  getIMUdata();
  uint32_t current_time = millis(); 
  if (current_time - prev_time >= _delay)
   {
     prev_time=current_time;
     printLCD();
   } 
   
    if (roll>-2 && roll <8 && pitch>-5 && pitch<4 && inibe1==0)
     {
      HC05.write("S");
 
     inibe1=1;
     inibe2=0;
     inibe3=0;
     inibe4=0;
     inibe5=0;
     inibe6=0;
     inibe7=0;
     inibe8=0;
     inibe9=0;
     }
     
    if (roll>20 && roll<32 && pitch>1 && pitch<6 && inibe2==0)
     {
      HC05.write("F");
      inibe1=0;
      inibe2=1;
      inibe3=0;
      inibe4=0;
      inibe5=0;
      inibe6=0;
      inibe7=0;
      inibe8=0;
      inibe9=0; 
     }

     if (roll<-15 && roll>-30 && pitch>-3 && pitch<3 && inibe3==0)
     {
      HC05.write("B");
      inibe1=0;
      inibe2=0;
      inibe3=1;
      inibe4=0;
      inibe5=0;
      inibe6=0;
      inibe7=0;
      inibe8=0;
      inibe9=0;
     }

     if (roll>-5 && roll<6 && pitch >12 && pitch <32 && inibe4==0)
     {
      HC05.write("L");
      inibe1=0;
      inibe2=0;
      inibe3=0;
      inibe4=1;
      inibe5=0;
      inibe6=0;
      inibe7=0;
      inibe8=0;
      inibe9=0;
      
     }

     if (roll>-5 && roll<8 && pitch <-12 && pitch >-32 && inibe5==0)
     {
      HC05.write("R");
      inibe1=0;
      inibe2=0;
      inibe3=0;
      inibe4=0;
      inibe5=1;
      inibe6=0;
      inibe7=0;
      inibe8=0;
      inibe9=0;
     }

      if (roll>20 && roll<50 && pitch<-15 && pitch>-30 && inibe6==0)
     {
      HC05.write("E"); //Diagonal direita frente
      inibe1=0;
      inibe2=0;
      inibe3=0;
      inibe4=0;
      inibe5=0;
      inibe6=1;
      inibe7=0;
      inibe8=0;
      inibe9=0;
     }

     if (roll>20 && roll<32 && pitch >12 && pitch <32 && inibe7==0)
     {
      HC05.write("G"); //Diagonal esquerda frente
      inibe1=0;
      inibe2=0;
      inibe3=0;
      inibe4=0;
      inibe5=0;
      inibe6=0;
      inibe7=1;
      inibe8=0;
      inibe9=0;
     }


     if (roll<-15 && roll>-30 && pitch <-15 && pitch >-30 && inibe8==0)
     {
      HC05.write("C"); //Diagonal direita trás
      inibe1=0;
      inibe2=0;
      inibe3=0;
      inibe4=0;
      inibe5=0;
      inibe6=0;
      inibe7=0;
      inibe8=1;
      inibe9=0;
     }

     
     if (roll<-15 && roll>-30 && pitch >12 && pitch <32 && inibe9==0)
     {
      HC05.write("H"); //Diagonal esquerda trás
      inibe1=0;
      inibe2=0;
      inibe3=0;
      inibe4=0;
      inibe5=0;
      inibe6=0;
      inibe7=0;
      inibe8=0;
      inibe9=1;
     }
     
    
   if (HC05.available())
    {  
        c = HC05.read();
        Serial.write(c);
    }

    if (Serial.available())
    {
        c =  Serial.read();
        
        Serial.write(c);   
        HC05.write(c);  
    }

}

//*************************************************************************************************************************

void getIMUdata(void)
{
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_vec_t   orientation;
    sensors_event_t event;
    accel.getEvent(&accel_event);
    if (ahrs.getOrientation(&orientation))
    {
      roll = (orientation.roll);
      pitch = (orientation.pitch);
      heading = (orientation.heading);
    }
    accel.getEvent(&event);
    x = event.acceleration.x;
    y = event.acceleration.y;
    z = event.acceleration.z;

}

void printLCD(void)
{
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("X:");
    lcd.print(roll);
    lcd.print(";");
    lcd.print("Y:");
    lcd.print(pitch);
}


void bluetooth(void)
{
 HC05.print("X:");
 HC05.print(roll);
 HC05.print(";");
 HC05.print("Y:");
 HC05.print(pitch);
 HC05.print("#"); 
}
