// include/ant-lora/LoRaRadio.h
#pragma once

#include <cstdint>
#include <functional>
#include "ant/radio/SX1262/Radio.h"

namespace ant {
namespace lora {

/** @brief Called when a TX finishes. */
using TxDoneCb = std::function<void()>;

/**
 * @brief Called when a packet arrives.
 * @param data Pointer to payload bytes.
 * @param len  Length of payload.
 */
using RxDoneCb = std::function<void(const uint8_t* data, size_t len)>;

/**
 * @class LoRaRadio
 * @brief High-level façade for mesh communications built on SX1262::Radio.
 *
 * Exposes a clean API (init, sendChunk, onReceive, etc.) and hides SX1262-specific details.
 */
class LoRaRadio {
public:
    LoRaRadio();
    ~LoRaRadio();

    /**
     * @brief Initialize the radio for mesh use.
     * @param csPin        Chip-select GPIO
     * @param resetPin     Reset GPIO
     * @param busyPin      Busy/status GPIO
     * @param dio1Pin      DIO1 interrupt GPIO
     * @param freqHz       RF frequency in Hz
     * @param txPowerDbm   TX power in dBm
     * @param modemCfg     LoRa modem settings
     * @param txCb         TX-done callback
     * @param rxCb         RX-done callback
     * @return true on successful init
     */
    bool init(
      uint8_t        csPin,
      uint8_t        resetPin,
      uint8_t        busyPin,
      uint8_t        dio1Pin,
      uint32_t       freqHz,
      int8_t         txPowerDbm,
      const SX1262::ModemConfig& modemCfg,
      TxDoneCb       txCb,
      RxDoneCb       rxCb
    );

    /** @brief Send a single mesh packet chunk. */
    bool sendChunk(const uint8_t* data, size_t len, uint32_t timeoutMs = 0);

    /** @brief Receive the next available chunk into buffer. */
    bool receiveChunk(uint8_t* buffer, size_t& len, uint32_t timeoutMs = 0);

    /** @brief Shutdown the radio (enter sleep). */
    void sleep();

    /** @brief Wake the radio from sleep. */
    void wakeup();

private:
    SX1262::Radio radio_;  ///< Internal SX1262 driver
};

} // namespace lora
} // namespace ant
