#include "smg_init.h"

u16 smgduan[17]={0x003f,0x0006,0x005b,0x004f,0x0066,0x006d,0x007d,0x0007,0x007f,0x006f,0x0077,0x007c,0x0039,0x005e,0x0079,0x0071};
//数码管段码0-9,A-E,.

/////////////////////////////////////////////////////////////////////////////////
//函数名：GPIO_smg_Init																												 //
//函数功能：数码管输出IO口的初始化																							 //
//输入值：无																																		 //
//输出值：无																																		 /
/////////////////////////////////////////////////////////////////////////////////
void GPIO_smg_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY0-KEY7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);        //初始化GPIOA 0,1,2,3,4,5,6,7
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;//KEY12-KEY14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);        //初始化GPIOB 12,13,14

	delay_init();	
}

/////////////////////////////////////////////////////////////////////////////////
//函数名：GPIO_smg_Init																												 //
//函数功能：对于一个八位数码管对其其中一位进行电亮																 //
//输入值：wei:数码管位数   zhi:数码管要显示的数																	 //
//输出值：无																																		 //
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
//函数名：weishupd																															 //
//函数功能：判断这个数有多少位																									 //
//输入值：date：要判断位数的数 小于等于八位数																		 //
//输出值：无																																		 //
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
//函数名：weixuan																															 //
//函数功能：在同时输出一个数的时候位的选择																				 //
//输入值：wei：要选择哪一位																											 //
//输出值：无																																		 //
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
//函数名：shuxuan																															 //
//函数功能：选择好位后对其要显示的数进行获取																			 //
//输入值：date：欲输出的八位数 wei：位选																				 //
//输出值：无																																		 //
/////////////////////////////////////////////////////////////////////////////////
u8 shuxuan(u32 date,u8 wei)
{
	u8 yushuchu;  																										//欲输出值
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
//函数名：GPIO_smgs_Scan																												 //
//函数功能：对于一个八位数码管  同时点亮																				   //
//输入值：date：欲显示的数																											 //
//输出值：无																																		 //
/////////////////////////////////////////////////////////////////////////////////
void GPIO_smgs_Scan(u32 date)
{
	u8 wei; 																												//储存date的位数
	u8 i;   																												//用于for循环变量
	wei = weishupd(date);
	for(i=wei;i>0;i--)
	{
	  weixuan(i);
		GPIO_Write(GPIOA,smgduan[shuxuan(date,i)]);
		delay_us(200);
	}	
}
