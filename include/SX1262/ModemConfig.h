// include/SX1262/ModemConfig.h
#pragma once

#include <cstdint>

namespace SX1262 {

/**
 * @file ModemConfig.h
 * @brief LoRa modem parameters: spreading factor, bandwidth, coding rate, header/CRC modes.
 */
struct ModemConfig {
  /// Spreading factor (SF5 … SF12)
  enum class SpreadingFactor : uint8_t {
    SF5  = 5,
    SF6  = 6,
    SF7  = 7,
    SF8  = 8,
    SF9  = 9,
    SF10 = 10,
    SF11 = 11,
    SF12 = 12
  } sf = SpreadingFactor::SF7;

  /// Bandwidth in Hz (available LoRa bandwidths)
  enum class Bandwidth : uint32_t {
    BW_7_8_KHZ   = 7800,
    BW_10_4_KHZ  = 10400,
    BW_15_6_KHZ  = 15600,
    BW_20_8_KHZ  = 20800,
    BW_31_25_KHZ = 31250,
    BW_41_7_KHZ  = 41700,
    BW_62_5_KHZ  = 62500,
    BW_125_KHZ   = 125000,
    BW_250_KHZ   = 250000,
    BW_500_KHZ   = 500000
  } bw = Bandwidth::BW_125_KHZ;

  /// Coding rate (4/5 … 4/8)
  enum class CodingRate : uint8_t {
    CR_4_5 = 1,
    CR_4_6 = 2,
    CR_4_7 = 3,
    CR_4_8 = 4
  } cr = CodingRate::CR_4_5;

  bool implicitHeader = false;  ///< true=implicit header mode, false=explicit
  bool crcOn          = true;   ///< true=CRC enabled on RX, false=CRC off
};

} // namespace SX1262
