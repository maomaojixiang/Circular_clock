#include "oled_init.h"
#include "oledfont.h"

/**********************************************
// IIC 写命令
**********************************************/
void Write_IIC_Command(u8 IIC_Command)
{
	IIC_Start();
  IIC_Send_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
  IIC_Send_Byte(0x00);			//write command
	IIC_Wait_Ack();	
  IIC_Send_Byte(IIC_Command); 
	IIC_Wait_Ack();	
  IIC_Stop();
}
/**********************************************
// IIC 写变量
**********************************************/
void Write_IIC_Data(u8 IIC_Data)
{
  IIC_Start();
  IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
  IIC_Send_Byte(0x40);			//write data
	IIC_Wait_Ack();	
  IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();	
  IIC_Stop();
}
/**********************************************
// OLED 发送数据   
**********************************************/
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	if(cmd)  
	{
		Write_IIC_Data(dat);   
	}
	else
	{
		Write_IIC_Command(dat);
	}
}
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
/**********************************************
//坐标设置  x:0-127   y:0-7
**********************************************/
void OLED_Set_Pos(u8 x,u8 y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}
/**********************************************
//OLED向指定地方写一个8*16字符	  将OLED分成了64份  x:横16格  y;竖4格
**********************************************/
void OLED_WR_char_0816(u8 x,u8 y,u8 date)
{
	u8 i,n;
	OLED_Set_Pos(x,y*2);
	for(i=0;i<2;i++)
	{				
		for(n=0;n<8;n++)
		{
			OLED_WR_Byte(date_0816[date-32][i*8+n],OLED_DATA);
		}
		OLED_Set_Pos(x,y*2+1);
	}
}
/**********************************************
//OLED向指定地方写一串8*16字符	  将OLED分成了64份  x:横16格  y;竖4格
**********************************************/
void OLED_WR_chars_0816(u8 x,u8 y,u8 *date,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		OLED_WR_char_0816(x,y,*date);
		date++;
		x+=8;
	}		
}
/**********************************************
//OLED向指定地方写一个16*16字符	  将OLED分成了32份  x:横8格  y;竖4格
**********************************************/
void OLED_WR_char_1616(u8 x,u8 y,u8 date)
{
	u8 i,n;
	OLED_Set_Pos(x,y*2);
	for(i=0;i<2;i++)
	{				
		for(n=0;n<16;n++)
		{
			OLED_WR_Byte(date_1616[date][i*16+n],OLED_DATA);
		}
		OLED_Set_Pos(x,y*2+1);
	}
}
/**********************************************
//初始化SSD1306	  
**********************************************/
void OLED_Init(void)
{ 	
	IIC_Init();//初始化iic
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off关闭显示
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address 设置显示时的起始列地址低四位
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address	设置显示时的起始列地址高四位
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  设置起始行   [5:0] 行数
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control  设置对比度
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   						对比度 越大越好
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap   设置段的重映射  bit0  0 0->0；  1 0->127    
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse  设置正常/逆向显示    bit0:1反相显示；0，正常显示
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64) 设置多路比例
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction  设置comm输出扫描方式  bit3 0，普通模式  1，重定义模式   COM[N-1] -> COM0;N驱动路数
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset设置显示偏移
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0x20,OLED_CMD);//设置内存地址模式
	OLED_WR_Byte(0x00,OLED_CMD);//[1:0]00列地址模式   01行地址模式  10页地址模式 默认10
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division设置时钟频率/振荡频率
	OLED_WR_Byte(0x80,OLED_CMD);//[3：0]分频因子   [7:4] 振荡频率
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off 设置低功耗显示模式
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period 设置装载之前的时间间隔  欲充电周期  
	OLED_WR_Byte(0xF1,OLED_CMD);//    			[3:0],PHASE 1;    [7:4]PHASE 2;
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion 设置com口引脚硬件配置
	OLED_WR_Byte(0x12,OLED_CMD);//			[5:4] 配置
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh  设置电压倍率
	OLED_WR_Byte(0x30,OLED_CMD);//				[6:4] 000,0.65*vcc;   001,0.77*vcc;  001,0.83*vcc
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable  设置电荷泵的开关状态  bit2 开启/关闭
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel  显示打开
}  
