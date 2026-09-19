#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "pwm.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "lcd.h"
#include "my1680.h"
#include "p1.h"

/* ==========================================================================
 * 个人信息：在自己的板子上烧录前，请把下面两个宏替换为你自己的姓名与学号。
 * 公开仓库中默认做了脱敏处理，不包含真实个人信息。
 * ========================================================================== */
#define STUDENT_NAME  "你的姓名"
#define STUDENT_ID    "你的学号"


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	int light = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组
	SysTick_Config(72000);	//系统定时器初始化 1ms的定时器配置
	Led_Init();
	USART1_Config();
	printf("hello world\r\n");
	Key_Init();
	Pwm_Config();
	LCD_Init();
	LCD_Fill(0,0,128,168,BLACK);
	LCD_ShowChinese(61,81,(uint8_t*)"欢迎使用护眼台灯！",WHITE,BLACK,16,0);
	Delay_ms(1000);
	LCD_Fill(0,0,128,168,WHITE);
	LCD_ShowChinese(0,0,(uint8_t*)STUDENT_NAME,RED,WHITE,16,0);
	LCD_ShowString(0,17,(uint8_t*)STUDENT_ID,RED,WHITE,16,0);
	LCD_ShowPicture(0,33,128,128,gImage_p1);
	MY1680_Init();
	Voice_PlayDirectoryMusic(1,1);

  while (1)
  {
			switch(Key_GetValue())
			{
				case 1:
					light+=25;
					if(light>=100) light=100;
					Pwm_LedSetLight(light);
					switch(light)
					{
						case 0:
							Voice_PlayDirectoryMusic(1,6);
						break;
						case 25:
							Voice_PlayDirectoryMusic(1,2);
						break;
						case 50:
							Voice_PlayDirectoryMusic(1,3);
						break;
						case 75:
							Voice_PlayDirectoryMusic(1,4);
						break;
						case 100:
							Voice_PlayDirectoryMusic(1,5);
						break;
					}
					break;
				case 2:
					light-=25;
					if(light<=0) light=0;
					Pwm_LedSetLight(light);
					switch(light)
					{
						case 0:
							Voice_PlayDirectoryMusic(1,6);
						break;
						case 25:
							Voice_PlayDirectoryMusic(1,2);
						break;
						case 50:
							Voice_PlayDirectoryMusic(1,3);
						break;
						case 75:
							Voice_PlayDirectoryMusic(1,4);
						break;
						case 100:
							Voice_PlayDirectoryMusic(1,5);
						break;
					}
				break;
			}
		
//		Led_On(LED1_Port, LED1_Pin);
//		Led_On(LED2_Port, LED2_Pin);
//		Led_On(LED3_Port, LED3_Pin);

//		Delay_ms(500);

//		Led_Off(LED1_Port, LED1_Pin);
//		Led_Off(LED2_Port, LED2_Pin);
//		Led_Off(LED3_Port, LED3_Pin);

//		Delay_ms(500);
	}
}
