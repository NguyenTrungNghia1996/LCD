#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 20, 4);
String cpuName, cpuLoad, cpuTemp, ramTotal, gpuName, gpuLoad, gpuTemp, net, ramUse;
String oldCPULoad, oldCPUTemp, oldRAMToal, oldGPULoad, oldGPUTemp, oldNet, oldRamUse;
int timer, demFlash, matKetNoiWiFi;
bool lanDauMoApp = true, ketNoiApp, doiWiFi;
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
  lcd.createChar(1, customChar);
  lcd.createChar(2, customChar2);
  lcd.setCursor(0, 0);
  lcd.print("Connecting to WiFi");
  lcd.clear();
}


void loop()
{
  if (ketNoiApp)
    getData();
  else {
    lcd.clear();
    ketNoiApp   = true;
    lanDauMoApp = true;
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
  if (Serial.available()) {

    String data = Serial.readStringUntil('\n');
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
  else {
    lcd.clear();
    ketNoiApp = false;
  }
}
