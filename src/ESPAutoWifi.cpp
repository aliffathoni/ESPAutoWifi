#include <ESPAutoWifi.h>
#include "Arduino.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>

#include <web.h>

AsyncWebServer server(80);
IPAddress IP_Address(192, 168, 4, 1);
IPAddress Gateway(192, 168, 1, 111);
IPAddress Subnet(255, 255, 255, 0);
IPAddress DNS(8, 8, 8, 8);

Preferences preferences;

String _ssid;
String _password;

ESPAutoWifi::ESPAutoWifi(){
    _ap_ssid = "ESP Wifi Portal";
    _ap_password = "";
}

ESPAutoWifi::ESPAutoWifi(String ap_ssid){
    _ap_ssid = ap_ssid;
    _ap_password = "";
}

ESPAutoWifi::ESPAutoWifi(String ap_ssid, String ap_password){
    _ap_ssid = ap_ssid;
    _ap_password = ap_password;
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

String ESPAutoWifi::getAPSSID(){
    return _ap_ssid;
}

String ESPAutoWifi::getAPPassword(){
    return _ap_password;
}

String ESPAutoWifi::getIP(){
  IPAddress IP = WiFi.localIP();
  return String(IP[0])+"."+String(IP[1])+"."+String(IP[2])+"."+String(IP[3]);
}

void ESPAutoWifi::reset(){
    preferences.begin("credentials", false);
    preferences.clear();
    preferences.end();
}

void ESPAutoWifi::autoConnect(){
    if(!checkCredentials()){
        startAP();
        startConfig();
    }
    while(checkCredentials() != true){
        //do nothing
    }
    connect();
}

void ESPAutoWifi::startAP(){
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IP_Address, Gateway, Subnet, DNS);
    if(_ap_password == ""){
        WiFi.softAP(_ap_ssid.c_str(), NULL);
    } else{
        WiFi.softAP(_ap_ssid.c_str(), _ap_password.c_str());
    }
}

String ESPAutoWifi::getAPIP(){
    IPAddress APIP = WiFi.softAPIP();
    return String(APIP);
}

void ESPAutoWifi::startConfig(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html);
    });

    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      if (request->hasParam("username")) {
        _ssid = request->getParam("username")->value();
      }
      if (request->hasParam("password")) {
        _password = request->getParam("password")->value();
      }
        preferences.begin("credentials",false);
        preferences.putString("ssid", _ssid);
        preferences.putString("password", _password);
        preferences.end();
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
    } else{
        Serial.println("Please Input Credentials!");
        startConfig();
    }
}