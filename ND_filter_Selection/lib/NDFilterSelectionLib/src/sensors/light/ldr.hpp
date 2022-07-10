#ifndef LDR_HPP
#define LDR_HPP
#include <Arduino.h>
#include <map>
#include "StateManager/StateManager.hpp"
#include "config/config.hpp"

#define LDR_PIN 33

/* const float const ldrLookupTable[] = {
    0.0001,
    0.00014,
    0.0002,
    0.002,
    0.01,
    0.25,
    0.0,
    40,
    200,
    400,
    2000,
    20000,
    109880,
    111000,
    120000}; */

class LDR
{
public:
    LDR();
    virtual ~LDR();
    void begin();

    void setRL10(float RL10);
    void setGamma(float gamma);
    void checkLuxState();
    //void printMap(String comment, const std::map<String, float> &map);

private:
    /* LDR_t _LDR; */
    float getLux();
    float _GAMMA;
    float _RL10;
};
extern LDR ldr;
#endif