/* SpiDma.cpp SPI via DMA for the Arduino DUE
Copyright DrOldies 2014
*/

#include "SpiDmaDUE.h"

spiClass spi;

spiClass::spiClass(void){
}

//--------------------------public functions----------------------------------------------------
void spiClass::begin() {
  begin(BOARD_SPI_DEFAULT_SS);
}

void spiClass::begin(uint8_t csPin) {
  static bool _first_cs_ = false;     //first time here? init DMA

  //The extended API can use pins 4, 10, and 52 for CS.
  uint8_t activePinIndex = BOARD_PIN_TO_SPI_CHANNEL(csPin);     //variant.h 10,4,52,78??
  // if pin not enabled - force enable
  if(!_base [activePinIndex] ){   //_base initalized to zero
    _base [activePinIndex] =  (&(digitalPinToPort(csPin)->PIO_PER));
    _mask [activePinIndex] = (digitalPinToBitMask(csPin));
    (*((_base [activePinIndex])+4)) = (_mask[activePinIndex]); //OUTPUT
    (*((_base [activePinIndex])+12)) = (_mask[activePinIndex]);  //HIGH
  }

  if (_first_cs_){return;}    //been here already?
   _first_cs_ = true;        //been here already

  //init only 1st time here
  PIO_Configure(
      g_APinDescription[PIN_SPI_MOSI].pPort,
      g_APinDescription[PIN_SPI_MOSI].ulPinType,
      g_APinDescription[PIN_SPI_MOSI].ulPin,
      g_APinDescription[PIN_SPI_MOSI].ulPinConfiguration);
  PIO_Configure(
      g_APinDescription[PIN_SPI_MISO].pPort,
      g_APinDescription[PIN_SPI_MISO].ulPinType,
      g_APinDescription[PIN_SPI_MISO].ulPin,
      g_APinDescription[PIN_SPI_MISO].ulPinConfiguration);
  PIO_Configure(
      g_APinDescription[PIN_SPI_SCK].pPort,
      g_APinDescription[PIN_SPI_SCK].ulPinType,
      g_APinDescription[PIN_SPI_SCK].ulPin,
      g_APinDescription[PIN_SPI_SCK].ulPinConfiguration);
  pmc_enable_periph_clk(ID_SPI0);
  #if USE_SAM3X_DMAC
    pmc_enable_periph_clk(ID_DMAC);
    dmac_disable();
    DMAC->DMAC_GCFG = DMAC_GCFG_ARB_CFG_FIXED;
    dmac_enable();
  #if USE_SAM3X_BUS_MATRIX_FIX
    MATRIX->MATRIX_WPMR = 0x4d415400;
    MATRIX->MATRIX_MCFG[1] = 1;
    MATRIX->MATRIX_MCFG[2] = 1;
    MATRIX->MATRIX_SCFG[0] = 0x01000010;
    MATRIX->MATRIX_SCFG[1] = 0x01000010;
    MATRIX->MATRIX_SCFG[7] = 0x01000010;
  #endif  // USE_SAM3X_BUS_MATRIX_FIX
  #endif  // USE_SAM3X_DMAC

  setClockDivider(SPI_CLOCK_DIV4);      // set default rate
  bitOrder[activePinIndex] = MSBFIRST;  //default bit order
}

//  initialize SPI controller - spiRate = acbr = 1-255
void spiClass::setClockDivider(uint8_t csPin, uint8_t scbr){      //pin is dummy for now <<<<<<
  if (scbr > 0 & scbr < 256){
    Spi* pSpi = SPI0;
    //  disable SPI
    pSpi->SPI_CR = SPI_CR_SPIDIS;
    // reset SPI
    pSpi->SPI_CR = SPI_CR_SWRST;
    // no mode fault detection, set master mode
    pSpi->SPI_MR = SPI_PCS(SPI_CHIP_SEL) | SPI_MR_MODFDIS | SPI_MR_MSTR;
    // mode 0, 8-bit,
    pSpi->SPI_CSR[SPI_CHIP_SEL] = SPI_CSR_SCBR(scbr) | SPI_CSR_NCPHA;
    // enable SPI
    pSpi->SPI_CR |= SPI_CR_SPIEN;
  }
}

