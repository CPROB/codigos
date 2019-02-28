#include <BnrOneA.h>
#include <EEPROM.h>
#include <SPI.h>
BnrOneA one;

#define led 5
#define SSPIN 2

String toneDuration = "";

void setup() {

  pinMode(led, OUTPUT);
  Serial.begin(9600);
  one.spiConnect(SSPIN);
  one.stop();
}

void loop() {

  String dados = "";
  if (Serial.available()) {
    dados = Serial.readString();
    Serial.print("String recebida:\n");
  }

  String command = (String) dados.charAt(0) + (String) dados.charAt(1);

  char cmd = dados.charAt(0);
  char space = dados.charAt(1);
  String vel = (String) dados.charAt(2) + (String) dados.charAt(3);
  int newVel = vel.toInt();


  int spd = 0;

  if (command.charAt(0) == 'M' && command.charAt(1) == ' ') {
    spd = dados.charAt(2) + dados.charAt(3);
    Serial.println(dados);
  }

  if (cmd == 'L' && space == ' ') {
    one.lcd1("V: " + String(newVel));
    one.move(newVel, newVel);

  }
  else if (cmd == 'L' && space == '-') {
    newVel *= -1;
    one.lcd1("V: " + String(newVel));
    one.move(newVel, newVel);
  }

  if (cmd == 'L' && space != ' ') {
    Serial.println("Comando inválido!");
  }
  if (cmd == 'R' && space == ' ') {
    one.lcd1("V: " + String(newVel));
    one.move(newVel, -newVel);

  } else if (cmd == 'R' && space == '-') {
    one.lcd1("V: " + String(newVel));
    one.move(-newVel, newVel);
  }

  if (cmd == 'T' && space == ' ') {
    String freq = (String) dados.charAt(2) + (String) dados.charAt(3) + (String) dados.charAt(4) + (String) dados.charAt(5);
    tone(9, freq.toInt(), toneDuration.toInt()); // T 200 //2345
    Serial.println("T: " + freq + " " + toneDuration);
  }

  if (cmd == 'D' && space == ' ') {
    toneDuration =
      (String)dados.charAt(2) +
      (String)dados.charAt(3) +
      (String) dados.charAt(4) +
      (String)dados.charAt(5);
    Serial.println("Duration set to: " + toneDuration);
  }
}

