#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

float lat=0, lon=0, sec_lat=0, sec_lon=0, dec_lat=0, dec_lon=0, prev_dec_lat=0, prev_dec_lon=0;
String s_dec_lat, s_dec_lon, sat_data;
int deg_lat=0, deg_lon=0, minu_lat=0, minu_lon=0;

SoftwareSerial mySerial(3, 2);

Adafruit_GPS GPS(&mySerial);

#define GPSECHO  true

boolean usingInterrupt = false;
void useInterrupt(boolean); 

void getGPScoord()
{
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
    if(dec_lat==0.000000)
    {
      dec_lat=prev_dec_lat;
    }
    prev_dec_lat=dec_lat;
    
    lon = GPS.longitude;
    deg_lon = int (lon / 100);
    minu_lon = int (lon - (deg_lon * 100));
    sec_lon = ((float) (lon - int(lon))) * 60;
    dec_lon = (float) deg_lon + (float) minu_lon / 60 + (float) sec_lon / 3600;
    if (GPS.lon == 'W')
    {
      dec_lon = dec_lon * -1;
    }
    if(dec_lon==0.000000)
    {
      dec_lon=prev_dec_lon;
    }
    prev_dec_lon=dec_lon;
  }
  else
  {
    dec_lat = 0;
    dec_lon = 0;
  }
}

void setup()  
{
  Serial.begin(9600);
  //Serial.println("Adafruit GPS library basic test!");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt(true);
  delay(1000);
  //mySerial.println(PMTK_Q_RELEASE);
}
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
//#ifdef UDR0
  //if (GPSECHO)
   // if (c) UDR0 = c;   
//#endif
}

void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

void loop()                
{
  /*if (! usingInterrupt) {
    char c = GPS.read();
    if (GPSECHO)
      if (c) 
      Serial.print(c);
  }*/
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return; 
  }

  if (timer > millis())  timer = millis();

  if (millis() - timer > 1000) { 
    timer = millis(); 
    sat_data="ETS3#";//starts data string with team code
    getGPScoord();// calls for procedure
    //create data string
    sat_data += String(dec_lat,6) + ";" + String(dec_lon,6);
    //send data to USB
    Serial.println(sat_data);
    /*Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);*/
    }
  }
