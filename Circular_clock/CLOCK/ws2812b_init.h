#ifndef __WS2812B_INIT_H__
#define __WS2812B_INIT_H__

#include "sys.h"
#include "delay.h"


#define GPIO_REG PAout(6)
#define REG_LED_len 72
void REG_LED_Write_24Bits(u8 red,u8 green,u8 blue);//����һ��24λ����
void REG_LED_reset(void);	//����reset�ź�
void REG_LED_Init(void);  //��ʼ��WS2812B

void REG_LED_Write_cooking(u8 LED_num,u8 red,u8 green,u8 blue);//�ı�cooking�е�����
void REG_LED_Refresh(void);		//ˢ��led  ��cooking��д��

void REG_LED_ALL_RED(void);		//��cooking��д���ɫ��ˢ��
void REG_LED_ALL_GREEN(void);	//��cooking��д����ɫ��ˢ��
void REG_LED_ALL_BLUE(void);	//��cooking��д����ɫ��ˢ��
void REG_LED_ALL_YELLO(void);	//��cooking��д���ɫ��ˢ��
void REG_LED_ALL_PINK(void);	//��cooking��д���ɫ��ˢ��
void REG_LED_ALL_AZURE(void);	//��cooking��д������ɫ��ˢ��
void REG_LED_ALL_WHITE(void);	//��cooking��д���ɫ��ˢ��
void REG_LED_ALL_BLACK(void); //��cooking��д���ɫ��ˢ��

u8 choise1_12(u8 n);		//ѡ����ɫ��λ��
u8 REG_LED_colour_write(u8 shi,u8 fen,u8 liangdu);  //��ʱ�����cooking

#endif
