#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Ultrasonic.h"
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);
Ultrasonic ultrasonic(8,9);
void setup() {
 
  Serial.begin(100);
  lcd.begin(16,2);
  lcd.clear();
  pinMode(7,OUTPUT);
}

void loop() {
  
  int medida = ultrasonic.Ranging(CM);
  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.print(medida);
  
  Serial.println(medida);

  if (medida < 20)
    digitalWrite(7,HIGH);
  else
    digitalWrite(7,LOW);    

}

/*outras funções
lcd.setBacklight(HIHG/LOW)
lcd.setBacklightPin(pin_number)

UNO usar SDA-A4 SCL-A5 ; MEGA usar SDA-20 SCL-21
*/
