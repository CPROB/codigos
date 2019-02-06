
//parte declarativa
#include "Ultrasonic.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Ultrasonic ultra1(8,9);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

const int tempo = 500;

//setup
void setup() {
  pinMode(7,OUTPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
    byte cedilla [8] =  {
    B00000,
    B00000,
    B01110,
    B10000,
    B10000,
    B01110,
    B00100,
    B01000,

  };
  byte full [8]= {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte empty [8]= {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
  };
  lcd.createChar(2,cedilla);
  lcd.createChar(1,full);
  lcd.createChar(0,empty);
}

//rotinas
void refreshbeeper(int pin, int output){
digitalWrite(pin,output);
}


//loop
void loop() {
  int distancia = ultra1.Ranging(CM);
  lcd.setCursor(0,1);
  lcd.print(distancia);
  lcd.print(" ");
  lcd.setCursor(5,1);

  
  if (distancia > 50){
    //Inserir comandos aqui para quando a distancia for segura
    lcd.print("Avan");
    lcd.write(byte(2));
    lcd.print("ar");
    refreshbeeper(7,0);
    }

    
  if (distancia < 50 && distancia > 20){
    //Inserir comandos aqui para quando for preciso ter cuidado com a distância
    lcd.print ("Cuidado");
    refreshbeeper(7,0);


    
  }if (distancia <=  20){
    //Inserir comandos de paragem ou rotinas de segurança para curtas distâncias
    lcd.print("Parar");
    refreshbeeper(7,0);
  }

  //REFRESH BAR
  int i=0;
  lcd.setCursor(0,0);
  for (i=0; i <= 15; i++){
    lcd.write(byte(1));
  }

  //FILL BAR
  lcd.setCursor(0,0);
  int blocos= distancia/10;
  for (i=0;i <= blocos;i++){
    lcd.write(byte(0));
  }
  delay(1000); 
  lcd.clear();
}


