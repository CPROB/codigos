#include <FreqCount.h>

int count = 0;

void setup() {
  Serial.begin(9600);        // connect to the serial port
}
long int frq;
void loop() {

    FreqCount::f_comp = 8; // Cal Value / Calibrate with professional Freq Counter
    FreqCount::start(1000);  // 100 ms Gate Time
    while (FreqCount::f_ready == 0)
    frq = FreqCount::f_freq;
    //Serial.print("S"); Serial.print(count); Serial.print("=");
    Serial.println(frq);
  }
//}
