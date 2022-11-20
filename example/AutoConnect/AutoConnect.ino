#include <ESPAutoWifi.h>

ESPAutoWifi wf;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // uncomment below for testing
  // wf.reset();

  //starting autoconnect wifi system
  wf.autoConnect();
}

void loop() {
  // put your main code here, to run repeatedly:
}
