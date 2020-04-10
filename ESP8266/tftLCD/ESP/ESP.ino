#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <time.h>
#include <Arduino.h>
WiFiServer server(80);
WiFiManager wifiManager;
WiFiClient client;
unsigned long time1 = 0;
unsigned long time2 = 0;
String dataPC = "";

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect("MCU", "123456789");
  server.begin();
}
void loop() {
  StaticJsonDocument<1000> root;
  if (client.connected()) {
    if (client.available() > 0) {
      dataPC = client.readStringUntil('\n');
      deserializeJson(root, (char*) dataPC.c_str());
    }
    String output;
    serializeJson(root, output);
    Serial.println(output);
    delay(1000);
  }
  else {
    client = server.available();
  }
  long rssi = WiFi.RSSI();

}
