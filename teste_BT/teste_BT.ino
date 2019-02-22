#include <SoftwareSerial.h>

/*
 * CONNECT TO OUTGOING PORT HC-05 'DEV-B' TO VIEW DATA ON SERIAL MONITOR
 * USE THIS SKETCH ONLY FOR VIEWING SENSOR DATA ON SERIAL MONITOR.....NOT FOR FILE WRITING
 */


#define led 6

SoftwareSerial BT(2,3);

void setup()
{
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);

}

void loop()
{
  if (BT.available()) 
  {
    BT.write("Comando recebido\n");
    char com = BT.read();
    if (com == 'L') 
    {
      digitalWrite(led,HIGH);
      BT.write("Led ligado!\n");
    }
  
    if (com =='D') 
    {
        digitalWrite(led,LOW);
        BT.write("Led desligado!\n");
    }
  }
}
