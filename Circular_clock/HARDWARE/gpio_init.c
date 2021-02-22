#include "gpio_init.h"


/////////////////////////////////////////////////////////////////////////////////
//��������GPIO_out_Init
//�������ܣ����IO�ڵĳ�ʼ��
//����ֵ����
//���ֵ����
/////////////////////////////////////////////////////////////////////////////////
void GPIO_out_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;											//����ṹ�����
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //ʹ��PCʱ��
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			   				//LED-->PC.13�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 				//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 				//IO���ٶ�Ϊ50MHz
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);			     					//��ʼ��GPIOC.13	
}
