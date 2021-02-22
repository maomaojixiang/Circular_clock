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


void oled_bj_inlt(void);					//oled背景初始化
void oled_temp_put(void);					//oled温湿度刷新
void oled_time_put(u16 nian,u8 yue, u8 ri,u8 xingqi);//oled时间刷新   在protocol.c中获取到时间后自动调用
void get_temp(void);							//获取温湿度 
void get_time(void);							//获取时间  //本地校时 定时器中断中每一小时执行一次
void init(void);									//初始化程序
void ws2812_power(u8 n);					//ws2812的显示
u8 wifi_powet(void);							//判断是否连接到云服务器
void get_wether(void);						//获取天气
void condition_choose(u8 condition[]);//判断哪一个天气
u8 wether_cooking(u8 r_x,u8 num,u8 number);//存放天气

u8 wether_utf8[44][21]=//有两个多云 不知道少了哪个
{
{0xE6,0x99,0xB4},/*晴*/{0xE5,0xA4,0x9A,0xE4,0xBA,0x91},/*多云*/{0xE9,0x98,0xB4},/*阴*/
{0xE5,0xB0,0x91,0xE4,0xBA,0x91},/*少云*/{0xE9,0x9C,0xBE},/*霾*/{0xE5,0xA4,0x9A,0xE4,0xBA,0x91},/*多云*/
{0xE9,0x9B,0xBE},/*雾*/{0xE9,0x9B,0xAA},/*雪*/{0xE5,0xB0,0x8F,0xE9,0x9B,0xAA},/*小雪*/
{0xE5,0xA4,0xA7,0xE9,0x9B,0xA8},/*大雨*/{0xE9,0x9B,0xA8},/*雨*/{0xE5,0xB0,0x8F,0xE9,0x9B,0xA8},/*小雨*/
{0xE9,0x98,0xB5,0xE9,0x9B,0xAA,},/*阵雪*/{0xE9,0x9B,0xB7,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*雷阵雨*/{0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*暴雨*/
{0xE9,0x9B,0xA8,0xE5,0xA4,0xB9,0xE9,0x9B,0xAA},/*雨夹雪*/{0xE5,0x86,0xB0,0xE9,0x9B,0xB9},/*冰雹*/{0xE4,0xB8,0xAD,0xE5,0x88,0xB0,0xE5,0xA4,0xA7,0xE9,0x9B,0xA8},/*中到大雨*/
{0xE5,0xA4,0xA7,0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*大暴雪*/{0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*阵雨*/{0xE4,0xB8,0xAD,0xE9,0x9B,0xAA},/*中雪*/
{0xE5,0xA4,0xA7,0xE9,0x9B,0xAA},/*大雪*/{0xE5,0xBC,0xBA,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*强阵雨*/{0xE6,0xB5,0xAE,0xE5,0xB0,0x98},/*浮尘*/
{0xE5,0xB1,0x80,0xE9,0x83,0xA8,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*局部阵雨*/{0xE6,0xB2,0x99,0xE5,0xB0,0x98,0xE6,0x9A,0xB4},/*沙尘暴*/{0xE5,0xB0,0x8F,0xE9,0x98,0xB5,0xE9,0x9B,0xA8},/*小阵雨*/
{0xE7,0x89,0xB9,0xE5,0xA4,0xA7,0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*特大暴雨*/{0xE6,0x9A,0xB4,0xE9,0x9B,0xAA},/*暴雪*/{0xE9,0x9B,0xB7,0xE7,0x94,0xB5},/*雷电*/
{0xE6,0x89,0xAC,0xE6,0xB2,0x99},/*扬沙*/{0xE5,0xB0,0x8F,0xE5,0x88,0xB0,0xE4,0xB8,0xAD,0xE9,0x9B,0xAA},/*小到中雪*/{0xE5,0xB0,0x8F,0xE9,0x98,0xB5,0xE9,0x9B,0xAA},/*小阵雪*/
{0xE5,0xA4,0xA7,0xE5,0x88,0xB0,0xE6,0x9A,0xB4,0xE9,0x9B,0xA8},/*大到暴雨*/{0xE5,0xA4,0xA7,0xE9,0x83,0xA8,0xE6,0x99,0xB4,0xE6,0x9C,0x97},/*大部晴朗*/{0xE5,0xB0,0x8F,0xE5,0x88,0xB0,0xE4,0xB8,0xAD,0xE9,0x9B,0xA8},/*小到中雨*/
{0xE5,0x86,0xBB,0xE9,0x9B,0xA8},/*冻雨*/{0xE9,0x9B,0xB7,0xE6,0x9A,0xB4},/*雷暴*/{0xE5,0x86,0xB0,0xE9,0x92,0x88},/*冰针*/
{0xE5,0xBC,0xBA,0xE6,0xB2,0x99,0xE5,0xB0,0x98,0xE6,0x9A,0xB4},/*强沙尘暴*/{0xE5,0x86,0xBB,0xE9,0x9B,0xBE},/*冻雾*/{0xE5,0xB0,0x98,0xE5,0x8D,0xB7,0xE9,0xA3,0x8E},/*尘卷风*/
{0xE5,0x86,0xB0,0xE7,0xB2,0x92},/*冰粒*/{0xE9,0x9B,0xB7,0xE9,0x98,0xB5,0xE9,0x9B,0xA8,0xE4,0xBC,0xB4,0xE6,0x9C,0x89,0xE5,0x86,0xB0,0xE9,0x9B,0xB9}/*雷阵雨伴有冰雹*/
};


int main(void)
{		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级		
	
	init();//初始化程序
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
	OLED_WR_char_0816(32,1,' ');//清空aqi位置数据
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
	OLED_WR_char_1616(112,3,7);//清空condition位置数据
	switch(condition)
	{
		case 0:OLED_WR_char_1616(96,3,36);OLED_WR_char_1616(112,3,37);break;//未知
		case 1:OLED_WR_char_1616(112,3,38);break;//晴
		case 2:OLED_WR_char_1616(96,3,39);OLED_WR_char_1616(112,3,40);break;//多云
		case 3:OLED_WR_char_1616(112,3,41);break;//阴
		case 4:OLED_WR_char_1616(96,3,43);OLED_WR_char_1616(112,3,40);break;//少云
		case 5:OLED_WR_char_1616(112,3,43);break;//霾
		case 6:OLED_WR_char_1616(96,3,39);OLED_WR_char_1616(112,3,40);break;//多云
		case 7:OLED_WR_char_1616(112,3,44);break;//雾
		case 8:OLED_WR_char_1616(112,3,45);break;//雪
		case 9:OLED_WR_char_1616(96,3,46);OLED_WR_char_1616(112,3,45);break;//小雪
		case 10:OLED_WR_char_1616(96,3,55);OLED_WR_char_1616(112,3,47);break;//大雨
		case 11:OLED_WR_char_1616(112,3,47);break;//雨
		case 12:OLED_WR_char_1616(96,3,46);OLED_WR_char_1616(112,3,47);break;//小雨
		case 13:OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//阵雨
		case 14:OLED_WR_char_1616(80,3,49);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//雷阵雨
		case 15:OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,47);break;//暴雨
		case 16:OLED_WR_char_1616(80,3,47);OLED_WR_char_1616(96,3,51);OLED_WR_char_1616(112,3,45);break;//雨夹雪
		case 17:OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,53);break;//冰雹
		case 18:OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,45);break;//中雪
		case 19:OLED_WR_char_1616(80,3,55);OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,45);break;//大暴雪
		case 20:OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//阵雨
		case 21:OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,45);break;//中雪
		case 22:OLED_WR_char_1616(96,3,55);OLED_WR_char_1616(112,3,45);break;//大雪
		case 23:OLED_WR_char_1616(80,3,56);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//强阵雨
		case 24:OLED_WR_char_1616(96,3,47);OLED_WR_char_1616(112,3,58);break;//浮尘
		case 25:OLED_WR_char_1616(96,2,59);OLED_WR_char_1616(112,2,60);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//局部阵雨
		case 26:OLED_WR_char_1616(80,3,61);OLED_WR_char_1616(96,3,58);OLED_WR_char_1616(112,3,50);break;//沙尘暴
		case 27:OLED_WR_char_1616(96,3,46);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//小阵雨
		case 28:OLED_WR_char_1616(96,2,62);OLED_WR_char_1616(112,2,55);OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,47);break;//特大暴雨
		case 29:OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,45);break;//暴雪
		case 30:OLED_WR_char_1616(96,3,49);OLED_WR_char_1616(112,3,64);break;//雷电
		case 31:OLED_WR_char_1616(96,3,65);OLED_WR_char_1616(112,3,61);break;//扬沙
		case 32:OLED_WR_char_1616(96,2,46);OLED_WR_char_1616(112,2,66);OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,45);break;//小到中雪
		case 33:OLED_WR_char_1616(80,3,46);OLED_WR_char_1616(96,3,48);OLED_WR_char_1616(112,3,47);break;//小阵雨
		case 34:OLED_WR_char_1616(96,2,55);OLED_WR_char_1616(112,2,66);OLED_WR_char_1616(96,3,50);OLED_WR_char_1616(112,3,47);break;//大到暴雨
		case 35:OLED_WR_char_1616(96,2,55);OLED_WR_char_1616(112,2,60);OLED_WR_char_1616(96,3,38);OLED_WR_char_1616(112,3,67);break;//大部晴朗
		case 36:OLED_WR_char_1616(96,2,46);OLED_WR_char_1616(112,2,66);OLED_WR_char_1616(96,3,54);OLED_WR_char_1616(112,3,47);break;//小到中雨
		case 37:OLED_WR_char_1616(96,3,68);OLED_WR_char_1616(112,3,47);break;//冻雨
		case 38:OLED_WR_char_1616(96,3,63);OLED_WR_char_1616(112,3,50);break;//雷暴
		case 39:OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,69);break;//冰针
		case 40:OLED_WR_char_1616(96,2,56);OLED_WR_char_1616(112,2,61);OLED_WR_char_1616(96,3,58);OLED_WR_char_1616(112,3,50);break;//强沙尘暴
		case 41:OLED_WR_char_1616(96,3,68);OLED_WR_char_1616(112,3,44);break;//冻雾
		case 42:OLED_WR_char_1616(80,3,58);OLED_WR_char_1616(96,3,70);OLED_WR_char_1616(112,3,74);break;//尘卷风
		case 43:OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,71);break;//冰粒
		case 44:OLED_WR_char_1616(80,2,49);OLED_WR_char_1616(96,2,48);OLED_WR_char_1616(112,2,47);OLED_WR_char_1616(80,3,72);OLED_WR_char_1616(96,3,52);OLED_WR_char_1616(112,3,53);break;//雷阵雨伴有冰雹
	}
	printf("cooking_condition=%d\r\n",condition);
}

