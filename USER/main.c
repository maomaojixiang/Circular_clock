#include "stm32f10x.h"
#include "delay.h"
#include "iic_init.h"
#include "usart_init.h"
#include "sht30.h"
#include "oled_init.h"
#include "wifi.h"
#include "exti_init.h"
#include "ws2812b_init.h"
#include "rtc.h" 	
#include "timer_init.h"
#include "adc.h"
#include <string.h>


void oled_bj_inlt(void);					//oled������ʼ��
void oled_temp_put(void);					//oled��ʪ��ˢ��
void oled_time_put(u16 nian,u8 yue, u8 ri,u8 xingqi);//oledʱ��ˢ��   ��protocol.c�л�ȡ��ʱ����Զ�����
void get_temp(void);							//��ȡ��ʪ�� 
void get_time(void);							//��ȡʱ��  //����Уʱ ��ʱ���ж���ÿһСʱִ��һ��
void init(void);									//��ʼ������
void ws2812_power(u8 n);					//ws2812����ʾ
u8 wifi_powet(void);							//�ж��Ƿ����ӵ��Ʒ�����
void get_wether(void);						//��ȡ����
void condition_choose(u8 condition[]);//�ж���һ������
u8 wether_cooking(u8 r_x,u8 num,u8 number);//�������

