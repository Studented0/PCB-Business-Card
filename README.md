[PCB Business Card-bom.csv](https://github.com/user-attachments/files/27329266/PCB.Business.Card-bom.csv)
# PCB-Business-Card
 
A PCB Business card that shows your Spotify activity using lanyard in real time. Tap the card with a phone to show what song is playing on Spotify.
 
Powered over USB-C from a phone or any power bank. Connects to a phone hotspot (or any wifi) for the Lanyard WebSocket. 
 
## Screenshots
<img width="1084" height="756" alt="image" src="https://github.com/user-attachments/assets/a8d840ab-513b-43b4-9d26-ffe24e27af58" />

<img width="1724" height="978" alt="lanyardpcbcard" src="https://github.com/user-attachments/assets/72dcc8d4-80f3-416c-8f40-d92322912cd9" />




## Why I built this
 
I wanted a business card that has a cool twist. Plain cards feel boring to show and they are all the same. This one shows what I'm listening to, lets someone tap it to hear the same song, and has my GitHub a QR scan away. Business cards are supposed to grab people's attention, and I would say this one is different than other business cards.
 
## Hardware
 The ESP32 rewrites the NDEF URL every time a track changes over I2C, NT3H2111 (NFC Module) has I2C and RF interface simultaneously. When you are playing something NFC goes to spotify when nothing is playing goes to GitHub (for now). 100x75mm PCB. USB-C powers it with any power bank (im personally planning to use my phone). ESP32 is going to be hand soldered to save money but everything else is JLCPCB assembly 
 
## Wiring
 
| Signal | GPIO |
|--------|------|
| DISP_MOSI | IO11 |
| DISP_SCLK | IO12 |
| DISP_CS | IO10 |
| DISP_DC | IO13 |
| DISP_RST | IO14 |
| DISP_BLK | IO9 |
| NFC_SDA | IO5 |
| NFC_SCL | IO6 |
| NFC_FD | IO4 |
| USB_DN | IO19 |
| USB_DP | IO20 |
| BOOT | IO0 |
| RESET | EN |
 
## Firmware
 
`firmware/main.ino` 
 
Libraries needed: TFT_eSPI, TJpg_Decoder, ArduinoWebsockets, ArduinoJson. Board: ESP32S3 Dev Module.
 
## Bill of Materials
 
SMD parts assembled by JLCPCB. ESP32 and the display are sourced separately.
 
| Component | Qty | LCSC | Note |
|-----------|-----|------|------|
| 100nF 0402 cap | 3 | C1525 | Basic |
| 10µF 0805 cap | 2 | C15850 | Basic |
| 56pF 0402 cap | 1 | C107007 | Extended — antenna tuning |
| 5.1kΩ 0402 resistor | 2 | C25905 | Basic — USB-C CC |
| 10kΩ 0402 resistor | 3 | C25744 | Basic — pullups |
| 4.7kΩ 0402 resistor | 2 | C25900 | Basic — I2C pullups |
| AMS1117-3.3 SOT-223 | 1 | C6186 | Basic |
| NT3H2111W0FHKH XQFN-8 | 1 | C710403 | Extended — NFC |
| TYPE-C 16PIN 2MD | 1 | C2765186 | Extended — USB-C |
| Tactile switch SMD | 2 | — | Extended |
 Name,Purpose,Quantity,Total Cost (USD),Link,Distributor
"Display","Displaying",1,14.00,"https://www.amazon.com/gp/product/B0GL1SWYP4/ref=ox_sc_act_title_1?smid=A2Z10KY0342329&psc=1","Amazon"
"ESP32-S3-WROOM-1-N4","microcontroller",1,5.21,"https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-WROOM-1-N4/16162639","DIGIKEY"
"JLCPCB","PCB",1,25.59,"","JLCPCB"
| Component | Qty | Source |
|-----------|-----|--------|
| ESP32-S3-WROOM-1-N4 | 1 | DigiKey 1965-ESP32-S3-WROOM-1-N4CT-ND |
| 2.0" ST7789V IPS 240×320 | 1 | Amazon |
 
## Repository Structure
 
```
PCB-Business-Card/
├── Firmware/
│   └── main.ino
├── lanyardpcbcard-backups/
├── libs/
│   └── easyeda/
│       ├── easyeda.3dshapes/
│       ├── easyeda.pretty/
│       └── easyeda.kicad_sym
├── production/
│   ├── backups/
│   ├── CPL-lanyardpcbcard.csv
│   ├── bom.csv
│   ├── lanyardpcbcard.step
│   ├── lanyardpcbcard.zip
│   └── ...
├── LICENSE
├── README.md
├── lanyardpcbcard.kicad_pcb
├── lanyardpcbcard.kicad_pro
├── lanyardpcbcard.kicad_sch
└── lanyardpcbcard.kicad_prl
```
 
## License
 
[MIT](LICENSE)
