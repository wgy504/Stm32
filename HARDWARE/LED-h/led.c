#include "led.h"
#include "sys.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK mini�SSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PA8��PD2Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PA,B,AFIO
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//jatg
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED-->Pc.13 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOc13 led
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  	
GPIO_ResetBits(GPIOB,GPIO_Pin_3);						
GPIO_ResetBits(GPIOB,GPIO_Pin_4);						

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� GPIOA15	
 						 
}
 
u8 scan(void)
//key  pb5 pu   
//return 0=> KEY=0=>  yes 
{

if(KEY==0)
{
delay_ms(10);
	if(KEY==0)
return 0;
}

return 1;
}
