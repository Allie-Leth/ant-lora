#pragma once
#include "ant/hal/IHal.h"   // single HAL interface – no HalInterface.h

namespace SX1262 {

/**
 * @brief Thin helper that forwards to a user‑supplied IHal.
 */
class LoRaHal {
public:
  static void init(ant::hal::IHal& h,
                   uint8_t csPin,
                   uint8_t resetPin,
                   uint8_t busyPin,
                   uint8_t dio1Pin)
  {
    _hal   = &h;
    _cs    = csPin;
    _reset = resetPin;
    _busy  = busyPin;
    _dio1  = dio1Pin;
  }

  /* 1‑to‑1 pass‑through helpers */
  inline static void select()        { _hal->csSelect(); }
  inline static void deselect()      { _hal->csDeselect(); }

  inline static uint8_t xfer(uint8_t b) {
    uint8_t rx;
    _hal->spiTransfer(&b, &rx, 1);
    return rx;
  }
  static void xferBuf(const uint8_t* buf, size_t len) {
    _hal->spiTransfer(buf, nullptr, len);
  }
  static void readBuf(uint8_t* buf, size_t len) {
    _hal->spiTransfer(nullptr, buf, len);
  }

  inline static void resetAssert()   { _hal->pinWrite(_reset, false); }
  inline static void resetDeassert() { _hal->pinWrite(_reset, true);  }

  inline static bool isBusy()        { return _hal->pinRead(_busy); }

  static bool waitBusy(uint32_t timeoutMs = 100) {
    uint64_t t0 = _hal->ticks_ms();
    while (isBusy()) {
      if ((_hal->ticks_ms() - t0) >= timeoutMs) return false;
    }
    return true;
  }

  // Interrupt helpers are no‑ops on host
  inline static void attachDio1(void (*isr)()) { _hal->attachIrq(_dio1, isr, /*RISING*/1); }
  inline static void detachDio1()              { _hal->detachIrq(_dio1); }

private:
  static inline ant::hal::IHal* _hal = nullptr;
  static inline uint8_t _cs{}, _reset{}, _busy{}, _dio1{};
};

} // namespace SX1262
