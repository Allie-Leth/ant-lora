#include <cstdint>
#include <cstddef>

namespace ant::radio {
struct RadioCfg;                   // forward
struct IRadio {
    virtual bool init(const RadioCfg&) = 0;
    virtual bool send(const uint8_t*, size_t, uint32_t) = 0;
    virtual bool startRx(uint32_t timeoutMs) = 0;
    virtual int  readRx(uint8_t*, size_t) = 0;      // returns length or <0 on error
    virtual void sleep() = 0;
    virtual void wake() = 0;
    virtual ~IRadio() = default;
};
} // namespace ant::radio
