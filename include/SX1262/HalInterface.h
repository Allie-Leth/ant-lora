#pragma once

#include <cstdint>
#include <functional>

/**
 * @file HalIface.h
 * @brief Abstract hardware interface for SX1262 driver.
 *
 * All timing uses a 64-bit millisecond counter to avoid rollover issues.
 */
namespace SX1262 {

/**
 * @brief Abstract HAL interface for platform-agnostic access to hardware primitives.
 *
 * Your core driver code (LoRaHal, Commands, Radio) talks only to this interface.
 * Concrete ports (Esp32Hal, MockHal, etc.) implement these methods.
 */
struct HalIface {
    /** @brief Initialize the SPI peripheral. */
    virtual void spiBegin() = 0;

    /** @brief Begin an SPI transaction (assert CS, set clock/polarity). */
    virtual void spiBeginTransaction() = 0;

    /** @brief End the current SPI transaction (deassert CS). */
    virtual void spiEndTransaction() = 0;

    /**
     * @brief Transfer a byte over SPI.
     * @param data The byte to send.
     * @return The byte received from the SPI slave.
     */
    virtual uint8_t spiTransfer(uint8_t data) = 0;

    /**
     * @brief Configure a GPIO pin's direction.
     * @param pin      The GPIO number.
     * @param isOutput True for output, false for input.
     */
    virtual void pinMode(uint8_t pin, bool isOutput) = 0;

    /**
     * @brief Write a logical level to a GPIO pin.
     * @param pin   The GPIO number.
     * @param level True for HIGH, false for LOW.
     */
    virtual void digitalWrite(uint8_t pin, bool level) = 0;

    /**
     * @brief Read a logical level from a GPIO pin.
     * @param pin The GPIO number.
     * @return True if HIGH, false if LOW.
     */
    virtual bool digitalRead(uint8_t pin) = 0;

    /**
     * @brief Delay execution for a number of milliseconds.
     * @param ms Delay duration in milliseconds.
     */
    virtual void delayMs(uint32_t ms) = 0;

    /**
     * @brief Get a monotonic, millisecond timestamp (64-bit, no rollover for ~584 million years).
     * @return Milliseconds since system start.
     */
    virtual uint64_t getTimeMs() = 0;

    /**
     * @brief Attach an interrupt handler to a GPIO pin.
     * @param pin  GPIO number.
     * @param cb   Callback invoked on interrupt.
     * @param mode Interrupt mode (e.g., RISING, FALLING).
     */
    virtual void attachInterrupt(uint8_t pin,
                                 std::function<void()> cb,
                                 int mode) = 0;

    /**
     * @brief Detach the interrupt handler from a GPIO pin.
     * @param pin GPIO number.
     */
    virtual void detachInterrupt(uint8_t pin) = 0;

    virtual ~HalIface() = default;
};

} // namespace SX1262
