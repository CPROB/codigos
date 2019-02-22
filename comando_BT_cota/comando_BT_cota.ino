#include <BnrOneA.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SoftwareSerial.h>

BnrOneA one;

#define led 5
#define SSPIN 2

boolean validaDados(String);
boolean readObstSensors();
char getComando(String);
int getVelocidade(String);
void moverFrente(String);
void moverTras(String);
void inverterSentido();
void parar();
void virarDireita();
void virarEsquerda();

int vel=0;
boolean frente=false;
boolean recua=false;
int bloqueio=0;

void setup() {

  pinMode(led, OUTPUT);
  Serial.begin(9600);
  //HC06.begin(9600);
  one.spiConnect(SSPIN);
  one.obstacleEmitters(ON);
  one.lcd1("   ENTA#CPROB");
  one.lcd2("                ");
  one.stop();
}

void loop() 
{
  String dados = "";
  while (Serial.available()) {
    dados = Serial.readString();
    Serial.print("Comando recebido:");
    Serial.println(dados);
  }
  if(dados!="")
  {
    if(validaDados(dados)==true)
    {
      switch(getComando(dados))
      {
        case 'F':
          moverFrente(dados);
          break;
        case 'B':
          moverTras(dados);
          break;
      case 'S':
          parar();
          break;
      case 'R':
          virarDireita();
          break;
      case 'L':
          virarEsquerda();
          break;     
      case 'I':
          inverterSentido();
          break;
      }
    }
    else
    {
      Serial.println("Comando invalido!");
      tone(9,1500,1000);
    }
  }
}
/****************************************************************/
boolean validaDados(String dados)
{
  int valida=false;
  char comandos[6] = {'F','B','S','L','R', 'I'};
  for (int i=0;i<6;i++)
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

void moverFrente(String dados)
{
  if(bloqueio==0)
  {
    frente=true;
    recua=false;
    digitalWrite(led,HIGH);
    tone(9,2000,500);
    one.move(getVelocidade(dados),getVelocidade(dados));
    Serial.println("Comando valido!");
    Serial.print("Em frente velocidade ");
    Serial.println(getVelocidade(dados));
    vel=getVelocidade(dados);
  }
}

void moverTras(String dados)
{
  frente=false;
  recua=true;
  digitalWrite(led,HIGH);
  tone(9,2000,500);
  one.move(getVelocidade(dados)*-1,getVelocidade(dados)*-1);
  Serial.println("Comando valido!");
  Serial.print("Recuar velocidade ");
  Serial.println(getVelocidade(dados));
  vel=getVelocidade(dados);
}

void inverterSentido()
{
  if(frente==1)
  {
    one.stop();
    one.move(40,-40);
    delay(600);
    Serial.println("Comando validado!");
    Serial.print("Sentido invertido");  
    tone(9,2000,500);
    one.move(vel,vel);
    Serial.print("Em frente velocidade ");
    Serial.println(vel);
  }
}

void parar()
{
    digitalWrite(led,LOW);
    one.stop();
    Serial.println("Comando valido!");
    Serial.println("Parar");
}

void virarDireita()
{
    if(frente==true)
    {
      one.move(15,-10);
      Serial.println("Comando valido!");
      Serial.print("Direita ");
      delay(500);
      tone(9,2000,500);
      one.move(vel,vel);
      Serial.print("Em frente velocidade ");
      Serial.println(vel);
    }
    if(recua==true)
    {
      one.move(-10,15);
      Serial.println("Comando validado!");
      Serial.print("Direita ");
      delay(500);
      tone(9,2000,500);
      one.move(-vel,-vel);
      Serial.print("Recua velocidade ");
      Serial.println(-vel);
    }
}

void virarEsquerda()
{
   if(frente==true)
   {
      one.move(-10,15);
      Serial.println("Comando valido!");
      Serial.print("Direita ");
      delay(500);
      tone(9,2000,500);
      one.move(vel,vel);
      Serial.print("Em frente velocidade ");
      Serial.println(vel);
   }
   if(recua==true)
   {
      one.move(15,-10);
      Serial.println("Comando validado!");
      Serial.print("Direita ");
      delay(500);
      tone(9,2000,500);
      one.move(-vel,-vel);
      Serial.print("Recua velocidade ");
      Serial.println(-vel);
   } 
}