//SMART_CONFIG_STATE: smartconfig配置状态
//AP_STATE: AP配置状态
//WIFI_NOT_CONNECTED: WIFI配置成功但未连上路由器
//WIFI_CONNECTED: WIFI配置成功且连上路由器
//WIFI_CONN_CLOUD: WIFI已经连接上云服务器
//WIFI_LOW_POWER: WIFI处于低功耗模式
//SMART_AND_AP_STATE: WIFI smartconfig&AP 模式
u8 wifi_powet()
{
	static u8 p=0;
	u8 wifi_zt;//wifi状态
	static u8 wifi_debug=0;//wifi解决网络问题	
	if(wifi_debug==0||wifi_debug==11)//刚进入解决网络问题时或者每一秒获取一下网络状态
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
			OLED_WR_char_1616(8,0,35);	//网
			OLED_WR_char_1616(24,0,25);	//络
			OLED_WR_char_1616(40,0,26);	//错
			OLED_WR_char_1616(56,0,27);	//误
			OLED_WR_char_1616(72,0,28);	//请
			OLED_WR_char_1616(88,0,29);	//检
			OLED_WR_char_1616(104,0,30);//查
			
			OLED_WR_char_1616(0,2,31);	//当
			OLED_WR_char_1616(16,2,32);	//前
			OLED_WR_char_1616(32,2,35);	//网
			OLED_WR_char_1616(48,2,25);	//络
			OLED_WR_char_1616(64,2,33);	//状
			OLED_WR_char_1616(80,2,34);	//态
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
				if(wifi_zt==WIFI_CONN_CLOUD)//连上网了
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
	OLED_WR_char_1616(32,0,8);	//年
	OLED_WR_char_1616(64,0,9);	//月
	OLED_WR_char_1616(96,0,10);	//日
	OLED_WR_char_1616(0,1,13);	//星
	OLED_WR_char_1616(80,1,11);	//星
	OLED_WR_char_1616(96,1,12);	//期
	OLED_WR_char_1616(0,2,1);	//温
	OLED_WR_char_1616(16,2,2);	//度
	OLED_WR_char_0816(48,2,'.');	//.
	OLED_WR_char_1616(64,2,0);	//℃
	OLED_WR_char_1616(0,3,3);	//湿
	OLED_WR_char_1616(16,3,2);	//度
	OLED_WR_char_0816(48,3,'.');	//.
	OLED_WR_char_0816(64,3,'%');	//%

}	

