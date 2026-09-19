#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED1_Port 	GPIOC
#define LED1_Pin 	GPIO_Pin_0
#define LED2_Port 	GPIOC
#define LED2_Pin 	GPIO_Pin_1
#define LED3_Port 	GPIOC
#define LED3_Pin 	GPIO_Pin_2


void Led_Init(void);
void Led_On(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Led_Off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Led_Task(void);


#endif
