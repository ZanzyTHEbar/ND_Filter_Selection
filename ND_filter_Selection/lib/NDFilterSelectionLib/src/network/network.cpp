/**
 * WiFi Events
 *0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
 *1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
 *2  SYSTEM_EVENT_STA_START                < ESP32 station start
 *3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
 *4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
 *5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
 *6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
 *7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
 *8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
 *9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
 *10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
 *11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
 *12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
 *13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
 *14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
 *15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
 *16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
 *17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
 *18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
 *19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
 *20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
 *21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
 *22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
 *23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
 *24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
 *25 SYSTEM_EVENT_MAX

 WiFi.status() return values:
    *0  WL_IDLE_STATUS                       < ESP32 station is in idle state
    *1  WL_NO_SSID_AVAIL                     < ESP32 station cannot find any SSID in scan results
    *2  WL_SCAN_COMPLETED                    < ESP32 station has finished the scan
    *3  WL_CONNECTED                         < ESP32 station has a connection to an AP
    *4  WL_CONNECT_FAILED                    < ESP32 station failed to connect to an AP
    *5  WL_CONNECTION_LOST                   < ESP32 station has lost connection to an AP
    *6  WL_DISCONNECTED                      < ESP32 station is in disconnected state
 */

#include "network.hpp"

AsyncWebServer server(80);

WiFiClient espClient;

/**
 * @brief Construct a new Network:: Network object
 *
 */
Network::Network() : _previousMillis(0),
                     _interval(30000),
                     _wifiConnected(false),
                     _jsonDocument("") {}

/**
 * @brief Destroy the Network:: Network object
 *
 */
Network::~Network() {}

void notFound(AsyncWebServerRequest *request)
{
    log_i("NOT_FOUND: ");
    if (request->method() == HTTP_GET)
        log_i("GET");
    else if (request->method() == HTTP_POST)
        log_i("POST");
    else if (request->method() == HTTP_DELETE)
        log_i("DELETE");
    else if (request->method() == HTTP_PUT)
        log_i("PUT");
    else if (request->method() == HTTP_PATCH)
        log_i("PATCH");
    else if (request->method() == HTTP_HEAD)
        log_i("HEAD");
    else if (request->method() == HTTP_OPTIONS)
        log_i("OPTIONS");
    else
        log_i("UNKNOWN");
    log_i(" http://%s%s\n", request->host().c_str(), request->url().c_str());
    request->send(404, "text/plain", "Not found.");
}

void Network::networkRoutes()
{
    static const char *MIMETYPE_HTML{"text/html"};
    /* static const char *MIMETYPE_CSS{"text/css"}; */
    /* static const char *MIMETYPE_JS{"application/javascript"}; */
    /* static const char *MIMETYPE_PNG{"image/png"}; */
    /* static const char *MIMETYPE_JPG{"image/jpeg"}; */
    /* static const char *MIMETYPE_ICO{"image/x-icon"}; */
    static const char *MIMETYPE_JSON{"application/json"};

    server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/frontend.html", MIMETYPE_HTML); });

    server.serveStatic("/", SPIFFS, "/");

    // preflight cors check
    server.on("/", HTTP_OPTIONS, [&](AsyncWebServerRequest *request)
              {
        AsyncWebServerResponse* response = request->beginResponse(204);
        response->addHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        request->send(response); });

    server.on("/data.json", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                  delay(100);
                  String temp = _jsonDocument;
                  request->send(200, MIMETYPE_JSON, temp);
                  temp = ""; });

    server.on("/api/reset/device", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                  request->send(200);
                  delay(1000);
                  ESP.restart(); });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.onNotFound(notFound);
    server.begin();
}

bool Network::initSPIFFS()
{
    if (!SPIFFS.begin(false))
    {
        log_e("An error has occurred while mounting SPIFFS");
        return false;
    }
    log_i("SPIFFS mounted successfully");
    return true;
}

