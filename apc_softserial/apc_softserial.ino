#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>

const int pinRX = 6;
const int pinTX = 7;

SoftwareSerial apc220(pinRX, pinTX);

Adafruit_BMP280 bme;

void setup() {
  apc220.begin(9600);
  Serial.begin(9600);
  bme.begin();
}

void loop() {
  float temp = bme.readTemperature();
  float pres = bme.readPressure()/100;
  Serial.print("T = ");
  Serial.print(temp);
  Serial.print(" ; ");
  Serial.print("Pressure = ");
  Serial.print(pres);
  Serial.println(" hPa");
  delay(1000);
}
