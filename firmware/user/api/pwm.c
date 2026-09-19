#include "pwm.h"

//TIM3 CH3	PB0 将灯珠的信号线接到PB0
void Pwm_Config(void)
{
	//1、开时钟 IO 、 定时器的
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//2、IO配置成复用推挽输出
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//3、配置定时器的时基部分  分频器 装载值 1ms周期
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);	
	
	//4、配置定时器的输出比较（占空）	比较值、有效电平状态、是否输出
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStruct);
	
	//5、打开了ARR（装载值）预装载
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	//6、打开了CCR（比较值）预装载
	TIM_CCPreloadControl(TIM3, ENABLE);
	//7、使能定时器就可以
	TIM_Cmd(TIM3, ENABLE);
}



#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

//设置LED灯亮度：0-100
void Pwm_LedSetLight(int leght)
{
	uint16_t l = CLAMP(leght, 0, 100);
	l = l*10;
	TIM_SetCompare3(TIM3, l);
}




