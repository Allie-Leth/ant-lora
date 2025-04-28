#pragma once
#include "HalInterface.h"

namespace SX1262 {  
  /**  
   * @brief Core hardware abstraction for SX1262 commands.  
   *  
   * Implements select/deselect, FIFO transfers, reset, busy-polling, and  
   * DIO1 interrupt attachment on top of a HalIface.  
   */  
  class LoRaHal {  
  public:  
    /**  
     * @brief Initialize pins and SPI bus.  
     * @param hal      Implementation of HalIface.  
     * @param csPin    Chip-select GPIO.  
     * @param resetPin RESET GPIO.  
     * @param busyPin  BUSY GPIO.  
     * @param dio1Pin  DIO1 GPIO.  
     */  
    static void init( HalIface& hal,  
                      uint8_t csPin,  
                      uint8_t resetPin,  
                      uint8_t busyPin,  
                      uint8_t dio1Pin );  

    inline static void select();  
    inline static void deselect();  
    inline static uint8_t xfer(uint8_t b);  
    static void xferBuf(const uint8_t* buf, size_t len);  
    static void readBuf(uint8_t* buf, size_t len);  

    inline static void resetAssert();  
    inline static void resetDeassert();  
    inline static bool isBusy();  
    static bool waitBusy(uint32_t timeoutMs = 100);  

    inline static void attachDio1(void(*handler)());  
    inline static void detachDio1();  

  private:  
    static HalIface* _hal;  
    static uint8_t   _cs, _reset, _busy, _dio1;  
  };  
}
