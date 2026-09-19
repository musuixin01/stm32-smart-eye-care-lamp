#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"

void SPI2_Config(void);
uint8_t SPI2_SendRecvByte(uint8_t data);


#endif
