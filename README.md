# ESP-Auto-Wifi

ESP Auto Connect Wifi without hardcode. As for now this code only works on ESP32 architecture, but i will update it soon on ESP8266.

## Dependencies
Make sure to download and install these library on your Arduino IDE to ESPAutoWifi

 - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
 - [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)


## Simple shortest code
You can use this code to use the main feature of this library.

```
  #include <ESPAutoWifi.h>

  ESPAutoWifi wf;

  void loop(){
    wf.autoConnect();
  }

  void loop(){

  }
```

If there is no saved network on your ESP then open your WiFi using other device(s) and connect to "ESP WiFi Portal" wifi, then type 192.168.4.1 on your browser to access the web portal.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details.