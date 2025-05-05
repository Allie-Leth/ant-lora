#include "ant/radio/SX1262/SX1262Radio.h"
#include <cstring>   
#include "ant/radio/SX1262/defs.h"

/* ──────────────────────────────
   Local constants & helpers
   ──────────────────────────── */
namespace {

constexpr uint8_t OP_SET_STANDBY   = 0x80;
constexpr uint8_t OP_SET_RF_FREQ   = 0x86;
constexpr uint8_t OP_WRITE_BUFFER  = 0x0E;
constexpr uint8_t OP_SET_TX        = 0x83;
constexpr uint8_t OP_GET_STATUS    = 0xC0;

/* Busy‑poll helper: BUSY pin must de‑assert within timeoutMs */
bool waitBusy(ant::hal::IHal& h,
              const ant::radio::RadioCfg& cfg,
              uint32_t timeoutMs = 5)
{
    const uint64_t t0 = h.ticks_ms();
    while (h.pinRead(cfg.pins.busy) == HIGH) {
        if (h.ticks_ms() - t0 >= timeoutMs) return false;
    }
    return true;
}

/* One‑shot SPI command helper */
bool spiCmd(ant::hal::IHal& h,
            uint8_t op,
            const uint8_t* tx = nullptr, size_t nTx = 0,
            uint8_t*       rx = nullptr, size_t nRx = 0)
{
    h.csSelect();
    h.spiTransfer(&op, nullptr, 1);
    if (nTx) h.spiTransfer(tx, nullptr, nTx);
    if (nRx) h.spiTransfer(nullptr, rx, nRx);
    h.csDeselect();
    return true;
}

} // namespace

/* ──────────────────────────────
   SX1262Radio implementation
   ──────────────────────────── */
bool SX1262Radio::init(const ant::radio::RadioCfg& cfg)
{
    cfg_ = cfg;                           // keep a local copy

    /* 1. Hardware RESET pulse (≈10 ms) */
    h_.pinWrite(cfg_.pins.reset, LOW);
    h_.delayMs(10);
    h_.pinWrite(cfg_.pins.reset, HIGH);
    if (!waitBusy(h_, cfg_)) return false;

    /* 2. Stand‑by with RC oscillator */
    uint8_t arg = 0x00;                   // STDBY_RC
    spiCmd(h_, OP_SET_STANDBY, &arg, 1);
    if (!waitBusy(h_, cfg_)) return false;

    /* 3. Set RF frequency */
    uint32_t frf = (static_cast<uint64_t>(cfg_.rf.freqHz) << 25) / 32000000UL;
    uint8_t  buf[4] = { uint8_t(frf >> 24), uint8_t(frf >> 16),
                        uint8_t(frf >> 8),  uint8_t(frf) };
    spiCmd(h_, OP_SET_RF_FREQ, buf, 4);
    if (!waitBusy(h_, cfg_)) return false;

    /* 4. Quick sanity check: radio should report STDBY_RC */
    uint8_t op = OP_GET_STATUS, stat = 0xFF;
    h_.csSelect();
    h_.spiTransfer(&op, nullptr, 1);
    h_.spiTransfer(nullptr, &stat, 1);
    h_.csDeselect();

    return (stat & 0x1F) == 0x00;         // 0x00 == STDBY_RC
}

/* --------------------------------------------------------- */
bool SX1262Radio::send(const uint8_t* data,
                       size_t         len,
                       uint32_t       timeoutMs)
{
    if (len == 0 || len > 255) return false;     // radio FIFO limit

    /* 1. Copy payload into radio FIFO (addr 0) */
    h_.csSelect();
    uint8_t op = OP_WRITE_BUFFER;
    uint8_t addr = 0x00;
    h_.spiTransfer(&op,   nullptr, 1);
    h_.spiTransfer(&addr, nullptr, 1);
    h_.spiTransfer(data,  nullptr, len);
    h_.csDeselect();
    if (!waitBusy(h_, cfg_)) return false;

    /* 2. Fire single TX (timeout = 0 disables watchdog) */
    uint8_t params[3] = { 0x00, 0x00, 0x00 };
    spiCmd(h_, OP_SET_TX, params, 3);

    /* 3. Wait for BUSY pin to de‑assert → TX done */
    return waitBusy(h_, cfg_, timeoutMs);
}

/* The remaining methods are stubs for now */
bool SX1262Radio::startRx(uint32_t)           { return false; }
int  SX1262Radio::readRx(uint8_t*, size_t)    { return -1; }
void SX1262Radio::sleep()                     {}
void SX1262Radio::wake()                      {}
