#include "timer_init.h"
#include "gpio_init.h" 	 
#include "usart_init.h"

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!


void oled_bj_inlt(void);
u8 oled_status1(u8 time);
u8 wifi_get_network(void);
void get_temp(void);
void get_time(void);
void ws2812_power(u8 n);
u8 wifi_powet(void);
void get_wether(void);
/////////////////////////////////////以上函数在main.c中

/////////////////////////////////////////////////////////////////////////////////
//函数名：TIM3_Int_Init
//函数功能：定时器中断初始化函数
//输入值：arr psc 可以通过自己写的易语言写的那个计算
//输出值：无
/////////////////////////////////////////////////////////////////////////////////
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx	

}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	static u32 time=0;
	static u8 ws2812_05s=0;
	static u8 yunxing_bz=0;
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			if(time<100)//初始化状态 判断联网状态
			{
				if(oled_status1(time)==1) time=100;
				if(oled_status1(time)==0)	time++;

			}
			if(time==100)//配网阶段  配网完毕自动跳出
			{	
				if(wifi_get_network()==1)
				{	
					delay_ms(200);
					oled_bj_inlt();//打印oled背景
					delay_ms(200);
					delay_ms(200);
					delay_ms(200);
					delay_ms(200);
					delay_ms(200);
					delay_ms(200);
					get_time();
					delay_ms(200);
					get_wether();
					time++;
				}
			}
			if(time>100)//正常状态 
			{				
				if(time%10==0&&yunxing_bz==0)  //每一秒获取一次温湿度  oled显示并上传服务器
				{
					get_temp();
					yunxing_bz=0;
				}				
				if(time%5==0&&yunxing_bz==0)//每半秒点一下ws2812
				{
					ws2812_power(ws2812_05s);
					ws2812_05s=!ws2812_05s;
					yunxing_bz=0;
				}
				if(time==6000&&yunxing_bz==0)//每小时校准一下本地时间
				{
					get_time();
					time=101;
				}
				if(time%100==0&&yunxing_bz==0)//每十秒刷新一次天气情况
				{
					get_wether();
				}
				if(time%100==0)//每十秒检验一下无线状态
				{
					if(wifi_powet()==1) yunxing_bz=0;
					else yunxing_bz=1;
				}
				if(yunxing_bz==0)	time++;
			}
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		}
}
