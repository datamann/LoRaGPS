// Modified by Stig Sivertsen
//#include <SPI.h>
#include <RH_RF95.h>

// Lookup RH_RF95.h for pin Slave Select and pin interrupt ovveride.

RH_RF95 rf95;
char sats[4] = "***";

struct dataToSend {
  double lat;
  double lng;
  char sats;
};
dataToSend DataToSend;

void setup()
{

  Serial.begin(9600);
  Serial.println("Setup!");
  while (!Serial) ; // Wait for serial port to be available
    if (!rf95.init())
      Serial.println("Init sender failed");
    else
      Serial.println("Init sender succeeded");

  rf95.setFrequency(868.00);
  rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  //  driver.setTxPower(23, false);
  rf95.setTxPower(13, false);
  
  uint8_t data[] = "Sender started";
  rf95.send(data, sizeof(data));  
  rf95.waitPacketSent();
}

unsigned long lastSendTime = 0;
void loop()
{
  unsigned long now = millis();
  if ( now - lastSendTime >= 1000 ) {
    
    DataToSend.lat = 56.123456;
    DataToSend.lng = 57.123456;
    DataToSend.sats = '13';
    
    rf95.send((uint8_t *)&DataToSend, sizeof(DataToSend));
    rf95.waitPacketSent();
    
    char lat[10];
    char lng[10];      
    dtostrf(DataToSend.lat, 0, 6, lat);
    dtostrf(DataToSend.lng, 0, 6, lng);
        
    char buf[45];
    //sprintf(buf,"%s %s %c", lat, lng, char(DataToSend.sats));
    sprintf(buf,"LAT: %s\, LNG: %s\, Sats: %c", lat, lng, char(DataToSend.sats));
    Serial.println(String("Buf: ") + buf);
    lastSendTime = now;
  }
}
