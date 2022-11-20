#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
String ssid = "free wifi";
String password = "71759900";

#include <Preferences.h>

Preferences preferences;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title> Login Page </title>
<style>
Body {
  font-family: Calibri, Helvetica, sans-serif;
  background-color: black;
  padding-left: 50px;
  padding-right: 50px;
}
button {
    background-color: #4CAF50;
    width: 100%;
    color: white;
    padding: 15px;
    margin: 10px 0px;
    border: none;
    cursor: pointer;
    border-radius: 7px;
}
form {
    border: 5px solid #f1f1f1;
    border-radius: 7px;
}
input[type=text], input[type=password] {
    width: 100%;
    margin: 8px 0;
    padding: 12px 20px;
    display: inline-block;
    border: 2px solid green;
    box-sizing: border-box;
    border-radius: 7px;
}
button:hover {
    opacity: 0.8;
}
.cancelbtn {
    width: 100%;
    background-color: tomato;
    color: white;
    padding: 15px;
    margin: 10px 0px;
    border: none;
    cursor: pointer;
    border-radius: 7px;   
}
.container { 
    padding: 25px;
    padding-left: 75px;
    padding-right: 75px;
    background-color: lightblue;
    border-radius: 1px;
}
</style>
</head>
<body>
    <center> <h1 style="color:white;"> Input WiFi Credentials </h1> </center>
    <form action="/get">
        <div class="container">
            <label>SSID : </label>
            <input type="text" placeholder="Enter WiFi SSID" name="username" required>
            <label>Password : </label>
            <input type="password" placeholder="Enter Password" name="password" required>
            <button type="submit">Connect</button>
            <button type="button" class="cancelbtn"> Cancel</button>
            <div style="text-align: center;"> Show saved <a href="#"> password</a> </div>
        </div>
    </form>
</body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  preferences.begin("credentials", false);
  ssid =  preferences.getString("ssid", "");
  password =  preferences.getString("password", "");
  
  if(ssid=="" && password==""){
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
      preferences.putString("ssid", ssid);
      preferences.putString("password", password);
      Serial.println(ssid);
      request->send(200, "text/html", "Saving wifi to " + ssid + "<br><a href=\"/\">Return to Home Page</a>");
    });
    server.onNotFound(notFound);
    server.begin();
  }
  while(ssid=="" && password==""){
    Serial.println(".");
    delay(500);
  }
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

  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}