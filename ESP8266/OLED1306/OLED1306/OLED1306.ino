#include <Wire.h>
#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, D2, D1);

#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

String cpuName, cpuLoad, cpuTemp, ramLoad, gpuName, gpuLoad, gpuTemp, net;
String oldCPULoad, oldCPUTemp, oldRAMLoad, oldGPULoad, oldGPUTemp, oldNet;
int timer = 0, demFlash, matKetNoiWiFi;
bool lanDauMoApp = true, ketNoiApp, doiWiFi;
#define flash 0
WiFiServer server(80);
WiFiManager wifiManager;
WiFiClient client;
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
  if (ketNoiApp)
  {
    getData();
  }
  else {
    if (!client.connected()) {
      client = server.available();
    }
    else {
      display.clear();
      ketNoiApp   = true;
      lanDauMoApp = true;
    }
  }
  display.display();
  delay(1000);
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
void wifi() {
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
void border() {
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "CPU:");
  display.drawString(0, 17, "GPU:");
  display.drawString(0, 34, "NET:");
  display.drawString(117, 0, "C");
  display.drawString(117, 18, "C");
  display.setFont(ArialMT_Plain_10);
  display.drawString(110, 0, "O");
  display.drawString(110, 18, "O");
}
void getData() {
  if (client.connected()) {
    if (client.available() > 0) {
      String data = client.readStringUntil('\n');
      Serial.println(data);
      DynamicJsonDocument root(300);
      deserializeJson(root, (char*) data.c_str());
      cpuName = root["CPU"]["Name"].as<String>();
      cpuLoad = root["CPU"]["Load"].as<String>();
      cpuTemp = root["CPU"]["Temp"].as<String>();
      gpuName = root["GPU"]["Name"].as<String>();
      gpuLoad = root["GPU"]["Load"].as<String>();
      gpuTemp = root["GPU"]["Temp"].as<String>();
      net = root["Net"]["net"].as<String>();
      if (cpuLoad.length() > 5)
        cpuLoad = cpuLoad.substring(0, 5);
      if (lanDauMoApp) {
        //border();
        lanDauMoApp = false;
      }
      else {
        border();
        display.setFont(ArialMT_Plain_16);
        display.drawString(37, 0, cpuLoad + "%");
        display.setFont(ArialMT_Plain_16);
        display.drawString(85, 0, cpuTemp);
        display.setFont(ArialMT_Plain_16);
        display.drawString(37, 18, gpuLoad + "%");
        display.setFont(ArialMT_Plain_16);
        display.drawString(85, 18, gpuTemp);
        display.setFont(ArialMT_Plain_16);
        display.drawString(37, 34, net + "Mbps");
      }
    }
  }
  else {
    display.clear();
    ketNoiApp = false;
  }
}
