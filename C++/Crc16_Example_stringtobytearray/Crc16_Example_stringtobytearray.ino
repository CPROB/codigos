#include <Crc16.h>

Crc16 crc; 

void setup()
{
    Serial.begin(9600); 
    Serial.println("CRC-16 bit test program");
    Serial.println("=======================");
}

void loop()
{
  String msg="ETS3#19;18,71;1019,92;44,91;52,93;0;ON;5,00;USB;-0,23;2,99;43,47;-0,51;-0,08;9,73;ON;0.000000;0.000000";
  byte data[200];
  int lenstr=msg.length();
  for(int j=0;j<lenstr;j++)
    data[j]=msg.charAt(j);
   
  /*Serial.println("Calculating crc incrementally");
  crc.clearCrc();
  for(byte i=0;i<lenstr;i++)
  {
     Serial.print("byte ");
     Serial.print(i);
     Serial.print(" = ");
     Serial.println(data[i]);
     crc.updateCrc(data[i]);
  }
  unsigned short value = crc.getCrc();
  Serial.print("crc = 0x");
  Serial.println(value, HEX);*/
  
  Serial.println("Calculating crc in a single call");
  unsigned short value = crc.XModemCrc(data,0,lenstr);
  Serial.print("crc = 0x");    
  Serial.println(value, HEX);
  //Reference crc
  Serial.println("Reference XModem crc");
  value = calcrc((char*)data,lenstr);
  Serial.print("crc = 0x");    
  Serial.println(value,HEX);
  Serial.println(lenstr);

  while(true);
}

int calcrc(char *ptr, int count)
{
    int  crc;
    char i;
    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    return (crc);
}
