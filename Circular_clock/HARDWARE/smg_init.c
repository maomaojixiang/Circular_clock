#include "smg_init.h"

u16 smgduan[17]={0x003f,0x0006,0x005b,0x004f,0x0066,0x006d,0x007d,0x0007,0x007f,0x006f,0x0077,0x007c,0x0039,0x005e,0x0079,0x0071};
//����ܶ���0-9,A-E,.

/////////////////////////////////////////////////////////////////////////////////
//��������GPIO_smg_Init																												 //
//�������ܣ���������IO�ڵĳ�ʼ��																							 //
//����ֵ����																																		 //
//���ֵ����																																		 /
/////////////////////////////////////////////////////////////////////////////////
void GPIO_smg_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTAʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY0-KEY7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);        //��ʼ��GPIOA 0,1,2,3,4,5,6,7
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;//KEY12-KEY14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);        //��ʼ��GPIOB 12,13,14

	delay_init();	
}

/////////////////////////////////////////////////////////////////////////////////
//��������GPIO_smg_Init																												 //
//�������ܣ�����һ����λ����ܶ�������һλ���е���																 //
//����ֵ��wei:�����λ��   zhi:�����Ҫ��ʾ����																	 //
//���ֵ����																																		 //
/////////////////////////////////////////////////////////////////////////////////
void GPIO_smg_Scan(u8 wei,u8 zhi)
{
	switch(wei)
	{
		case 8:smg_a=0;smg_b=0;smg_c=0;break;
		case 7:smg_a=0;smg_b=1;smg_c=0;break;
		case 6:smg_a=1;smg_b=0;smg_c=0;break;
		case 5:smg_a=1;smg_b=1;smg_c=0;break;
		case 4:smg_a=0;smg_b=0;smg_c=1;break;
		case 3:smg_a=0;smg_b=1;smg_c=1;break;
		case 2:smg_a=1;smg_b=0;smg_c=1;break;
		case 1:smg_a=1;smg_b=1;smg_c=1;break;
	}
	GPIO_Write(GPIOA,smgduan[zhi]);
}

/////////////////////////////////////////////////////////////////////////////////
//��������weishupd																															 //
//�������ܣ��ж�������ж���λ																									 //
//����ֵ��date��Ҫ�ж�λ������ С�ڵ��ڰ�λ��																		 //
//���ֵ����																																		 //
/////////////////////////////////////////////////////////////////////////////////
u8 weishupd(u32 date)
{
	if(date<100000000&&date>9999999)return 8;
	else if(date<10000000&&date>999999)return 7;
	else if(date<1000000&&date>99999)return 6;
	else if(date<100000&&date>9999)return 5;
	else if(date<10000&&date>999)return 4;
	else if(date<1000&&date>99)return 3;
	else if(date<100&&date>9)return 2;
	else if(date<10)return 1;
	else return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//��������weixuan																															 //
//�������ܣ���ͬʱ���һ������ʱ��λ��ѡ��																				 //
//����ֵ��wei��Ҫѡ����һλ																											 //
//���ֵ����																																		 //
/////////////////////////////////////////////////////////////////////////////////
void weixuan(u8 wei)
{
	switch(wei)
	{
		case 1:smg_a=0;smg_b=0;smg_c=0;break;
		case 2:smg_a=0;smg_b=1;smg_c=0;break;
		case 3:smg_a=1;smg_b=0;smg_c=0;break;
		case 4:smg_a=1;smg_b=1;smg_c=0;break;
		case 5:smg_a=0;smg_b=0;smg_c=1;break;
		case 6:smg_a=0;smg_b=1;smg_c=1;break;
		case 7:smg_a=1;smg_b=0;smg_c=1;break;
		case 8:smg_a=1;smg_b=1;smg_c=1;break;
	}
}

/////////////////////////////////////////////////////////////////////////////////
//��������shuxuan																															 //
//�������ܣ�ѡ���λ�����Ҫ��ʾ�������л�ȡ																			 //
//����ֵ��date��������İ�λ�� wei��λѡ																				 //
//���ֵ����																																		 //
/////////////////////////////////////////////////////////////////////////////////
u8 shuxuan(u32 date,u8 wei)
{
	u8 yushuchu;  																										//�����ֵ
	switch(wei)
	{
		case 1:yushuchu=date%10;return yushuchu;
		case 2:yushuchu=date%100/10;return yushuchu;
		case 3:yushuchu=date%1000/100;return yushuchu;
		case 4:yushuchu=date%10000/1000;return yushuchu;
		case 5:yushuchu=date%100000/10000;return yushuchu;
		case 6:yushuchu=date%1000000/100000;return yushuchu;
		case 7:yushuchu=date%10000000/1000000;return yushuchu;
		case 8:yushuchu=date%100000000/10000000;return yushuchu;
		default:return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////
//��������GPIO_smgs_Scan																												 //
//�������ܣ�����һ����λ�����  ͬʱ����																				   //
//����ֵ��date������ʾ����																											 //
//���ֵ����																																		 //
/////////////////////////////////////////////////////////////////////////////////
void GPIO_smgs_Scan(u32 date)
{
	u8 wei; 																												//����date��λ��
	u8 i;   																												//����forѭ������
	wei = weishupd(date);
	for(i=wei;i>0;i--)
	{
	  weixuan(i);
		GPIO_Write(GPIOA,smgduan[shuxuan(date,i)]);
		delay_us(200);
	}	
}
