#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 26
#define BME_MISO 24
#define BME_MOSI 25
#define BME_CS 5
//#define mSD_CS 6
#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);
//int  BME_CS=5;
int  SD_CS=6;
File myFile;

void setup() {
  Serial.begin(9600);

  digitalWrite(BME_CS, HIGH);
  
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    Serial.println();
    /*myFile = SD.open("ets3SPItest.txt", FILE_WRITE);
    myFile.print(bme.readTemperature()); myFile.print(";"); myFile.print(bme.readPressure()); myFile.print(";");myFile.println(bme.readHumidity());
    myFile.close();*/
    delay(500);
}