void spiClass::setBitOrder(BitOrder _order) { 
  bitOrder[BOARD_PIN_TO_SPI_CHANNEL(BOARD_SPI_DEFAULT_SS)] = _order; 
}

void spiClass::setClockDivider(uint8_t _div) { 
  setClockDivider(BOARD_SPI_DEFAULT_SS, _div); 
}

void spiClass::setBitOrder(uint8_t csPin, BitOrder _bitOrder) {
  bitOrder[BOARD_PIN_TO_SPI_CHANNEL(csPin)] = _bitOrder;
}


/** SPI receive multiple bytes */
uint8_t spiClass::receive(uint8_t csPin, uint8_t* buf, size_t len) {    //pin is dummy for now <<<<<<
  Spi* pSpi = SPI0;
  int rtn = 0;

  uint8_t activePinIndex = BOARD_PIN_TO_SPI_CHANNEL(csPin); 
  (*((_base [activePinIndex])+13)) = (_mask[activePinIndex]);   //LOW

#if USE_SAM3X_DMAC
  // clear overrun error
  uint32_t s = pSpi->SPI_SR;

  DmaRX(buf, len);
  DmaTX(0, len);

  uint32_t m = millis();
  while (!dmac_channel_transfer_done(SPI_DMAC_RX_CH)) {
    if ((millis() - m) > SAM3X_DMA_TIMEOUT)  {
      dmac_channel_disable(SPI_DMAC_RX_CH);
      dmac_channel_disable(SPI_DMAC_TX_CH);
      rtn = 2;
      break;
    }
  }
  if (pSpi->SPI_SR & SPI_SR_OVRES) rtn |= 1;
#else  // USE_SAM3X_DMAC
  for (size_t i = 0; i < len; i++) {
    pSpi->SPI_TDR = 0XFF;
    while ((pSpi->SPI_SR & SPI_SR_RDRF) == 0) {} 
    buf[i] = pSpi->SPI_RDR;
  }
#endif  // USE_SAM3X_DMAC

  (*((_base [activePinIndex])+12)) = (_mask[activePinIndex]);   //HIGH

  if (bitOrder[activePinIndex] == LSBFIRST) {    // Reverse bit order
    //uint8_t recBuf[len];
    for (uint16_t i = len; i > 0; i--) {
      buf[i-1] = __REV(__RBIT(buf[i-1]));       //LSBFIRST transfers will be slower
    }
  }

  return rtn;
}

/** SPI send a byte */
inline uint8_t spiClass::transfer(uint8_t csPin, uint8_t byteData) {
  return send(csPin, byteData);
}

/* SPI send a string */
void spiClass::transfer(uint8_t csPin, uint8_t* buf, size_t len) {  //buf is const in DmaTX()
//void spiClass::transfer(uint8_t csPin, const uint8_t* buf, size_t len) {
  Spi* pSpi = SPI0;

  uint8_t activePinIndex = BOARD_PIN_TO_SPI_CHANNEL(csPin); 

  if (bitOrder[activePinIndex] == LSBFIRST) {    // Reverse bit order
    //uint8_t recBuf[len];
    for (uint16_t i = len; i > 0; i--) {
      buf[i-1] = __REV(__RBIT(buf[i-1]));       //LSBFIRST transfers will be slower
    }
  }

  (*((_base [activePinIndex])+13)) = (_mask[activePinIndex]);   //LOW
  
#if USE_SAM3X_DMAC
  DmaTX(buf, len);
  while (!dmac_channel_transfer_done(SPI_DMAC_TX_CH)) {}
#else  // #if USE_SAM3X_DMAC
  while ((pSpi->SPI_SR & SPI_SR_TXEMPTY) == 0) {}
  for (size_t i = 0; i < len; i++) {
    pSpi->SPI_TDR = buf[i];
    while ((pSpi->SPI_SR & SPI_SR_TDRE) == 0) {}
  }
#endif  // #if USE_SAM3X_DMAC
  while ((pSpi->SPI_SR & SPI_SR_TXEMPTY) == 0) {}
  (*((_base [activePinIndex])+12)) = (_mask[activePinIndex]);   //HIGH
  // leave RDR empty
  uint8_t b = pSpi->SPI_RDR;
}

//----------------------private functions ---------------

