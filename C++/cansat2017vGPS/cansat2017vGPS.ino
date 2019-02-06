/*ENTA Team SAT3 flight software code v.1-2017*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <SD.h>

Adafruit_GPS GPS(&Serial2);
#define GPSECHO  true
boolean usingInterrupt = false;
void useInterrupt(boolean);

float temp, pressure, altitude, hum, roll, pitch, heading, voltage, real_voltage, lat, lon, sec_lat, sec_lon, dec_lat, dec_lon;
String s_temp, s_pressure, s_altitude, s_hum, s_roll, s_pitch, s_heading, s_dec_lat, s_dec_lon, sat_data, sensor_status, s_volts, buzz;
int rate, i=0, buzzer=3, set_buzzer=0, remainder, h, t, SD_CS=27, deg_lat, deg_lon, minu_lat, minu_lon, BMP_vcc=4, lnumb=0, count_loops;
char o;
const float T0=25.0, P0=1013,  H0=45; // values to be uploaded before each mission + etslog
const float z=0.0065, R=287.06, g=9.81;



//File myFile;

/* Assign a unique ID to the sensors */

//******************************************************************
void setup()
{
  Serial.begin(9600); 
  Serial1.begin(9600);
  t=0; h=0; o=' '; set_buzzer=0; buzz="0", rate=500;
  GPS.begin(9600);
  GPS.sendCommand("$PMTK251,19200*22");
  GPS.sendCommand("$PMTK220,100*2F");
  Serial2.end();
  GPS.begin(19200);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_NOANTENNA);
  useInterrupt(false);
  delay(1000);
  Serial2.println(PMTK_Q_RELEASE);
}
SIGNAL(TIMER0_COMPA_vect) {
  while (Serial2.available())
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


//***********************************************************
void loop()
{
  if (! usingInterrupt)
  {
   while (Serial2.available())
    {
      char c = GPS.read();
      //if (GPSECHO)
      //if (c) Serial3.print(c);

    }
  }
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }
  if (timer > millis())  
    timer = millis();
  if (millis() - timer > rate) 
  {
    timer = millis();
    
    if (GPS.fix)
    {
      lat = GPS.latitude;
      deg_lat = int (lat / 100);
      minu_lat = int (lat - (deg_lat * 100));
      sec_lat = ((float) (lat - int(lat))) * 60;
      dec_lat = (float) deg_lat + (float) minu_lat / 60 + (float) sec_lat / 3600;
      if (GPS.lat == 'S')
      {
        dec_lat = dec_lat * -1;
      }
      lon = GPS.longitude;
      deg_lon = int (lon / 100);
      minu_lon = int (lon - (deg_lon * 100));
      sec_lon = ((float) (lon - int(lon))) * 60;
      dec_lon = (float) deg_lon + (float) minu_lon / 60 + (float) sec_lon / 3600;
      if (GPS.lon == 'W')
      {
        dec_lon = dec_lon * -1;
      }
    }
    else
    {
      dec_lat = 0;
      dec_lon = 0;
    }
   Serial.print(dec_lat, 8); Serial.print(";"); Serial.println(dec_lon, 8);
  }
}
