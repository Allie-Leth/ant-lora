// include/SX1262/Callbacks.h
#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>

namespace SX1262 {

/**
 * @file Callbacks.h
 * @brief User‐supplied callbacks for TX and RX completion.
 */

/**
 * @brief Called when a transmission completes.
 */
using TxDoneCb = std::function<void()>;

/**
 * @brief Called when a reception completes.
 * @param data Pointer to the received payload.
 * @param len  Number of bytes in the payload.
 */
using RxDoneCb = std::function<void(const uint8_t* data, size_t len)>;

} // namespace SX1262