inline uint8_t spiClass::send(uint8_t csPin, uint8_t byteData) {
  Spi* pSpi = SPI0;

  pSpi->SPI_TDR = byteData;
  uint8_t activePinIndex = BOARD_PIN_TO_SPI_CHANNEL(csPin); 

  if (bitOrder[activePinIndex] == LSBFIRST)               // Reverse bit order
    byteData = __REV(__RBIT(byteData));

  (*((_base [activePinIndex])+13)) = (_mask[activePinIndex]);   //LOW
  while ((pSpi->SPI_SR & SPI_SR_RDRF) == 0) {}
  (*((_base [activePinIndex])+12)) = (_mask[activePinIndex]);   //HIGH
  byteData = pSpi->SPI_RDR;
  if (bitOrder[activePinIndex] == LSBFIRST)               // Reverse bit order
    byteData = __REV(__RBIT(byteData));
  return byteData;
}

// start RX DMA
void spiClass::DmaRX(uint8_t* dst, uint16_t count) {
  dmac_channel_disable(SPI_DMAC_RX_CH);
  DMAC->DMAC_CH_NUM[SPI_DMAC_RX_CH].DMAC_SADDR = (uint32_t)&SPI0->SPI_RDR;
  DMAC->DMAC_CH_NUM[SPI_DMAC_RX_CH].DMAC_DADDR = (uint32_t)dst;
  DMAC->DMAC_CH_NUM[SPI_DMAC_RX_CH].DMAC_DSCR =  0;
  DMAC->DMAC_CH_NUM[SPI_DMAC_RX_CH].DMAC_CTRLA = count |
    DMAC_CTRLA_SRC_WIDTH_BYTE | DMAC_CTRLA_DST_WIDTH_BYTE;
  DMAC->DMAC_CH_NUM[SPI_DMAC_RX_CH].DMAC_CTRLB = DMAC_CTRLB_SRC_DSCR |
    DMAC_CTRLB_DST_DSCR | DMAC_CTRLB_FC_PER2MEM_DMA_FC |
    DMAC_CTRLB_SRC_INCR_FIXED | DMAC_CTRLB_DST_INCR_INCREMENTING;
  DMAC->DMAC_CH_NUM[SPI_DMAC_RX_CH].DMAC_CFG = DMAC_CFG_SRC_PER(SPI_RX_IDX) |
    DMAC_CFG_SRC_H2SEL | DMAC_CFG_SOD | DMAC_CFG_FIFOCFG_ASAP_CFG;
  dmac_channel_enable(SPI_DMAC_RX_CH);
}

// start TX DMA
void spiClass::DmaTX(const uint8_t* src, uint16_t count) {
  uint8_t ff = 0XFF;
  uint32_t src_incr = DMAC_CTRLB_SRC_INCR_INCREMENTING;
  if (!src) {
    src = &ff;
    src_incr = DMAC_CTRLB_SRC_INCR_FIXED;
  }
  dmac_channel_disable(SPI_DMAC_TX_CH);
  DMAC->DMAC_CH_NUM[SPI_DMAC_TX_CH].DMAC_SADDR = (uint32_t)src;
  DMAC->DMAC_CH_NUM[SPI_DMAC_TX_CH].DMAC_DADDR = (uint32_t)&SPI0->SPI_TDR;
  DMAC->DMAC_CH_NUM[SPI_DMAC_TX_CH].DMAC_DSCR =  0;
  DMAC->DMAC_CH_NUM[SPI_DMAC_TX_CH].DMAC_CTRLA = count |
    DMAC_CTRLA_SRC_WIDTH_BYTE | DMAC_CTRLA_DST_WIDTH_BYTE;

  DMAC->DMAC_CH_NUM[SPI_DMAC_TX_CH].DMAC_CTRLB =  DMAC_CTRLB_SRC_DSCR |
    DMAC_CTRLB_DST_DSCR | DMAC_CTRLB_FC_MEM2PER_DMA_FC |
    src_incr | DMAC_CTRLB_DST_INCR_FIXED;

  DMAC->DMAC_CH_NUM[SPI_DMAC_TX_CH].DMAC_CFG = DMAC_CFG_DST_PER(SPI_TX_IDX) |
      DMAC_CFG_DST_H2SEL | DMAC_CFG_SOD | DMAC_CFG_FIFOCFG_ALAP_CFG;

  dmac_channel_enable(SPI_DMAC_TX_CH);
}

