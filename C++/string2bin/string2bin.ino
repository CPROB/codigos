//#include <string.h>

//String text = "ETS3#25,4;1023,5;45,6;90;0;9;ON;1,34;0,34;180,56;0,34;2,34;10,54;OFF;37,22222222;-25,99999999";
String msg = "ETS3";

void setup()
{
  Serial.begin(9600);
  }
  
unsigned int crc16 (String msg)
{
  int i,j;
  unsigned int b, crc, mask;
  i=0;
  crc=0xFFFF;
  for(i=0;i<msg.length();i++)
  {
    b=msg.charAt(i);
    crc=crc^b;
    for(j=7;j>=0;j--)
    {
      mask=-(crc&1);
      crc=(crc>>1) ^ (0x1021 & mask);
    }

  }
  return ~crc;
}
void loop()
{
unsigned long int y=crc16(msg);
for(int i=0;i<msg.length();i++)
  Serial.print(msg.charAt(i)); //imprime msg
Serial.println("");
Serial.println(msg.length());//imprime tamanho da mensagem em caracteres
Serial.println(y,HEX);
delay(500);
}

