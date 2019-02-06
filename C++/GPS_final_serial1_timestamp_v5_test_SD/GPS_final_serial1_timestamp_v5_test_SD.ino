// ENTA TEAM SAT v5 software
// read sensor data and GPS information
// analyze GPS information
// writes data into microSD card

// library declaration and definitions
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <SD.h>
#include <SPI.h>
#include <dht.h>
#define dht_dpin A0
File myFile;
HardwareSerial mySerial = Serial3;
Adafruit_GPS GPS(&mySerial);
dht DHT;

// variable declaration
int count, remainder, deg, minu, h;
const int chipSelect = 27, buzzer = 3;
long t;
float analog6, analog2, analog4, voltage, temp, pressure_MPX, altitude, lat, lon, sec, dec_coord, z=0.0065, R=287.06, g=9.81;
float T0=18, P0=1016, H0=45; // inicialize before mission
char s[15], lat_dir, lon_dir;

#define GPSECHO  true
boolean usingInterrupt = false;
void useInterrupt(boolean);

void setup()
{
  Serial1.begin(9600);
  count=1; // inicializes timestamp counter
  t=0; // inicializes timestamp
  h=0;
  pinMode(buzzer, OUTPUT);
  pinMode(SS, OUTPUT);
  SD.begin(chipSelect);
  Wire.begin();
  //**************************************************************************
  GPS.begin(9600);
  GPS.sendCommand("$PMTK251,19200*22");
  GPS.sendCommand("$PMTK220,100*2F");
  mySerial.end();
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  GPS.sendCommand(PGCMD_NOANTENNA);
  useInterrupt(false);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);
  }
SIGNAL(TIMER0_COMPA_vect) {
  while (mySerial.available())
  {
    char c = GPS.read();
  }
}
void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
uint32_t timer = millis();
//******************************************************************************
//******************************************************************************
void loop()               
{
  if (! usingInterrupt) {
    
    while (mySerial.available())
    {
      char c = GPS.read();
      if (GPSECHO)
      if (c) Serial.print(c);
   }
  }
  if (GPS.newNMEAreceived()) {
  if (!GPS.parse(GPS.lastNMEA()))   
      return; 
  }
  if (timer > millis())  timer = millis();
  if (millis() - timer > 250) {
    timer = millis(); 
    myFile = SD.open("etslog.txt", FILE_WRITE);
    // sends latitude, longitude, altitude, tracking angle and speed; if no GPS sends {0,0,0,0,0}
    if (GPS.fix) 
    {
      Serial1.print("#ETS{");
      myFile.print("#ETS{");
      // reads latitude and longitude; converts to decimal degrees
      lat=GPS.latitude;  
      deg=int (lat/100);
      minu=int (lat-(deg*100));
      sec=((float) (lat - int(lat)))*60;
      dec_coord= (float) deg + (float) minu/60 + (float) sec/3600;
    if (GPS.lat=='S')
     {
      dec_coord=dec_coord*-1;
     }
      Serial1.print(dec_coord, 8);
      myFile.print(dec_coord, 8);
      Serial1.print(",");
      myFile.print(",");
      // reads other GPS data
      lon=GPS.longitude;
      deg=int (lon/100);
      minu=int (lon-(deg*100));
      sec=((float) (lon - int(lon)))*60;
      dec_coord= (float) deg + (float) minu/60 + (float) sec/3600;
    if (GPS.lon=='W')
     {
      dec_coord=dec_coord*-1;
     }
      Serial1.print(dec_coord, 8);
      myFile.print(dec_coord, 8);      
      Serial1.print(",");
      myFile.print(",");
      digit_corr(GPS.altitude,s,4,2);
      Serial1.print(s);
      myFile.print(s);
      Serial1.print(",");
      myFile.print(",");
      digit_corr(GPS.angle,s,4,2);
      Serial1.print(s);
      myFile.print(s);
      Serial1.print(",");
      myFile.print(",");
      digit_corr((GPS.speed),s,3,2); //converts knots to km/h doing *1.852
      Serial1.print(s);
      myFile.print(s);
      Serial1.print("}");
      myFile.print("}");
    }
      else {
      Serial1.print("ETS{0,0,0,0,0}");
      myFile.print("ETS{0,0,0,0,0}");
      }
      // reads pressure, temperature, humidity and battery voltage; calculates altitude
      analog2=analogRead(A2);
      pressure_MPX=((analog2/float(1023)+0.095)/0.009)*10;// *10 to convert to hPa
      Serial1.print(";P:"); 
      myFile.print(";P:");
      digit_corr(pressure_MPX,s,4,2);
      Serial1.print(s);
      myFile.print(s);
      Serial1.print(";T:");
      myFile.print(";T:");
      analog4=analogRead(A4);
      temp=(((float)analog4*5)/1023)/(0.010)-3.53;
      digit_corr(temp,s,3,1);
      Serial1.print(s);
      myFile.print(s);
      //myFile.print(";TH:");
      //nd(DHT.temperature,s,3);
      //myFile.print(s);      
      altitude=((T0+273.15)/z)*(1-(pow(pressure_MPX/P0,(z*R)/g)))+H0; // calculates altitude in meters
      Serial1.print(";H:");
      myFile.print(";H:");
      digit_corr(altitude,s,4,2);
      Serial1.print(s);
      myFile.print(s);
      DHT.read11(dht_dpin);
      Serial1.print(";HR:");
      myFile.print(";HR:");
      nd(DHT.humidity,s,3);
      Serial1.print(s);   
      myFile.print(s);  
      Serial1.print(";Time:");
      myFile.print(";Time:");
      nd(t,s,6);
      Serial1.print(s);
      myFile.print(s);
      //analizes battery voltage
      analog6 =analogRead(A6);
      voltage=((((float)analog6*5)/1023)*9)/5;
      Serial1.print(";V:");
      myFile.print(";V:");
      digit_corr(voltage,s,1,2);
      Serial1.print(voltage);
      myFile.print(voltage);
      Serial1.println("#");
      myFile.println("#");
      myFile.close();
      Serial1.flush();
   count++;   
      if (altitude>100)
   {
     h=1;
   }
   if ((altitude)<100 && (h==1)); 
    remainder = t%2;
     if (remainder==0 && h==1)
     {
     tone (buzzer,1800);
     }
    else noTone(buzzer);
    // evaluates counter and after 4 times (4*250=1s) increments timestamp in one second; also restarts counting
    if (count==5)
    {
     count=1;
     t=t+1;
    }
}
}
