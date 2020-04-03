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
  boder();
  delay(10000);
}
void boder() {
  //heder
  tft.fillScreen(BLACK);
  showmsgXY(135, 16, 1, &FreeSerif12pt7b, "MCU Hardware Monitor", WHITE, 0, 0);
  tft.drawFastHLine(0, 17, tft.width(), WHITE);
  //CPU
  showmsgXY(10, 38, 1, &FreeSans12pt7b, "Intel Core i5-9400F", CYAN, 0, 0);
  showmsgXY(1, 70, 1, &FreeSans12pt7b, "Load :", CYAN, 0, 0);
  showmsgXY(198, 74, 1, &FreeSans12pt7b, "%", CYAN, 0, 0);
  showmsgXY(1, 105, 1, &FreeSans12pt7b, "Temp:", CYAN, 0, 0);
  showmsgXY(198, 113, 1, &FreeSans12pt7b, "*C", CYAN, 0, 0);
  tft.drawFastHLine(0, 40, 230 , CYAN);
  tft.drawRoundRect(0, 19, 230, 100, 7, CYAN);
  //  tft.setTextColor(YELLOW);
  //  tft.setCursor(80, 75);
  //  tft.setTextSize(2);
  //  tft.print("100");
  //  tft.setCursor(80, 113);
  //  tft.setTextSize(2);
  //  tft.print("100");
  //GPU
  showmsgXY(238, 38, 1, &FreeSans12pt7b, "Radeon RX580 Series", RED, 0, 0);
  showmsgXY(232, 70, 1, &FreeSans12pt7b, "Load :", RED, 0, 0);
  showmsgXY(445, 74, 1, &FreeSans12pt7b, "%", RED, 0, 0);
  showmsgXY(232, 105, 1, &FreeSans12pt7b, "Temp:", RED, 0, 0);
  showmsgXY(445, 113, 1, &FreeSans12pt7b, "*C", RED, 0, 0);
  tft.drawFastHLine(231, 40, 249 , RED);
  tft.drawRoundRect(231, 19, 249, 100, 7, RED);
  //  tft.setTextColor(YELLOW);
  //  tft.setCursor(320, 75);
  //  tft.setTextSize(2);
  //  tft.print("100");
  //  tft.setCursor(320, 113);
  //  tft.setTextSize(2);
  //  tft.print("100");
  //RAM
  showmsgXY(90, 138, 1, &FreeSans12pt7b, "RAM", GREEN, 0, 0);
  showmsgXY(8, 162, 1, &FreeSans12pt7b, "USED:", GREEN, 0, 0);
  showmsgXY(180, 162, 1, &FreeSans12pt7b, "GB", GREEN, 0, 0);
  tft.drawFastHLine(0, 140, 230 , GREEN);
  tft.drawRoundRect(0, 120, 230, 48, 7, GREEN);
  //INTERNET
  showmsgXY(60, 188, 1, &FreeSans12pt7b, "INTERNET", MAGENTA, 0, 0);
  showmsgXY(40, 213, 1, &FreeSans12pt7b, "0.00/0.00", YELLOW, 0, 0);
  showmsgXY(160, 213, 1, &FreeSans12pt7b, "Mbps", MAGENTA, 0, 0);
  tft.drawFastHLine(0, 190, 230 , MAGENTA);
  tft.drawRoundRect(0, 169, 230, 50, 7, MAGENTA);
  //TIME
  showmsgXY(90, 239, 1, &FreeSans12pt7b, "TIME", WHITE, 0, 0);
  showmsgXY(3, 295, 1, &FreeSevenSegNumFont, "12 59 59", WHITE, 0, 0);
  showmsgXY(45, 316, 1, &FreeSans12pt7b, "12/12/2020", WHITE, 0, 0);
  tft.drawFastHLine(0, 241, 230 , WHITE);
  tft.drawRoundRect(0, 220, 230, 100, 7, WHITE);
  //TEMP
  showmsgXY(310, 138, 1, &FreeSans12pt7b, "SENSOR", YELLOW, 0, 0);
  tft.drawFastHLine(231, 140, 249 , YELLOW);
  tft.drawRoundRect(231, 119, 249, 200, 7, YELLOW);
}
void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg, int cl, int wd, int hg)
{
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextSize(sz);
  tft.setTextColor(cl);
  if (wd > 0 && hg > 0) {
    tft.fillRect(x, y - 25, wd, hg, BLACK);
  }
  tft.print(msg);
}
