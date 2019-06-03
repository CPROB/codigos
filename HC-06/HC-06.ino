#include <SoftwareSerial.h>
#include <BnrOneA.h>
#include <EEPROM.h>
#include <SPI.h>

#define SSPIN 2

int carro;
char c;
String str="";

//criar objeto serial
SoftwareSerial mySerial(0,1); //RX e TX

//criar objeto BotnRoll
BnrOneA one;

void setup() 
{
  Serial.begin(9600);
  Serial.println("comandos AT");
  one.spiConnect(SSPIN);
  mySerial.begin(9600);
  one.stop(); 
  pinMode(9,OUTPUT);
  one.obstacleEmitters(ON);
}


void loop() 
{
  one.lcd2("                ");
  while(Serial.available())
  {
    c = Serial.read();
    //str +=(String)c;
  }
    byte ir_sensors=0;
    ir_sensors=one.readIRSensors();
    if (ir_sensors==1)
    {
      tone(9,2000,1000);  
      one.stop();
      delay(4000);
    }
    
  
  Serial.println(c); 


  switch(c){
    case 'S':
    one.lcd2("     Stop   ");
    one.stop();
    break;
    
    case 'F':
    one.lcd2("    Forward "); 
    one.move(50,50);          
    break;

    case 'E':                          //Frente diagonal direita
    one.lcd2(" Diagonal direita "); 
    one.move(40,20);  
         
    break;

    case 'G':                          //Frente diagonal esquerda
    one.lcd2(" Diagonal esquerda "); 
    one.move(20,40);  
           
    break;

   case 'B':                          //Back
   one.lcd2("   Backward ");
    one.move(-50,-50);        
   break;
   
   case 'C':
   one.lcd2("   Backward Right ");
   one.move(-30,-20);        
   break;

   case 'H':                          //Back diagonal esquerda
    one.lcd2(" Backward Left "); 
    one.move(-20,-40);  
         
    break;
   
   case 'L':
    one.lcd2("  Rotate Left ");
    one.move(-20,20);         
   break;

   case 'R':
    one.lcd2("  Rotate Right ");
    one.move(20,-20);                 // Rodar direita
   break;
   }
}

  
