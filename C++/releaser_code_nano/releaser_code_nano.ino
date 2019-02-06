//SCL - pin A5
//SDA - pin A4
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Servo.h>

Adafruit_BMP280 bmp;

Servo servo;

const int h_max=25.00; //alterar aqui a altitude de lançamento

const float z = 0.0065, R = 287.06, g = 9.81;
float pressure=0, altitude=0;
float T0=1;
float P0=1;
int rate=1000;
int servoPin = 9;
int servoPower = 5;
int butPower = 6;
int button = 5;
int LidState;
int lastButtonState = LOW;
int buttonState;
int BMP_power = 10;
int input_eat = 0;
int ciclo=0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long int timer=0;
boolean bootup_complete = false;

void setup() {
  Serial.begin(9600);
  pinMode(BMP_power,OUTPUT);
  digitalWrite(BMP_power,HIGH);
  pinMode(butPower,OUTPUT);
  digitalWrite(butPower,HIGH);
  pinMode(button,INPUT);
  LidState=1;
  delay(500);
  servo.attach(servoPin);
  servo.write(90);
  bmp.begin();
  delay(500);
}

void loop() {
  if (millis() - timer > rate) {
    timer = millis();
    bmp.begin();
    pressure = (bmp.readPressure() / 100);
    altitude = ((T0 + 273.15) / z) * (1 - (pow(pressure / P0, (z * R) / g)));
    Serial.print("P0:");
    Serial.print(P0); 
    Serial.print(";T0: ");
    Serial.print(T0);
    Serial.print(";p: ");
    Serial.print(pressure); 
    Serial.print(";h: ");
    Serial.print(altitude);
    Serial.print(";t: ");
    Serial.println(bmp.readTemperature());
    ciclo++; 
    if (ciclo == 1) {
      bootup_complete = true;
      P0=bmp.readPressure()/100;
      T0=bmp.readTemperature();
      Serial.println ("ALL VALUES ABOVE WERE IGNORED");
    }
  }  
  //se altitude >= x metros abre porta  
  if (altitude >= h_max && bootup_complete == true)
      servo.write(90);

  //comando manual da porta
  int reading = digitalRead(button);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH)
        LidState = !LidState;
    }
  }
  if (LidState==0)
    servo.write(0);
  if (LidState==1)
    servo.write(55);
  lastButtonState = reading;  
}
