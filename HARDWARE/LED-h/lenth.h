#ifndef __LENGTH_H
#define __LENGTH_H
#include "sys.h" 
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO��������
//GPIOB->CRH|=0X00000030;//PB9 �������
//GPIOB->CRH&=0XFFFFFF0F; //PB9 ���ó�����
void Lenth_Init(void);
extern u32 Tim1_Sta;

#endif
