void oled_temp_put(void)
{
	OLED_WR_char_0816(32,2,(SHT30_temperature/100)+32+16);
	OLED_WR_char_0816(40,2,(SHT30_temperature%100/10)+32+16);
	OLED_WR_char_0816(56,2,(SHT30_temperature%10)+32+16);//显示温度

	OLED_WR_char_0816(32,3,(SHT30_humidity/100)+32+16);
	OLED_WR_char_0816(40,3,(SHT30_humidity%100/10)+32+16);
	OLED_WR_char_0816(56,3,(SHT30_humidity%10)+32+16);//显示湿度
}

u8 oled_status1(u8 time)
{
	static u8 t=0;//记录点的个数
	OLED_WR_char_1616(0,1,14);	//正
	OLED_WR_char_1616(16,1,15);	//在
	OLED_WR_char_1616(32,1,16);	//初
	OLED_WR_char_1616(48,1,17);	//始
	OLED_WR_char_1616(64,1,18);	//化
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

u8 wifi_get_network()//配网
{
	static u8 o=1;//是否在配网状态

	if(mcu_get_wifi_work_state()==4)
	{
		return 1;
	}
	else 
	{
		if(o==1)
		{
			mcu_reset_wifi();
			printf("wifi重置完成\r\n");
			delay_ms(1000);
			mcu_set_wifi_mode(SMART_CONFIG);
			printf("wifi模式设置成功\r\n");
			delay_ms(1000);
			
			OLED_Clear();
			OLED_WR_char_1616(32,1,4);	//请
			OLED_WR_char_1616(48,1,5);	//配
			OLED_WR_char_1616(64,1,6);	//网
			o=0;
		}
		else
		{
			OLED_WR_char_1616(0,3,31);	//当
			OLED_WR_char_1616(16,3,32);	//前
			OLED_WR_char_1616(32,3,35);	//网
			OLED_WR_char_1616(48,3,25);	//络
			OLED_WR_char_1616(64,3,33);	//状
			OLED_WR_char_1616(80,3,34);	//态
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
		oled_temp_put();//oled对温湿度进行显示
		mcu_dp_value_update(DPID_HUMIDITY_VALUE,SHT30_humidity/10); //向服务器上报湿度
		mcu_dp_value_update(DPID_TEMP_CURRENT,SHT30_temperature); //向服务器上报温度
	}
	else if(zhuangtai==1) 
	{
		printf("温度获取失败\r\n");
		SHT_Init();
	}
	else if(zhuangtai==2)
	{
		printf("湿度获取失败\r\n");
		SHT_Init();
	}
	else
	{
		printf("读取数据失败\r\n");
		SHT_Init();
	}
	//由于使用的软件iic   iic很容易被涂鸦模块的心跳影响到  所以检验失败后对sht30进行再次初始化
}

void get_time(void)
{
	mcu_get_system_time();	
}
void ws2812_power(u8 n)
{
	u16 liangdu;
	liangdu=Get_Adc(0);//获得光敏电阻到的adc值
	
	if(liangdu<50) liangdu=0;
	else if(liangdu<500)	liangdu=1;
	else if(liangdu<800)	liangdu=2;
	else if(liangdu<1000)	liangdu=4;
	else if(liangdu<1500)	liangdu=6;
	else if(liangdu<2000)	liangdu=9;
	else if(liangdu<3000)	liangdu=13;
	else liangdu=18;									//根据adc值设定相应的亮度
	
	if(n==0)
	{
		REG_LED_colour_write(calendar.hour,calendar.min,liangdu);
	}
	if(n==1)
	{
		REG_LED_colour_write(calendar.hour,calendar.min-1,liangdu);
	}
	//ws2812对时分秒的显示  n=0或n=1实现秒的闪烁显示
}

void oled_time_put(u16 nian,u8 yue, u8 ri,u8 xingqi)
{
	OLED_WR_char_0816(0,0,(nian/1000)+32+16);
	OLED_WR_char_0816(8,0,(nian%1000/100)+32+16);
	OLED_WR_char_0816(16,0,(nian%100/10)+32+16);
	OLED_WR_char_0816(24,0,(nian%10)+32+16);//显示年
	
	OLED_WR_char_0816(48,0,(yue%100/10)+32+16);
	OLED_WR_char_0816(56,0,(yue%10)+32+16);//显示月
	
	OLED_WR_char_0816(80,0,(ri%100/10)+32+16);
	OLED_WR_char_0816(88,0,(ri%10)+32+16);//显示秒
	
	OLED_WR_char_1616(112,1,xingqi+18);//显示星期

}
void init(void)
{
	delay_init();					//初始化定时器
	IIC_Init();						//初始化软件IIC
	OLED_Init();					//初始化OLED
	OLED_Clear();					//清空oled
	SHT_Init();						//初始化SHT30
	wifi_protocol_init();	//初始化涂鸦模块
	REG_LED_Init();				//初始化ws2812
	REG_LED_ALL_BLACK();	//清空ws2812
	RTC_Init();						//初始化rtc时钟
	Adc_Init();						//初始化adc
	
	USART1_init();						//初始化串口1
	EXTI_init();							//初始化外部中断
	TIM3_Int_Init(119,59999);	//初始化定时器中断	
	USART2_init();						//初始化串口2
	
	printf("初始化完成\r\n");
	
}
