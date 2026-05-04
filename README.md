# PCB-Business-Card
 
A PCB business card that shows your Discord Spotify activity in real time. Album art, song title, artist, and a live progress bar on a 2.0" IPS display. Tap the card with any phone to open the exact song you're listening to in Spotify. When nothing is playing, the NFC rewrites itself to GitHub. QR code on the board links there too (for now).
 
Powered over USB-C from a phone or any power bank. Connects to a phone hotspot (or any wifi) for the Lanyard WebSocket. A little bigger than a credit card, with a lanyard notch.
 
## Screenshots
[lanyardpcbcard.pdf](https://github.com/user-attachments/files/27328969/lanyardpcbcard.pdf)
<img width="1724" height="978" alt="lanyardpcbcard" src="https://github.com/user-attachments/assets/e74c7391-5224-4d8d-99e6-b06289b47a4d" />



## Why I built this
 
I wanted a business card that has a cool twist. Plain cards feel like a waste of time to show. This one shows what I'm listening to, lets someone tap it to hear the same song, and has my GitHub a QR scan away. Business cards are supposed to grab peoples attention and I would say this one is different than the rest.
 
## Hardware
 
Two-layer board, 100×75mm, designed in KiCad 9. SMD components assembled by JLCPCB. ESP32-S3-MINI-1 soldered by hand after the board arrives since it requires standard assembly and economic was MUCH cheaper without it. Display plugs into a through-hole header and gets soldered against the board.
 
The ESP32-S3 connects to Lanyard's WebSocket API over WiFi and parses the Spotify presence data. Album art is fetched over HTTPS and decoded on-device with TJpg_Decoder, rendered directly to the ST7789V display over SPI. The NT3H2111 NFC chip sits in the bottom right corner with a two-turn rectangular antenna loop on the back copper layer. The ESP32 rewrites the NDEF record over I2C every time the track changes.
 
A QR code etched in copper on the front links to GitHub (subject to change). Concentric copper circles around the NFC area are exposed through a soldermask opening so they show as bare metal.
 
Power comes from USB-C through a pair of 5.1kΩ CC resistors and an AMS1117-3.3 LDO. The ESP32's native USB peripheral handles programming directly through the same connector. Two tactile switches on the bottom for boot and reset.
 
## How the NFC works
 
The NT3H2111 has both an RF interface and an I2C interface. The phone reads it over RF like any NFC tag. The ESP32 writes to it over I2C. Every time Lanyard fires a presence update, the ESP32 checks the Spotify object — if a track is playing it writes `open.spotify.com/track/{id}` as the NDEF URL. If nothing is playing it writes the GitHub URL. The Field Detect pin goes high when a phone is actively reading the tag.
 
## How the progress bar works
 
Lanyard's Spotify object includes `timestamps.start` and `timestamps.end` as Unix milliseconds. On boot the ESP32 syncs with NTP to get real Unix time. Progress is calculated locally every 100ms:
 
```
progress_ms = (ntp_unix_ms_at_boot + millis()) - timestamps.start
```
 
When Lanyard sends a new presence update the timestamps resync. No polling, no drift.
 
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
 
`firmware/main.ino` — WiFi connection, NTP sync, Lanyard WebSocket, Spotify presence parsing, album art fetch and decode, display rendering, progress bar, NFC NDEF writes over I2C, reconnection logic.
 
Libraries needed: TFT_eSPI, TJpg_Decoder, ArduinoWebsockets, ArduinoJson. Install through Arduino IDE library manager. Board: ESP32S3 Dev Module.
 
## Bill of Materials
 
SMD parts assembled by JLCPCB. ESP32 and display sourced separately.
 
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
 
MIT(LISENCE)
