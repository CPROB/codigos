String msg = "ETS3#22,5;1024,32";

unsigned int reverse(unsigned int x)
{
 for (char i=0;i<8;i++)
   {char aux= bitRead(x,i);
    bitWrite(x,i,bitRead(x,15-i));
    bitWrite(x,15-i,aux);
   }   
}
unsigned int crc32(String msg)
{ int i,j;
  unsigned int byte1, crc;
  i=0;
  crc=0xFFFF;
  while (msg[i]!=0){
    byte1=msg[i]<<8;
    byte1= reverse(byte1);
    for (int j=0;j<8;j++) {
      if ((int)(crc ^byte1)<0)
              crc= (crc<<1)^0x4C11;
      else   crc= (crc<<1);
    }
    i++;  
  }
   return reverse(~crc);       
  }

void setup() 
{
  Serial.begin(9600);

}



void loop() 
{
  int i;
unsigned int x=crc32(msg);
//unsigned int y=reverse(1);
Serial.println(msg);
Serial.println(msg.length());
Serial.println("");
Serial.println(x,DEC);
delay(1000);
}
