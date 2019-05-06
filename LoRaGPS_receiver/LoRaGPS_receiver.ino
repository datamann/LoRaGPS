// Modified by Stig Sivertsen
#include <SPI.h>
#include <RH_RF95.h>
#include "U8glib.h"

RH_RF95 rf95;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  // Fast I2C / TWI

struct dataToSend {
  long lat;
  long lng;
  int sats;
}DataToSend;
dataToSend dataToReceive;

void setup() 
{ 
  u8g.setColorIndex(1);
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("Init failed");
  else
    Serial.println("Init succeeded");
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
//  driver.setTxPower(14, true);
}

unsigned long lastScreenUpdate = 0;
char printSATSToScreen[8];
char printLATToScreen[15];
char printLNGToScreen[15];
char printRSSIToScreen[16];
char sats[3];
char lat[10];
char lng[10];
String strLat;
String strLng;
String strSats;
String recvStr;
int intRSSI;
int intSNR;

char* spinner = "/-\\|";
byte screenRefreshSpinnerPos = 0;
byte gpsUpdateSpinnerPos = 0;

void loop()
{
  //String printToScreen;
  screenRefreshSpinnerPos = (screenRefreshSpinnerPos + 1) % 4;
  updateScreen();
  
  if (rf95.available())
  {
    gpsUpdateSpinnerPos = (gpsUpdateSpinnerPos + 1) % 4;  
    
    // Should be a message for us now   
    uint8_t data[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(data);
    
    if (rf95.recv(data, &len))
    {
      recvStr = (char*)data;

      dataToSend* dataToReceive = (dataToSend*)data;
        char buf[len];
        sprintf(buf, "%d %d %ld", dataToReceive->lat, dataToReceive->lng, dataToReceive->sats);
        Serial.println(String("Buf:") + buf);
      
      strLat = recvStr.substring(0,9);
      strLat.toCharArray(lat, 10);
      
      strLng = recvStr.substring(10,19);
      strLng.toCharArray(lng, 10);
      
      strSats = recvStr.substring(20,22);
      strSats.toCharArray(sats, 3);
      
      intRSSI = rf95.lastRssi(), DEC;
      intSNR = rf95.lastSNR(), DEC;

      Serial.println("Lat: " + strLat);
      Serial.println("Lng: " + strLng);
      Serial.println(String("Sats: ") + (char*)sats);
      Serial.println(String("RSSI: ") + intRSSI);
      Serial.println(String("SNR: ") + intSNR);
      updateScreen();
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}

void draw() {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 2, 10, printSATSToScreen);
  u8g.drawStr( 2, 25, printRSSIToScreen);
  u8g.drawStr( 2, 40, printLATToScreen);
  u8g.drawStr( 2, 55, printLNGToScreen);
}

void updateScreen() {

  sprintf(printRSSIToScreen, "Rssi:%i Snr:%i", intRSSI, intSNR);
  sprintf(printLATToScreen, "Lat:%s", lat);
  sprintf(printLNGToScreen, "Lng:%s", lng);
  sprintf(printSATSToScreen, "%c %c %s", spinner[screenRefreshSpinnerPos], spinner[gpsUpdateSpinnerPos], (char*)sats);  
  u8g.firstPage();
  do {
    draw();
  } while( u8g.nextPage() );
}
