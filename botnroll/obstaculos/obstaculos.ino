#include <BnrOneA.h>  
#include <EEPROM.h>   
#include <SPI.h>      

BnrOneA one;           

#define SSPIN  2      // Slave Select (SS)

void setup() 
{ 
    delay(1500);			
    Serial.begin(57600);        
    one.spiConnect(SSPIN);  
    one.stop();              
    one.obstacleEmitters(ON);
    one.lcd1("ENTA#CPROB");
    one.lcd2("Pressionar PB1!!");
    while(one.readButton()==0)
       delay(50);
}

void loop()
{
    byte obstacles=one.obstacleSensors(); //Read obstacle sensors
    switch(obstacles)
    {
        case 0:   // nenhum obstaculo detetado
            one.lcd2("Sem obstaculos");
            one.move(30,30);     // em frente
            break;
        case 1:   // obstaculo no sensor esquerda
            one.lcd2("Sensor esquerda");
            one.move(30,-30);    // rodar direita
            break;
        case 2:   // obstaculo no sensor direita
            one.lcd2("Sensor direita");
            one.move(-30,30);    // rodar esquerda
            break;
        case 3:   // obstaculo nos dois sensores
            one.lcd2("Dois sensores");
            one.move(-30,-30);   // rodar atrás
            delay(1000);
            one.move(-30,30);    // rodar esquerda    
            delay(500);     
            break;
    }
    delay(50);
}

