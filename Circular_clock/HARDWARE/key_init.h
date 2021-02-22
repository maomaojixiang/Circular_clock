#ifndef __KEY_INIT_H
#define __KEY_INIT_H	

#include "sys.h"
#include "delay.h"

/////////////////////////////////////////////////////////////////////////////////
//程序名：key_init.h																												     //
//函数功能：对独立按键以及矩阵按键的初始化																			   //
//																																						 //
//																																						 //
/////////////////////////////////////////////////////////////////////////////////


//////////////////////////////独立按键的命名//////////////////////////////////////

#define key1 PAin(6)		//读取GPIOA.0的状态
#define key2 PAin(5)		//读取GPIOA.1的状态

/////////////////////////////////////////////////////////////////////////////////



///////////////////////////////矩阵按键的命名/////////////////////////////////////

#define keys_out1 PAout(0)
#define keys_out2 PAout(1)
#define keys_out3 PAout(2)
#define keys_out4 PAout(3)
#define keys_in1 PAin(4)
#define keys_in2 PAin(5)
#define keys_in3 PAin(6)
#define keys_in4 PAin(7)

/////////////////////////////////////////////////////////////////////////////////


//函数的定义
void GPIO_key_Init(void);								//独立按键的初始化
void GPIO_keys_Init(void);							//矩阵按键的初始化
u8 GPIO_keys_Scan(void);								//矩阵按键驱动
#endif
