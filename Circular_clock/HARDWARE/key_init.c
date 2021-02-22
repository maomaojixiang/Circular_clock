#include "key_init.h"


/////////////////////////////////////////////////////////////////////////////////
//函数名：GPIO_key_Init																												 //
//函数功能：独立按键所需IO口的初始化																						 //
//输入值：无																																		 //
//输出值：无																																		 //
/////////////////////////////////////////////////////////////////////////////////
void GPIO_key_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;											//定义结构体变量
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能PA时钟
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5;			//KEY1-->PA.0 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 					//上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//IO口速度为50MHz
	
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     					//初始化GPIOA.0
}

/////////////////////////////////////////////////////////////////////////////////
//函数名：GPIO_keys_Init																												 //
//函数功能：按键矩阵所需IO口的初始化																						 //
//输入值：无																																		 //
//输出值：无																																		 //
/////////////////////////////////////////////////////////////////////////////////
void GPIO_keys_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;																						//定义结构体变量
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    											//使能PA时钟
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;			//PA.0-PA.3端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 															//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																//IO口速度为50MHz
	
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     																//初始化GPIOA.0-GPIOA.3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;			//PA.4-PA.7端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 													  		//上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																//IO口速度为50MHz
	
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     																//初始化GPIOA.4-GPIOA.7
}

/////////////////////////////////////////////////////////////////////////////////
//函数名：GPIO_keys_Scan																												 //
//函数功能：按键矩阵驱动																												 //
//输入值：无																																		 //
//输出值：0-16按键位数																													 //
/////////////////////////////////////////////////////////////////////////////////
u8 GPIO_keys_Scan()
{
	delay_init();
	keys_out1 = 1;
	keys_out2 = 1;
	keys_out3 = 1;
	keys_out4 = 1;
	
	keys_out1 = 0;
	delay_ms(10);
	if(keys_in1 == 0) return 1;
	if(keys_in2 == 0) return 2;
	if(keys_in3 == 0) return 3;
	if(keys_in4 == 0) return 4;	
	keys_out1 = 1;
	
	keys_out2 = 0;
	delay_ms(10);
	if(keys_in1 == 0) return 5;
	if(keys_in2 == 0) return 6;
	if(keys_in3 == 0) return 7;
	if(keys_in4 == 0) return 8;
	keys_out2 = 1;
	
	keys_out3 = 0;
	delay_ms(10);
	if(keys_in1 == 0) return 9;
	if(keys_in2 == 0) return 10;
	if(keys_in3 == 0) return 11;
	if(keys_in4 == 0) return 12;
	keys_out3 = 1;
	
	keys_out4 = 0;
	delay_ms(10);
	if(keys_in1 == 0) return 13;
	if(keys_in2 == 0) return 14;
	if(keys_in3 == 0) return 15;
	if(keys_in4 == 0) return 16;

	return 0;
}
