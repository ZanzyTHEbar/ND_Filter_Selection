/*
 Network.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
*/
#pragma once
#ifndef Network_hpp
#define Network_hpp

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

extern WiFiClient espClient;

class Network
{
public:
    // constructors
    Network();
    virtual ~Network();
    // Functions
    void SetupWebServer();
    void SetupServer();
    void networkRoutes();
    void CheckNetworkLoop();
    void CheckConnectionLoop_Active();
    void createJSONDocument(float lux, String recommendation);

    // variables
private:
    bool initSPIFFS();
    int CheckWifiState();
    // Timer variables
    unsigned long _previousMillis;
    const long _interval; // interval to wait for Wi-Fi connection (milliseconds)
    bool _wifiConnected;
    String _jsonDocument;
};

extern Network network;
#endif
