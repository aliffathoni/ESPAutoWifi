/**
 * @file       ESPAutoWifi.h
 * @author     Alif Fathoni
 * @license    MIT License
 * @copyright  Copyright (c) 2022 AkaY
 * @date       Nov 2022
 */

#ifndef ESPAutoWifi_h
#define ESPAutoWifi_h

#include "Arduino.h"

/*!
    @brief  ESPAutoWifi class for function and variables
 */

class ESPAutoWifi {
    public:
        
        /*!
            @brief  Instantiate ESPAutoWifi with default AP SSID
        */
        ESPAutoWifi();
        
        /*!
            @brief  Instantiate ESPAutoWifi with custom SSID
            @param  ap_ssid Custom AP SSID for web portal
        */
        ESPAutoWifi(String ap_ssid);
        
        /*!
            @brief  Instantiate ESPAutoWifi with custom SSID
            @param  ap_ssid Custom AP SSID for WiFi
            @param  ap_password Custom AP Password for WiFi
        */
        ESPAutoWifi(String ap_ssid, String ap_password);

        /*!
            @brief  Check saved credentials on flash memory storage
            @return True if there is saved credentials on system
        */
        bool checkCredentials();
        
        /*!
            @brief  Check wether device already connected with WiFi
            @return True if connected properly
        */
        bool checkConnection();
        
        /*!
            @brief  Connect to WiFi using saved credentials
        */
        void connect();
        
        /*!
            @brief  Connect to WiFi using saved credentials
        */
        void reset();
        
        /*!
            @brief  Connect to WiFi using saved credentials
                    and open web portal to input credentials
                    if there is no saved credentials
        */
        void autoConnect();
        
        /*!
            @brief  Create WiFi AP for user to connectand save
                    credentials using web portal
        */
        void startAP();

        /*!
            @brief  Create web portal to input credentials and
                    save SSID and Password for ESP to connect
        */
        void startConfig();

        /*!
            @brief  Get ESP AP IP Address to connect with the web portal
            @return ESP AP IP Address
        */
        String getAPIP();
        
        /*!
            @brief  Get ESP saved SSID to connect with WiFi
            @return ESP saved SSID
        */
        String getSSID();
        
        /*!
            @brief  Get ESP saved Password to connect with WiFi
            @return ESP saved Password
        */
        String getPassword();
        
        /*!
            @brief  Get ESP saved AP SSID to connect with web portal
            @return ESP saved AP SSID
        */
        String getAPSSID();
        
        /*!
            @brief  Get ESP saved AP Password to connect with web portal
            @return ESP saved AP Password
        */
        String getAPPassword();

        /*!
            @brief  Get ESP IP Address
            @return ESP IP Address
        */
        String getIP();

    private:
        String _ap_ssid;
        String _ap_password;
};

#endif