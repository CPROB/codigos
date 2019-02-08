#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_Simple_AHRS.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f,2,1,0,4,5,6,7,3,POSITIVE);

float roll, pitch, heading, x, y, z;
unsigned long prev_time = 0;
const long _delay = 250;

/* Assign a unique ID to the sensors */
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
    lcd.setCursor(3,0);
    lcd.print("ENTA#CPROB");
    lcd.setCursor(0,1);
    lcd.print("X:");
    lcd.print(roll);
    lcd.print(";");
    lcd.print("Y:");
    lcd.print(pitch);
}
