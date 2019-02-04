

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_Simple_AHRS.h>


float roll, pitch, heading, x, y, z;

/* Assign a unique ID to the sensors */
Adafruit_9DOF dof = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_Simple_AHRS ahrs(&accel, &mag);

 void getIMUdata(void);

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
}

//uint32_t timer = millis();


void loop()
{
  //if (timer > millis())  timer = millis();
  //if (millis() - timer > rate) 
  //{
    //timer = millis();
    getIMUdata();// calls for procedure
    Serial.print(roll);
    Serial.print(";");
    Serial.print(pitch);
    Serial.print(";");
    Serial.println(heading);
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
