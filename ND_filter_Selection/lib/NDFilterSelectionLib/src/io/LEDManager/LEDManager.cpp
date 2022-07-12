#include "LEDManager.hpp"

LEDManager::LEDManager(uint8_t pin) : _ledPin(pin), _previousMillis(0), _ledState(false) {}

LEDManager::~LEDManager() {}

void LEDManager::begin()
{
    pinMode(_ledPin, OUTPUT);
    onOff(false);

    _leds[0] = 0;
    _leds[1] = 0;
    _leds[2] = 0;

    for (auto &led : _leds)
    {
        if (led > 0)
        {
            pinMode(led, OUTPUT);
        }
    }
}

void LEDManager::onOff(bool state) const
{
    digitalWrite(_ledPin, state);
}

void LEDManager::blink(unsigned long time)
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= time)
    {
        _previousMillis = currentMillis;
        _ledState = !_ledState;
        onOff(_ledState);
    }
}

void LEDManager::displayStatus()
{
}

void LEDManager::indicateWaterLevel(int level)
{

    if (level <= 25)
    {
        digitalWrite(_leds[1], LOW);
        digitalWrite(_leds[2], LOW);
        digitalWrite(_leds[0], HIGH);
    }
    else if (level > 25 && level < 50)
    {
        digitalWrite(_leds[0], LOW);
        digitalWrite(_leds[2], LOW);
        digitalWrite(_leds[1], HIGH);
    }
    else if (level > 75)
    {
        digitalWrite(_leds[0], LOW);
        digitalWrite(_leds[1], LOW);
        digitalWrite(_leds[2], HIGH);
    }
}

LEDManager ledManager(26); //! TODO: replace with a custom pin number