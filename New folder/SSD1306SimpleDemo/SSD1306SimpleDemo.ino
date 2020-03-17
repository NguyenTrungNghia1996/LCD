#include <Wire.h>
#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, D6, D7);
#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif
int relay = 5;
int button = 0;
DHTesp dht;
float humidity, temperature;
String role;
int status1, t = 0;
void setup() {
  Serial.begin(115200);
  dht.setup(14, DHTesp::DHT11);
  pinMode(button, INPUT);
  pinMode(relay, OUTPUT);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}
void loop() {
  //delay(dht.getMinimumSamplingPeriod());
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  oled();
  int buttonState = digitalRead(button);
  status1 = chong_doi();
  if (status1 == true)
  {
    {
      t = !t; // đảo trạng thái
    }
    while (status1 == true) {
      status1 = chong_doi();
    }
  }
  if (t == 1)
  {
    digitalWrite(relay, LOW);
    role = "OFF";
  }
  else
  {
    if (temperature >= 30) {
      digitalWrite(relay, LOW);
    } else {
      digitalWrite(relay, HIGH);
    }
    role = "ON";
  }
}
void oled() {
  delay(1000);
  display.clear();
  display.drawLine(0, 12, 128, 12);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(65, 0, "IN CASE");
  display.drawString(37, 12, "O");
  display.setFont(ArialMT_Plain_16);
  display.drawString(110, 19, "%");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(5, 12, String(temperature, 0) + " c");
  display.drawString(75, 12, String(humidity, 0));
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 37, "AUTO FAN:" + role);
  display.display();
}
boolean chong_doi()
{
  int sta = !digitalRead(button);
  return sta;
}
