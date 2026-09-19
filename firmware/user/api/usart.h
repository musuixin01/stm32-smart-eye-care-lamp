#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

#define USART1_MAX_RECV_SIZE 512

typedef struct{
	uint8_t buff[USART1_MAX_RECV_SIZE];
	uint16_t cnt;
	uint8_t flag;
}USART_RecvDef;

extern USART_RecvDef usart1Recv;

void USART1_Config(void);
uint8_t USART1_RecvByte(void);
void USART1_SendByte(uint8_t data);
void USART1_ReavBuffAnalysis(void);


#endif
