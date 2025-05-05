#include "ant/radio/SX1262/Commands.h"
#include "ant/radio/SX1262/Defs.h"
#include <ant/hal/IHal.h>
#include <ant/radio/IRadio.h>
#include <ant/radio/RadioCfg.h>
#include <ant/radio/SX1262/Radio.h>

using ant::hal::IHal;
using ant::radio::RadioCfg;

/* ────────── helpers local to this TU ────────── */
namespace
{

    bool waitBusy(IHal &h, const RadioCfg &cfg, uint32_t ms = 5)
    {
        uint64_t t0 = h.ticks_ms();
        while (h.pinRead(cfg.pins.busy) == HIGH)
            if (h.ticks_ms() - t0 >= ms)
                return false;
        return true;
    }

    bool spiCmd(IHal &h, uint8_t op, const uint8_t *tx = nullptr,
                size_t nTx = 0,
                uint8_t *rx = nullptr,
                size_t nRx = 0)
    {
        h.csSelect();
        h.spiTransfer(&op, nullptr, 1);
        if (nTx)
            h.spiTransfer(tx, nullptr, nTx);
        if (nRx)
            h.spiTransfer(nullptr, rx, nRx);
        h.csDeselect();
        return true;
    }

} // namespace

/* ────────── Implementation of public wrappers ────────── */
namespace SX1262::Cmd {

    bool setStandby(ant::hal::IHal&        h,   // SPI/GPIO abstraction
        const ant::radio::RadioCfg& cfg, // holds BUSY pin, etc.
        StandbyMode mode);

    bool setRfFrequency(IHal &h, const RadioCfg &cfg, uint32_t hz)
    {
        /* Semtech frf = freq(Hz) * 2^25 / f_xosc (32 MHz) */
        uint32_t frf = (static_cast<uint64_t>(hz) << 25) / 32000000UL;
        uint8_t buf[4] = {uint8_t(frf >> 24), uint8_t(frf >> 16),
                          uint8_t(frf >> 8), uint8_t(frf)};
        spiCmd(h, SX1262::OP_SET_RF_FREQ, buf, 4);
        return waitBusy(h, cfg);
    }

    bool writeBuffer(IHal &h, const RadioCfg &, const uint8_t *data, size_t len)
    {
        uint8_t op = SX1262::OP_WRITE_BUFFER;
        uint8_t addr = 0x00;
        h.csSelect();
        h.spiTransfer(&op, nullptr, 1);
        h.spiTransfer(&addr, nullptr, 1);
        h.spiTransfer(data, nullptr, len);
        h.csDeselect();
        return true; // waitBusy done by caller if required
    }

    bool setTx(IHal &h, const RadioCfg &cfg)
    {
        uint8_t buf[3] = {0x00, 0x00, 0x00}; // Timeout = 0 (single pkt)
        spiCmd(h, SX1262::OP_SET_TX, buf, 3);
        return waitBusy(h, cfg, /*ms*/ 5000);
    }

    bool getStatus(IHal &h, uint8_t &out)
    {
        uint8_t op = SX1262::OP_GET_STATUS;
        h.csSelect();
        h.spiTransfer(&op, nullptr, 1);
        h.spiTransfer(nullptr, &out, 1);
        h.csDeselect();
        return true;
    }

} // namespace sx1262
