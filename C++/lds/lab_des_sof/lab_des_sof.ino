#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

Adafruit_BME280 bme;

int BUZZER=5, BME_vcc=2, LED_G=3, LED_R=4, rate=100;

String data, s_temp, s_pressure, s_hum, BME280_status, BUZZER_status, LED_status;

double temp, pressure, hum;

long int timer=0;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(BME_vcc, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  digitalWrite(BUZZER, LOW);
  digitalWrite(BME_vcc, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, HIGH);
  bme.begin();
  BME280_status="0";
  BUZZER_status="0";
}

void loop() {
  data="#;";
  if (timer > millis())  timer = millis();
  if (millis() - timer > rate) 
  {
    timer = millis();
    getTempPressHumAlt();
    data += s_temp + s_pressure + s_hum + BUZZER_status + ";" + BME280_status + ";" + "#";
    Serial.println(data); 
  }
  if ( Serial.available () )
  {
    char o = Serial.read();
    if (o == 'O') // if O turn BMP280 off
    { 
      digitalWrite(BME_vcc, LOW);
      BME280_status = "0";
      digitalWrite(LED_R, 1);
      digitalWrite(LED_G, 0);
    }
    if (o == 'F') // if F turn BMP280 on
    {
      digitalWrite(BME_vcc, HIGH);
      digitalWrite(LED_R, 0);
      digitalWrite(LED_G, 1);
      delay(15);
      bme.begin();
      BME280_status = "1";   
    }
    if (o== 'B')
    {
      digitalWrite(BUZZER, HIGH);
      BUZZER_status = "1";  
    }
    if (o== 'N')
    {
      digitalWrite(BUZZER, LOW);
      BUZZER_status = "0";
    }
}
}

void getTempPressHumAlt()
{
  if (BME280_status == "1")
  {
    temp=bme.readTemperature();
    s_temp=(String)temp;
    s_temp.replace(".", ",");
    s_temp+=";";
    pressure=bme.readPressure()/100;
    s_pressure=(String)pressure;
    s_pressure.replace(".", ",");
    s_pressure+=";";
    hum=bme.readHumidity();
    s_hum=(String)hum;
    s_hum.replace(".", ",");
    s_hum+=";";
  }
  if (BME280_status == "0")
  {
    s_pressure = "0;";
    s_temp = "0;";
    s_hum = "0;";
  }
}