u8 wether_utf8[44][21]=//���������� ��֪�������ĸ�
{
{0xE6,0x99,0xB4},/*��*/{0xE5,0xA4,0x9A,0xE4,0xBA,0x91},/*����*/{0xE9,0x98,0xB4},/*��*/
{0xE5,0xB0,0x91,0xE4,0xBA,0x91},/*����*/{0xE9,0x9C,0xBE},/*��*/{0xE5,0xA4,0x9A,0xE4,0xBA,0x91},/*����*/
{0xE9,0x9B,0xBE},/*��*/{0xE9,0x9B,0xAA},/*ѩ*/{0xE5,0xB0,0x8F,0xE9,0x9B,0xAA},/*Сѩ*/
{0xE5,0xA4,0xA7,0xE9,0x9B,0xA8},/*����*/{0xE9,0x9B,0xA8},/*��*/{0xE5,0xB0,0x8F,0xE9,0x9B,0xA8},/*С��*/
{0xE9,0x98,0xB5,0xE9,0x9B,0xAA,},/*��ѩ*/{0xE9,0x9B,0xB7,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*������*/{0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*����*/
{0xE9,0x9B,0xA8,0xE5,0xA4,0xB9,0xE9,0x9B,0xAA},/*���ѩ*/{0xE5,0x86,0xB0,0xE9,0x9B,0xB9},/*����*/{0xE4,0xB8,0xAD,0xE5,0x88,0xB0,0xE5,0xA4,0xA7,0xE9,0x9B,0xA8},/*�е�����*/
{0xE5,0xA4,0xA7,0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*��ѩ*/{0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*����*/{0xE4,0xB8,0xAD,0xE9,0x9B,0xAA},/*��ѩ*/
{0xE5,0xA4,0xA7,0xE9,0x9B,0xAA},/*��ѩ*/{0xE5,0xBC,0xBA,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*ǿ����*/{0xE6,0xB5,0xAE,0xE5,0xB0,0x98},/*����*/
{0xE5,0xB1,0x80,0xE9,0x83,0xA8,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*�ֲ�����*/{0xE6,0xB2,0x99,0xE5,0xB0,0x98,0xE6,0x9A,0xB4},/*ɳ����*/{0xE5,0xB0,0x8F,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*С����*/
{0xE7,0x89,0xB9,0xE5,0xA4,0xA7,0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*�ش���*/{0xE6,0x9A,0xB4,0xE9,0x9B,0xAA},/*��ѩ*/{0xE9,0x9B,0xB7,0xE7,0x94,0xB5},/*�׵�*/
{0xE6,0x89,0xAC,0xE6,0xB2,0x99},/*��ɳ*/{0xE5,0xB0,0x8F,0xE5,0x88,0xB0,0xE4,0xB8,0xAD,0xE9,0x9B,0xAA},/*С����ѩ*/{0xE5,0xB0,0x8F,0xE9,0x98,0xB5,0xE9,0x9B,0xAA},/*С��ѩ*/
{0xE5,0xA4,0xA7,0xE5,0x88,0xB0,0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*�󵽱���*/{0xE5,0xA4,0xA7,0xE9,0x83,0xA8,0xE6,0x99,0xB4,0xE6,0x9C,0x97},/*������*/{0xE5,0xB0,0x8F,0xE5,0x88,0xB0,0xE4,0xB8,0xAD,0xE9,0x9B,0xA8},/*С������*/
{0xE5,0x86,0xBB,0xE9,0x9B,0xA8},/*����*/{0xE9,0x9B,0xB7,0xE6,0x9A,0xB4},/*�ױ�*/{0xE5,0x86,0xB0,0xE9,0x92,0x88},/*����*/
{0xE5,0xBC,0xBA,0xE6,0xB2,0x99,0xE5,0xB0,0x98,0xE6,0x9A,0xB4},/*ǿɳ����*/{0xE5,0x86,0xBB,0xE9,0x9B,0xBE},/*����*/{0xE5,0xB0,0x98,0xE5,0x8D,0xB7,0xE9,0xA3,0x8E},/*�����*/
{0xE5,0x86,0xB0,0xE7,0xB2,0x92},/*����*/{0xE9,0x9B,0xB7,0xE9,0x98,0xB5,0xE9,0x9B,0xA8,0xE4,0xBC,0xB4,0xE6,0x9C,0x89,0xE5,0x86,0xB0,0xE9,0x9B,0xB9}/*��������б���*/
};


int main(void)
{		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�		
	
	init();//��ʼ������
	while(1)
	{
		wifi_uart_service();		
	}
}

u8 wether_cooking(u8 r_x,u8 num,u8 number)
{
	static u8 cooking_condition=0;
	static u8 cooking_aqi;
	if(r_x==0)
	{
		switch(num)
		{
			case 1:cooking_condition=number;break;
			case 2:cooking_aqi=number;break;
		}
	}
	if(r_x==1)
	{
		switch(num)
		{
			case 1:return cooking_condition;
			case 2:return cooking_aqi;
		}
	}
	return 0;
}
void condition_choose(u8 condition[])
{
	u8 i;
	for(i=0;i<44;i++)
		{
			if(strcmp(condition,wether_utf8[i])==0) 
			{
				wether_cooking(0,1,i+1);
				i=45;
			}			
		}
}
void get_wether()
{
	u8 aqi;
	u8 condition;
	
	aqi=wether_cooking(1,2,1);
	condition=wether_cooking(1,1,1);
	
	OLED_WR_char_0816(16,1,' ');
	OLED_WR_char_0816(24,1,' ');
	OLED_WR_char_0816(32,1,' ');//���aqiλ������
	if(aqi<100)
	{
		OLED_WR_char_0816(16,1,(aqi%100/10)+32+16);
		OLED_WR_char_0816(24,1,(aqi%10)+32+16);
	}
	else 
	{
		OLED_WR_char_0816(16,1,(aqi/100)+32+16);
		OLED_WR_char_0816(24,1,(aqi%100/10)+32+16);
		OLED_WR_char_0816(32,1,(aqi%10)+32+16);
	}
	
	OLED_WR_char_1616(80,2,7);
	OLED_WR_char_1616(96,2,7);
	OLED_WR_char_1616(112,2,7);
	OLED_WR_char_1616(80,3,7);
	OLED_WR_char_1616(96,3,7);
	OLED_WR_char_1616(112,3,7);//���conditionλ������
	switch(condition)
	{
		case 0:OLED_WR_char_1616(96,3,36);OLED_WR_char_1616(112,3,37);break;//δ֪
		case 1:OLED_WR_char_1616(112,3,38);break;//��
		case 2:OLED_WR_char_1616(96,3,39);OLED_WR_char_1616(112,3,40);break;//����
		case 3:OLED_WR_char_1616(112,3,41);break;//��
		case 4:OLED_WR_char_1616(96,3,43);OLED_WR_char_1616(112,3,40);break;//����
		case 5:OLED_WR_char_1616(112,3,43);break;//��
		case 6:OLED_WR_char_1616(96,3,39);OLED_WR_char_1616(112,3,40);break;//����
		case 7:OLED_WR_char_1616(112,3,44);break;//��
		case 8:OLED_WR_char_1616(112,3,45);break;//ѩ
		case 9:OLED_WR_char_1616(96,3,46);OLED_WR_char_1616(112,3,45);break;//Сѩ
		case 10:OLED_WR_char_1616(96,3,55);OLED_WR_char_1616(112,3,47);break;//����
		case 11:OLED_WR_char_1616(112,3,47);break;//��
		case 12:OLED_WR_char_1616(96,3,46);OLED_WR_char_1616(112,3,47);break;//С��
		case 13:OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//����
		case 14:OLED_WR_char_1616(80,3,49);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//������
		case 15:OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,47);break;//����
		case 16:OLED_WR_char_1616(80,3,47);OLED_WR_char_1616(96,3,51);OLED_WR_char_1616(112,3,45);break;//���ѩ
		case 17:OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,53);break;//����
		case 18:OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,45);break;//��ѩ
		case 19:OLED_WR_char_1616(80,3,55);OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,45);break;//��ѩ
		case 20:OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//����
		case 21:OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,45);break;//��ѩ
		case 22:OLED_WR_char_1616(96,3,55);OLED_WR_char_1616(112,3,45);break;//��ѩ
		case 23:OLED_WR_char_1616(80,3,56);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//ǿ����
		case 24:OLED_WR_char_1616(96,3,47);OLED_WR_char_1616(112,3,58);break;//����
		case 25:OLED_WR_char_1616(96,2,59);OLED_WR_char_1616(112,2,60);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//�ֲ�����
		case 26:OLED_WR_char_1616(80,3,61);OLED_WR_char_1616(96,3,58);OLED_WR_char_1616(112,3,50);break;//ɳ����
		case 27:OLED_WR_char_1616(96,3,46);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//С����
		case 28:OLED_WR_char_1616(96,2,62);OLED_WR_char_1616(112,2,55);OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,47);break;//�ش���
		case 29:OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,45);break;//��ѩ
		case 30:OLED_WR_char_1616(96,3,49);OLED_WR_char_1616(112,3,64);break;//�׵�
		case 31:OLED_WR_char_1616(96,3,65);OLED_WR_char_1616(112,3,61);break;//��ɳ
		case 32:OLED_WR_char_1616(96,2,46);OLED_WR_char_1616(112,2,66);OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,45);break;//С����ѩ
		case 33:OLED_WR_char_1616(80,3,46);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//С����
		case 34:OLED_WR_char_1616(96,2,55);OLED_WR_char_1616(112,2,66);OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,47);break;//�󵽱���
		case 35:OLED_WR_char_1616(96,2,55);OLED_WR_char_1616(112,2,60);OLED_WR_char_1616(96,3,38);OLED_WR_char_1616(112,3,67);break;//������
		case 36:OLED_WR_char_1616(96,2,46);OLED_WR_char_1616(112,2,66);OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,47);break;//С������
		case 37:OLED_WR_char_1616(96,3,68);OLED_WR_char_1616(112,3,47);break;//����
		case 38:OLED_WR_char_1616(96,3,63);OLED_WR_char_1616(112,3,50);break;//�ױ�
		case 39:OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,69);break;//����
		case 40:OLED_WR_char_1616(96,2,56);OLED_WR_char_1616(112,2,61);OLED_WR_char_1616(96,3,58);OLED_WR_char_1616(112,3,50);break;//ǿɳ����
		case 41:OLED_WR_char_1616(96,3,68);OLED_WR_char_1616(112,3,44);break;//����
		case 42:OLED_WR_char_1616(80,3,58);OLED_WR_char_1616(96,3,70);OLED_WR_char_1616(112,3,74);break;//�����
		case 43:OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,71);break;//����
		case 44:OLED_WR_char_1616(80,2,49);OLED_WR_char_1616(96,2,48);OLED_WR_char_1616(112,2,47);OLED_WR_char_1616(80,3,72);OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,53);break;//��������б���
	}
	printf("cooking_condition=%d\r\n",condition);
}

