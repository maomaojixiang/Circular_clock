#include "oled_init.h"
#include "oledfont.h"

/**********************************************
// IIC д����
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
// IIC д����
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
// OLED ��������   
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
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}
/**********************************************
//��������  x:0-127   y:0-7
**********************************************/
void OLED_Set_Pos(u8 x,u8 y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}
/**********************************************
//OLED��ָ���ط�дһ��8*16�ַ�	  ��OLED�ֳ���64��  x:��16��  y;��4��
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
//OLED��ָ���ط�дһ��8*16�ַ�	  ��OLED�ֳ���64��  x:��16��  y;��4��
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
//OLED��ָ���ط�дһ��16*16�ַ�	  ��OLED�ֳ���32��  x:��8��  y;��4��
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
//��ʼ��SSD1306	  
**********************************************/
void OLED_Init(void)
{ 	
	IIC_Init();//��ʼ��iic
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off�ر���ʾ
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address ������ʾʱ����ʼ�е�ַ����λ
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address	������ʾʱ����ʼ�е�ַ����λ
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  ������ʼ��   [5:0] ����
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control  ���öԱȶ�
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   						�Աȶ� Խ��Խ��
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap   ���öε���ӳ��  bit0  0 0->0��  1 0->127    
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse  ��������/������ʾ    bit0:1������ʾ��0��������ʾ
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64) ���ö�·����
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction  ����comm���ɨ�跽ʽ  bit3 0����ͨģʽ  1���ض���ģʽ   COM[N-1] -> COM0;N����·��
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset������ʾƫ��
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0x20,OLED_CMD);//�����ڴ��ַģʽ
	OLED_WR_Byte(0x00,OLED_CMD);//[1:0]00�е�ַģʽ   01�е�ַģʽ  10ҳ��ַģʽ Ĭ��10
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division����ʱ��Ƶ��/��Ƶ��
	OLED_WR_Byte(0x80,OLED_CMD);//[3��0]��Ƶ����   [7:4] ��Ƶ��
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off ���õ͹�����ʾģʽ
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period ����װ��֮ǰ��ʱ����  ���������  
	OLED_WR_Byte(0xF1,OLED_CMD);//    			[3:0],PHASE 1;    [7:4]PHASE 2;
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion ����com������Ӳ������
	OLED_WR_Byte(0x12,OLED_CMD);//			[5:4] ����
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh  ���õ�ѹ����
	OLED_WR_Byte(0x30,OLED_CMD);//				[6:4] 000,0.65*vcc;   001,0.77*vcc;  001,0.83*vcc
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable  ���õ�ɱõĿ���״̬  bit2 ����/�ر�
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel  ��ʾ��
}  
