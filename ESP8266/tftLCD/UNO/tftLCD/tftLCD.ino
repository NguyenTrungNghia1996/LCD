#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <ArduinoJson.h>
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "FreeDefaultFonts.h"

const int XP = 6, XM = A2, YP = A1, YM = 7; //320x480 ID=0x9486
const int TS_LEFT = 924, TS_RT = 205, TS_TOP = 965, TS_BOT = 214;

#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F
String data = "";
String cpuName, cpuLoad, cpuTemp, ramLoad, gpuName, gpuLoad, gpuTemp, net;
String oldCPULoad, oldCPUTemp, oldRAMLoad, oldGPULoad, oldGPUTemp, oldNET, oldCPUName, oldGPUName;
void setup() {
  Serial.begin(115200);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  boder();
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    if (data.length() > 225) {
      int i = data.indexOf("{\"CPU\":", 2);
      data.remove(0, i);
    }
    while (Serial.available()) {
      char incomingByte = (char)Serial.read();
      delay(1);
    }
    DynamicJsonDocument root(300);
    deserializeJson(root, (char*) data.c_str());
    cpuName = root["CPU"]["Name"].as<String>();
    cpuLoad = root["CPU"]["Load"].as<String>();
    cpuTemp = root["CPU"]["Temp"].as<String>();
    gpuName = root["GPU"]["Name"].as<String>();
    gpuLoad = root["GPU"]["Load"].as<String>();
    gpuTemp = root["GPU"]["Temp"].as<String>();
    String Total = root["RAM"]["Total"].as<String>();
    String Use = root["RAM"]["Use"].as<String>();
    ramLoad = Use + "/" + Total;
    net = root["Net"]["net"].as<String>();
    if (cpuLoad.length() > 5) {
      cpuLoad.remove(3);
    }
    if (oldCPUName != cpuName && cpuName != "") {
      showmsgXY(10, 38, 1, &FreeSans12pt7b, cpuName.c_str(), CYAN, 210, 18, 17);
      oldCPUName = cpuName;
    }
    if (oldCPULoad != cpuLoad && cpuLoad != "") {
      showmsgXY(80, 75, 2, &FreeSans12pt7b, cpuLoad.c_str(), YELLOW, 114, 36, 34);
      oldCPULoad = cpuLoad;
    }
    if (oldCPUTemp != cpuTemp && cpuTemp != "") {
      showmsgXY(80, 113, 2, &FreeSans12pt7b, cpuTemp.c_str(), YELLOW, 114, 36, 34);
      oldCPUTemp = cpuTemp;
    }
    if (oldGPUName != gpuName && gpuName != "") {
      showmsgXY(238, 38, 1, &FreeSans12pt7b, gpuName.c_str(), RED, 238, 18, 17);
      oldGPUName = gpuName;
    }
    if (oldGPULoad != gpuLoad && gpuLoad != "") {
      showmsgXY(320, 75, 2, &FreeSans12pt7b, gpuLoad.c_str(), YELLOW, 114, 36, 34);
      oldGPULoad = gpuLoad;
    }
    if (oldGPUTemp != gpuTemp && gpuTemp != "") {
      showmsgXY(320, 113, 2, &FreeSans12pt7b, gpuTemp.c_str(), YELLOW, 114, 36, 34);
      oldGPUTemp = gpuTemp;
    }
    if (oldRAMLoad != ramLoad && ramLoad != "") {
      showmsgXY(85, 163, 1, &FreeSans12pt7b, ramLoad.c_str(), CYAN, 80, 18, 17);
      oldRAMLoad = ramLoad;
    }
    if (oldNET != net && net != "") {
      showmsgXY(40, 213, 1, &FreeSans12pt7b, net.c_str(), CYAN, 120, 18, 17);
      oldNET = net;
    }
    //delay(1000);
  }
}
void boder() {
  //heder
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  showmsgXY(135, 16, 1, &FreeSerif12pt7b, "MCU Hardware Monitor", WHITE, 0, 0, 0);
  tft.drawFastHLine(0, 17, tft.width(), WHITE);
  //CPU
  showmsgXY(1, 70, 1, &FreeSans12pt7b, "Load :", CYAN, 0, 0, 0);
  showmsgXY(198, 74, 1, &FreeSans12pt7b, "%", CYAN, 0, 0, 0);
  showmsgXY(1, 105, 1, &FreeSans12pt7b, "Temp:", CYAN, 0, 0, 0);
  showmsgXY(198, 113, 1, &FreeSans12pt7b, "*C", CYAN, 0, 0, 0);
  tft.drawFastHLine(0, 40, 230 , CYAN);
  tft.drawRoundRect(0, 19, 230, 100, 7, CYAN);
  //GPU
  showmsgXY(232, 70, 1, &FreeSans12pt7b, "Load :", RED, 0, 0, 0);
  showmsgXY(445, 74, 1, &FreeSans12pt7b, "%", RED, 0, 0, 0);
  showmsgXY(232, 105, 1, &FreeSans12pt7b, "Temp:", RED, 0, 0, 0);
  showmsgXY(445, 113, 1, &FreeSans12pt7b, "*C", RED, 0, 0, 0);
  tft.drawFastHLine(231, 40, 249 , RED);
  tft.drawRoundRect(231, 19, 249, 100, 7, RED);
  //RAM
  showmsgXY(90, 138, 1, &FreeSans12pt7b, "RAM", GREEN, 0, 0, 0);
  showmsgXY(8, 162, 1, &FreeSans12pt7b, "USED:", GREEN, 0, 0, 0);
  showmsgXY(180, 162, 1, &FreeSans12pt7b, "GB", GREEN, 0, 0, 0);
  tft.drawFastHLine(0, 140, 230 , GREEN);
  tft.drawRoundRect(0, 120, 230, 48, 7, GREEN);
  //INTERNET
  showmsgXY(60, 188, 1, &FreeSans12pt7b, "INTERNET", MAGENTA, 0, 0, 0);
  showmsgXY(160, 213, 1, &FreeSans12pt7b, "Mbps", MAGENTA, 0, 0, 0);
  tft.drawFastHLine(0, 190, 230 , MAGENTA);
  tft.drawRoundRect(0, 169, 230, 50, 7, MAGENTA);
  //TIME
  showmsgXY(90, 239, 1, &FreeSans12pt7b, "TIME", WHITE, 0, 0, 0);
  showmsgXY(3, 295, 1, &FreeSevenSegNumFont, "12 59 59", WHITE, 0, 0, 0);
  showmsgXY(45, 316, 1, &FreeSans12pt7b, "12/12/2020", WHITE, 0, 0, 0);
  tft.drawFastHLine(0, 241, 230 , WHITE);
  tft.drawRoundRect(0, 220, 230, 100, 7, WHITE);
  //TEMP
  showmsgXY(310, 138, 1, &FreeSans12pt7b, "SENSOR", YELLOW, 0, 0, 0);
  tft.drawFastHLine(231, 140, 249 , YELLOW);
  tft.drawRoundRect(231, 119, 249, 200, 7, YELLOW);
}
void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg, int cl, int wd, int hg, int t)
{
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextSize(sz);
  tft.setTextColor(cl);
  if (wd > 0 && hg > 0 && t > 0) {
    tft.fillRect(x, y - t, wd, hg, BLACK);
  }
  tft.print(msg);
}
