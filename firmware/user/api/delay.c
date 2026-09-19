#include "delay.h"


volatile uint32_t systemTick = 0;

//滴答定时器中断服务函数
void SysTick_Handler(void)
{
	systemTick++;	
}

uint32_t System_GetTick(void)
{
	return systemTick;
}


//ms延时函数
void Delay_ms(unsigned int ms)
{
	uint32_t time = systemTick+ms;	
	while(systemTick < time)
	{}
}



//us延时函数
void Delay_us(unsigned int us)
{
	while(us--)
	{
		delay_1us();
	}
}

////ms延时函数
//void Delay_ms(unsigned int ms)
//{
//	unsigned int t = ms*1000;
//	while(t--)
//	{
//		delay_1us();
//	}
//}




