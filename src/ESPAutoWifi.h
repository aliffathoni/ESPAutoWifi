#ifndef _ESP_AUTO_WIFI_H
#define _ESP_AUTO_WIFI_H

#include "Arduino.h"
#include "String.h"

class ESPAutoWifi {
    public:

        ESPAutoWifi();
        
        bool checkCredentials();
        
        bool checkConnection();
        
        void reset();
        
        void autoConnect();
        
        void startConfig();
        
        void connect();
        
};

#endif