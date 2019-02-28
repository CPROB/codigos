
long int cont = 0;
long int value = 0;
bool contagem = false;
int controlo=0;
long int last_cont =0;
int leitura = 0;
int i = 0;

int contadores[4];

 
void setup () {
    Serial.begin (9600);
 }

void loop ()
{ 
  while(controlo<3)
  {
    leitura = analogRead (A0);
    if(leitura>750)
    {
      cont++;
      contagem = true;
      
    }
    if(leitura<750&&cont>0)
    {
      contagem=false;
      contadores[controlo]=cont;
      cont=0;
      controlo++;
    }
  }
   
  if(controlo==3)
  {
    Serial.println("\nCONTADORES:");
    if(controlo==3)
    {
      for(i=0;i<3;i++)
      {
        
        Serial.print(contadores[i]);
        Serial.print('\t');
      }
      controlo=0;
    }
  }
}
