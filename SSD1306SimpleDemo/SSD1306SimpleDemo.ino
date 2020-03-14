#include <Wire.h>
#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, D7, D6);
#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif
int relay = 5;
DHTesp dht;
float humidity, temperature;
String role;
void setup() {
  Serial.begin(115200);
  dht.setup(14, DHTesp::DHT11);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  pinMode(relay, OUTPUT);
}
void loop() {
  delay(dht.getMinimumSamplingPeriod());
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  if (temperature >= 30) {
    if (digitalRead(relay) == LOW) {
      digitalWrite(relay, HIGH);
      role = "ON";
    }
  } else {
    digitalWrite(relay, LOW);
    role = "OFF";
  }
  ole();
}
void ole() {
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
  display.drawString(0, 37, "FAN CASE: "+role);
  display.display();
}
