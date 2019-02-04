/***********************************
 * APC220 - Parametrização e leitura da frequência
 * Version: 1.0
 * Author: Duarte Cota
 * Date: 01-11-2018
************************************/

#include <SoftwareSerial.h>

#define str_set "WR 435000 3 9 3 0"

const int pinRX=6;
const int pinTX=7;
const int pinSet=8;

bool set_val=false;
bool get_val=false;

//cria objeto com nome apc220
SoftwareSerial apc220(pinRX, pinTX); 

//protótipos de funções
void setAPC220freq(void);
void getAPC220freq(void);

void setup() 
{
  Serial.begin(9600);
  apc220.begin(9600);
  pinMode(pinSet, OUTPUT);
  digitalWrite(pinSet, HIGH);
  //chama a função para gravar parâmetros
  setAPC220freq();
  //chama a função para ler os parâmetros
  getAPC220freq();
  delay(1000);
  if(set_val==true && get_val==true) {
    Serial.print("SETUP TERMINADO...\n");
    Serial.println("A enviar teste... ");
  }
  else 
    Serial.println("Ocorreu um problema... Verifique as ligacoes e tente de novo!");
}

void loop() 
{
  if(set_val==true && get_val==true)
    apc220.println("TESTE APC220");
  delay(1000);
  
}


//função para gravar a frequência desejada no módulo APC220
void setAPC220freq(void)
{
  //Colocar o pino SET em low e habilita o modo config do APC220
  digitalWrite(pinSet, LOW);
  delay(10);
  //Escreve os parametro no modulo
  apc220.println(str_set);
  delay(10);
  //Colocar o pino SET em high e desabilita o modo config do APC220
  digitalWrite(pinSet, HIGH);
  delay(10);
  set_val=true;
}

//função para ler a frequência parametrizada
void getAPC220freq(void)
{
  //Rotina de leitura dos parametros
  digitalWrite(pinSet, LOW);
  delay(10);
  apc220.println("RD");
  delay(200);
  if (apc220.available()) {
    Serial.println("APC220 encontrado!");
    get_val=true;
  }
  else
    Serial.println("APC220 não encontrado!");
  while (apc220.available())
    Serial.write(apc220.read());
  delay(200);
  //Colocar o pino SET em high e desabilita o modo config do APC220
  digitalWrite(pinSet, HIGH);
  delay(200);
}
