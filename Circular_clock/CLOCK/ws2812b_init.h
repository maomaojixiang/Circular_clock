#ifndef __WS2812B_INIT_H__
#define __WS2812B_INIT_H__

#include "sys.h"
#include "delay.h"


#define GPIO_REG PAout(6)
#define REG_LED_len 72
void REG_LED_Write_24Bits(u8 red,u8 green,u8 blue);//发送一个24位数据
void REG_LED_reset(void);	//发送reset信号
void REG_LED_Init(void);  //初始化WS2812B

void REG_LED_Write_cooking(u8 LED_num,u8 red,u8 green,u8 blue);//改变cooking中的数据
void REG_LED_Refresh(void);		//刷新led  从cooking中写入

void REG_LED_ALL_RED(void);		//向cooking中写入红色并刷新
void REG_LED_ALL_GREEN(void);	//向cooking中写入绿色并刷新
void REG_LED_ALL_BLUE(void);	//向cooking中写入蓝色并刷新
void REG_LED_ALL_YELLO(void);	//向cooking中写入黄色并刷新
void REG_LED_ALL_PINK(void);	//向cooking中写入粉色并刷新
void REG_LED_ALL_AZURE(void);	//向cooking中写入天蓝色并刷新
void REG_LED_ALL_WHITE(void);	//向cooking中写入白色并刷新
void REG_LED_ALL_BLACK(void); //向cooking中写入黑色并刷新

u8 choise1_12(u8 n);		//选择颜色的位数
u8 REG_LED_colour_write(u8 shi,u8 fen,u8 liangdu);  //按时间更改cooking

#endif
