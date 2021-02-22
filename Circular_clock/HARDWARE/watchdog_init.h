#ifndef __WATCHDOG_INIT_H
#define __WATCHDOG_INIT_H	
#include "sys.h"
#include "delay.h"
#include "math.h"
#include "usart_init.h"
/////////////////////////////////////////////////////////////////////////////////
//��������watchdog_init.h																												   
//�������ܣ����Ź���ʼ������																					   
//																																						 
//																																						 
/////////////////////////////////////////////////////////////////////////////////


//�����Ķ���
void IWDG_init(u16 time);						//��ʼ���������Ź�  ʱ��ҪΪ8��������
void IWDG_Feed(void);								//ι��

void WWDG_Init(u8 tr,u8 wr,u32 fprer);//��ʼ��WWDG  �ϴ���  �´��� ��ƵWWDG_Prescaler_x  x=1,2,4,8
void WWDG_Set_Counter(u8 cnt);       //����WWDG�ļ�����
#endif
