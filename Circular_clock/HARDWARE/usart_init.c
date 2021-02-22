#include "usart_init.h"
#include "wifi.h"

u8 USART1_RX_BUF[RX_BUF_LEN];
u8 USART1_RX_ok;//������ϱ�־λ
u8 USART1_RX_r;//���յ�0x0d��־
u8 USART1_RX_BUF_long;//���ܵ����ݵĳ���

u8 USART2_RX_BUF[RX_BUF_LEN];
u8 USART2_RX_ok;//������ϱ�־λ
u8 USART2_RX_r;//���յ�0x0d��־
u8 USART2_RX_BUF_long;//���ܵ����ݵĳ���
	
	
////////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}  //ѭ������,ֱ���������   
	
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 




/////////////////////////////////////////////////////////////////////////////////
//��������USART1_init													  															 
//�������ܣ���USART1��IO�����ò���ʼ��USART1     																 
//����ֵ����   																																 
//���ֵ����																																		 
/////////////////////////////////////////////////////////////////////////////////
void USART1_init()
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStructer;
	USART_InitTypeDef USART_InitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;									
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1��ʱ��
	
	//USART1_TX   GPIOA.9��ʼ��
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_9;						//PA.9
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//��ʼ��IO��
	
	//USART1_RX   GPIOA.10��ʼ��
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_10;						//PA.10
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//��ʼ��IO��  �������ģʽ��������
	
	//USART ��ʼ������
	USART_InitStructer.USART_BaudRate = 115200;					//�����ʣ�115200
	USART_InitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructer.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;//����ʹ�ܻ�������ʹ�ܻ��߶�ʹ��
	USART_InitStructer.USART_Parity = USART_Parity_No; //��żУ�飺����ż����
	USART_InitStructer.USART_StopBits = USART_StopBits_1; //ֹͣλ:1��
	USART_InitStructer.USART_WordLength = USART_WordLength_8b; //�ֳ�����λ���ݸ�ʽ
	USART_Init(USART1,&USART_InitStructer);//��ʼ�����ڵĲ���
	
	USART_Cmd(USART1,ENABLE);    //ʹ��USART1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���������ж�
	
	//Usart1 NVIC ����
	NVIC_InitStructer.NVIC_IRQChannel =USART1_IRQn;					//����������������
	NVIC_InitStructer.NVIC_IRQChannelCmd =ENABLE;						//IRQͨ��ʹ��
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =1;	//
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructer);//�ж����ȼ��ĳ�ʼ��
	USART1_get_ok();
}

/////////////////////////////////////////////////////////////////////////////////
//��������USART1_get_ok																												 
//�������ܣ���USART1�ı�־λ����																							 
//����ֵ����																																	 
//���ֵ����																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART1_get_ok()
{
	USART1_RX_ok=0;//������ϱ�־λ
	USART1_RX_r=0;//���յ�0x0d��־
	USART1_RX_BUF_long=0;//���ܵ����ݵĳ���
}

/////////////////////////////////////////////////////////////////////////////////
//��������USART1_IRQHandler																										 
//�������ܣ�USART1���жϺ���	�����ν��յ�һ�����ݷ��뻺��������־�Խ������	 
//����ֵ����																																	 
//���ֵ����																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART1_IRQHandler()
{
	u8 res;//ÿ�ν��յ�������
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		if(USART1_RX_ok==0)
		{
			if(USART1_RX_r==1)//�������
			{
				if(res!=0x0a) USART1_get_ok();
				else USART1_RX_ok=1;
			}
			else
			{
				if(res==0x0d)USART1_RX_r=1;
				else
				{
					USART1_RX_BUF[USART1_RX_BUF_long]=res;
					USART1_RX_BUF_long+=1;
					if(USART1_RX_BUF_long>=200)USART1_get_ok();
				}
			}
		}
	}	
}





/////////////////////////////////////////////////////////////////////////////////
//��������USART2_init													  															 
//�������ܣ���USART2��IO�����ò���ʼ��USART1     																 
//����ֵ����   																																 
//���ֵ����																																		 
/////////////////////////////////////////////////////////////////////////////////
void USART2_init()
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStructer;
	USART_InitTypeDef USART_InitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;									
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOA��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2��ʱ��
	
	//USART2_TX   GPIOB.10��ʼ��
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_2;						//PA.2
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//��ʼ��IO��
	
	//USART2_RX   GPIOB.11��ʼ��
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_3;						//PA.3
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//��ʼ��IO��  �������ģʽ��������
	
	//USART2 ��ʼ������
	USART_InitStructer.USART_BaudRate = 115200;					//�����ʣ�115200
	USART_InitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructer.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;//����ʹ�ܻ�������ʹ�ܻ��߶�ʹ��
	USART_InitStructer.USART_Parity = USART_Parity_No; //��żУ�飺����ż����
	USART_InitStructer.USART_StopBits = USART_StopBits_1; //ֹͣλ:1��
	USART_InitStructer.USART_WordLength = USART_WordLength_8b; //�ֳ�����λ���ݸ�ʽ
	USART_Init(USART2,&USART_InitStructer);//��ʼ�����ڵĲ���
	
	USART_Cmd(USART2,ENABLE);    //ʹ��USART2
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//���������ж�
	
	//USART2 NVIC ����
	NVIC_InitStructer.NVIC_IRQChannel =USART2_IRQn;					//����������������
	NVIC_InitStructer.NVIC_IRQChannelCmd =ENABLE;						//IRQͨ��ʹ��
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =0;	//
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(&NVIC_InitStructer);//�ж����ȼ��ĳ�ʼ��
	USART2_get_ok();
}

/////////////////////////////////////////////////////////////////////////////////
//��������USART2_get_ok																												 
//�������ܣ���USART2�ı�־λ����																							 
//����ֵ����																																	 
//���ֵ����																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART2_get_ok()
{
	USART2_RX_ok=0;//������ϱ�־λ
	USART2_RX_r=0;//���յ�0x0d��־
	USART2_RX_BUF_long=0;//���ܵ����ݵĳ���
}

/////////////////////////////////////////////////////////////////////////////////
//��������USART2_IRQHandler																										 
//�������ܣ�USART2���жϺ���	�����ν��յ�һ�����ݷ��뻺��������־�Խ������	 
//����ֵ����																																	 
//���ֵ����																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler()
{
	u8 res;//ÿ�ν��յ�������
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		uart_receive_input(res);
	}	
}
