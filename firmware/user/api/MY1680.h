/**
  ******************************************************************************
  * @file    my1680.h
  * @author  郑州信盈达-霍文光
  * @version V2.0.0
  * @date    2025-5-23
  * @brief   my1680/1690等模组的基本驱动代码.
	* 				 使用者可在此基础上二次封装各种操作指令及播报内容。
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#ifndef _MY1680_H_
#define _MY1680_H_


#include "stm32f10x.h"

/*********************MY1680底层接口宏定义************************************/
#define MY1680_USART					USART2
#define MY1680_UART_CLKCMD		RCC_APB1PeriphClockCmd
#define MY1680_UART_CLK				RCC_APB1Periph_USART2

#define MY1680_TX_CLK					RCC_APB2Periph_GPIOA
#define MY1680_TX_PORT				GPIOA
#define MY1680_TX_PIN					GPIO_Pin_2

#define MY1680_RX_CLK					RCC_APB2Periph_GPIOA
#define MY1680_RX_PORT				GPIOA
#define MY1680_RX_PIN					GPIO_Pin_3

#define MY1680_BUSY_CLK				RCC_APB2Periph_GPIOB
#define MY1680_BUSY_PORT			GPIOB
#define MY1680_BUSY_PIN				GPIO_Pin_11

/**********************MY1680命令宏定义*******************************/

#define CMD_PLAY 		  		0x11            //播放
#define CMD_STOP 		  		0x12            //暂停
#define CMD_NEXT 	 	  		0x13            //下一曲
#define CMD_PREV 					0x14            //上一曲
#define CMD_VOICEADD  		0x15            //音量加
#define CMD_VOICESUB  		0x16            //音量减
#define CMD_RESET     		0x19            //复位
#define CMD_SPEED     		0x1A            //快进
#define CMD_SLOW      		0x1B            //快退
#define CMD_STARTorSTOP 	0x1C            //播放/暂停
#define CMD_END         	0x1E            //停止
#define CMD_ROOTCHO_MUSIC 0x41            //根目录选择歌曲
#define CMD_CHOOSE_MUSIC 	0x42            //任意目录选择歌曲

//数据帧结构体
typedef struct
{
	u8 frame_head;//帧头
	u8 lenth;     //数据长度
	u8 cmd;       //命令
	u8 arg[3];    //参数个数0 1 2   arg[2] -- 参数的个数 arg[0] -- 第一个参数 arg[1] -- 第二个参数
	u8 xorcheck;  //校验码
	u8 frame_end; //帧尾
}VOICE_DEV;

//忙状态检测
#define VoicePlay_Busy	GPIO_ReadInputDataBit(MY1680_BUSY_PORT, MY1680_BUSY_PIN)

void MY1680_UARTConfig(u32 brr);
void MY1680_Init(void);
void MY1680_SendString(u8 *str,u8 lenth);
u8 Voice_XorCheck(u8 *pdata,u8 lenth);
void Voice_SendCmd(u8 cmd,u8 arg1,u8 arg2,u8 arg_lenth);	
void Voice_PlayDirectoryMusic(u8 directorynum, u8 musicnum);
void Voice_PlayNumber(u16 number);
#endif

