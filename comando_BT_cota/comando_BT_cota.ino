#include <BnrOneA.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SoftwareSerial.h>

BnrOneA one;

SoftwareSerial HC06(0,1); //RX, TX

#define led 5
#define SSPIN 2

boolean validaDados(String dados);
boolean readObstSensors();
char getComando(String dados);
int getVelocidade(String dados);


void setup() {

  pinMode(led, OUTPUT);
  Serial.begin(9600);
  HC06.begin(9600);
  one.spiConnect(SSPIN);
  one.obstacleEmitters(ON);
  one.lcd1("ENTA#CPROB");
  one.stop();
}

void loop() 
{
  String dados = "";
  while (HC06.available()) {
    dados = Serial.readString();
    HC06.print("String recebida:");
    HC06.println(dados);
    one.lcd1("ENTA#CPROB");
    one.lcd2(dados);
  }
  if(dados!="")
  {
    if(validaDados(dados)==true)
    {
      HC06.println("String validada!");
      HC06.println(getComando(dados));
      HC06.println(getVelocidade(dados));
    }
    else 
      HC06.println("String invalida!");
  }
  
  /*String comando = (String) dados.charAt(0) + (String) dados.charAt(1);
  char cmd = dados.charAt(0);
  char space = dados.charAt(1);
  String vel = (String) dados.charAt(2) + (String) dados.charAt(3);
  int newVel = vel.toInt();


  int spd = 0;

  if (command.charAt(0) == 'M' && command.charAt(1) == ' ') {
    spd = dados.charAt(2) + dados.charAt(3);
    Serial.println(dados);
  }

  if (cmd == 'L' && space == ' ') {
    one.lcd1("V: " + String(newVel));
    one.move(newVel, newVel);

  }
  else if (cmd == 'L' && space == '-') {
    newVel *= -1;
    one.lcd1("V: " + String(newVel));
    one.move(newVel, newVel);
  }

  if (cmd == 'L' && space != ' ') {
    Serial.println("Comando inválido!");
  }
  if (cmd == 'R' && space == ' ') {
    one.lcd1("V: " + String(newVel));
    one.move(newVel, -newVel);

  } else if (cmd == 'R' && space == '-') {
    one.lcd1("V: " + String(newVel));
    one.move(-newVel, newVel);
  }

  if (cmd == 'T' && space == ' ') {
    String freq = (String) dados.charAt(2) + (String) dados.charAt(3) + (String) dados.charAt(4) + (String) dados.charAt(5);
    tone(9, freq.toInt(), toneDuration.toInt()); // T 200 //2345
    Serial.println("T: " + freq + " " + toneDuration);
  }

  if (cmd == 'D' && space == ' ') {
    toneDuration =
      (String)dados.charAt(2) +
      (String)dados.charAt(3) +
      (String) dados.charAt(4) +
      (String)dados.charAt(5);
    Serial.println("Duration set to: " + toneDuration);
  }
}*/
}
/****************************************************************/
boolean validaDados(String dados)
{
  int valida=false;
  char comandos[6] = {'F','B','S','L','R'};
  for (int i=0;i<5;i++)
  {
    if(dados.charAt(0)==comandos[i] && dados)
      valida=true;
  }
  return valida;
}

char getComando(String dados)
{
  return dados.charAt(0);
}

int getVelocidade(String dados)
{
  String velStr;
  for (int i=1;i<dados.length();i++)
    velStr += dados.charAt(i);
  return velStr.toInt();
}

boolean readObstSensors()
{
  byte sensores = one.obstacleSensors();
  //boolean obstaculo = false;
  switch(sensores)
  {
    case 0:
      return false;
      break;
    case 1:
      return true;
      break;
    case 2:
      return true;
      break;
    case 3:
      return true;
      break;
  }
}
