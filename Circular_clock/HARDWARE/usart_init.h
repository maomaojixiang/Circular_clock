#ifndef __USART_INIT_H
#define __USART_INIT_H	
#include "sys.h"
#include "stdio.h"
/////////////////////////////////////////////////////////////////////////////////
//程序名：usart_init.h																												   //
//函数功能：对usart的初始化程序     																					   //
//																																						 //
//																																						 //
/////////////////////////////////////////////////////////////////////////////////


#define RX_BUF_LEN 256																			//接受到数据的最大长度

extern u8 USART1_RX_BUF[RX_BUF_LEN];												//USART1 接收到数据的缓冲区
extern u8 USART1_RX_ok;																			//USART1 接收完毕标志位
extern u8 USART1_RX_r;																			//USART1 接收到0x0d标志
extern u8 USART1_RX_BUF_long;																//USART1 接受到数据的长度

extern u8 USART2_RX_BUF[RX_BUF_LEN];												//USART2 接收到数据的缓冲区
extern u8 USART2_RX_ok;																			//USART2 接收完毕标志位
extern u8 USART2_RX_r;																			//USART2 接收到0x0d标志
extern u8 USART2_RX_BUF_long;																//USART2 接受到数据的长度


//函数的定义
void USART1_init(void);																			//对USART1的IO口配置并初始化USART1
void USART1_get_ok(void);																		//将USART1的标志位清零

void USART2_init(void);																			//对USART2的IO口配置并初始化USART2
void USART2_get_ok(void);																		//将USART2的标志位清零
#endif
