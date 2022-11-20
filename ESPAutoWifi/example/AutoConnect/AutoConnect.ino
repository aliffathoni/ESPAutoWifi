#include "ESP32ChipID.h"

ESP32ChipID esp;

void setup() {
  esp.begin();
}

void loop() {
	Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
	Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.print("Chip ID 64 bit: "); Serial.println(esp.getChipId12());
  Serial.print("Chip ID 32 bit: "); Serial.println(esp.getChipId());
  Serial.print("Chip ID 16 bit: "); Serial.println(esp.getChipId3());
  Serial.print("Chip ID 8 bit: "); Serial.println(esp.getChipId2());
  
	delay(3000);
}