#include "usart.h"
#include "stdio.h"
#include "string.h"


USART_RecvDef usart1Recv = {0};

void USART1_IRQHandler(void)
{
	uint8_t data = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		data = USART_ReceiveData(USART1);
//		USART_SendData(USART1, data);//回显验证	
		usart1Recv.buff[usart1Recv.cnt++] = data;//缓存数据
		usart1Recv.cnt %= USART1_MAX_RECV_SIZE;//循环队列	
	}
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET) {
		data = USART_ReceiveData(USART1);
		usart1Recv.flag = 1;
	}
	
}

void USART1_ReavBuffAnalysis(void)
{
	if(usart1Recv.flag == 0) return;
	//解析数据
	if(usart1Recv.cnt == 6) {
		uint8_t check = 0;
		check = usart1Recv.buff[2]+usart1Recv.buff[3]+usart1Recv.buff[4];
		if(check == usart1Recv.buff[5]) {
			printf("id=%d\r\n", usart1Recv.buff[2]);
			if(usart1Recv.buff[2] == 1) {//地址匹配:第一组 
				
				if(usart1Recv.buff[3] == 1) {
					printf("收到控制灯的指令:%d\r\n", usart1Recv.buff[4]);
					if(usart1Recv.buff[4] == 1) {
						printf("开灯\r\n");
					}
					else {
						printf("关灯\r\n");
					}	
				}
				
				if(usart1Recv.buff[3] == 2) {
					printf("收到控制蜂鸣器的指令:%d\r\n", usart1Recv.buff[4]);
					
				}
					
			}else printf("地址不匹配\r\n");
			
			
		}else printf("校验未通过\r\n");
	}
	else printf("数据包长度不对\r\n");
	//清空缓存区及标志位
	memset(&usart1Recv, 0, sizeof(usart1Recv));
}




void USART1_Config(void)
{
	//串口1  TX PA9			RX PA10
	//1、	配置TX和RX引脚的模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//2、	配置串口工作模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);	

	USART_Cmd(USART1, ENABLE);
}





void USART1_SendByte(uint8_t data)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	USART_SendData(USART1, data);
}

void USART1_SendBuff(uint8_t * buff, uint16_t len)
{
	for(uint16_t i=0; i<len; i++) {
		USART1_SendByte(buff[i]);
	}
}

void USART1_SendStr(char * str)
{
	while(*str != '\0') {
		USART1_SendByte(*str);
		str++;
	}
}

uint8_t USART1_RecvByte(void)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{}
	return USART_ReceiveData(USART1);
}

int fputc(int ch, FILE * Stream)
{
	USART1_SendByte((uint8_t)ch);
	return ch;
}



