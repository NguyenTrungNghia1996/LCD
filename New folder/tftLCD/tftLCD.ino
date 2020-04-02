#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include "TouchScreen.h"

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
void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
}

void loop() {
  //heder
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  showmsgXY(135, 16, 1, &FreeSerif12pt7b, "MCU Hardware Monitor");
  tft.drawFastHLine(0, 17, tft.width(), WHITE);
  //CPU
  tft.setTextColor(CYAN);
  showmsgXY(10, 38, 1, &FreeSans12pt7b, "Intel Core i5-9400F");
  showmsgXY(1, 70, 1, &FreeSans12pt7b, "Load :");
  showmsgXY(198, 74, 1, &FreeSans12pt7b, "%");
  showmsgXY(1, 105, 1, &FreeSans12pt7b, "Temp:");
  showmsgXY(198, 113, 1, &FreeSans12pt7b, "*C");
  tft.setTextColor(YELLOW);
  tft.setCursor(80, 75);
  tft.setTextSize(2);
  tft.print("100");
  tft.setCursor(80, 113);
  tft.setTextSize(2);
  tft.print("100");
  tft.drawFastHLine(0, 40, 230 , CYAN);
  tft.drawRoundRect(0, 19, 230, 100, 5, CYAN);
  //GPU
  tft.setTextColor(RED);
  showmsgXY(238, 38, 1, &FreeSans12pt7b, "Radeon RX580 Series");
  showmsgXY(232, 70, 1, &FreeSans12pt7b, "Load :");
  showmsgXY(445, 74, 1, &FreeSans12pt7b, "%");
  showmsgXY(232, 105, 1, &FreeSans12pt7b, "Temp:");
  showmsgXY(445, 113, 1, &FreeSans12pt7b, "*C");
  tft.setTextColor(YELLOW);
  tft.setCursor(320, 75);
  tft.setTextSize(2);
  tft.print("100");
  tft.setCursor(320, 113);
  tft.setTextSize(2);
  tft.print("100");
  tft.drawFastHLine(231, 40, 249 , RED);
  tft.drawRoundRect(231, 19, 249, 200, 5, RED);
  //Ram
  tft.setTextColor(GREEN);
  showmsgXY(90, 140, 1, &FreeSans12pt7b, "RAM");
  tft.setTextColor(YELLOW);
  tft.setCursor(8, 165);
  tft.setTextSize(1);
  tft.print("USED: 00.0/00.0 GB");
  tft.drawFastHLine(0, 143, 230 , GREEN);
  tft.drawRoundRect(0, 120, 230, 50, 5, GREEN);
  //internet
  tft.setTextColor(PURPLE);
  showmsgXY(60, 190, 1, &FreeSans12pt7b, "INTERNET");
  tft.setTextColor(YELLOW);
  tft.setCursor(40, 213);
  tft.setTextSize(1);
  tft.print("0.00/0.00 Mbps");
  tft.drawFastHLine(0, 193, 230 , PURPLE);
  tft.drawRoundRect(0, 170, 230, 50, 5, PURPLE);
  //Time
  tft.setTextColor(WHITE);
  showmsgXY(90, 239, 1, &FreeSans12pt7b, "TIME");
  showmsgXY(-1, 295, 1, &FreeSevenSegNumFont, "12 59 59");
  showmsgXY(45, 316, 1, &FreeSans12pt7b, "12/12/2020");
  tft.drawFastHLine(0, 241, 230 , WHITE);
  tft.drawRoundRect(0, 220, 230, 100, 5, WHITE);
  //Temp
  showmsgXY(232, 262, 1, &FreeSans12pt7b, "IN ROOM: ");
  showmsgXY(232, 292, 1, &FreeSans12pt7b, "IN CASE: ");
  tft.drawFastHLine(231, 241, 249 , RED);
  tft.drawRoundRect(231, 220, 249, 100, 5, RED);
  delay(10000);
}
void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextSize(sz);
  tft.print(msg);
}
