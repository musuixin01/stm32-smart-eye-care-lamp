/**
  ******************************************************************************
  * @file    my1680.c
  * @author  郑州信盈达-霍文光
  * @version V2.0.0
  * @date    2025-5-23
  * @brief   my1680/1690等模组的基本驱动代码.
	* 				 使用者可在此基础上二次封装各种操作指令及播报内容。
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "MY1680.h"
#include "delay.h"

VOICE_DEV voice1={.frame_head = 0x7E,.frame_end = 0xEF};
	
//MY1680通信接口初始化
//接线 MY1680_RX  	-- USART2_TX	PA2
//接线 MY1680_BUSY  -- PB11
void MY1680_UARTConfig(u32 brr)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//开时钟
	RCC_APB2PeriphClockCmd(MY1680_TX_CLK | MY1680_RX_CLK, ENABLE);
	MY1680_UART_CLKCMD(MY1680_UART_CLK, ENABLE);
	//结构体赋值
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Pin = MY1680_TX_PIN;
	GPIO_Init(MY1680_TX_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = MY1680_RX_PIN;
	GPIO_Init(MY1680_RX_PORT, &GPIO_InitStructure);
	
	//初始化USART  波特率brr 打开发送和接收器  无奇偶校验  一个停止位  8个数据位
	USART_InitStructure.USART_BaudRate = brr;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(MY1680_USART, &USART_InitStructure);
	USART_Cmd(MY1680_USART,ENABLE);//使能USART
}

//MY1680发送数据函数
void MY1680_SendString(u8 *str,u8 lenth)
{
	for(u8 i=0; i<lenth; i++)
	{
		while(USART_GetFlagStatus(MY1680_USART,USART_FLAG_TC) == RESET);
		USART_SendData(MY1680_USART, str[i]);		
	}
}


/********************************************************
函数名称：MY1680_Init
函数功能：MY1680初始化
函数传参：无
函数返回值：无
*******************************************************/
void MY1680_Init(void)
{
	//BUSY管脚初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(MY1680_BUSY_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MY1680_BUSY_PIN;
	GPIO_Init(MY1680_BUSY_PORT, &GPIO_InitStructure);
	//USART初始化
	MY1680_UARTConfig(9600);
	Voice_SendCmd(0x38,0x01,0x00,1);//设置BUSY引脚 为高电平
	Delay_ms(100);
	Voice_SendCmd(0x31,0x1E,0x00,1);//设置音量最高
	Delay_ms(100);
}

/********************************************************
 *函数名称：Voice_XorCheck
 *函数功能：^检测 
 *函数传参：
 *			*pdata -- 校准参数
 *			lenth  -- 参数的个数
 *函数返回值：^后的数
 ********************************************************/
u8 Voice_XorCheck(u8 *pdata,u8 lenth)
{
	u8 r_value = *pdata;
	u8 i = 0;
	for(i=1;i<lenth;i++)
	{
		r_value ^= pdata[i];
	}
	return r_value;
}

/**********************************************************
 *函数名称：Voice_SendCmd
 *函数功能：发送命令 
 *函数传参：
 *				cmd -- 命令
 *				arg_lenth -- 参数个数
 *				arg1 -- 第一个参数
 *				arg2 -- 第二个参数
 *函数返回值：无
 **********************************************************/
void Voice_SendCmd(u8 cmd,u8 arg1,u8 arg2,u8 arg_lenth)
{
	//给voice1传递参数
	//命令填充
	voice1.cmd = cmd;
	//参数填充
	voice1.arg[0] = arg1;
	voice1.arg[1] = arg2;
	voice1.arg[2] = arg_lenth;
	//长度填充
	voice1.lenth = 3+arg_lenth;//长度 + cmd + xor + 参数长度
	voice1.xorcheck = Voice_XorCheck(&voice1.lenth,2+arg_lenth);//lenth+cmd + 参数
	//发送
	MY1680_SendString(&voice1.frame_head,3);//帧起始+长度+命令
	MY1680_SendString(voice1.arg,arg_lenth);//发送参数
	MY1680_SendString(&voice1.xorcheck,2);//异或值+帧尾
}

//播放指定目录文件下的音乐
//directorynum：目录编号
//musicnum：音乐编号
void Voice_PlayDirectoryMusic(u8 directorynum, u8 musicnum)
{
	while(VoicePlay_Busy);
	Voice_SendCmd(CMD_CHOOSE_MUSIC,directorynum,musicnum,2);
}




