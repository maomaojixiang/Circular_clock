#include "gpio_init.h"


/////////////////////////////////////////////////////////////////////////////////
//函数名：GPIO_out_Init
//函数功能：输出IO口的初始化
//输入值：无
//输出值：无
/////////////////////////////////////////////////////////////////////////////////
void GPIO_out_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;											//定义结构体变量
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //使能PC时钟
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			   				//LED-->PC.13端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 				//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 				//IO口速度为50MHz
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);			     					//初始化GPIOC.13	
}
