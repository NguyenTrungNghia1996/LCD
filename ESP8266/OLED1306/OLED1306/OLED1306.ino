#include <Wire.h>
#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, D2, D1);

#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

String cpuName, cpuLoad, cpuTemp, ramLoad, gpuName, gpuLoad, gpuTemp;
String oldCPULoad, oldCPUTemp, oldRAMLoad, oldGPULoad, oldGPUTemp;
int timer = 0, demFlash, matKetNoiWiFi;
bool lanDauMoApp = true, ketNoiApp, doiWiFi;
#define flash 0
WiFiServer server(80);
WiFiManager wifiManager;
WiFiClient client;
Ticker flipper;
void setup() {
  Serial.begin(115200);
  pinMode(flash, INPUT_PULLUP);
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Connecting to WiFi");
  display.display();
  display.clear();
  display.drawString(0, 0, "Connection Fail");
  display.drawString(0, 10, "Connect to WiFi MCU");
  display.drawString(0, 20, "with password 123456789");
  display.drawString(0, 30, "to config new WiFi");
  display.display();
  wifiManager.autoConnect("MCU", "123456789");
  server.begin();

}
void loop() {
  display.clear();
  wifi();
  display.display();
}
int getBarsSignal(long rssi) {
  int bars;
  if (rssi > -55) {
    bars = 5;
  } else if (rssi < -55 & rssi > -65) {
    bars = 4;
  } else if (rssi < -65 & rssi > -75) {
    bars = 3;
  } else if (rssi < -75 & rssi > -85) {
    bars = 2;
  } else if (rssi < -85 & rssi > -96) {
    bars = 1;
  } else {
    bars = 0;
  }
  return bars;
}
void wifi(){
  if (WiFi.status() == WL_CONNECTED)
  { 
    display.drawLine(0, 52, 128, 52);
    String ip = WiFi.localIP().toString();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 54, ip);
    long rssi = WiFi.RSSI();
    int bars = getBarsSignal(rssi);
    display.drawLine(0, 52, 128, 52);

    for (int  i = 0; i <= bars; i++) {
      display.fillRect(117 + (i * 2), 64 - (i * 2), 1, i * 2);
    }
  } else {
    display.drawString(0, 54, "There is no connection!");
  }
  if (digitalRead(flash) == LOW) {
    wifiManager.resetSettings();
    ESP.restart();
  }
  }
