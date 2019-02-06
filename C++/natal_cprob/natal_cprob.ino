#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

int i,j, col;
char cprob[]="ENTA#CPROB";
char feliz[]="Feliz";
char natal[]="Natal";
char bom[]="BOM";
char ano[]="ANO";
char novo[]="NOVO";
char votos[]="Sao os votos do";
  
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
}

void loop() {

    //faz feliz natal
    col=2;
    for(j=0;j<=4;j++){
      lcd.setCursor(col,0);
      lcd.print(feliz[j]);
      delay(50);
      col++;

    }
    col++;
    for(j=0;j<=4;j++){
      lcd.setCursor(col,0);
      lcd.print(natal[j]);
      delay(50);
      col++;
    }
    lcd.print("!");


    //faz bom ano novo
    col=13;
    for(j=3;j>=0;j--){
      lcd.setCursor(col,1);
      lcd.print(novo[j]);
      delay(50);
      col--;
    }
    col--;
    for(j=2;j>=0;j--){
      lcd.setCursor(col,1);
      lcd.print(ano[j]);
      delay(50);
      col--;
    }
    col--;
    for(j=2;j>=0;j--){
      lcd.setCursor(col,1);
      lcd.print(bom[j]);
      delay(50);
      col--;
    }
    delay(3000);
    lcd.clear();
    delay(500);

    //faz entacprob
    col=3;
    for(j=0;j<=9;j++){
      lcd.setCursor(col,0);
      lcd.print(cprob[j]);
      delay(50);
      col++;
    }
    delay(1000);

    //faz *
    for(j=0;j<=2;j++){
      lcd.setCursor(j,0);
      lcd.print("*");
      delay(200);
    }
    for(j=13;j<=15;j++){
      lcd.setCursor(j,0);
      lcd.print("*");
      delay(200);
    }
    for(j=0;j<=15;j++){
      lcd.setCursor(j,1);
      lcd.print("*");
      delay(100);
    }
    delay(3000);
    lcd.clear();
    delay(500);
}


