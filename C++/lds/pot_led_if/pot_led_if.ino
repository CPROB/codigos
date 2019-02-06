void setup() {
  Serial.begin(9600);
  pinMode(9,OUTPUT);
}
void loop() {
  int pot = 0;
  float potVolts = 0;
  pot = analogRead(A0);
  potVolts = (pot*5)/1023;
  if ( pot < 500) {
    digitalWrite(9,LOW);
    }
    else {
      digitalWrite(9,HIGH);
    }
  Serial.print(pot);
  Serial.print(";");
  Serial.println(potVolts);
  delay(250);
}
