#pragma once
#include <cstdint>
#include "ant/hal/esp32/Esp32Hal.h" // for Esp32Pins (reuse)

namespace ant::radio
{

    struct RfParams
    {
        uint32_t freqHz;
        int8_t powerDbm;

        /** @brief RF settings ctor */
        constexpr RfParams(uint32_t f = 915000000,
                           int8_t p = 14) noexcept
            : freqHz(f), powerDbm(p) {}
    };

    struct RadioCfg
    {
        ant::hal::Esp32Pins pins; // opaque to everything above the HAL
        RfParams rf;
    };

} // namespace ant::radio
