#include <stdint.h>

#pragma once
namespace SX1262 {
inline constexpr uint8_t OP_SET_SLEEP      = 0x84;
inline constexpr uint8_t OP_SET_STANDBY    = 0x80;
inline constexpr uint8_t OP_SET_FS         = 0xC1;
inline constexpr uint8_t OP_SET_TX         = 0x83;
inline constexpr uint8_t OP_SET_RX         = 0x82;
inline constexpr uint8_t OP_SET_RF_FREQ    = 0x86;
inline constexpr uint8_t OP_WRITE_BUFFER   = 0x0E;
inline constexpr uint8_t OP_READ_BUFFER    = 0x1E;
inline constexpr uint8_t OP_GET_STATUS     = 0xC0;
} // namespace SX1262
