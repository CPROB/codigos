
 long int cont = 0;
 long int value = 0;
 bool contagem = false;
 void setup () {
    Serial.begin (9600);

    
 }

 void loop (){ 
   int leitura = analogRead (A0);
   //Serial.println (leitura);
   if(leitura>1000)
 {
    cont++;
    contagem = true;
 }
   else
      contagem=false;
    if (contagem==false){
      Serial.println(cont);
      cont=0;
 }

    /*if (leitura<1000)
     {
        Serial.println (cont);
        cont=0;
       
    }*/
   
 }
