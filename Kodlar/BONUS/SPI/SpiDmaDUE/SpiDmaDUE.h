/* SpiDma.h SPI via DMA for the Arduino DUE
Copyright (C) 2014 by DrOldies (Paul Dymerski)
This library is extracted from a sketch dmaspi.ino by 
  manitou48 DUEZoo https://github.com/manitou48/DUEZoo
It in turn was from the Arduino SdFat Library
  Copyright (C) 2012 by William Greiman

 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.

SPI.h and SpiDmaDUE.h differences:
        SPI.h                                           SpiDmaDUE.h
-Can set setClockDivider for EACH CS         Only one speed for all CS transfers
-Only single byte send via 'transfer'        Has single byte and array transfers
-Has CONTINUE and LAST for array transfers   Does not need CONTINUE and LAST options
-Only 1 byte receive via 'transfer'          Has 1 byte receive via 'transfer'
                                             Also have array receive via 'receive'
-Has 4 setDataMode modes                     Mode 0 only
-Has MSBFIRST or LSBFIRST setBitOrder*       LSBFIRST is 1/2 speed of MSBFIRST for 1024 byte buffer
  * Byte order determined by user loop       **Byte order of array determined by user 

Note: only SPI0 is used currently

The extended API can use pins 10, 4, and 52 for CS.
(The default CS pin is actually 78 based on SPI.h)

Data transfer speed. any integer from 1 to 255
The SPI_CLOCK_DIVx macros are defined below
Here are the data rates for the divisors
DIV Megabit
    /sec    Mhz     khz
           /byte    /byte
--------------------------
1   54.79   6.8 
2   41.24   5.2 
4   20.78   2.6 
8   10.46   1.3 
16  5.24            655
32  2.62            328
64  1.31            164
128 0.66             83
255 0.33             41
*/

#ifndef SPI_DMA_H
#define SPI_DMA_H

#include <Arduino.h>

/** Use SAM3X DMAC if nonzero */
#define USE_SAM3X_DMAC  1
/** Use extra Bus Matrix arbitration fix if nonzero */
#define USE_SAM3X_BUS_MATRIX_FIX 0
/** Time in ms for DMA receive timeout */
#define SAM3X_DMA_TIMEOUT 100
/** chip select register number */
#define SPI_CHIP_SEL 3
/** DMAC receive channel */
#define SPI_DMAC_RX_CH  1
/** DMAC transmit channel */
#define SPI_DMAC_TX_CH  0
/** DMAC Channel HW Interface Number for SPI TX. */
#define SPI_TX_IDX  1
/** DMAC Channel HW Interface Number for SPI RX. */
#define SPI_RX_IDX  2

/** Enable DMA Controller. */
#define dmac_enable()                   DMAC->DMAC_EN = DMAC_EN_ENABLE
/** Disable DMA Controller. */
#define dmac_disable()                  DMAC->DMAC_EN &= (~DMAC_EN_ENABLE)
/** Disable DMA Channel. */
#define dmac_channel_disable(ul_num)    DMAC->DMAC_CHDR = DMAC_CHDR_DIS0 << ul_num
/** Enable DMA Channel. */
#define dmac_channel_enable(ul_num)     DMAC->DMAC_CHER = DMAC_CHER_ENA0 << ul_num
/** Poll for transfer complete. */
#define dmac_channel_transfer_done(ul_num) (DMAC->DMAC_CHSR & (DMAC_CHSR_ENA0 << ul_num)) ? false : true

//speed divsors range from 1 - 255. 
#undef SPI_CLOCK_DIV2
#undef SPI_CLOCK_DIV4
#undef SPI_CLOCK_DIV8
#undef SPI_CLOCK_DIV16
#undef SPI_CLOCK_DIV32
#undef SPI_CLOCK_DIV64
#undef SPI_CLOCK_DIV128
#undef SPI_CLOCK_DIV256

#define SPI_CLOCK_DIV1      1   //0 prohibited
#define SPI_CLOCK_DIV2      2
#define SPI_CLOCK_DIV4      4
#define SPI_CLOCK_DIV8      8
#define SPI_CLOCK_DIV16    16
#define SPI_CLOCK_DIV32    32
#define SPI_CLOCK_DIV64    64
#define SPI_CLOCK_DIV128  128
#define SPI_CLOCK_DIV256  255   //256 prohibited

#define PIN_TO_BASEREG(pin)             (&(digitalPinToPort(pin)->PIO_PER))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_BASE_TYPE volatile uint32_t *
#define IO_MASK_TYPE uint32_t
#define DIRECT_READ(base, mask)         (((*((base)+15)) & (mask)) ? 1 : 0) //return HIGH or LOW
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+5)) = (mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+4)) = (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+13)) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+12)) = (mask))
#define DIRECT_TOGGLE(base, mask)       ((*((base)+12)) = (mask));((*((base)+13)) = (mask))


class spiClass {
public:
    spiClass(void);
    void begin(void);
    void begin(uint8_t csPin);
    
    // These methods sets a parameter on a single pin
    void setBitOrder(uint8_t _pin, BitOrder);
    void setDataMode(uint8_t _pin, uint8_t);
    void setClockDivider(uint8_t _pin, uint8_t);

    // These methods sets the same parameters but on default pin BOARD_SPI_DEFAULT_SS
    void setBitOrder(BitOrder _order);
    void setClockDivider(uint8_t _div);

    inline uint8_t  transfer(uint8_t csPin, uint8_t b);
    void transfer(uint8_t csPin, uint8_t* buf, size_t len);
    uint8_t receive(uint8_t csPin, uint8_t* buf, size_t len);

private:
    inline uint8_t send(uint8_t csPin, uint8_t b) ;
    void DmaTX(const uint8_t* src, uint16_t count) ;
    void DmaRX(uint8_t* dst, uint16_t count); 

private:
    //for fast bit banging the  cs pins
    IO_BASE_TYPE  _base [SPI_CHANNELS_NUM];
    IO_MASK_TYPE _mask [SPI_CHANNELS_NUM];
    BitOrder bitOrder [SPI_CHANNELS_NUM];
};
extern spiClass spi;

#endif //SPI_DMA_H