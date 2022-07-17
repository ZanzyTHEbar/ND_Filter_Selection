#include <Arduino.h>
#include "sensors/light/ldr.hpp"
#include "io/LEDManager/LEDManager.hpp"
#include "network/network.hpp"

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup");
  ldr.begin();
  ledManager.begin();
  network.SetupServer();
  network.SetupWebServer();
  // put your setup code here, to run once:
}

void loop()
{
  ldr.checkLuxState();
  ledManager.displayStatus();
  ledManager.blink(1000L);
  network.createJSONDocument(ldr.getData().lux, ldr.getData().recommendation);
}