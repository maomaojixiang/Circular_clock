#include "exti_init.h"
#include "wifi.h"
#include "adc.h"

void get_time(void);							//��ȡʱ��  //����Уʱ ��ʱ���ж���ÿһСʱִ��һ��

/////////////////////////////////////////////////////////////////////////////////
//��������EXTI_key_init													  															 
//�������ܣ���EXTI4��EXTI5��IO������  																 
//����ֵ����   																																 
//���ֵ����																																		 
/////////////////////////////////////////////////////////////////////////////////
void EXTI_key_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;											//����ṹ�����
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //ʹ��PAʱ��
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			//EXTI5_KEY-->PA.5 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 					//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//IO���ٶ�Ϊ50MHz	
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     					//��ʼ��GPIOA.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			//EXTI4_KEY-->PA.4 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 					//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//IO���ٶ�Ϊ50MHz	
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     					//��ʼ��GPIOA.4
}

/////////////////////////////////////////////////////////////////////////////////
//��������EXTI0_init													  															 
//�������ܣ���ʼ��EXTI4��EXTI5													 
//����ֵ����   																																 
//���ֵ����																																		 
/////////////////////////////////////////////////////////////////////////////////
void EXTI_init()
{
	EXTI_InitTypeDef EXTI_InitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;//����ṹ�����
	
	//��EXTI0��IO������
	EXTI_key_init();
	
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//�ж���5����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
	
	//�ж���5����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	//��ʼ��EXTI5
	EXTI_InitStructer.EXTI_Line=EXTI_Line5;//�ж���:��5
	EXTI_InitStructer.EXTI_LineCmd=ENABLE;//ʹ��
	EXTI_InitStructer.EXTI_Mode=EXTI_Mode_Interrupt;//ģʽ���ж�  ����һ�����¼�
	EXTI_InitStructer.EXTI_Trigger=EXTI_Trigger_Falling;//������ʽ���½���
	EXTI_Init(&EXTI_InitStructer);
	
	//��ʼ��EXTI4
	EXTI_InitStructer.EXTI_Line=EXTI_Line4;//�ж���:��4
	EXTI_InitStructer.EXTI_LineCmd=ENABLE;//ʹ��
	EXTI_InitStructer.EXTI_Mode=EXTI_Mode_Interrupt;//ģʽ���ж�  ����һ�����¼�
	EXTI_InitStructer.EXTI_Trigger=EXTI_Trigger_Falling;//������ʽ���½���
	EXTI_Init(&EXTI_InitStructer);
	
	//�ж�5��ʼ������
	NVIC_InitStructer.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructer);
	
	//�ж�4��ʼ������
	NVIC_InitStructer.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructer);

}

/////////////////////////////////////////////////////////////////////////////////
//��������EXTI9_5_IRQHandler													  															 
//�������ܣ�EXTI5�жϺ���     																 
//����ֵ����   																																 
//���ֵ����																																		 
/////////////////////////////////////////////////////////////////////////////////
void EXTI9_5_IRQHandler()
{
	delay_ms(10);//����
	if(EXTI5_KEY==0)
	{
		get_time();
		//do you want
	}
		EXTI_ClearITPendingBit(EXTI_Line5);//����ж�0��־λ
}

/////////////////////////////////////////////////////////////////////////////////
//��������EXTI4_IRQHandler													  															 
//�������ܣ�EXTI4�жϺ���     																 
//����ֵ����   																																 
//���ֵ����																																		 
/////////////////////////////////////////////////////////////////////////////////
void EXTI4_IRQHandler()
{
	delay_ms(10);//����
	if(EXTI4_KEY==0)
	{
		printf("���ж���KEY1");
		mcu_reset_wifi();
		printf("wifi�������\r\n");
		//do you want
	}
		EXTI_ClearITPendingBit(EXTI_Line4);//����ж�0��־λ
}
