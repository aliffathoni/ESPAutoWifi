#include <ESPAutoWifi.h>

//instantiate wifi class
//uncomment based on ap type you want to generate
ESPAutoWifi wf; //default ap ssid without password
// ESPAutoWifi wf("ESP32 Wifi Portal"); //custom ap ssid without password
// ESPAutoWifi wf("ESP32 Wifi Portal", "12345678"); //custom ap ssid with password

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // uncomment below for testing
  // wf.reset();

  //starting autoconnect wifi system
  wf.autoConnect();

  while(wf.checkConnection()){
    Serial.println("Please connect to "+wf.getAPSSID());
    Serial.println("And proceed to "+wf.getAPIP()+"on your browser\n");
    delay(1000);
  }

  Serial.print("Connected with ");
  Serial.println(wf.getIP());
}

void loop() {
  // put your main code here, to run repeatedly:
}
