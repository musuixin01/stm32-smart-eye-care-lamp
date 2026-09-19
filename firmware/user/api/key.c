#include "key.h"
#include "delay.h"


//按键初始化函数
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//KEY1 PA0	浮空输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


//按键检测函数
uint8_t Key_GetValue(void)
{
		
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
	{
		Delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
			{}
			return 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == Bit_RESET)
	{
		Delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == Bit_RESET)
		{
			while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == Bit_RESET)
			{}
			return 2;
		}
	}
	return 0;
}







