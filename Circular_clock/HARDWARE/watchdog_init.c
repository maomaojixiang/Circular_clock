#include "watchdog_init.h"

/////////////////////////////////////////////////////////////////////////////////
//��������IWDG_Scan
//�������ܣ���ʼ���������Ź�
//����ֵ��prer��Ԥ��Ƶϵ�� rlr����װ��ֵ
//���ֵ����
/////////////////////////////////////////////////////////////////////////////////

void IWDG_Scan(u8 prer,u16 rlr) 
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
	
	IWDG_SetPrescaler(prer);  //����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ64
	
	IWDG_SetReload(rlr);  //����IWDG��װ��ֵ
	
	IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
	
	IWDG_Enable();  //ʹ��IWDG
}

/////////////////////////////////////////////////////////////////////////////////
//��������IWDG_init
//�������ܣ�����������Ź���Ԥ��Ƶϵ������װ��ֵ �����ö������Ź���ʼ������
//����ֵ��time���������Ź���ʱ��
//���ֵ����
/////////////////////////////////////////////////////////////////////////////////
void IWDG_init(u16 time)
{
	u8 prer;
	u16 rlr;
	if(time<400) prer=0;
	else if(time<800) prer=1;
	else if(time<1600) prer=2;
	else if(time<3200) prer=3;
	else if(time<6400) prer=4;
	else if(time<12800) prer=5;
	else prer=6;								//ͨ��ʱ��ѡ����ʵ�Ԥ��Ƶϵ��
	
	//������װ��ֵ
	rlr=(40*time)/(4*pow(2,prer));
	
	//���ö������Ź���ʼ������
	IWDG_Scan(prer,rlr);
}


//ι�������Ź�
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();//�������Ź�ι��								   
}




//���ڿ��Ź��жϷ������
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ2�������ȼ�3����2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //��ռ2�������ȼ�3����2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
}

//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 
//��ʼ�����ڿ��Ź� 	
//tr   :T[6:0],������ֵ 
//wr   :W[6:0],����ֵ 
//fprer:��Ƶϵ����WDGTB��,�����2λ��Ч 
//Fwwdg=PCLK1/(4096*2^fprer). 

void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDGʱ��ʹ��

	WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT.   
	WWDG_SetPrescaler(fprer);////����IWDGԤ��Ƶֵ

	WWDG_SetWindowValue(wr);//���ô���ֵ

	WWDG_Enable(WWDG_CNT);	 //ʹ�ܿ��Ź� ,	���� counter .                  

	WWDG_ClearFlag();//�����ǰ�����жϱ�־λ 

	WWDG_NVIC_Init();//��ʼ�����ڿ��Ź� NVIC

	WWDG_EnableIT(); //�������ڿ��Ź��ж�
} 
//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//ʹ�ܿ��Ź� ,	���� counter .	 
}


void WWDG_IRQHandler(void)
	{

	WWDG_SetCounter(WWDG_CNT);	  //�������˾��,���ڿ��Ź���������λ

	WWDG_ClearFlag();	  //�����ǰ�����жϱ�־λ
	}
