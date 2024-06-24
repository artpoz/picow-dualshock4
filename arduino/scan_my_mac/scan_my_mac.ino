#include <WiFi.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(5000);
  String macAddress = WiFi.macAddress();
  Serial.print("MAC Address: ");
  Serial.println(macAddress);
}
