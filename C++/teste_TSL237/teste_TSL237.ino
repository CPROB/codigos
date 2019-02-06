 
#include <FreqCounter.h>
long  pulses;
const float area = 0.0092;
int cnt;
int pinLed=13;
short period = 1000; // 100 ms Gate Time

void light(){
  digitalWrite(pinLed, HIGH);
  FreqCounter::f_comp=1;   // Cal Value / Calibrate with professional Freq Counter
  FreqCounter::start(period);  // 100 ms Gate Time
  while (FreqCounter::f_ready == 0)
  pulses=FreqCounter::f_freq;
  //delay(20);
  digitalWrite(pinLed, LOW);
  //freq = (pulses*1000)/(period*area);
  //irradiance = (freq/1000);
  return ;
}

void setup() {
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);        // connect to the serial port
  Serial.println("Pulse count:");
}

void loop() {
light();
Serial.print(cnt++);
Serial.print("  Pulses/sec: ");
Serial.println(pulses);
/*Serial.print(";  Freq: ");
Serial.print(freq);
Serial.print(" Hz;  Irradiance: ");
Serial.print(irradiance);
Serial.print(" uW/cm2 = KHz  ");
Serial.println(irradiance*0.01);
//delay(20);*/
} 


