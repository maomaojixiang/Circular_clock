#ifndef __OLED_INIT_H
#define __OLED_INIT_H	
#include "sys.h"
#include "delay.h"
#include "IIC_init.h"

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/**********************************************
// IIC 写命令
**********************************************/
void Write_IIC_Command(u8 IIC_Command);
/**********************************************
// IIC 写变量
**********************************************/
void Write_IIC_Data(u8 IIC_Data);

void OLED_Clear(void);					//清屏
void OLED_Set_Pos(u8 x,u8 y);  //设置坐标

void OLED_Init(void);   //初始化OLED   
void OLED_Display_On(void);	//开启OLED显示 
void OLED_Display_Off(void);	//关闭OLED显示 
void OLED_WR_Byte(u8 dat,u8 cmd);	// OLED 发送数据  

void OLED_WR_char_0816(u8 x,u8 y,u8 date);   //OLED向指定地方写一个8*16字符	 x:横坐标0-127   y:页数1-4
void OLED_WR_chars_0816(u8 x,u8 y,u8 *date,u8 len);    //OLED向指定地方写一串8*16字符	 len:字符串长度
void OLED_WR_char_1616(u8 x,u8 y,u8 date);   //OLED向指定地方写一个16*16文字 x:横坐标0-127   y:页数1-4
#endif
