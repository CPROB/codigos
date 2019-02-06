/* 
    Exemplo de codificação da Ground Station
    Recebe dados emitidos por outro qbcan
*/

//inclui livrarias necessárias
#include <qbcan.h>
#include <Wire.h>
#include <SPI.h>

//parâmetros RF
#define NODEID        1  //nó do recetor
#define NETWORKID     0  //o mesmo valor parametrizado no emissor
#define ENCRYPTKEY    "#ENTA_TEAM_SAT_3" //chave de encriptação de 16 caracteres
#define FREQUENCY     RF69_433MHZ //define a frequência base

//cria o objeto RF
RFM69 RF;

//define as variáveis globais utilizadas
bool promiscuousMode = false; //mudar para TRUE para receber todos os pacotes de todos os nós
uint32_t packetCount = 0;

void setup()
{
  //inicializa a comunicação série
  Serial.begin(9600);
  //inicializa o módulo RFM69 (recetor)
  RF.initialize(FREQUENCY,NODEID,NETWORKID);
  RF.setHighPower(); //Use the high power capabilities of the RFM69HW
  RF.encrypt(ENCRYPTKEY);
  RF.promiscuous(promiscuousMode);
}

void loop() {
  if (RF.receiveDone()) {
    Serial.print("#[");
    Serial.print(++packetCount);
    Serial.print(']');
    Serial.print('[');
    Serial.print(RF.SENDERID, DEC);
    Serial.print("] ");

    //verifica se o modo "lê todos" está ativo
    if (promiscuousMode) {
      Serial.print("to [");
      Serial.print(RF.TARGETID, DEC);
      Serial.print("] ");
    }
    
    for (byte i=0; i<RF.DATALEN; i++)
      Serial.print((char)RF.DATA[i]);
    
    //apenas executar se for importante os dados RSSI
    Serial.print(" [RX_RSSI:");
    Serial.print(RF.RSSI);
    Serial.print("] ");
    Serial.println(RF.DATALEN);
  }
}




