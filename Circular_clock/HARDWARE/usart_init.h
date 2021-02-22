#ifndef __USART_INIT_H
#define __USART_INIT_H	
#include "sys.h"
#include "stdio.h"
/////////////////////////////////////////////////////////////////////////////////
//��������usart_init.h																												   //
//�������ܣ���usart�ĳ�ʼ������     																					   //
//																																						 //
//																																						 //
/////////////////////////////////////////////////////////////////////////////////


#define RX_BUF_LEN 256																			//���ܵ����ݵ���󳤶�

extern u8 USART1_RX_BUF[RX_BUF_LEN];												//USART1 ���յ����ݵĻ�����
extern u8 USART1_RX_ok;																			//USART1 ������ϱ�־λ
extern u8 USART1_RX_r;																			//USART1 ���յ�0x0d��־
extern u8 USART1_RX_BUF_long;																//USART1 ���ܵ����ݵĳ���

extern u8 USART2_RX_BUF[RX_BUF_LEN];												//USART2 ���յ����ݵĻ�����
extern u8 USART2_RX_ok;																			//USART2 ������ϱ�־λ
extern u8 USART2_RX_r;																			//USART2 ���յ�0x0d��־
extern u8 USART2_RX_BUF_long;																//USART2 ���ܵ����ݵĳ���


//�����Ķ���
void USART1_init(void);																			//��USART1��IO�����ò���ʼ��USART1
void USART1_get_ok(void);																		//��USART1�ı�־λ����

void USART2_init(void);																			//��USART2��IO�����ò���ʼ��USART2
void USART2_get_ok(void);																		//��USART2�ı�־λ����
#endif