//SMART_CONFIG_STATE: smartconfig����״̬
//AP_STATE: AP����״̬
//WIFI_NOT_CONNECTED: WIFI���óɹ���δ����·����
//WIFI_CONNECTED: WIFI���óɹ�������·����
//WIFI_CONN_CLOUD: WIFI�Ѿ��������Ʒ�����
//WIFI_LOW_POWER: WIFI���ڵ͹���ģʽ
//SMART_AND_AP_STATE: WIFI smartconfig&AP ģʽ
u8 wifi_powet()
{
	static u8 p=0;
	u8 wifi_zt;//wifi״̬
	static u8 wifi_debug=0;//wifi�����������	
	if(wifi_debug==0||wifi_debug==11)//�ս�������������ʱ����ÿһ���ȡһ������״̬
	{
		wifi_zt=mcu_get_wifi_work_state();
	}
	if(p==0)
	{
		if(wifi_zt==WIFI_CONN_CLOUD) return 1;
		else 
		{
			p++;
			return 0;
		}
	}
	else if(p<100)
	{
		if(wifi_zt==WIFI_CONN_CLOUD) 
		{
			p=0;	
			return 1;
		}					
		else 
		{
			p++;
			return 0;
		}
	}
	else if(p==100)
	{
		if(wifi_debug==0)
		{
			OLED_Clear();
			OLED_WR_char_1616(8,0,35);	//��
			OLED_WR_char_1616(24,0,25);	//��
			OLED_WR_char_1616(40,0,26);	//��
			OLED_WR_char_1616(56,0,27);	//��
			OLED_WR_char_1616(72,0,28);	//��
			OLED_WR_char_1616(88,0,29);	//��
			OLED_WR_char_1616(104,0,30);//��
			
			OLED_WR_char_1616(0,2,31);	//��
			OLED_WR_char_1616(16,2,32);	//ǰ
			OLED_WR_char_1616(32,2,35);	//��
			OLED_WR_char_1616(48,2,25);	//��
			OLED_WR_char_1616(64,2,33);	//״
			OLED_WR_char_1616(80,2,34);	//̬
			OLED_WR_char_0816(96,2,':');//:
			REG_LED_ALL_BLACK();
			wifi_zt=mcu_get_wifi_work_state();
			OLED_WR_char_0816(104,2,wifi_zt+32+16);
			wifi_debug++;
			return 0;
		}
		else
		{
			if(wifi_debug==11)
			{
				wifi_debug=1;
				OLED_WR_char_0816(104,2,' ');
				OLED_WR_char_0816(104,2,wifi_zt+32+16);
				if(wifi_zt==WIFI_CONN_CLOUD)//��������
				{
					p=0;
					wifi_debug=0;
					oled_bj_inlt();
					oled_time_put(calendar.w_year,calendar.w_month,calendar.w_date,calendar.week);
					return 1;
				}
			}
			else
			{
				wifi_debug++;
				return 0;
			}
		}
	}
	return 0;
}

