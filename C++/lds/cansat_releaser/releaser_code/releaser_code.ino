#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

Adafruit_BMP280 bmp;

const float z = 0.0065, R = 287.06, g = 9.81;
float pressure, altitude, T0, P0;
char o;
int rate=500;

long int timer=0;

void setup() {
  Serial.begin(9600);
  bmp.begin();
  delay(1000);
  P0=bmp.readPressure() / 100;
  delay(1000);
  P0=bmp.readPressure() / 100;
  delay(1000);
  T0=bmp.readTemperature();
  delay(500);

}

void loop() {
  if (timer > millis())  
    timer = millis();
  if (millis() - timer > rate) 
  {
    timer = millis();
    pressure = bmp.readPressure() / 100;
    altitude = ((T0 + 273.15) / z) * (1 - (pow(pressure / P0, (z * R) / g)));
    Serial.print("P0:");
    Serial.print(P0); 
    Serial.print(";T0: ");
    Serial.print(T0);
    Serial.print(";P: ");
    Serial.print(pressure); 
    Serial.print(";h: ");
    Serial.print(altitude);
    Serial.print(";t: ");
    Serial.println(bmp.readTemperature());
  }
  if ( Serial.available () ) {
    char o = Serial.read();
    if (o == 't')// t simboliza uma pressão de teste no valor de 950hPa
      Serial.print("larga");    
  }
}


