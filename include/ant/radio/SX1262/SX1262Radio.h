#pragma once
#include "ant/hal/IHal.h"              // the abstract HAL
#include "ant/hal/esp32/Esp32Hal.h"    // pin struct reuse (optional)
#include "ant/radio/IRadio.h"          // <── ADD THIS
#include "ant/radio/RadioCfg.h"

class SX1262Radio : public ant::radio::IRadio
{
public:
    explicit SX1262Radio(ant::hal::IHal& h) : h_(h) {}

    bool init (const ant::radio::RadioCfg&) override;
    bool send (const uint8_t*, size_t, uint32_t) override;
    bool startRx(uint32_t) override;
    int  readRx (uint8_t*, size_t) override;
    void sleep() override;
    void wake () override;

private:
    ant::hal::IHal&      h_;
    ant::radio::RadioCfg cfg_;
};
