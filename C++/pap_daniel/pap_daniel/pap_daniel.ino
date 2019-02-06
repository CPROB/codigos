#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
long Time = 0;
long Time2 = 0;
int D4 = 4;
int D9 = 9;
int D10 = 10;
int rele = 0;
double TemperaturaRegulada = 10;
uint32_t delayMS;

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define DHTPIN            2         
#define DHTTYPE           DHT22     

DHT_Unified dht(DHTPIN, DHTTYPE);

void Regulacao();

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  pinMode(D9, INPUT);
  pinMode(D10, INPUT);
  pinMode(D4, OUTPUT);
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  // Delay between measurements.
  if(millis() > (Time2 + 200)) {
    Time2 = millis();
    Regulacao();
  }
  if (millis() > (Time + 1000)) {
    Time = millis();
    lcd.setCursor(0, 0);
    if (isnan(event.temperature)) {
      Serial.println("Error reading temperature!");
    }
    else {
      Serial.print("Temperature: ");
      Serial.print(event.temperature);
      lcd.print(event.temperature);
      lcd.print((char) 223);
      lcd.print("C");
      lcd.setCursor(9, 0);
      lcd.print("TEMP");
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println("Error reading humidity!");
    }
    else {
      Serial.print("Humidity: ");
      Serial.print(event.relative_humidity);
      Serial.println("%");
      lcd.setCursor(0, 1);
      lcd.print(event.relative_humidity);
      lcd.print("%");
      lcd.setCursor(9, 1);
      lcd.print(TemperaturaRegulada);
      lcd.print((char) 223);
      lcd.print("C");
    }
  }
   if(event.temperature > TemperaturaRegulada && rele == 0)  {
    digitalWrite(D4,HIGH);
    rele = 1;
  }
  if(event.temperature <= TemperaturaRegulada && rele == 1)  {
    digitalWrite(D4,LOW);
    rele = 0;
  }
}

void Regulacao() {
  if (digitalRead(D9) > 0) {
    TemperaturaRegulada += 1.0f;
    lcd.setCursor(9, 1);
    lcd.print(TemperaturaRegulada);
    lcd.print((char) 223);
    lcd.print("C");
  }
  if (digitalRead(D10) > 0) {
    TemperaturaRegulada -= 1.0f;
    lcd.setCursor(9, 1);
    lcd.print(TemperaturaRegulada);
    lcd.print((char) 223);
    lcd.print("C");
  }
}
