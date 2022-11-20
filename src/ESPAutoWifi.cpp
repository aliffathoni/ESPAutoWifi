#include <ESPAutoWifi.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <Preferences.h>

#include <web.h>

AsyncWebServer server(80);

Preferences preferences;

ESPAutoWifi::ESPAutoWifi(){}

String  ssid,
        password,
        ap_ssid;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

bool ESPAutoWifi::checkCredentials(){
    preferences.begin("credentials", false);
    ssid =  preferences.getString("ssid", "");
    password =  preferences.getString("password", "");
    preferences.end();

    if((ssid!="") && (password!="")){
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
    WiFi.softAP("ESP32", NULL);

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
        ssid = request->getParam("username")->value();
      }
      // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
      if (request->hasParam("password")) {
        password = request->getParam("password")->value();
      }
      preferences.begin("credentials",false);
      preferences.putString("ssid", ssid);
      preferences.putString("password", password);
      preferences.end();
      Serial.println(ssid);
      request->send(200, "text/html", "Saving wifi to " + ssid + "<br><a href=\"/\">Return to Home Page</a>");
    });
    server.onNotFound(notFound);
    server.begin();
}

void ESPAutoWifi::connect(){
    if(checkCredentials()){
        server.end();
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
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