#include "timer_init.h"
#include "gpio_init.h" 	 
#include "usart_init.h"

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!


void oled_bj_inlt(void);
u8 oled_status1(u8 time);
u8 wifi_get_network(void);
void get_temp(void);
void get_time(void);
void ws2812_power(u8 n);
u8 wifi_powet(void);
void get_wether(void);
/////////////////////////////////////���Ϻ�����main.c��

/////////////////////////////////////////////////////////////////////////////////
//��������TIM3_Int_Init
//�������ܣ���ʱ���жϳ�ʼ������
//����ֵ��arr psc ����ͨ���Լ�д��������д���Ǹ�����
//���ֵ����
/////////////////////////////////////////////////////////////////////////////////
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	

}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	static u32 time=0;
	static u8 ws2812_05s=0;
	static u8 yunxing_bz=0;
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			if(time<100)//��ʼ��״̬ �ж�����״̬
			{
				if(oled_status1(time)==1) time=100;
				if(oled_status1(time)==0)	time++;

			}
			if(time==100)//�����׶�  ��������Զ�����
			{	
				if(wifi_get_network()==1)
				{	
					delay_ms(200);
					oled_bj_inlt();//��ӡoled����
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
			if(time>100)//����״̬ 
			{				
				if(time%10==0&&yunxing_bz==0)  //ÿһ���ȡһ����ʪ��  oled��ʾ���ϴ�������
				{
					get_temp();
					yunxing_bz=0;
				}				
				if(time%5==0&&yunxing_bz==0)//ÿ�����һ��ws2812
				{
					ws2812_power(ws2812_05s);
					ws2812_05s=!ws2812_05s;
					yunxing_bz=0;
				}
				if(time==6000&&yunxing_bz==0)//ÿСʱУ׼һ�±���ʱ��
				{
					get_time();
					time=101;
				}
				if(time%100==0&&yunxing_bz==0)//ÿʮ��ˢ��һ���������
				{
					get_wether();
				}
				if(time%100==0)//ÿʮ�����һ������״̬
				{
					if(wifi_powet()==1) yunxing_bz=0;
					else yunxing_bz=1;
				}
				if(yunxing_bz==0)	time++;
			}
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		}
}
