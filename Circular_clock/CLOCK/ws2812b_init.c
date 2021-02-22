#include "ws2812b_init.h"

u8 REG_LED_cooking[216]={0};//每次刷新时的颜色数据

void REG_LED_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;											//定义结构体变量
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能PB时钟
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			   				//REG-->PB.12端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 				//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 				//IO口速度为50MHz
	
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     					//初始化GPIOC.13	
	GPIO_REG=0;
	delay_us(80);
}
void REG_LED_write0()
{
	GPIO_REG=1;
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	GPIO_REG=0;
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();
}
void REG_LED_write1()
{
	GPIO_REG=1;
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	GPIO_REG=0;
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
}
void REG_LED_reset()
{
	GPIO_REG=0;
	delay_us(80);
}
void REG_LED_Write_Byte(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(byte&0x80) REG_LED_write1();
		else REG_LED_write0();
		byte<<=1;
	}
}
void REG_LED_Write_24Bits(u8 red,u8 green,u8 blue)
{
	REG_LED_Write_Byte(green);
	REG_LED_Write_Byte(red);
	REG_LED_Write_Byte(blue);
}

void REG_LED_Write_cooking(u8 LED_num,u8 red,u8 green,u8 blue)
{
	REG_LED_cooking[LED_num*3]=red;
	REG_LED_cooking[LED_num*3+1]=green;
	REG_LED_cooking[LED_num*3+2]=blue;
}
void REG_LED_Refresh(void)
{
	u8 i=0;
	for(i=0;i<REG_LED_len;i++)
	{		
		REG_LED_Write_24Bits(REG_LED_cooking[i*3],REG_LED_cooking[i*3+1],REG_LED_cooking[i*3+2]);
		
	}
	REG_LED_reset();
}
void REG_LED_ALL_RED(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=255;
		REG_LED_cooking[i*3+1]=0;
		REG_LED_cooking[i*3+2]=0;
	}
	REG_LED_Refresh();
}
void REG_LED_ALL_GREEN(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=0;
		REG_LED_cooking[i*3+1]=255;
		REG_LED_cooking[i*3+2]=0;
	}
	REG_LED_Refresh();
}
void REG_LED_ALL_BLUE(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=0;
		REG_LED_cooking[i*3+1]=0;
		REG_LED_cooking[i*3+2]=255;
	}
	REG_LED_Refresh();
}
void REG_LED_ALL_YELLO(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=255;
		REG_LED_cooking[i*3+1]=255;
		REG_LED_cooking[i*3+2]=0;
	}
	REG_LED_Refresh();
}
void REG_LED_ALL_PINK(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=255;
		REG_LED_cooking[i*3+1]=0;
		REG_LED_cooking[i*3+2]=255;
	}
	REG_LED_Refresh();
}
void REG_LED_ALL_AZURE(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=0;
		REG_LED_cooking[i*3+1]=255;
		REG_LED_cooking[i*3+2]=255;
	}
	REG_LED_Refresh();
}
void REG_LED_ALL_WHITE(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=255;
		REG_LED_cooking[i*3+1]=255;
		REG_LED_cooking[i*3+2]=255;
	}
	REG_LED_Refresh();
}
void REG_LED_ALL_BLACK(void)
{
	u8 i;
	for(i=0;i<REG_LED_len;i++)
	{
		REG_LED_cooking[i*3]=0;
		REG_LED_cooking[i*3+1]=0;
		REG_LED_cooking[i*3+2]=0;
	}
	REG_LED_Refresh();
}

u8 REG_LED_colour_write(u8 shi,u8 fen,u8 liangdu)
{
	u8 i;
	u8 colour_wei;
	if(fen>60||shi>23) return 1;  //进入数据错误
	if(shi>12) shi-=12;
		for(i=0;i<60;i++)
		{
			if(i<fen)
			{
				colour_wei=choise1_12(i);
				switch (colour_wei)
				{
					case 1: REG_LED_Write_cooking(i,liangdu,0,0);break;
					case 2: REG_LED_Write_cooking(i,0,liangdu,0);break;
					case 3:	REG_LED_Write_cooking(i,liangdu,liangdu,0);break;
					case 4:	REG_LED_Write_cooking(i,0,0,liangdu);break;
					case 5:	REG_LED_Write_cooking(i,liangdu,0,liangdu);break;
					case 6:	REG_LED_Write_cooking(i,0,liangdu,liangdu);break;
					case 7:	REG_LED_Write_cooking(i,liangdu,0,0);break;
					case 8:	REG_LED_Write_cooking(i,0,liangdu,0);break;
					case 9:	REG_LED_Write_cooking(i,liangdu,liangdu,0);break;
					case 10:REG_LED_Write_cooking(i,0,0,liangdu);break;
					case 11:REG_LED_Write_cooking(i,liangdu,0,liangdu);break;
					case 12:REG_LED_Write_cooking(i,0,liangdu,liangdu);break;
				}
			}
			else REG_LED_Write_cooking(i,0,0,0);
		}
		for(i=1;i<13;i++)
		{
			if(i<=shi)
			{
				switch (i)
				{
					case 12: REG_LED_Write_cooking(i+60-11,liangdu,0,0);break;
					case 1: REG_LED_Write_cooking(i+60,0,liangdu,0);break;
					case 2:	REG_LED_Write_cooking(i+60,liangdu,liangdu,0);break;
					case 3:	REG_LED_Write_cooking(i+60,0,0,liangdu);break;
					case 4:	REG_LED_Write_cooking(i+60,liangdu,0,liangdu);break;
					case 5:	REG_LED_Write_cooking(i+60,0,liangdu,liangdu);break;
					case 6:	REG_LED_Write_cooking(i+60,liangdu,0,0);break;
					case 7:	REG_LED_Write_cooking(i+60,0,liangdu,0);break;
					case 8:	REG_LED_Write_cooking(i+60,liangdu,liangdu,0);break;
					case 9:REG_LED_Write_cooking(i+60,0,0,liangdu);break;
					case 10:REG_LED_Write_cooking(i+60,liangdu,0,liangdu);break;
					case 11:REG_LED_Write_cooking(i+60,0,liangdu,liangdu);break;
				}
			}
			else 
			{
				if(i!=12) REG_LED_Write_cooking(i+60,0,0,0);
				else REG_LED_Write_cooking(i+60-12,0,0,0);
			}
		}
	REG_LED_Refresh();
	return 0;
}
	
u8 choise1_12(u8 n)
{
	
	if(n<5) return 1;
	else if(n<10)return 2;
	else if(n<15)return 3;
	else if(n<20)return 4;
	else if(n<25)return 5;
	else if(n<30)return 6;
	else if(n<35)return 7;
	else if(n<40)return 8;
	else if(n<45)return 9;
	else if(n<50)return 10;
	else if(n<55)return 11;
	else if(n<60)return 12;
	else return 0;
	
}
