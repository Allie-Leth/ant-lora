#pragma once

#include <SX1262/HalInterface.h>
#include <Arduino.h>
#include <SPI.h>

namespace SX1262 {

/**
 * @file Esp32Hal.h
 * @brief ESP32 implementation of HalIface using Arduino SPI/GPIO, with 64-bit timing.
 */
class Esp32Hal : public HalIface {
public:
  /**
   * @brief Construct with SPI bus and clock speed.
   * @param spiBus        Reference to the SPIClass instance (default ::SPI).
   * @param spiFrequency  SPI clock in Hz (default 8 MHz).
   */
  explicit Esp32Hal(SPIClass& spiBus = ::SPI, uint32_t spiFrequency = 8000000);

  // SPI
  void    spiBegin() override;
  void    spiBeginTransaction() override;
  void    spiEndTransaction() override;
  uint8_t spiTransfer(uint8_t data) override;

  // GPIO
  void    pinMode(uint8_t pin, bool isOutput) override;
  void    digitalWrite(uint8_t pin, bool level) override;
  bool    digitalRead(uint8_t pin) override;

  // Timing
  void    delayMs(uint32_t ms) override;
  uint64_t getTimeMs() override;

  // Interrupts
  void    attachInterrupt(uint8_t pin,
                          std::function<void()> cb,
                          int mode) override;
  void    detachInterrupt(uint8_t pin) override;

private:
  SPIClass& _spi;           ///< Underlying SPI bus
  uint32_t  _spiFrequency;  ///< SPI clock rate
};

} // namespace SX1262
 