#ifndef __OLED_INIT_H
#define __OLED_INIT_H	
#include "sys.h"
#include "delay.h"
#include "IIC_init.h"

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

/**********************************************
// IIC д����
**********************************************/
void Write_IIC_Command(u8 IIC_Command);
/**********************************************
// IIC д����
**********************************************/
void Write_IIC_Data(u8 IIC_Data);

void OLED_Clear(void);					//����
void OLED_Set_Pos(u8 x,u8 y);  //��������

void OLED_Init(void);   //��ʼ��OLED   
void OLED_Display_On(void);	//����OLED��ʾ 
void OLED_Display_Off(void);	//�ر�OLED��ʾ 
void OLED_WR_Byte(u8 dat,u8 cmd);	// OLED ��������  

void OLED_WR_char_0816(u8 x,u8 y,u8 date);   //OLED��ָ���ط�дһ��8*16�ַ�	 x:������0-127   y:ҳ��1-4
void OLED_WR_chars_0816(u8 x,u8 y,u8 *date,u8 len);    //OLED��ָ���ط�дһ��8*16�ַ�	 len:�ַ�������
void OLED_WR_char_1616(u8 x,u8 y,u8 date);   //OLED��ָ���ط�дһ��16*16���� x:������0-127   y:ҳ��1-4
#endif
