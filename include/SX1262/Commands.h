#pragma once
#include <cstdint>
#include <array>
#include "HalInterface.h"

namespace SX1262 {

/**
 * @file Commands.h
 * @brief Mid-level wrappers for each SX1262 command (opcodes + params).
 *
 * These functions know nothing of your application logic—they just
 * push bytes over SPI via LoRaHal and wait for BUSY to clear.
 */

namespace Cmd {

/** @return Status byte (reads 0xC0, 0x00) */
uint8_t getStatus();

/** Read a single register */
uint8_t readRegister(uint16_t address);

/** Write a single register */
void writeRegister(uint16_t address, uint8_t value);

/** Read a block from FIFO into buffer; returns number of bytes read */
size_t readBuffer(uint8_t* out, size_t maxLen);

/** Write a block of data into the FIFO */
void writeBuffer(const uint8_t* data, size_t len);

/** Put the radio into standby mode (RC or XOSC) */
void setStandby(bool useXosc = false);

/** Calibrate image for LF & HF, mask bits per datasheet */
void calibrateImage();

/** Set the RF frequency in Hz */
void setRfFrequency(uint32_t freqHz);

/** Set the PA output power in dBm */
void setTxPower(int8_t dBm);

/** Configure LoRa modem: SF, BW, CR, header mode, CRC */
void setLoRaModem(
  uint8_t  spreadingFactor,
  uint32_t bandwidth,
  uint8_t  codingRate,
  bool     implicitHeader,
  bool     crcOn
);

/** Begin TX with timeout (ms) */
void setTx(uint32_t timeoutMs);

/** Begin RX with timeout (ms) */
void setRx(uint32_t timeoutMs);

/** Clear all IRQ flags */
void clearIrqs();

/** Read IRQ flags */
uint16_t getIrqFlags();

} // namespace Cmd

} // namespace SX1262
