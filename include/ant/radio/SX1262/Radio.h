#pragma once

#include "Config.h"
#include "Callbacks.h"
#include "ModemConfig.h"

namespace SX1262 {

/**
 * @class Radio
 * @brief Core SX1262 driver: initialize, send, receive, sleep, wakeup.
 *
 * This class wraps the mid-level Cmd::* calls into convenient member functions.
 */
class Radio {
public:
    /** @brief Construct an uninitialized Radio. */
    Radio() = default;

    /**
     * @brief Initialize the hardware and apply default radio parameters.
     * @param cfg Configuration (pins, SPI, callbacks, defaults).
     * @return true on success, false on hardware error.
     */
    bool init(const Config& cfg);

    /** @brief Change RF frequency (Hz). */
    void setFrequency(uint32_t freqHz);

    /** @brief Change TX output power (dBm). */
    void setTxPower(int8_t dBm);

    /** @brief Apply a LoRa modem config (SF/BW/CR, header, CRC). */
    void setModemConfig(const ModemConfig& mcfg);

    /**
     * @brief Send a packet.
     * @param data Pointer to payload.
     * @param len  Number of bytes to send.
     * @param timeoutMs How long to wait for TX_DONE (0 = blocking forever).
     * @return true if TX completed before timeout.
     */
    bool send(const uint8_t* data, size_t len, uint32_t timeoutMs = 0);

    /**
     * @brief Receive a packet.
     * @param outBuffer Buffer to store incoming data.
     * @param inoutLen  [in] max buffer size, [out] actual bytes received.
     * @param timeoutMs How long to wait for RX_DONE (0 = blocking forever).
     * @return true if a packet was received and CRC passed.
     */
    bool receive(uint8_t* outBuffer, size_t& inoutLen, uint32_t timeoutMs = 0);

    /** @brief Put the radio into sleep (low-power) mode. */
    void sleep();

    /** @brief Wake the radio up from sleep. */
    void wakeup();

private:
    Config cfg_;         ///< Copy of user‐supplied configuration
};

} // namespace SX1262
