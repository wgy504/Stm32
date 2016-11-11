#include "lenth.h"
#include "delay.h"

//��ʼ��IIC
void Lenth_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
	TIM_ICInitTypeDef TIM1_ICInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;  //????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//---------------------pwm_test----------------
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//---------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn; //TIM3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
	NVIC_Init(&NVIC_InitStructure); 

	TIM_TimBaseStructure.TIM_Period = 10000;		//10000  ->100ms-> 17M
	TIM_TimBaseStructure.TIM_Prescaler = 7200; //7200  -> 10k 
	TIM_TimBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//????
//	TIM_TimBaseStructure.TIM_RepetitionCounter=
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimBaseStructure);
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_4; //ѡ������� IC1 ӳ�䵽 TI1 ��
	TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //�����ز���
	TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽 TI1 ��
	TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //���������Ƶ,����Ƶ
	TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	//---------------------pwm_test------------------------
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //�������ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //�۳�ʼ������ TIMx
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //��MOE �����ʹ��
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH1 Ԥװ��ʹ�� 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
	TIM_SetCompare1(TIM1,5000-1);
	//---------------------pwm_test------------------------TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //�������ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); //�۳�ʼ������ TIMx
//	TIM_CtrlPWMOutputs(TIM2,ENABLE);  //��MOE �����ʹ��
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //CH1 Ԥװ��ʹ�� 
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
	TIM_SetCompare1(TIM2,500-1);
	TIM_ITConfig( TIM2,TIM_IT_Update,ENABLE);//��������жϺͲ����ж�
	//---------------------pwm_test2------------------------

	TIM_ITConfig( TIM1,TIM_IT_Update|TIM_IT_CC4,ENABLE);//��������жϺͲ����ж�
	
	TIM_Cmd(TIM1,ENABLE ); //ʹ�ܶ�ʱ�� 2
}

void pwm_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// ��ʹ�� tim1 ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
//��ʹ�� GPIO ����ʱ��ʹ��
//���ø�����Ϊ�����������,��� TIM1 CH1 �� PWM ���岨��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
TIM_TimeBaseStructure.TIM_Period = 10000;
//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  80K
TIM_TimeBaseStructure.TIM_Prescaler =7200;
//����������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ����Ƶ
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //�ڳ�ʼ�� TIMx
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //�������ģʽ 2
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
TIM_OC1Init(TIM1, &TIM_OCInitStructure); //�۳�ʼ������ TIMx
TIM_CtrlPWMOutputs(TIM1,ENABLE);  //��MOE �����ʹ��
//TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH1 Ԥװ��ʹ�� 
//TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
TIM_Cmd(TIM1, ENABLE); //��ʹ�� TIM1
}
u32 Tim1_Sta;

void TIM1_UP_IRQHandler(void)
{
TIM_ClearITPendingBit(TIM1,TIM_IT_Update); //����жϱ�־λ

}

void TIM1_CC_IRQHandler(void)
{u16 date;
if(TIM_GetITStatus(TIM1,TIM_IT_CC4)==SET)
{
	if ((Tim1_Sta&0x01)==0)
	{
		TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Falling);
		TIM_SetCounter(TIM1,0);
		Tim1_Sta|=1;
	}
	else
	{
		date=TIM_GetCapture4(TIM1);
		TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Rising);
		Tim1_Sta&=~(1);
		Tim1_Sta&=~(0xff<<16);
		Tim1_Sta|=date<<16;
	}
}
TIM_ClearITPendingBit(TIM1,TIM_IT_CC4); //����жϱ�־λ
}




