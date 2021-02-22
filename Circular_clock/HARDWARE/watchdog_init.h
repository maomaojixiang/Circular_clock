#ifndef __WATCHDOG_INIT_H
#define __WATCHDOG_INIT_H	
#include "sys.h"
#include "delay.h"
#include "math.h"
#include "usart_init.h"
/////////////////////////////////////////////////////////////////////////////////
//程序名：watchdog_init.h																												   
//函数功能：看门狗初始化程序																					   
//																																						 
//																																						 
/////////////////////////////////////////////////////////////////////////////////


//函数的定义
void IWDG_init(u16 time);						//初始化独立看门狗  时间要为8的整数倍
void IWDG_Feed(void);								//喂狗

void WWDG_Init(u8 tr,u8 wr,u32 fprer);//初始化WWDG  上窗口  下窗口 分频WWDG_Prescaler_x  x=1,2,4,8
void WWDG_Set_Counter(u8 cnt);       //设置WWDG的计数器
#endif
