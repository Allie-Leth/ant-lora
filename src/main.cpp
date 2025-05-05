#include <ant-lora.h>
#include <Arduino.h>
using ant::hal::Esp32Pins;
using ant::hal::Esp32Hal;
using ant::lora::SX1262Radio;
using ant::radio::RadioCfg;

SPIClass spi(HSPI);
Esp32Pins pins { 10, 8, 9, 3};
Esp32Hal  hal(spi, pins, 8000000);
SX1262Radio radio(hal);

// **Aggregate‐init `RadioCfg` with a nested braced `RfParams`**
RadioCfg cfg {
  pins,
  { 915000000UL,    // freqHz
    14 }            // powerDbm
};


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting up...");
  hal.spiBegin();
  if (!radio.init(cfg)) {
    Serial.println("SX1262 init failed!");
    while (true) delay(1000);
  }
  Serial.println("SX1262 init OK – sending beacons");
}

void loop() {
  const char msg[] = "hi";
  if (radio.send((const uint8_t*)msg, sizeof(msg)-1, 5000)) {
    Serial.println("sent");
  } else {
    Serial.println("tx failed");
  }
  delay(1000);
}
