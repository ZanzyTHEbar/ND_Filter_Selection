#ifndef LDR_HPP
#define LDR_HPP
#include <Arduino.h>

// These constants, define values needed for the LDR readings and ADC
#define LDR_PIN 33
#define MAX_ADC_READING 4096
#define ADC_REF_VOLTAGE 3.3
#define REF_RESISTANCE 10030 // measure this for best results
#define LUX_CALC_SCALAR 12518931
#define LUX_CALC_EXPONENT -1.405

const float ldrLookupTable[] = {
    0.0,
    10.9,
    11.0,
    50.9,
    51.0,
    200.9,
    201.0,
    400.9,
    401.0,
    1000.9,
    1001.0,
    5000.9,
    5001.0,
    10000.9,
    10001.0,
    30000.9,
    30001.0,
    100000.9,
    100001.0,
    110000.9,
    111000.0,
    120000.9,
    121000.0,
    300000.9,
    300001.0,
    1000000.9,
};

class LDR
{
public:
    LDR();
    virtual ~LDR();
    void begin();
    void checkLuxState();

private:
    float getLux();
};
extern LDR ldr;
#endif // LDR_HPP