void Network::SetupServer()
{
    initSPIFFS();
    log_i("\n");
    log_i("System Information:\n");
    log_i("\n");
    log_i("PLatformI0 compile time: %s (%s)\n", __DATE__, __TIME__);
    log_i("PLatformI0 Unix compile time: %d\n", COMPILE_UNIX_TIME);
    log_i("Version: %d\n", VERSION);
    log_i("Heap: %d\n", ESP.getFreeHeap());
    log_i("SDK: %s\n", ESP.getSdkVersion());
    log_i("MAC address: %s\n", WiFi.macAddress().c_str());
    log_i("CPU Speed: %dMHz\n", ESP.getCpuFreqMHz());
    log_i("Flash Size: %dKB\n", ESP.getFlashChipSize());
    log_i("[INFO]: System Information Sent\n");
    log_i("\n");

    // FS debug information
    // THIS NEEDS TO BE PAST THE WIFI SETUP!! OTHERWISE WIFI SETUP WILL BE DELAYED
    log_i("SPIFFS contents:\n");
#ifdef ESP32
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        log_i("FS File: %s, size: %i\n", file.name(), file.size());
        file = root.openNextFile();
    }
    log_i("\n");
    unsigned int totalBytes = SPIFFS.totalBytes();
    unsigned int usedBytes = SPIFFS.usedBytes();
    if (usedBytes == 0)
    {
        log_i("NO WEB SERVER FILES PRESENT: \n");
    }
    log_i("FS Size: %iKB, used: %iKB, %0.2f%%\n",
          totalBytes, usedBytes,
          (float)100 / totalBytes * usedBytes);
    log_i("\n");
#endif
}

/**
 * @brief Setup the network stack and routes - fail to AP mode if STA fails
 *
 */
void Network::SetupWebServer()
{
    // TODO: Route for root to  "Please Scan QR code"
    // TODO: There should be a reset mode that will reset the device to factory settings and restart the device.
    // TODO: Should be a physical reset button on the PCB itself - not a touch button - hold for 5 seconds to reset. Flash LED to indicate reset per second.
    // Connect to Wi-Fi Network with SSID and password

    log_i("[INFO]: Setting Access Point...\n");

    log_i("[INFO]: Configuring access point...\n");
    WiFi.mode(WIFI_AP);
    WiFi.setTxPower(WIFI_POWER_11dBm);

    // You can remove the password parameter if you want the AP to be open.
    log_i("Wifi Connection Failed. \r\nStarting AP. \r\nAP IP address: ");
    IPAddress IP = WiFi.softAPIP();
    log_i("[INFO]: AP IP address: %s.\r\n", IP.toString().c_str());

    if (!PRODUCTION)
    {
        WiFi.softAP(DEFAULT_HOSTNAME, NULL, 10, 0, 2); // AP mode without password
    }
    else
    {
        WiFi.softAP(WIFI_SSID, WIFI_PASS, 10, 0, 3); // AP mode with password
    }

    networkRoutes(); // call the network routes function to setup the routes
}

// ######################## server functions #########################

/******************************************************************************
 * Function: Check Network Connection Loop
 * Description: This function checks the Network connection and reconnects if necessary - is called in the loop() function every 5 seconds
 * Parameters: None
 * Return: None
 ******************************************************************************/
void Network::CheckNetworkLoop()
{
    // run current function every 5 seconds
    if (WiFi.status() != WL_CONNECTED)
    {
        _wifiConnected = false;
        log_i("Wifi is not connected\n");
    }
    else
    {
        _wifiConnected = true;
        log_i("Wifi is connected\n");
        log_i("[INFO]: WiFi Connected! Open http://%s in your browser\n", WiFi.localIP().toString().c_str());
    }
}

void Network::CheckConnectionLoop_Active()
{
    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting
    if (!_wifiConnected && (currentMillis - _previousMillis >= _interval))
    {
        Serial.print(millis());
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect(); // disconnect from previous Access Point's - if connected
        WiFi.reconnect();
        _previousMillis = currentMillis;
    }
}

void Network::createJSONDocument(float lux, String recommendation)
{
    String json = "";
    json += R"====({)====";
    json += R"====("lux_level":)====";
    json += (String)lux + ",\n";
    R"====("recommendation":)====";
    json += recommendation + "\n";
    json += R"====(})====";
    _jsonDocument = json;
}

Network network;
