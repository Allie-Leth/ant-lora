#pragma once
/**
 * @file Esp32Hal.h
 * @brief ESP32-specific IHal implementation (with host stubs).
 *
 * On ESP32 builds (ESP_PLATFORM or ARDUINO), pulls in the real
 * <Arduino.h>, <SPI.h>, and <esp_timer.h>.  On native builds,
 * defines minimal stubs so this header still compiles.
 */

#include "ant/hal/IHal.h" // your single HAL interface

#if defined(ESP_PLATFORM) || defined(ARDUINO)

#include <Arduino.h>
#include <SPI.h>
#include <esp_timer.h>

#else   // ====================  host / unit‑test stubs  ====================

  #include <cstdint>
  #include <cstddef>

  /* --- Arduino‑style constants ----------------------------------------- */
  constexpr int HIGH      = 1;
  constexpr int LOW       = 0;
  constexpr int OUTPUT    = 1;
  constexpr int INPUT     = 0;

  constexpr uint8_t MSBFIRST  = 0;
  constexpr uint8_t SPI_MODE0 = 0;

  /* --- Minimal SPISettings / SPIClass stubs ---------------------------- */
  struct SPISettings {
    SPISettings(uint32_t, uint8_t, uint8_t) {}
  };

  class SPIClass {
  public:
    void begin() {}
    void beginTransaction(const SPISettings&) {}
    void transferBytes(uint8_t* /*tx*/, uint8_t* /*rx*/, uint32_t /*len*/) {}
  };

  /* --- GPIO & timing stubs -------------------------------------------- */
  inline void pinMode(uint8_t, int)               {}
  inline void digitalWrite(uint8_t, int)          {}
  inline bool digitalRead(uint8_t)                { return false; }
  inline void delay(uint32_t)                     {}
  inline void delayMicroseconds(uint32_t)         {}
  inline uint64_t esp_timer_get_time()            { return 0; }

#endif  // ESP_PLATFORM / ARDUINO


namespace ant::hal
{

  /** Bundle of pins used by Esp32Hal and the SX1262 driver */
  struct Esp32Pins {
    uint8_t cs;
    uint8_t reset;
    uint8_t busy;
    uint8_t dio1;

    /** @brief pin bundle ctor (with defaults matching old initials) */
    constexpr Esp32Pins(uint8_t cs_    = 10,
                        uint8_t reset_ =  8,
                        uint8_t busy_  =  9,
                        uint8_t dio1_  =  3) noexcept
      : cs(cs_), reset(reset_), busy(busy_), dio1(dio1_) {}
};


  /**
   * @class Esp32Hal
   * @brief Concrete IHal for ESP32-class targets (or stubbed on host).
   */
  class Esp32Hal final : public IHal
  {
  public:
    explicit Esp32Hal(SPIClass &spiBus,
                      Esp32Pins pins,
                      uint32_t spiFreqHz = 8000000UL) noexcept
        : spi(spiBus), pins(pins), spiFreqHz(spiFreqHz) {}

    // --- IHal interface ---
    void spiBegin() override
    {
      pinMode(pins.cs, OUTPUT);
      pinMode(pins.reset, OUTPUT);
      pinMode(pins.busy, INPUT);
      pinMode(pins.dio1, INPUT);

      digitalWrite(pins.cs, HIGH);
      digitalWrite(pins.reset, HIGH);

      spi.begin();
      spi.beginTransaction(SPISettings(spiFreqHz, MSBFIRST, SPI_MODE0));
    }

    void spiTransfer(const uint8_t *tx, uint8_t *rx, size_t len) override
    {
      // Note: cast away const only in stub; on real hardware it's fine too
      spi.transferBytes(const_cast<uint8_t *>(tx), rx,
                        static_cast<uint32_t>(len));
    }

    void csSelect() override { digitalWrite(pins.cs, LOW); }
    void csDeselect() override { digitalWrite(pins.cs, HIGH); }

    void pinWrite(uint8_t pin, bool level) override
    {
      digitalWrite(pin, level ? HIGH : LOW);
    }
    bool pinRead(uint8_t pin) override
    {
      return digitalRead(pin);
    }

    void delayMs(uint32_t ms) override { delay(ms); }

    uint64_t ticks_ms() override
    {
      // real: esp_timer_get_time() in μs; stub: always returns 0
      return static_cast<uint64_t>(esp_timer_get_time() / 1000ULL);
    }

    // Optional IRQ hooks left as no-ops by default
    void attachIrq(uint8_t pin,
                   void (*cb)(),
                   int mode) override
    {
#if defined(ESP_PLATFORM) || defined(ARDUINO)
      ::attachInterrupt(pin, cb, mode);
#else
      (void)pin;
      (void)cb;
      (void)mode;
#endif
    }
    void detachIrq(uint8_t pin) override
    {
#if defined(ESP_PLATFORM) || defined(ARDUINO)
      ::detachInterrupt(pin);
#else
      (void)pin;
#endif
    }

  private:
    SPIClass &spi;
    Esp32Pins pins;
    uint32_t spiFreqHz;
  };

} // namespace ant::hal
