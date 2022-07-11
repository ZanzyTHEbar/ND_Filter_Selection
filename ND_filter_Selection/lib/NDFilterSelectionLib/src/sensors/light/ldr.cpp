#include "ldr.hpp"

Config config("config", "Project_Config");

LDR::LDR() : _GAMMA(0.7), _RL10(50) {}

LDR::~LDR() {}

void LDR::begin()
{
    pinMode(LDR_PIN, INPUT);
    Config::LDR_t ldr_map[] = {
        {"Starlight-overcast-no moon", 0.0001},
        {"Venus at brightest", 0.00014},
        {"Starlight-no moon-no airglow", 0.0002},
        {"Starlight-no moon", 0.002},
        {"Quarter Moon", 0.01},
        {"Full Moon", 0.25},
        {"Night Time", 0.0},
        {"Fully OverCast", 40},
        {"Extreme of thickest storm clouds, midday", 200},
        {"Sunrise or Sunset", 400},
        {"Overcast", 2000},
        {"Shade with clear skies", 20000},
        {"Daylight", 109880},
        {"Bright sunlight", 111000},
        {"Brightest sunlight", 120000},
    };
    config.begin();
    for (int i = 0; i < sizeof(ldr_map) / sizeof(ldr_map[0]); ++i)
    {
        config.write("LDR_Map", ldr_map[i]);
    }
    delay(100);
    // config.write("ldr_map", ldrLookupTable);
}

void LDR::setRL10(float RL10)
{
    _RL10 = RL10;
    config.write("LDR_RL10", _RL10);
}

void LDR::setGamma(float gamma)
{
    _GAMMA = gamma;
    config.write("LDR_GAMMA", _GAMMA);
}

float LDR::getLux()
{
    // Convert the analog value into lux value:
    float readLDR = analogRead(LDR_PIN);
#if defined(ESP32)
    float voltage = readLDR / 4096.0 * 3.3;
    float resistance = 2000.0 * voltage / (1.0 - voltage / 3.3);
#else  // defined(ESP32)
    float voltage = readLDR / 1024.0 * 5.0;
    float resistance = 2000.0 * voltage / (1.0 - voltage / 5.0);
#endif // defined(ESP32)

    float lux = pow(_RL10 * 1e3 * pow(10.0, _GAMMA) / resistance, (1.0 / _GAMMA));
    char buffer[100];
    dtostrf(lux, 10, 3, buffer);
    log_i("%s\n", buffer);
    return lux;
}

void LDR::checkLuxState()
{
    float lux = getLux();

    float ldr_map_temp[15];
    config.read("LDR_Map", *ldr_map_temp);
    for (auto &it : ldr_map_temp)
    {
        log_i("%f\n", it);
    }
}

/* void LDR::printMap(String comment, const std::map<String, float> &map)
{
    log_i("%s\n", comment.c_str());
    for (auto &it : map)
    {
        log_i("%s: %f\n", it.first.c_str(), it.second);
    }
} */

LDR ldr;