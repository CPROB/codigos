//com reverse dados e reverse CRC

String msg = "ETS3";

unsigned int reverse(unsigned int x)
{
 
 for (char i=0;i<=7;i++)
   {
    char aux= bitRead(x,i);
    bitWrite(x,i,bitRead(x,15-i));
    bitWrite(x,15-i,aux);
   }   
  return x;
}
unsigned int crc16(String msg)
{ int i,j;
  unsigned int byte1, crc;
  crc=0xFFFF;
  for(int i=0;i<msg.length();i++)
  {
    byte1=msg.charAt(i);
    byte1=reverse(byte1);
    for (int j=0;j<=7;j++) 
    {
      if ((int)(crc^byte1)<0)
              crc=(crc<<1)^0x1021;
      else   crc=(crc<<1);
    } 
  }
   return reverse(~crc);       
  }

void setup() 
{
  Serial.begin(9600);
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
