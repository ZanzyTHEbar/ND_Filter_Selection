#include <Arduino.h>
#include "sensors/light/ldr.hpp"
#include "io/LEDManager/LEDManager.hpp"

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup");
  ldr.begin();
  // put your setup code here, to run once:
}

void loop()
{
  ldr.checkLuxState();
  delay(1000);
  // put your main code here, to run repeatedly:
}