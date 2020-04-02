#include "MCUFRIEND_kbv.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <OneWire.h>
#include <FanController.h>

#include "icons.h"

#define THERMAL_SENSOR_PIN 10

#define FAN_SENSOR_THRESHOLD 1000
#define FAN_PWM_PIN 13
#define FAN_SENSOR_PIN 11

// fan 
FanController fan(FAN_SENSOR_PIN, FAN_SENSOR_THRESHOLD, FAN_PWM_PIN);
int fanDutyCycle = 50;

// thermal 
OneWire ds(THERMAL_SENSOR_PIN);  

// TFT
MCUFRIEND_kbv tft;

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410
#define ORANGE  0xE880

int rotation = 3;

unsigned long lastSignal = 0;
String data = "";

void setup() {   
    //init fan
    pinMode(FAN_SENSOR_PIN, INPUT);
    digitalWrite(FAN_SENSOR_PIN,HIGH);
    
    fan.begin();
    fan.setDutyCycle(fanDutyCycle);

    //intit TFT
    uint16_t ID = tft.readID();
    if (ID == 0xD3) ID = 0x9481;
    tft.begin(ID);
    initDisplay ();
    
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {    
        char incomingByte = (char)Serial.read();
        if (incomingByte == '>') {
          data = ">";
        }
        else if (incomingByte == '^') {
            displayInfo();
            
            data = "";            
        }
        else if (incomingByte == '?') {
        }
        else {
            if (data.indexOf('>') == 0) data += String(incomingByte);
        }
    }    
    
    //delay(1000);
}

void displayInfo() {   
    if (data.length() < 2) return;

    String cpuInfo, gpuInfo, ramInfo, netInfo, fpsInfo, fanInfo;
    int ind1, ind2, ind3, ind4, ind5, ind6, ind7;

    ind1 = data.indexOf('|', 1);
    int _rotation = data.substring(1, ind1).toInt();
    if (rotation != _rotation) {
        rotation = _rotation;
        initDisplay ();
    }
    
    ind2 = data.indexOf('|', ind1 + 1);
    cpuInfo = data.substring(ind1 + 1, ind2);
    
    ind3 = data.indexOf('|', ind2 + 1);
    gpuInfo = data.substring(ind2 + 1, ind3);
    
    ind4 = data.indexOf('|', ind3 + 1);
    ramInfo = data.substring(ind3 + 1, ind4);
  
    ind5 = data.indexOf('|', ind4 + 1);
    netInfo = data.substring(ind4 + 1, ind5);
    
    ind6 = data.indexOf('|', ind5 + 1);
    fpsInfo = data.substring(ind5 + 1, ind6);
  
    ind7 = data.indexOf('|', ind6 + 1);
    fanInfo = data.substring(ind6 + 1, ind7);
    
    lastSignal = millis();
    
    
    //SHOW INFOS//////////////////////
    char buffer[10]; // Enough room for the digits you want and more to be safe    

    showMsg(50, 35, 1, WHITE, &FreeSans12pt7b, cpuInfo.c_str(), 260, 40);
    showMsg(50, 95, 1, WHITE, &FreeSans12pt7b, gpuInfo.c_str(), 260, 40);
    showMsg(220, 105, 3, GREEN, &FreeSans9pt7b, fpsInfo.c_str(), 0, 0);
    
    showMsg(50, 155, 1, WHITE, &FreeSans9pt7b, ramInfo.c_str(), 104, 40);
    showMsg(210, 155, 1, WHITE, &FreeSans9pt7b, netInfo.c_str(), 109, 40);

    //temp
    int temperature = getTemperature();
    if (temperature >= 0) {
        itoa(temperature, buffer, 10);
        showMsg(70, 220, 2, WHITE, &FreeSans9pt7b, buffer, 80, 40);
    }

    //fan
    int _fan = fanInfo.toInt();
    if (_fan < 0) {
        if (temperature >= 60) _fan = 100;
        else if (temperature >= 50) _fan = 75;
        else if (temperature >= 40) _fan = 50;
        else _fan = 20;
    }    
    
    if (_fan <= 100 && _fan != fanDutyCycle) {
        fan.setDutyCycle(_fan);

        fanDutyCycle = _fan;
    }
    
    char buf1[5];
    unsigned int rpms = getFanSpeed();
    itoa(rpms, buffer, 10);    
    itoa(fanDutyCycle, buf1, 10);
    strcat(buffer, " ");
    strcat(buffer, buf1);
    strcat(buffer, "%");
    showMsg(210, 215, 1, WHITE, &FreeSans9pt7b, buffer, 104, 40);   
}


