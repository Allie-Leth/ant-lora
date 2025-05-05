#pragma once
#include <cstdint>
#include "ant/hal/IHal.h"
#include "ant/radio/RadioCfg.h"
#include "ant/radio/SX1262/Defs.h" // for RadioCfg

using SX1262::OP_SET_STANDBY;
using SX1262::OP_SET_RF_FREQ;
using SX1262::OP_WRITE_BUFFER;
using SX1262::OP_SET_TX;
using SX1262::OP_GET_STATUS;

namespace SX1262 {

/** Which oscillator to use while in standby. */
enum class StandbyMode : uint8_t { RC = 0x00, XOSC = 0x01 };

namespace Cmd {

// Bring the long names into this namespace so we can use them unqualified
using ant::hal::IHal;
using ant::radio::RadioCfg;

/** Put the radio into standby (RC or XOSC). */
bool setStandby(ant::hal::IHal&               h,
                const ant::radio::RadioCfg&     cfg,
                StandbyMode         mode = StandbyMode::RC);

/** Read status byte (opcode 0xC0). */
uint8_t getStatus(IHal& h);

/** Register R/W helpers */
uint8_t readRegister (IHal& h, uint16_t addr);
void    writeRegister(IHal& h, uint16_t addr, uint8_t val);

/** FIFO helpers */
size_t  readBuffer (IHal& h, uint8_t* out, size_t maxLen);
bool    writeBuffer(IHal& h, const RadioCfg& cfg,
                    const uint8_t* data, size_t len);

/** RF configuration */
bool setRfFrequency(IHal& h, const RadioCfg& cfg, uint32_t freqHz);
void setTxPower    (IHal& h, int8_t dBm);

/** Simple TX/RX entry points */
void setTx(IHal& h, const RadioCfg& cfg, uint32_t timeoutMs);
void setRx(IHal& h, const RadioCfg& cfg, uint32_t timeoutMs);

/** IRQ helpers */
void     clearIrqs (IHal& h);
uint16_t getIrqFlags(IHal& h);

} // namespace Cmd
} // namespace SX1262
