#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <Arduino.h>
#include "StateManager/StateManager.hpp"
#include <Preferences.h>

struct LDR_t
{
    /*  std::map<String, float> lightValues{
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
     }; */

    const char *key;
    float value;
};

class Config
{
public:
    Config(const char *configName, const char *partitionName);
    virtual ~Config();
    void begin();
    template <typename T>
    void write(const char *key, T &buff);
    template <typename T>
    void read(const char *key, T *buff);
    void clear();
    void remove(const char *key);
    size_t getValueLength(const char *key);
    byte getType(const char *key);
    size_t freeEntries();
    void checkConfigState();

private:
    Preferences *_preferences;
    const char *_configName;
    const char *_partitionName;
};
extern Config config;
#endif