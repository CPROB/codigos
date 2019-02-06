/*ENTA Team SAT3 flight software code v.1-2017
This version integrates 9DOF IMU*/
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
#include <FreqCount.h>

Adafruit_GPS GPS(&Serial2);
#define GPSECHO  true
boolean usingInterrupt = false;
void useInterrupt(boolean);

float temp, pressure, altitude, hum, roll, pitch, heading,x,y,z, voltage, real_voltage, lat, lon, sec_lat, sec_lon, dec_lat, dec_lon;
String s_temp, s_pressure, s_altitude, s_hum, s_roll, s_pitch, s_heading, s_x, s_y, s_z, s_dec_lat, s_dec_lon, sat_data, BME280_status,DOF_status, s_volts, buzz;
int rate, i=0, buzzer=3, set_buzzer=0, remainder, h, t, SD_CS=27, deg_lat, deg_lon, minu_lat, minu_lon, BME_vcc=4, DOF_vcc=5, lnumb=0, count_loops;
char o;
const float T0=25.0, P0=1013,  H0=45; // values to be uploaded before each mission + etslog
const float Z=0.0065, R=287.06, g=9.81;

Adafruit_BME280 bme;

File myFile;

/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

//******************************************************************
void setup()
{
  Serial.begin(9600); 
  Serial1.begin(9600);
  bme.begin();
  Wire.begin();
  accel.begin();
  mag.begin();
  SD.begin(SD_CS);
  pinMode(buzzer, OUTPUT);
  pinMode(BME_vcc, OUTPUT);
  pinMode(DOF_vcc, OUTPUT);
  digitalWrite(BME_vcc, HIGH);
  digitalWrite(DOF_vcc, HIGH);
  BME280_status = "ON";
  DOF_status = "ON";
  //pinMode(SS, OUTPUT);
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
  bme.begin();
  accel.begin();
  mag.begin();
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
  if (timer > millis())  timer = millis();
  if (millis() - timer > rate) {
    timer = millis();
    sat_data = "ETS3#";
    if (BME280_status == "ON")
    {
      temp = bme.readTemperature()-3;
      s_temp = (String)temp;
      s_temp.replace(".", ",");
      pressure = bme.readPressure() / 100;
      s_pressure = (String)pressure;
      s_pressure.replace(".", ",");
      altitude = ((T0 + 273.15) / z) * (1 - (pow(pressure / P0, (Z * R) / g))) + H0;
      s_altitude = (String)altitude;
      s_altitude.replace(".", ",");
      hum = bme.readHumidity();
      s_hum = (String)hum;
      s_hum.replace(".", ",");
    }
    if (BME280_status == "OFF")
    {
      s_pressure = "0";
      s_temp = "0";
      s_altitude = "0";
      s_hum = "0";
    }
    sat_data = sat_data + s_temp + ";" + s_pressure + ";" + s_altitude + ";" + s_hum + ";";
    
    buzz = (String) set_buzzer;
    
    sat_data = sat_data + buzz + ";" + BME280_status + ";";
    
    voltage = (((float)analogRead(A9) * 5) / 1023);
    real_voltage = (voltage * 7.8)/5;
    s_volts=(String) real_voltage;
    s_volts.replace(".", ","); 
    
    sat_data = sat_data + s_volts + ";";
    
    if (DOF_status == "ON")
    {
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_vec_t   orientation;
    sensors_event_t event;
    accel.getEvent(&accel_event);
    if (dof.accelGetOrientation(&accel_event, &orientation))
      {
      roll=(orientation.roll);
      s_roll=(String)roll;
      s_roll.replace(".",",");
      pitch=(orientation.pitch);
      s_pitch=(String)pitch;
      s_pitch.replace(".",",");
      }
    mag.getEvent(&mag_event);
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
      {
      heading=(orientation.heading);
      s_heading=(String)heading;
      s_heading.replace(".",",");
      }
      accel.getEvent(&event);
      x=event.acceleration.x;
      s_x=(String)x;
      s_x.replace(".",",");
      y=event.acceleration.y;
      s_y=(String)y;
      s_y.replace(".",",");
      z=event.acceleration.z;
      s_z=(String)z;
      s_z.replace(".",",");
      }
    if (DOF_status == "OFF")
      {
      s_roll="0"; 
      s_pitch="0"; 
      s_heading="0"; 
      s_x="0"; 
      s_y="0"; 
      s_z="0"; 
      }

    sat_data = sat_data + s_roll + ";" + s_pitch + ";" + s_heading + ";" + s_x + ";" + s_y + ";" + s_z + ";" + DOF_status + ";";
    
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
    if (altitude > 200) h=1;
    if ((altitude < 200) && (h == 1)) //evaluates altitude and if CanSat is descending
    { set_buzzer = 1;
      remainder = t % 2;
      if ((remainder == 0) && (h == 1))
        digitalWrite(buzzer, HIGH);
      else
        digitalWrite(buzzer, LOW);
    }
     if (o == 'B')
     {
     set_buzzer = 1;
     remainder = t % 2;
     if (remainder == 0)
        {
          digitalWrite(buzzer, HIGH);
        }
      else
        digitalWrite(buzzer, LOW);
      }
    Serial1.print(sat_data); Serial1.print(dec_lat, 8); Serial1.print(";"); Serial1.println(dec_lon, 8);
    Serial.print(sat_data); Serial.print(dec_lat, 8); Serial.print(";"); Serial.println(dec_lon, 8);
    myFile = SD.open("ets3log.txt", FILE_WRITE);
    myFile.print(sat_data); myFile.print(dec_lat, 8); myFile.print(";"); myFile.println(dec_lon, 8);
    myFile.close();
    count_loops++;
    if (count_loops == 2)
    {
      count_loops = 0;
      t = t + 1;
    }
  }
if ( Serial.available () ) //search for ground commands
  {
    o = Serial.read();
    if (o == 'D') { // if D turn BMP280 off
      digitalWrite(BME_vcc, LOW);
      BME280_status = "OFF";
    }
    if (o == 'L') { // if L turn BMP2800 on
      digitalWrite(BME_vcc, HIGH);
      delay(15);
      bme.begin();
      BME280_status = "ON";
    }
    if (o == 'C') {
      digitalWrite(DOF_vcc, LOW);
      DOF_status = "OFF";
    }
    if (o == 'A') {
      digitalWrite(DOF_vcc, HIGH);
      delay(15);
      DOF_status = "ON";
      accel.begin();
      mag.begin();
    }    
    if (o == 'S') { // if S turn buzzer off
      digitalWrite(buzzer, LOW);
      set_buzzer = 0;
    }
    if (o == 'Q') { // if Q sets data send rate to 500ms
      rate = 500;
    }
    if (o == 'M') { // if M sets data send rate to 2000ms
      rate = 2000;
    }
  }
}
    
    

  