void oled_bj_inlt(void)
{
	OLED_Clear();
	OLED_WR_char_1616(32,0,8);	//��
	OLED_WR_char_1616(64,0,9);	//��
	OLED_WR_char_1616(96,0,10);	//��
	OLED_WR_char_1616(0,1,13);	//��
	OLED_WR_char_1616(80,1,11);	//��
	OLED_WR_char_1616(96,1,12);	//��
	OLED_WR_char_1616(0,2,1);	//��
	OLED_WR_char_1616(16,2,2);	//��
	OLED_WR_char_0816(48,2,'.');	//.
	OLED_WR_char_1616(64,2,0);	//��
	OLED_WR_char_1616(0,3,3);	//ʪ
	OLED_WR_char_1616(16,3,2);	//��
	OLED_WR_char_0816(48,3,'.');	//.
	OLED_WR_char_0816(64,3,'%');	//%

}	

void oled_temp_put(void)
{
	OLED_WR_char_0816(32,2,(SHT30_temperature/100)+32+16);
	OLED_WR_char_0816(40,2,(SHT30_temperature%100/10)+32+16);
	OLED_WR_char_0816(56,2,(SHT30_temperature%10)+32+16);//��ʾ�¶�

	OLED_WR_char_0816(32,3,(SHT30_humidity/100)+32+16);
	OLED_WR_char_0816(40,3,(SHT30_humidity%100/10)+32+16);
	OLED_WR_char_0816(56,3,(SHT30_humidity%10)+32+16);//��ʾʪ��
}

u8 oled_status1(u8 time)
{
	static u8 t=0;//��¼��ĸ���
	OLED_WR_char_1616(0,1,14);	//��
	OLED_WR_char_1616(16,1,15);	//��
	OLED_WR_char_1616(32,1,16);	//��
	OLED_WR_char_1616(48,1,17);	//ʼ
	OLED_WR_char_1616(64,1,18);	//��
	if(time%5==0)
	{
		OLED_WR_char_0816(80+(t*2),1,95);
		t++;
		if(t==5)
		{
			t=0;
			OLED_WR_char_0816(80,1,' ');
			OLED_WR_char_0816(88,1,' ');
			OLED_WR_char_0816(96,1,' ');
		}
	}
	if(mcu_get_wifi_work_state()==4)
	{
		return 1;
	}
	return 0;
}

