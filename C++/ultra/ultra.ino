#include "Ultrasonic.h"

Ultrasonic ultrasonic(12,13);

void leds(int l1, int l2, int l3, int l4, int l5, int l6, int l7, int l8) {
  
  digitalWrite(14,l1);
  digitalWrite(15,l2);
  digitalWrite(16,l3); 
  digitalWrite(17,l4);
  digitalWrite(18,l5);
  digitalWrite(19,l6);
  digitalWrite(20,l7);
  digitalWrite(21,l8);
}

void setup() {
  Serial.begin(9600);
  for (int i=14;i<22;i++)
    pinMode(i,OUTPUT);
}

void loop() {
  int dis = ultrasonic.Ranging(CM);
  Serial.println(dis);
  if(dis>50)
    leds(1,1,0,0,0,0,0,0);
    
  if(dis<=50 && dis>42)
    leds(1,1,1,0,0,0,0,0);

  if(dis<=42 && dis>36)
    leds(1,1,1,1,0,0,0,0);

  if(dis<=36 && dis>28)
    leds(1,1,1,1,0,0,0,0);

  if(dis<=28 && dis>20)
    leds(1,1,1,1,1,0,0,0);

  if(dis<=20 && dis>12)
    leds(1,1,1,1,1,1,0,0);

  if(dis<=12 && dis>6)
    leds(1,1,1,1,1,1,1,0);

  if(dis<=6)
    leds(1,1,1,1,1,1,1,1);

  delay(100);
}