/////////UTILS////////////////////////////////////////////////////////////////
unsigned int getFanSpeed() {
   return fan.getSpeed();
}
int getTemperature() {
    byte i;
    byte present = 0;
    byte type_s;
    byte data[12];
    byte addr[8];
    float celsius;
    
    if ( !ds.search(addr)) {
      /*Serial.println("No more addresses.");
      Serial.println();*/
      ds.reset_search();
      delay(250);
      return -1;
    }
   
    if (OneWire::crc8(addr, 7) != addr[7]) {
        //Serial.println("CRC is not valid!");
        return -2;
    }
   
    // the first ROM byte indicates which chip
    switch (addr[0]) {
      case 0x10:
        //Serial.println("  Chip = DS18S20");  // or old DS1820
        type_s = 1;
        break;
      case 0x28:
        //Serial.println("  Chip = DS18B20");
        type_s = 0;
        break;
      case 0x22:
        //Serial.println("  Chip = DS1822");
        type_s = 0;
        break;
      default:
        //Serial.println("Device is not a DS18x20 family device.");
        return -3;
    } 
  
    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    delay(1000);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.
    
    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);         // Read Scratchpad
    
    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
    }
  
    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    } else {
      byte cfg = (data[4] & 0x60);
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      //// default is 12 bit resolution, 750 ms conversion time
    }
    
    return (float)raw / 16;
}

void initDisplay () {
    tft.setRotation(rotation);

    tft.fillScreen(BLACK);
    
    tft.drawRect(0, 0, 320, 59, YELLOW);
    //showMsg(5, 25, 2, RED, NULL, "CPU", 0, 0);
    tft.drawRGBBitmap(5, 15, _iconCPU, 32, 32);    
    
    tft.drawRect(0, 60, 320, 59, YELLOW);
    tft.drawRGBBitmap(5, 70, _iconGPU, 32, 32);
    //showMsg(5, 80, 2, RED, NULL, "GPU", 0, 0);
    
    tft.drawRect(0, 120, 159, 59, YELLOW);
    tft.drawRect(160, 120, 160, 59, YELLOW);
    showMsg(5, 145, 2, RED, NULL, "RAM", 0, 0);
    showMsg(165, 145, 2, RED, NULL, "NET", 0, 0);
    //tft.drawRGBBitmap(5, 135, _iconRAM, 32, 32);
    //tft.drawRGBBitmap(165, 135, _iconNetwork, 32, 32);
    
    tft.drawRect(0, 180, 159, 60, YELLOW);
    tft.drawRect(160, 180, 160, 60, YELLOW);    
    showMsg(5, 205, 2, RED, NULL, "SENS", 0, 0);
    showMsg(165, 205, 2, RED, NULL, "FAN", 0, 0);
    //tft.drawRGBBitmap(5, 195, _iconTemp, 32, 32);
    //tft.drawRGBBitmap(165, 195, _iconFan, 32, 32);
}

void showMsg(int x, int y, float sz, int cl, const GFXfont *f, const char *msg, int wd, int hg) {
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(cl);
    tft.setTextSize(sz);
    
    if (wd > 0 && hg > 0) tft.fillRect(x, y - 25, wd, hg, BLACK);
    tft.print(msg);
}
