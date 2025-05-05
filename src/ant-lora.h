#pragma once
/**
 * @file ant-lora.h
 * @brief Single include to pull in ant‑labs LoRa driver & HALs.
 *
 * Include this from your application:
 *
 *   #include <ant-lora.h>
 *
 * Then instantiate a platform HAL (e.g. Esp32Hal) and a radio driver
 * (e.g. SX1262Radio) as demonstrated in the example sketch.
 */

#include "ant/hal/esp32/Esp32Hal.h"          // concrete HAL for ESP32
#include "ant/radio/RadioCfg.h"              // pin + RF parameter struct
#include "ant/radio/SX1262/SX1262Radio.h"    // high‑level radio class

// optional convenience aliases
namespace ant::lora {
  using Esp32Hal    = ant::hal::Esp32Hal;
  using Esp32Pins   = ant::hal::Esp32Pins;
  using RadioCfg    = ant::radio::RadioCfg;
  using SX1262Radio = ::SX1262Radio;
} // namespace ant::lora