u8 wifi_get_network()//����
{
	static u8 o=1;//�Ƿ�������״̬

	if(mcu_get_wifi_work_state()==4)
	{
		return 1;
	}
	else 
	{
		if(o==1)
		{
			mcu_reset_wifi();
			printf("wifi�������\r\n");
			delay_ms(1000);
			mcu_set_wifi_mode(SMART_CONFIG);
			printf("wifiģʽ���óɹ�\r\n");
			delay_ms(1000);
			
			OLED_Clear();
			OLED_WR_char_1616(32,1,4);	//��
			OLED_WR_char_1616(48,1,5);	//��
			OLED_WR_char_1616(64,1,6);	//��
			o=0;
		}
		else
		{
			OLED_WR_char_1616(0,3,31);	//��
			OLED_WR_char_1616(16,3,32);	//ǰ
			OLED_WR_char_1616(32,3,35);	//��
			OLED_WR_char_1616(48,3,25);	//��
			OLED_WR_char_1616(64,3,33);	//״
			OLED_WR_char_1616(80,3,34);	//̬
			OLED_WR_char_0816(96,3,':');//:
			OLED_WR_char_0816(104,3,mcu_get_wifi_work_state()+32+16);
		}		
	}
	return 0;
}

void get_temp(void)
{
	u8 zhuangtai;
	zhuangtai=sht30_data_process();
	if(zhuangtai==0)
	{		
		oled_temp_put();//oled����ʪ�Ƚ�����ʾ
		mcu_dp_value_update(DPID_HUMIDITY_VALUE,SHT30_humidity/10); //��������ϱ�ʪ��
		mcu_dp_value_update(DPID_TEMP_CURRENT,SHT30_temperature); //��������ϱ��¶�
	}
	else if(zhuangtai==1) 
	{
		printf("�¶Ȼ�ȡʧ��\r\n");
		SHT_Init();
	}
	else if(zhuangtai==2)
	{
		printf("ʪ�Ȼ�ȡʧ��\r\n");
		SHT_Init();
	}
	else
	{
		printf("��ȡ����ʧ��\r\n");
		SHT_Init();
	}
	//����ʹ�õ����iic   iic�����ױ�Ϳѻģ�������Ӱ�쵽  ���Լ���ʧ�ܺ��sht30�����ٴγ�ʼ��
}

void get_time(void)
{
	mcu_get_system_time();	
}
void ws2812_power(u8 n)
{
	u16 liangdu;
	liangdu=Get_Adc(0);//��ù������赽��adcֵ
	
	if(liangdu<50) liangdu=0;
	else if(liangdu<500)	liangdu=1;
	else if(liangdu<800)	liangdu=2;
	else if(liangdu<1000)	liangdu=4;
	else if(liangdu<1500)	liangdu=6;
	else if(liangdu<2000)	liangdu=9;
	else if(liangdu<3000)	liangdu=13;
	else liangdu=18;									//����adcֵ�趨��Ӧ������
	
	if(n==0)
	{
		REG_LED_colour_write(calendar.hour,calendar.min,liangdu);
	}
	if(n==1)
	{
		REG_LED_colour_write(calendar.hour,calendar.min-1,liangdu);
	}
	//ws2812��ʱ�������ʾ  n=0��n=1ʵ�������˸��ʾ
}

void oled_time_put(u16 nian,u8 yue, u8 ri,u8 xingqi)
{
	OLED_WR_char_0816(0,0,(nian/1000)+32+16);
	OLED_WR_char_0816(8,0,(nian%1000/100)+32+16);
	OLED_WR_char_0816(16,0,(nian%100/10)+32+16);
	OLED_WR_char_0816(24,0,(nian%10)+32+16);//��ʾ��
	
	OLED_WR_char_0816(48,0,(yue%100/10)+32+16);
	OLED_WR_char_0816(56,0,(yue%10)+32+16);//��ʾ��
	
	OLED_WR_char_0816(80,0,(ri%100/10)+32+16);
	OLED_WR_char_0816(88,0,(ri%10)+32+16);//��ʾ��
	
	OLED_WR_char_1616(112,1,xingqi+18);//��ʾ����

}
void init(void)
{
	delay_init();					//��ʼ����ʱ��
	IIC_Init();						//��ʼ�����IIC
	OLED_Init();					//��ʼ��OLED
	OLED_Clear();					//���oled
	SHT_Init();						//��ʼ��SHT30
	wifi_protocol_init();	//��ʼ��Ϳѻģ��
	REG_LED_Init();				//��ʼ��ws2812
	REG_LED_ALL_BLACK();	//���ws2812
	RTC_Init();						//��ʼ��rtcʱ��
	Adc_Init();						//��ʼ��adc
	
	USART1_init();						//��ʼ������1
	EXTI_init();							//��ʼ���ⲿ�ж�
	TIM3_Int_Init(119,59999);	//��ʼ����ʱ���ж�	
	USART2_init();						//��ʼ������2
	
	printf("��ʼ�����\r\n");
	
}
