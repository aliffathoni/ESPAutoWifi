#ifndef ESPAutoWifi_h
#define ESPAutoWifi_h

#include "Arduino.h"
#include "String.h"

class ESPAutoWifi {
    public:
        ESPAutoWifi(String ap);
        bool checkCredentials();
        bool checkConnection();
        void reset();
        void autoConnect();
        void startConfig();
        void connect();
        String getSSID();
        String getPassword();
    private:
        String _ap_ssid;
};

#endif