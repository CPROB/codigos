#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(5,0);
  lcd.print("Ol");
  lcd.print((char)248);
  lcd.print("!");
  delay(2000);
  lcd.clear();

}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Arduino");
  //delay(1000);
  lcd.setCursor(0,1);
  lcd.print("Test"); 
  Serial.println("Arduino Test");
}

/*outras funções
lcd.setBacklight(HIHG/LOW)
lcd.setBacklightPin(pin_number)

UNO usar SDA-A4 SCL-A5 ; MEGA usar SDA-20 SCL-21
*/
