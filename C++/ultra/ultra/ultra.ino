
//parte declarativa

#include <Ultrasonic.h>

Ultrasonic ultrasonic(12,13);

void leds (int lg1, int lg2, int ly1, int ly2, int lo1, int lo2, int lr1, int lr2) {
  
    digitalWrite(14,lg1);
    digitalWrite(15,lg2);
    digitalWrite(16,ly1);
    digitalWrite(17,ly2);
    digitalWrite(18,lo1);
    digitalWrite(19,lo2);
    digitalWrite(20,lr1);
    digitalWrite(21,lr2);
}

//setup é executa uma só vez

void setup() {

  Serial.begin(9600);
  
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  pinMode(20,OUTPUT);
  pinMode(21,OUTPUT);
}


// loop é executado continuamente

void loop() {

  int medida = ultrasonic.Ranging(CM);

  Serial.println(medida);
  
  if (medida > 50) {
  
    leds(1,1,0,0,0,0,0,0);
      
  }

  if (medida > 30 && medida <=50) {

    leds(1,1,1,0,0,0,0,0);
  }
 
 if (medida > 20 && medida <=30) {

  leds(1,1,1,1,0,0,0,0);
 }
 
 if (medida > 15 && medida <=20) {

  leds(1,1,1,1,1,0,0,0);
 }

 if (medida > 5 && medida <=15) {

  leds(1,1,1,1,1,1,0,0);
  
 }

 if (medida <=5) {
      
  leds(1,1,1,1,1,1,1,1);
    
  }

delay (100);

}
    
    




