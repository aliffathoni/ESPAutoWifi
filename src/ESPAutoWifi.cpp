#include <ESPAutoWifi.h>
#include "Arduino.h"

#ifdef ESP32
    #include <WiFi.h>
    #include <AsyncTCP.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
#else
#error Platform not supported
#endif

#include <Preferences.h>

#include <web.h>

AsyncWebServer server(80);

Preferences preferences;

String _ssid;
String _password;

ESPAutoWifi::ESPAutoWifi(String ap){
    _ap_ssid = ap;
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

bool ESPAutoWifi::checkCredentials(){
    preferences.begin("credentials", false);
    _ssid =  preferences.getString("ssid", "");
    _password =  preferences.getString("password", "");
    preferences.end();

    if((_ssid!="") && (_password!="")){
        return true;
    } else{
        return false;
    }
}

bool ESPAutoWifi::checkConnection(){
    if(WiFi.waitForConnectResult() != WL_CONNECTED){
        return true;
    } else{
        return false;
    }
}

String ESPAutoWifi::getSSID(){
    if(checkCredentials())  return _ssid;
}

String ESPAutoWifi::getPassword(){
    if(checkCredentials())  return _password;
}

void ESPAutoWifi::reset(){
    preferences.begin("credentials", false);
    preferences.clear();
    preferences.end();
}

void ESPAutoWifi::autoConnect(){
    if(!checkCredentials()){
        startConfig();
    }
    while(checkCredentials() != true){
        Serial.println(".");
        delay(500);
    }
    connect();
}

void ESPAutoWifi::startConfig(){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(_ap_ssid.c_str(), NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html);
    });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
      if (request->hasParam("username")) {
        _ssid = request->getParam("username")->value();
      }
      // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
      if (request->hasParam("password")) {
        _password = request->getParam("password")->value();
      }
      preferences.begin("credentials",false);
      preferences.putString("ssid", _ssid);
      preferences.putString("password", _password);
      preferences.end();
      Serial.println(_ssid);
      request->send(200, "text/html", "Saving wifi to " + _ssid + "<br><a href=\"/\">Return to Home Page</a>");
    });
    server.onNotFound(notFound);
    server.begin();
}

void ESPAutoWifi::connect(){
    if(checkCredentials()){
        server.end();
        WiFi.mode(WIFI_STA);
        WiFi.begin(_ssid.c_str(), _password.c_str());
        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.println("WiFi Failed!");
            preferences.clear();
            return;
        }
        Serial.println();
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else{
        Serial.println("Please Input Credentials!");
        startConfig();
    }
}