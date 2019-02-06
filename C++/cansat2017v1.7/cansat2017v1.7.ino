/*ENTA Team SAT3 flight software code v.5-20
Includes procedures to get information from all sensors, builds the data string and sends
IMU values int. Includes string number and CRC*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_Simple_AHRS.h>
#include <SD.h>
#include <Crc16.h>


Adafruit_GPS GPS(&Serial2);
#define GPSECHO  true
boolean usingInterrupt = false;
void useInterrupt(boolean);

float temp, pressure, altitude, hum, roll, pitch, heading, x, y, z, voltage, real_voltage,
      lat, lon, sec_lat, sec_lon, dec_lat, dec_lon;
String s_temp, s_pressure, s_altitude, s_hum, s_roll, s_pitch, s_heading, s_x, s_y, s_z, s_dec_lat, s_dec_lon, sat_data, 
       BME280_status, DOF_status, s_volts, s_power_origin, buzz, s_lat, s_lon;
int rate, i=0, set_buzzer=0, remainder, h, t,  deg_lat, deg_lon, minu_lat, minu_lon, lnumb=0, count_loops;
const int POT=A8, BUZZER=3, SD_CS=27, BME_vcc=4, DOF_vcc=5;
char o;
const float T0 = 19.0, P0 = 1004,  H0 = 45; // values to be uploaded before each mission + etslog
const float Z = 0.0065, R = 287.06, g = 9.81;

Adafruit_BME280 bme;
File myFile;
Crc16 crc;

/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

Adafruit_Simple_AHRS          ahrs(&accel, &mag);

//******************************************************************
//setup declaration begin
void setup()
{
  initSensors();
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  SD.begin(SD_CS);
  pinMode(BUZZER, OUTPUT);
  pinMode(BME_vcc, OUTPUT);
  pinMode(DOF_vcc, OUTPUT);
  digitalWrite(BME_vcc, HIGH);
  digitalWrite(DOF_vcc, HIGH);
  BME280_status = "ON";
  DOF_status = "ON";
  t = 0; h = 0; o = ' '; set_buzzer = 0; buzz = "0", rate = 500;
  altitude=H0;
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

//setup declaration end
//***********************************************************
//begin main program
void loop()
{
  if (timer > millis())  timer = millis();
  if (millis() - timer > rate) {
    timer = millis();
    bme.begin();
    sat_data="ETS3#";//starts data string with team code
    getTempPressHumAlt();// calls for procedure
    getVoltage();// calls for procedure
    getIMUdata();// calls for procedure
    getGPScoord();// calls for procedure
    buzz = (String)set_buzzer;
    //evaluates altitude for buzzer activation
    if (altitude > 200 && altitude <1500) h = 1;
    if ((altitude < 200) && (h == 1)) activateBuzzer(); // calls for procedure if bellow 200 meters
    //test buzzer if ground comand 
    if (o == 'B') testBuzzer();
    
    //create data string and calculate CRC
    sat_data += (String)lnumb + ";" + s_temp + s_pressure + s_altitude + s_hum + buzz + ";" + BME280_status + ";" 
      + s_volts + s_power_origin + s_roll + s_pitch + s_heading + s_x + s_y + s_z + DOF_status + ";"
        + String (dec_lat,6) + ";" + String (dec_lon,6) + "%";
    
    byte data[150];
    int lenstr=sat_data.length();
    for(int j=0;j<lenstr;j++)
      data[j]=sat_data.charAt(j);

    crc.clearCrc();
    for(byte i=0;i<lenstr;i++)
    {
     crc.updateCrc(data[i]);
    }
    unsigned short value = crc.getCrc();
    //unsigned short value = calcrc((char*)data,lenstr);
    //sat_data+=String (value,HEX);
    //send data string with CRC to ground stations   
    //Serial1.print(sat_data);
    Serial.print(sat_data);Serial.println(value,HEX);
    
    //save datas string in SD card
    myFile = SD.open("ets3log.txt", FILE_WRITE);
    myFile.print(sat_data); myFile.print(dec_lat, 8); myFile.print(";"); myFile.println(dec_lon, 8);
    myFile.close();
    
    lnumb++;
    count_loops++;
    if (count_loops == 2)
    {
      count_loops = 0;
      t = t + 1;
    }
  }
  //search for ground commands
  if ( Serial1.available () )
  {
    o = Serial1.read();
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
      digitalWrite(DOF_vcc, 0);
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
      noTone(BUZZER);
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
//end of main program
//*************************************************************************************************************************
//function and procedure code
void initSensors()
{
  bme.begin();
  accel.begin();
  mag.begin();
  Wire.begin();
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
}
void getTempPressHumAlt()
{
  if (BME280_status == "ON")
  {
    temp=bme.readTemperature()-3;
    s_temp=(String)temp;
    s_temp.replace(".", ",");
    s_temp+=";";
    pressure=bme.readPressure()/100;
    s_pressure=(String)pressure;
    s_pressure.replace(".", ",");
    s_pressure+=";";
    altitude=((T0+273.15)/z)*(1-(pow(pressure/P0,(Z*R)/g)))+H0;
    s_altitude=(String)altitude;
    s_altitude.replace(".", ",");
    s_altitude+=";";
    hum=bme.readHumidity();
    s_hum=(String)hum;
    s_hum.replace(".", ",");
    s_hum+=";";
  }
  if (BME280_status == "OFF")
  {
    s_pressure = "0;";
    s_temp = "0;";
    s_altitude = "0;";
    s_hum = "0;";
  }
}
void getVoltage()
{
  voltage = (((float)analogRead(A10)*5)/1023);
  if (voltage>2)
  {
    real_voltage = (voltage * 10) / 5;
    s_volts = (String) real_voltage;
    s_volts.replace(".", ",");
    s_volts += ";";
    s_power_origin = "BAT;";
  }
  else
  {
    s_volts = "5,00;";
    s_power_origin = "USB;";
  }
}
void getIMUdata()
{
  if (DOF_status == "ON")
  {
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_vec_t   orientation;
    sensors_event_t event;
    accel.getEvent(&accel_event);

    if (ahrs.getOrientation(&orientation))
    {
      roll = (orientation.roll);
      s_roll = (String)roll;
      s_roll.replace(".", ",");
      s_roll += ";";
      pitch = (orientation.pitch);
      s_pitch = (String)pitch;
      s_pitch.replace(".", ",");
      s_pitch += ";";
      heading = (orientation.heading);
      s_heading = (String)heading;
      s_heading.replace(".", ",");
      s_heading += ";";
    }
    accel.getEvent(&event);
    x = event.acceleration.x;
    s_x = (String)x;
    s_x.replace(".", ",");
    s_x += ";";
    y = event.acceleration.y;
    s_y = (String)y;
    s_y.replace(".", ",");
    s_y += ";";
    z = event.acceleration.z;
    s_z = (String)z;
    s_z.replace(".", ",");
    s_z += ";";
  }
  if (DOF_status == "OFF")
  {
    s_roll = "0;";
    s_pitch = "0;";
    s_heading = "0;";
    s_x = "0;";
    s_y = "0;";
    s_z = "0;";
  }
}
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
}
void activateBuzzer()
{
  set_buzzer = 1;
  remainder = t % 2;
  if ((remainder == 0) && (h == 1))
    digitalWrite(BUZZER, HIGH);
  else
    digitalWrite(BUZZER, LOW);
}
void testBuzzer()
{
  set_buzzer = 1;
  remainder = t % 2;
  if (remainder == 0)
    digitalWrite(BUZZER, HIGH);
  else
    digitalWrite(BUZZER, LOW);

}
int calcrc(char *ptr, int count)
{
    int  crc;
    char i;
    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    return (crc);
}
