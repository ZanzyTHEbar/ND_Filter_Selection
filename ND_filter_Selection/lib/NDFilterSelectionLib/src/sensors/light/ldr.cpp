#include "ldr.hpp"

LDR::LDR() {}

LDR::~LDR() {}

void LDR::begin()
{
    pinMode(LDR_PIN, INPUT);
}

float LDR::getLux()
{
    // Perform the analog to digital conversion
    int _ldrRawData = analogRead(LDR_PIN);
    // RESISTOR VOLTAGE_CONVERSION
    // Convert the raw digital data back to the voltage that was measured on the analog pin
    float _resistorVoltage = (float)_ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
    // voltage across the LDR is the 3V supply minus the 5k resistor voltage
    float _ldrVoltage = ADC_REF_VOLTAGE - _resistorVoltage;
    // LDR_RESISTANCE_CONVERSION
    // resistance that the LDR would have for that voltage
    float _ldrResistance = _ldrVoltage / _resistorVoltage * REF_RESISTANCE;
    // LDR_LUX
    // Change the code below to the proper conversion from ldrResistance to
    // ldrLux
    float _ldrLux = LUX_CALC_SCALAR * pow(_ldrResistance, LUX_CALC_EXPONENT);
    // print out the results
    log_i("LDR Raw Data   : %d", _ldrRawData);
    log_i("LDR Voltage : %.3f volts", _ldrVoltage);
    log_i("LDR Resistance : %.3f Ohms", _ldrResistance);
    log_i("LDR Illuminance: %.3f\n", _ldrLux);
    return _ldrLux;
}

LDR::Data_t LDR::getData()
{
    return data;
}

void LDR::setData(LDR::Data_t data)
{
    this->data = data;
}

void LDR::checkLuxState()
{
    float lux = getLux();

    if (lux <= 0)
    {
        log_i("Pitch Black: %.3f\n", lux);
        data = {lux, "No Filter"};
    }
    else if (lux >= ldrLookupTable[0] && lux <= ldrLookupTable[1])
    {
        log_i("Pitch Black: %.3f\n", lux);
        data = {lux, "No Filter"};
    }
    else if (lux >= ldrLookupTable[2] && lux <= ldrLookupTable[3])
    {
        log_i("Very Dark: %.3f\n", lux);
        data = {lux, "No Filter"};
    }
    else if (lux >= ldrLookupTable[4] && lux <= ldrLookupTable[5])
    {
        log_i("Dark Indoors: %.3f\n", lux);
        data = {lux, "No Filter"};
    }
    else if (lux >= ldrLookupTable[6] && lux <= ldrLookupTable[7])
    {
        log_i("Dim Indoors: %.3f\n", lux);
        data = {lux, "ND4"};
    }
    else if (lux >= ldrLookupTable[8] && lux <= ldrLookupTable[9])
    {
        log_i("Normal Indoors: %.3f\n", lux);
        data = {lux, "ND4"};
    }
    else if (lux >= ldrLookupTable[10] && lux <= ldrLookupTable[11])
    {
        log_i("Bright Indoors: %.3f\n", lux);
        data = {lux, "ND4"};
    }
    else if (lux >= ldrLookupTable[12] && lux <= ldrLookupTable[13])
    {
        log_i("Dim Outdoors: %.3f\n", lux);
        data = {lux, "ND8"};
    }
    else if (lux >= ldrLookupTable[14] && lux <= ldrLookupTable[15])
    {
        log_i("Cloudy Outdoors: %.3f\n", lux);
        data = {lux, "ND8"};
    }
    else if (lux >= ldrLookupTable[16] && lux <= ldrLookupTable[17])
    {
        log_i("Bright Overcast: %.3f\n", lux);
        data = {lux, "ND8"};
    }
    else if (lux >= ldrLookupTable[18] && lux <= ldrLookupTable[19])
    {
        log_i("Normal Daylight: %.3f\n", lux);
        data = {lux, "ND16"};
    }
    else if (lux >= ldrLookupTable[20] && lux <= ldrLookupTable[21])
    {
        log_i("Bright Daylight: %.3f\n", lux);
        data = {lux, "ND16"};
    }
    else if (lux >= ldrLookupTable[22] && lux <= ldrLookupTable[23])
    {
        log_i("Very Bright: %.3f\n", lux);
        data = {lux, "ND16 or ND32"};
    }
    else if (lux >= ldrLookupTable[24] && lux <= ldrLookupTable[25])
    {
        log_i("Brightest: %.3f\n", lux);
        data = {lux, "ND32"};
    }
    else
    {
        log_i("Error: %.3f\n", lux);
        data = {lux, "ND32 or ND64"};
    }
}

LDR ldr;