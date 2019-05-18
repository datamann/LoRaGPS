// Modified by Stig Sivertsen
//#include <SPI.h>
#include <RH_RF95.h>

// Lookup RH_RF95.h for pin Slave Select and pin interrupt ovveride.

RH_RF95 rf95;

struct dataToReceive {
  double lat;
  double lng;
  char sats;
};
dataToReceive DataToReceive;

char sats[5];
char lat[10];
char lng[10];

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
    if (!rf95.init())
      Serial.println("Init receiver failed");
    else
      Serial.println("Init receiver succeeded");

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  rf95.setFrequency(868.00);
  rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  //  driver.setTxPower(23, false);
  rf95.setTxPower(13, false);
}

int intRSSI;
int intSNR;
unsigned long lastSendTime = 0;

void loop(){

  unsigned long now = millis();
  if ( now - lastSendTime >= 1000 ) {
    if (rf95.available()){
      
      // Should be a message for us now
      uint8_t data[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(data);

      if (rf95.recv(data, &len)){
        dataToReceive* DataToReceive = (dataToReceive*)data;
        dtostrf(DataToReceive->lat, 0, 6, lat);
        dtostrf(DataToReceive->lng, 0, 6, lng);
        sprintf(sats,"%c", DataToReceive->sats);
        intRSSI = rf95.lastRssi(), DEC;
        intSNR = rf95.lastSNR(), DEC;
      
        char buf[66];
        sprintf(buf,"LAT: %s\, LNG: %s\, Sats: %s\, RSSI: %i\, SNR: %i", lat, lng, sats, intRSSI, intSNR);
        Serial.println(String("Buf: ") + buf);
      }
      else
      {
        Serial.println("recv failed");
      }
    }
    else{
      Serial.println("RF95 not available!");
    }    
    lastSendTime = now;
  }
}
