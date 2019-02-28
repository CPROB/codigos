
String palavra;
//String posLetra;

int num_caract =0;

int t1 = 200; //1
int t2 = 400; //2
int t3 = 600; //3

void setup() {
 Serial.begin(9600);
 pinMode(7,OUTPUT);
 digitalWrite(7, LOW);
}

void letra(int var1, int var2, int var3) {
  int tempo_desligado=200;
    //1º digito do codigo
  digitalWrite(7, HIGH);
  delay(var1);
  digitalWrite(7, LOW);
  delay(tempo_desligado);
  //2º digito do codigo
  digitalWrite(7, HIGH);
  delay(var2);
  digitalWrite(7, LOW);
  delay(tempo_desligado);
  //3º digito do codigo
  digitalWrite(7, HIGH);
  delay(var3);
  digitalWrite(7, LOW);
  delay(tempo_desligado);
}

void loop() 
{
  while(Serial.available())
  {
    palavra = Serial.readString();
  }
  num_caract = palavra.length();
  Serial.print(palavra);
  Serial.print(" ; ");
  Serial.print(num_caract);
  Serial.print(" ; ");
  Serial.println(palavra.charAt(num_caract - 2));
  if (palavra != " ")
  {
    Serial.println("entra");
    for(int i=0; i<num_caract-1;i++)
    {
      Serial.println(i);
      char l = palavra.charAt(i);
      switch(l){
      case 'A': 
        letra(t1,t1,t1);
        break;
      case 'B':
        letra(t1,t1,t2);
        break;
      case 'C':
        letra(t1,t1,t3);
        break;
      case 'D':
        letra(t1,t2,t1);
        break;
      case 'E':
        letra(t1,t2,t2);
        break;
      case 'F':  
        letra(t1,t2,t3);
        break;
      case 'G': 
        letra(t1,t3,t1);
        break;
      case 'H': 
        letra(t1,t3,t2);
        break;
      case 'I': 
        letra(t1,t3,t3);
        break;
      case 'J':   
         letra(t2,t1,t1);
        break;
      case 'K':                              
         letra(t2,t1,t2);
        break;
      case 'L':  
          letra(t2,t1,t3);
        break;
      case 'M': 
          letra(t2,t2,t1);
        break;
      case 'N': 
          letra(t2,t2,t2);
         break;
      case 'O':
          letra(t2,t2,t3);
         break;
      case 'P':
          letra(t2,t3,t1);
          break;
      case 'Q':
          letra(t2,t3,t2);
          break;
      case 'R':
          letra(t2,t3,t3);
          break;
      case 'S':
          letra(t3,t1,t1);
          break;
      case 'T':
          letra(t3,t1,t2);
          break;
      case 'U':
          letra(t3,t1,t3);
          break;
      case 'V':
          letra(t3,t2,t1);
          break;
      case 'W':
          letra(t3,t2,t2);
          break;
      case 'X':
          letra(t3,t2,t3);
          break;
      case 'Y':
          letra(t3,t3,t1);
          break;
      case 'Z':
          letra(t3,t3,t2);
          break;
      case ' ':
          letra(t3,t3,t3);
          break;
      }
    }
    Serial.println("sai");
  }
  palavra=" ";
  Serial.flush();
}
