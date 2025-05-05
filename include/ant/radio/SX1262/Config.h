#pragma once

#include <cstdint>
#include <functional>
#include "Callbacks.h"
#include "ModemConfig.h"

namespace SX1262 {

/**
 * @file Config.h
 * @brief User-supplied configuration for SX1262 radio initialization.
 *
 * Holds all hardware pins, SPI parameters, radio defaults, and callbacks.
 * Hardware-specific HALs should populate reasonable defaults.
 */

/**
 * @struct Config
 * @brief Configuration passed to Radio::init().
 */
struct Config {
    // --- Hardware pins (required) ---
    uint8_t csPin    = 0xFF; ///< Chip-select (NSS) GPIO (mandatory)
    uint8_t resetPin = 0xFF; ///< RESET GPIO (mandatory)
    uint8_t busyPin  = 0xFF; ///< BUSY GPIO (mandatory)
    uint8_t dio1Pin  = 0xFF; ///< DIO1 interrupt GPIO (mandatory)

    // --- SPI settings (optional) ---
    uint32_t spiFrequency = 8000000; ///< Default 8 MHz safe for most chips

    // --- RF settings (optional) ---
    uint32_t rfFrequencyHz = 0; ///< RF frequency (must be supplied, e.g., 915000000)
    int8_t   txPowerDbm    = 14; ///< TX output power (dBm), safe legal default
    ModemConfig modem      = {}; ///< Default SF7, BW125kHz, CR4/5

    // --- Callbacks (optional) ---
    TxDoneCb txDoneCb = nullptr;
    RxDoneCb rxDoneCb = nullptr;

    /**
     * @brief Quick check if config is valid (all mandatory pins and RF freq set).
     */
    bool isValid() const {
        return csPin    != 0xFF &&
               resetPin != 0xFF &&
               busyPin  != 0xFF &&
               dio1Pin  != 0xFF &&
               rfFrequencyHz != 0;
    }
};

} // namespace SX1262
