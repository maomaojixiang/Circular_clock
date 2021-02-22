#include "usart_init.h"
#include "wifi.h"

u8 USART1_RX_BUF[RX_BUF_LEN];
u8 USART1_RX_ok;//接收完毕标志位
u8 USART1_RX_r;//接收到0x0d标志
u8 USART1_RX_BUF_long;//接受到数据的长度

u8 USART2_RX_BUF[RX_BUF_LEN];
u8 USART2_RX_ok;//接收完毕标志位
u8 USART2_RX_r;//接收到0x0d标志
u8 USART2_RX_BUF_long;//接受到数据的长度
	
	
////////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}  //循环发送,直到发送完毕   
	
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 




/////////////////////////////////////////////////////////////////////////////////
//函数名：USART1_init													  															 
//函数功能：对USART1的IO口配置并初始化USART1     																 
//输入值：无   																																 
//输出值：无																																		 
/////////////////////////////////////////////////////////////////////////////////
void USART1_init()
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructer;
	USART_InitTypeDef USART_InitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;									
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1的时钟
	
	//USART1_TX   GPIOA.9初始化
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_9;						//PA.9
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//初始化IO口
	
	//USART1_RX   GPIOA.10初始化
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_10;						//PA.10
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//初始化IO口  输入输出模式查表得来的
	
	//USART 初始化设置
	USART_InitStructer.USART_BaudRate = 115200;					//波特率：115200
	USART_InitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructer.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;//发射使能或者输入使能或者都使能
	USART_InitStructer.USART_Parity = USART_Parity_No; //奇偶校验：无奇偶检验
	USART_InitStructer.USART_StopBits = USART_StopBits_1; //停止位:1个
	USART_InitStructer.USART_WordLength = USART_WordLength_8b; //字长：八位数据格式
	USART_Init(USART1,&USART_InitStructer);//初始化串口的参数
	
	USART_Cmd(USART1,ENABLE);    //使能USART1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启接收中断
	
	//Usart1 NVIC 配置
	NVIC_InitStructer.NVIC_IRQChannel =USART1_IRQn;					//不晓得哪里来的了
	NVIC_InitStructer.NVIC_IRQChannelCmd =ENABLE;						//IRQ通道使能
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =1;	//
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructer);//中断优先级的初始化
	USART1_get_ok();
}

/////////////////////////////////////////////////////////////////////////////////
//函数名：USART1_get_ok																												 
//函数功能：将USART1的标志位清零																							 
//输入值：无																																	 
//输出值：无																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART1_get_ok()
{
	USART1_RX_ok=0;//接收完毕标志位
	USART1_RX_r=0;//接收到0x0d标志
	USART1_RX_BUF_long=0;//接受到数据的长度
}

/////////////////////////////////////////////////////////////////////////////////
//函数名：USART1_IRQHandler																										 
//函数功能：USART1的中断函数	将本次接收的一段数据放入缓冲区并标志以接收完毕	 
//输入值：无																																	 
//输出值：无																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART1_IRQHandler()
{
	u8 res;//每次接收到的数据
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		res =USART_ReceiveData(USART1);	//读取接收到的数据
		if(USART1_RX_ok==0)
		{
			if(USART1_RX_r==1)//接收完毕
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
//函数名：USART2_init													  															 
//函数功能：对USART2的IO口配置并初始化USART1     																 
//输入值：无   																																 
//输出值：无																																		 
/////////////////////////////////////////////////////////////////////////////////
void USART2_init()
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructer;
	USART_InitTypeDef USART_InitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;									
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2的时钟
	
	//USART2_TX   GPIOB.10初始化
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_2;						//PA.2
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//初始化IO口
	
	//USART2_RX   GPIOB.11初始化
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_3;						//PA.3
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);								//初始化IO口  输入输出模式查表得来的
	
	//USART2 初始化设置
	USART_InitStructer.USART_BaudRate = 115200;					//波特率：115200
	USART_InitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructer.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;//发射使能或者输入使能或者都使能
	USART_InitStructer.USART_Parity = USART_Parity_No; //奇偶校验：无奇偶检验
	USART_InitStructer.USART_StopBits = USART_StopBits_1; //停止位:1个
	USART_InitStructer.USART_WordLength = USART_WordLength_8b; //字长：八位数据格式
	USART_Init(USART2,&USART_InitStructer);//初始化串口的参数
	
	USART_Cmd(USART2,ENABLE);    //使能USART2
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断
	
	//USART2 NVIC 配置
	NVIC_InitStructer.NVIC_IRQChannel =USART2_IRQn;					//不晓得哪里来的了
	NVIC_InitStructer.NVIC_IRQChannelCmd =ENABLE;						//IRQ通道使能
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =0;	//
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(&NVIC_InitStructer);//中断优先级的初始化
	USART2_get_ok();
}

/////////////////////////////////////////////////////////////////////////////////
//函数名：USART2_get_ok																												 
//函数功能：将USART2的标志位清零																							 
//输入值：无																																	 
//输出值：无																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART2_get_ok()
{
	USART2_RX_ok=0;//接收完毕标志位
	USART2_RX_r=0;//接收到0x0d标志
	USART2_RX_BUF_long=0;//接受到数据的长度
}

/////////////////////////////////////////////////////////////////////////////////
//函数名：USART2_IRQHandler																										 
//函数功能：USART2的中断函数	将本次接收的一段数据放入缓冲区并标志以接收完毕	 
//输入值：无																																	 
//输出值：无																																	 
/////////////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler()
{
	u8 res;//每次接收到的数据
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		res =USART_ReceiveData(USART2);	//读取接收到的数据
		uart_receive_input(res);
	}	
}
