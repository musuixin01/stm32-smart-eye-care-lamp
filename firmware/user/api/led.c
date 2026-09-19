#include "led.h"

//LED初始化函数
void Led_Init(void)
{
	//LED1 2 3 PC0 1 2	通用推挽输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	Led_Off(LED1_Port, LED1_Pin);
	Led_Off(LED2_Port, LED2_Pin);
	Led_Off(LED3_Port, LED3_Pin);
	
}


//LED灯亮的函数
void Led_On(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}



//LED灯灭的函数
void Led_Off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_SetBits(GPIOx, GPIO_Pin);
}


void Led_Task(void)
{
	static uint8_t i=0;
	switch(i)
	{
		case 0:Led_On(LED1_Port, LED1_Pin);Led_Off(LED3_Port, LED3_Pin);break;
		case 1:Led_Off(LED1_Port, LED1_Pin);Led_On(LED2_Port, LED2_Pin);break;
		case 2:Led_Off(LED2_Port, LED2_Pin);Led_On(LED3_Port, LED3_Pin);break;
	}
	i++;
	i%=3;
}


