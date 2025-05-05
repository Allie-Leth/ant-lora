# ant-lora
`Work In Progress: This is not yet functional`

`ant-lora` is the LoRa communications module of the Ant Firmware suite, designed for the ESP32 series devices but intending to be expanded overtime. V1 provides a portable C++ driver for the Semtech SX1262 radio, cleanly separating core logic from platform-specific details. Built on zero-allocation buffer utilities and optional structured logging, it’s designed to be unit-testable on the desktop with a mock HAL and easily integrated on-device. The simple public API lets you initialize the radio, send and receive packets, and manage power modes without getting bogged down in low-level SPI or GPIO calls.  

Built for low overhead, this is meant to be a modular item to a larger suite. 

This library provides:

- **Platform-agnostic HAL layer** (`HalIface`) so that the same core logic can run on ESP32 (Arduino or ESP-IDF), STM32, or host-side mocks.  
- **Complete SX1262 command set** exposed as simple C++ functions (`SX1262::Cmd`), with busy-polling, timeouts and structured logging via LogAnywhere.  
- **High-level façade** (`ant::lora::LoRaRadio`) offering `init()`, `sendChunk()`, `receiveChunk()`, `sleep()`, and `wakeup()` for mesh-style packet exchange.  
- **Zero-allocation buffer utilities** (ByteBuffer, MessageBuffer, RingBuffer) from the `ant-buffer` library.  
- **Testable**: full unit-test coverage under CMake/CTest with a mock HAL; optional on-device integration tests via PlatformIO.  
