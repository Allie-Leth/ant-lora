#pragma once
/**
 *  @file IHal.h
 *  @brief Generic Hardware‑Abstraction Layer used by *all* drivers.
 *
 *  Any concrete port (Esp32Hal, MockHal, Stm32Hal, …) must implement
 *  this interface so the radio code remains 100 % platform‑agnostic.
 *
 *  © 2025 ant‑labs. MIT‑licensed.
 */
#include <cstdint>
#include <cstddef>
#include <functional>

namespace ant::hal
{

    /**
     * @interface IHal
     * @brief Pure‑virtual interface for SPI, GPIO, and timing primitives.
     *
     * Minimal but sufficient for SX126x, SX127x, and other Semtech radios.
     */
    struct IHal
    {
        /* ----------- SPI bus ----------- */
        virtual void spiBegin() = 0;

        /**
         * Full‑duplex SPI transfer.
         * @param tx  Pointer to bytes to transmit (may be nullptr for read‑only)
         * @param rx  Pointer to buffer for received bytes (may be nullptr for write‑only)
         * @param len Number of bytes
         */
        virtual void spiTransfer(const uint8_t *tx,
                                 uint8_t *rx,
                                 size_t len) = 0;

        /** Assert / de‑assert chip‑select. */
        virtual void csSelect() = 0;   ///< Drive CS LOW
        virtual void csDeselect() = 0; ///< Drive CS HIGH

        /* ----------- GPIO ------------- */
        virtual void pinWrite(uint8_t pin, bool level) = 0;
        virtual bool pinRead(uint8_t pin) = 0;

        /* ----------- Timing ----------- */
        virtual void delayMs(uint32_t ms) = 0; ///< Blocking millisecond delay
        virtual uint64_t ticks_ms() = 0;       ///< 64‑bit monotonic ms since boot

        /* ----------- Interrupt helpers (optional) ----------- */
        virtual void attachIrq(uint8_t /*pin*/,
                               std::function<void()> /*cb*/,
                               int /*mode*/)
        { /* default: do nothing */
        }

        /**
         * @brief Attach a rising-edge (or CHANGE/FALLING) ISR on a GPIO.
         * @param pin  GPIO number
         * @param cb   Plain C-function pointer with signature `void cb()`
         * @param mode one of RISING, FALLING, CHANGE
         */
        virtual void attachIrq(uint8_t pin,
                               void (*cb)(),
                               int mode) = 0;

        /**
         * @brief Detach previously attached ISR.
         */
        virtual void detachIrq(uint8_t pin) = 0;
        /* ----------- Polymorphic cleanup ----------- */
        virtual ~IHal() = default;
    };

} // namespace ant::hal
