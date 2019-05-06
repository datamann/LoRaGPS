// Modified by Stig Sivertsen
#include <SPI.h>
#include <RH_RF95.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h> // Will use serial when all code changes are done.

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial GPS_PORT(RXPin, TXPin);

RH_RF95 rf95;
char sats[4] = "***";

struct dataToSend {
  double lat;
  double lng;
  int sats;
}DataToSend1;
dataToSend DataToSend;

void setup() 
{
  Serial.begin(9600);
  GPS_PORT.begin(GPSBaud);
  
  rf95.init();
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  rf95.setFrequency(433.00);
  rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  //  driver.setTxPower(23, false);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
  // Failure to do that will result in extremely low transmit powers.
  // driver.setTxPower(14, true);

  uint8_t data[] = "Sender started";
  rf95.send(data, sizeof(data));  
  rf95.waitPacketSent();
}

void loop()
{
  while (GPS_PORT.available() > 0) // Wait for serial port to be available
    if (gps.encode(GPS_PORT.read()))
        displayInfo();
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  
  /*static unsigned long lastSendTime = 0;
  unsigned long now = millis();
  uint8_t data[] = "Sender started";

  if ( now - lastSendTime > 1000 ) {    
    rf95.send((uint8_t*)&data, sizeof(data));
    rf95.waitPacketSent();  
    lastSendTime = now;
  }else{
    uint8_t data[] = "No GPS";
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
  }*/
}

void displayInfo()
{
  static unsigned long lastSendTime = 0;
  unsigned long now = millis();
  
    if (gps.location.isValid() && now - lastSendTime > 1000){

      getSats(gps.satellites.value(), gps.satellites.isValid());
      DataToSend.lat = gps.location.lat();
      DataToSend.lng = gps.location.lng();
      DataToSend.sats = sats;
      
      smartDelay(500);
      String latlng = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," + sats;

      /* Code to go on receive end */
      //char buff[sizeof(DataToSend)];
      char buflatToString[10];
      char buflngToString[10];
      char latToString[10];
      char lngToString[10];
      /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
      dtostrf(DataToSend.lat, 2, 6, latToString);
      sprintf(buflatToString,"%s", latToString);
      dtostrf(DataToSend.lng, 2, 6, lngToString);
      sprintf(buflngToString,"%s", lngToString);
      
      //sprintf(buff, "%l %lf %ld", DataToSend.lat, DataToSend.lng, DataToSend.sats);
      Serial.println(String("Buf:") + buflatToString + "-" + buflngToString);

      //double latt=gps.location.lat(); 
      Serial.println(DataToSend.lat, 6);
      Serial.println(DataToSend.lng, 6);
      Serial.println(DataToSend.sats);
      
      byte buf[31];
      //latlng.toCharArray(buf,31);
      //DataToSend.toCharArray(buf,64);
      //char [] buffer = new char[sizeof(DataToSend)];
      //char [] buffer = new char[sizeof(DataToSend)];
      //memcpy(&buffer, &DataToSend, sizeof(DataToSend));
     
      //if (rf95.available()){
        rf95.send((uint8_t *)&DataToSend, sizeof(DataToSend));
        rf95.waitPacketSent();
        lastSendTime = now;
      //}else{
      //  Serial.println("LoRa Radio not available!");
      //}
      
      //unsigned long sats = gps.satellites.value();
      //String sats = String(gps.satellites.value(), 6);
      /*Serial.print(gps.location.lat(), 6);
      Serial.print(F(","));
      Serial.print(gps.location.lng(), 6);
      Serial.print(F(","));
      String test = sats + String("."); 
      Serial.print(test);*/
      //Serial.print(F(","));
      //Serial.print(gps.hdop.value());
      //smartDelay(500);      
    }else{
      
      Serial.print(F("INVALID"));
    }
  Serial.println();
}

static void getSats(unsigned long val, bool valid)
//static void getSats(int val, bool valid)
{
  int len = 3;
  if (valid)
      sprintf(sats, "%ld", val);
    sats[len] = 0;
    for (int i=strlen(sats); i<len; ++i)
      sats[i] = ' ';
    if (len > 0) 
      sats[len-1] = ' ';
    //Serial.print(sats);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_PORT.available())
      gps.encode(GPS_PORT.read());
  } while (millis() - start < ms);
}
