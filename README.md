## LoRaGPS

Sending GPS coordinates via LoRa Radio 433MHz and 868MHz

Please look here for more documentation:
https://aconfluence.no/x/BgA5AQ

### LoRaGPS/Intro

- As of now this project is only to learn how to use LoRa modules and to have radio devices to test antennas. For now I only use fabricated antennas but I will build my own.
- End goal for this project is to control an antenna tracker, this used as an telemetry link.

### Core Technical Concepts/Inspiration

- I was not able to find a userfriendly telemetry link so I started to build my own and to learn the technology. 
- This project are so far not really userfriendly either but the goal is to try to create a step by step user guide with a complete parts list and "How to" bullet list.

### Getting Started/Requirements/Prerequisites/Dependencies
- Arduino Nano - https://www.banggood.com/ATmega328P-Nano-V3-Controller-Board-Compatible-Arduino-p-940937.html
- LoRa modules - https://www.hoperf.com/data/upload/portal/20190301/RFM98W.pdf
- LoRa modules break out boards - https://github.com/lyusupov/SoftRF/wiki/SoftRF-LoRa-module
- GPS - https://www.banggood.com/UBLOX-NEO-M8N-BN-880-Flight-Control-GPS-Module-Dual-Module-Compass-p-971082.html
- Display - https://www.banggood.com/3Pcs-0_96-Inch-4Pin-IIC-I2C-Blue-OLED-Display-Module-For-Arduino-p-1162507.html

Arduino library used:
- SPI
- RH_RF95
- U8glib
- TinyGPS++
- SoftwareSerial (Temporarily)

### Contributing
- No other than me are contributing so far and no guide lines are specified.

### TODO
- Basic function are complete but code optimization is needed.
- Next step is to be able to control an antenna tracker with telemetry link.
- None known bugs as of now

### Contact
- Email address: sbsivertsen@gmail.com
- Twitter: @StigSivertsen

### License
This project is under GNU General Public License v3.0
# LoraCodeOnly
