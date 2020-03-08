#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ESP8266WiFi.h"                // Version 2.6.3
#include <ArduinoJson.h>                // Version 6.13.0
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>                // Version 0.15.0 beta
#include <Ticker.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int timezone = 25200; // 7*60*60
int dst = 0;
LiquidCrystal_I2C lcd(0x3f, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
String cpuName, cpuLoad, cpuTemp, ramTotal, gpuName, gpuLoad, gpuTemp, net, ramUse;
String oldCPULoad, oldCPUTemp, oldRAMToal, oldGPULoad, oldGPUTemp, oldNet, oldRamUse;
int timer, demFlash, matKetNoiWiFi;
bool lanDauMoApp = true, ketNoiApp, doiWiFi;

WiFiServer server(80);
WiFiManager wifiManager;
WiFiClient client;
Ticker flipper;

byte customChar[8] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};
byte customChar2[8] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  pinMode(flash, INPUT_PULLUP);
  lcd.createChar(1, customChar);
  lcd.createChar(2, customChar2);
  lcd.setCursor(0, 0);
  lcd.print("Connecting to WiFi");
  wifiManager.autoConnect("MCU", "123456");
  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.print("IP:");
  lcd.setCursor(5, 3);
  lcd.print(WiFi.localIP().toString());
  server.begin();
  configTime(timezone, dst, "pool.ntp.org", "nist.time.gov");
  lcd.setCursor(0, 0);
  lcd.print("Waiting NTP Server");
  int i = 0;
  while (!time(nullptr)) {
    lcd.setCursor(i, 1);
    lcd.print("#");
    i = i + 1;
  }
  lcd.setCursor(0, 0);
  lcd.print("                   ");
}


void loop()
{
  if (ketNoiApp)
    getData();
  else {
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    lcd.setCursor(4, 0);
    lcd.print("  ");
    lcd.setCursor(4, 0);
    lcd.print(p_tm->tm_hour);
    lcd.setCursor(6, 0);
    lcd.print(":");
    lcd.setCursor(7, 0);
    lcd.print("  ");
    lcd.setCursor(7, 0);
    lcd.print(p_tm->tm_min);
    lcd.setCursor(9, 0);
    lcd.print(":");
    lcd.setCursor(10, 0);
    lcd.print("  ");
    lcd.setCursor(10, 0);
    lcd.print(p_tm->tm_sec);

    lcd.setCursor(4, 1);
    lcd.print("  ");
    lcd.setCursor(4, 1);
    lcd.print(p_tm->tm_mday);
    lcd.setCursor(6, 1);
    lcd.print("/");
    lcd.setCursor(7, 1);
    lcd.print("  ");
    lcd.setCursor(7, 1);
    lcd.print(p_tm->tm_mon + 1);
    lcd.setCursor(9, 1);
    lcd.print("/");
    lcd.setCursor(10, 1);
    lcd.print("    ");
    lcd.setCursor(10, 1);
    lcd.print(p_tm->tm_year + 1900);
    
    lcd.setCursor(0, 3);
    lcd.print("IP:");
    lcd.setCursor(5, 3);
    lcd.print(WiFi.localIP().toString());
    if (!client.connected()) {
      client = server.available();
    }
    else {
      lcd.clear();
      ketNoiApp   = true;
      lanDauMoApp = true;
    }
  }
}
void border() {
  lcd.setCursor(0, 0);
  lcd.print("CPU:");

  lcd.setCursor(0, 1);
  lcd.print("GPU:");

  lcd.setCursor(0, 2);
  lcd.print("RAM:");
  lcd.setCursor(0, 3);
  lcd.print("NET:");
}
void getData() {
  if (client.connected()) {
    if (client.available() > 0) {
      String data = client.readStringUntil('\n');
      DynamicJsonDocument root(300);
      deserializeJson(root, (char*) data.c_str());
      cpuName = root["CPU"]["Name"].as<String>();
      cpuLoad = root["CPU"]["Load"].as<String>();
      cpuTemp = root["CPU"]["Temp"].as<String>();
      ramTotal = root["RAM"]["Total"].as<String>();
      ramUse = root["RAM"]["Use"].as<String>();
      gpuName = root["GPU"]["Name"].as<String>();
      gpuLoad = root["GPU"]["Load"].as<String>();
      gpuTemp = root["GPU"]["Temp"].as<String>();
      net = root["Net"]["net"].as<String>();
      if (cpuLoad.length() > 5)
        cpuLoad = cpuLoad.substring(0, 5);
      if (lanDauMoApp) {
        border();
        lanDauMoApp = false;
      }
      else {
        if (oldCPULoad != cpuLoad) {
          lcd.setCursor(5, 0);
          lcd.print("      ");
          lcd.setCursor(5, 0);
          lcd.print(cpuLoad + "% ");
          oldCPULoad = cpuLoad;
        }
        if (oldCPUTemp != cpuTemp) {
          lcd.setCursor(11, 0);
          lcd.print(char(1));
          lcd.setCursor(13, 0);
          lcd.print("      ");
          lcd.setCursor(13, 0);
          lcd.print(cpuTemp + char(2) + "C");
          oldCPUTemp = cpuTemp;
        }

        lcd.setCursor(5, 2);
        lcd.print("               ");
        lcd.setCursor(5, 2);
        lcd.print(ramUse + " / " + ramTotal + " GB");

        if (oldGPULoad != gpuLoad) {
          lcd.setCursor(5, 1);
          lcd.print("      ");
          lcd.setCursor(5, 1);
          lcd.print(gpuLoad + "%");
          oldGPULoad = gpuLoad;
        }
        if (oldGPUTemp != gpuTemp) {
          lcd.setCursor(11, 1);
          lcd.print(char(1));
          lcd.setCursor(13, 1);
          lcd.print("      ");
          lcd.setCursor(13, 1);
          lcd.print(gpuTemp + char(2) + "C");
          oldGPUTemp = gpuTemp;
        }
        if (oldNet != net) {
          lcd.setCursor(5, 3);
          lcd.print("              ");
          lcd.setCursor(5, 3);
          lcd.print(net + "Mbps");
          oldNet = net;
        }
      }
    }
  }
  else {
    lcd.clear();
    ketNoiApp = false;
  }
}
