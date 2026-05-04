#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <time.h>

#define NFC_SDA 5
#define NFC_SCL 6
#define NFC_FD  4

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASSWORD";
const char* DISCORD_USER_ID = "YOUR_ID";
const char* GITHUB_URL = "https://github.com/Studented0";
const char* NTP_SERVER = "pool.ntp.org";

using namespace websockets;

TFT_eSPI tft = TFT_eSPI();
WebsocketsClient ws;

String currentTrackId = "";
String currentTitle = "";
String currentArtist = "";
String currentAlbumArt = "";
long timestampStart = 0;
long timestampEnd = 0;
bool isPlaying = false;

bool tftOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    tft.pushImage(x, y, w, h, bitmap);
    return true;
}

void writeNFC(const char* url) {
    Wire.beginTransmission(0x55);
    Wire.endTransmission();
}

void fetchAndDisplayAlbumArt(String url) {
    HTTPClient http;
    http.begin(url);
    int code = http.GET();
    if (code == 200) {
        WiFiClient* stream = http.getStreamPtr();
        TJpgDec.drawJpg(0, 0, (const uint8_t*)stream, http.getSize());
    }
    http.end();
}

void displayNowPlaying() {
    tft.fillRect(0, 240, 240, 80, TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setCursor(0, 245);
    tft.println(currentTitle);
    tft.println(currentArtist);
}

void displayProgressBar() {
    long now = time(nullptr) * 1000L;
    long progress = now - timestampStart;
    long duration = timestampEnd - timestampStart;
    int barWidth = map(progress, 0, duration, 0, 240);
    tft.fillRect(0, 310, barWidth, 4, TFT_GREEN);
    tft.fillRect(barWidth, 310, 240 - barWidth, 4, TFT_DARKGREY);
}

void displayIdle() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(20, 100);
    tft.println("Yahya Abdelazim");
    tft.setCursor(20, 120);
    tft.println("github.com/Studented0");
    writeNFC(GITHUB_URL);
}

void handleLanyardMessage(WebsocketsMessage msg) {
    JsonDocument doc;
    deserializeJson(doc, msg.data());
    
    int op = doc["op"];
    
    if (op == 1) {
        ws.send("{\"op\":2,\"d\":{\"subscribe_to_id\":\"" + String(DISCORD_USER_ID) + "\"}}");
    }
    
    if (op == 0) {
        JsonObject spotify = doc["d"]["spotify"];
        
        if (spotify.isNull()) {
            isPlaying = false;
            displayIdle();
            return;
        }
        
        isPlaying = true;
        currentTitle = spotify["song"].as<String>();
        currentArtist = spotify["artist"].as<String>();
        currentAlbumArt = spotify["album_art_url"].as<String>();
        timestampStart = spotify["timestamps"]["start"].as<long>();
        timestampEnd = spotify["timestamps"]["end"].as<long>();
        
        String trackUrl = spotify["track_id"].as<String>();
        String nfcUrl = "https://open.spotify.com/track/" + trackUrl;
        writeNFC(nfcUrl.c_str());
        
        fetchAndDisplayAlbumArt(currentAlbumArt);
        displayNowPlaying();
    }
}

void setup() {
    Serial.begin(115200);
    
    Wire.begin(NFC_SDA, NFC_SCL);
    
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    
    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tftOutput);
    
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    
    configTime(0, 0, NTP_SERVER);
    while (time(nullptr) < 1000) delay(100);
    
    ws.onMessage(handleLanyardMessage);
    ws.connect("wss://api.lanyard.rest/socket");
}

void loop() {
    ws.poll();
    
    if (isPlaying) {
        displayProgressBar();
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
    }
    
    if (!ws.available()) {
        ws.connect("wss://api.lanyard.rest/socket");
    }
    
    delay(100);
}
