#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int temp=0;     //define o valor da temperatura 
int bomba=3;
int ev1=4;
int ev2=5;
int cooler=6;
float lm35;
int hum_terra=1;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  pinMode(bomba,OUTPUT);
  pinMode(ev1,OUTPUT);
  pinMode(ev2,OUTPUT);
  pinMode(cooler,OUTPUT);
  digitalWrite(bomba,LOW);
  digitalWrite(ev1,LOW);
  digitalWrite(ev2,LOW);
  digitalWrite(cooler,LOW);
  
}
void loop() {
  lcd.clear();
  float lm35 = dht.readTemperature();
  lcd.setCursor(0,0);
  lcd.print("TEMP:");
  lcd.setCursor(6,0);
  lcd.print(lm35, 0);
  lcd.setCursor(8,0);
  lcd.print(char(223));
  lcd.setCursor(9,0);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("HUM:");
  lcd.setCursor(6,1);
  float hum = dht.readHumidity();
  lcd.print(hum, 0);
  lcd.setCursor(8,1);
  lcd.print("%");
  Serial.println(analogRead(hum_terra));
  if (lm35 >= 25){
    digitalWrite(ev1, HIGH);
    digitalWrite(ev2, LOW);
  }
  else {
  digitalWrite(ev1, LOW);
  digitalWrite(ev2, HIGH);
  }
  delay(5000);
}
