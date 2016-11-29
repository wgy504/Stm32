#include "myiic.h"
#include "delay.h"
#include "usart2.h"
#include "malloc.h"
#include <string.h>   
#include <stdio.h>   
#include "text.h"
#include "gsm.h"
#include "delay.h"
 GPRS_Data Data={"188","2015-03-02T02:31:12",12}; 
void sim_at_response(u8 mode);
 //2016 11  is ok!!!
 // 1 is error
u8 sim900a_check_time(void)
{
	extern GPRS_Data Data; 
	char *b="2016******";  //  find
	char k[30]="*****************************",k2[10];//  TimeData, Month
	char *p,i;//  λ�ã� ѭ������

	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
	USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
printf("This is time receive:");
sim_at_response(0);
	
	b="2016";
	p=strstr((char*)USART2_RX_BUF,b);

	p=strstr(USART2_RX_BUF,b);
	if (p==0)
		return 2;
	//------------------
	k[19]='\0';
	for(i=0;i<=3;i++)
		{k[i]=*p;p++;}//year
	
	k[4]='-';
	for(i=11;i<=18;i++)
		{p++;k[i]=*p;}//time
	k[7]='-';
	k[10]='T';
	
	p=strstr(USART2_RX_BUF,b);
	p-=7;
	for(i=8;i<=9;i++)
		{k[i]=*p;p++;}//data	
	for(i=0;i<=2;i++)
		{p++;k2[i]=*p;}//2	
	k2[3]='\0';
	
	b="Nov";
	if(strstr(k2,b))
	{k[5]='1';k[6]='1';b="Dec";//next month
	}//month
	else if(strstr(k2,b))
	{k[5]='1';k[6]='2';}
	
	strncpy(Data.time,k,19);
	Data.time[19]='\0';
printf("The response is:%s\r\n",Data.time);
	} 
	b="{\"errno\":";
	if(strstr(USART2_RX_BUF,b))
	return 0;
	else
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart֧�ֲ��� 
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART2_RX_STA;
//     1,����USART2_RX_STA;
void sim_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A �������(���Ų��ԡ����Ų��ԡ�GPRS����)���ô���

//sim900a���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
printf("This is cmd receive:");
sim_at_response(0);
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��sim900a��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(sim900a_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	delay_ms(10);
	return res;
}
u8 gprs2(u8 data)
{
	char *p1,*p2,*p3;//p1 cmd!, p2, data p3 json, p4  stolen cipstatus data
//	u8 waitTime=0;
	char res=0;    //retutn res 0,  1,2,10
Data.id="ShuiPing";
//Data.time="2015-03-02T02:31:12";
//Data.data=223;
	p1 = mymalloc(20);
	p2 = mymalloc(300);
	p3 = mymalloc(50);
	if(p1==NULL)res= 10;
	if(p2==NULL)res= 10;
	sprintf((char*)p3,"{\"%s\":{\"%s\":%d}}\0",Data.id,Data.time,Data.data);//4λ����
	sprintf((char*)p2,"POST http://api.heclouds.com/devices/4059208/datapoints?type=4 HTTP/1.1\r\nUser-Agent: sim900A\r\napi-key: XgD=K7o=EhKrAbiWDgr462z1pQU=\r\nHost: api.heclouds.com\r\nContent-Length: %d\r\n\r\n%s\r\n",strlen(p3)+2,p3);//4λ����
	sprintf((char*)p1,"AT+CIPSEND=%d",strlen(p2));//4λ����

	if(sim900a_send_cmd("AT+CIPSTATUS","STATE: CONNECT OK",200))		//link  ERROR	
		sim900a_send_cmd("AT+CIPSTART=\"TCP\",\"api.heclouds.com\",80","OK",300);		//link  ERROR	
	sim900a_send_cmd(p1,">",100);		//length_all//send  ERROR 
	sim900a_send_cmd(p2,"SEND OK",300);
	sim900a_check_time();
	
	myfree(p1);
	myfree(p2);
	delay_ms(50);
	return res;
}


/*
in GPRS_Data 
out 0 --ok
	1 --net?
	10 --big wrong 
*/


u8 gprs_send( GPRS_Data  Data)
{
	u8 *p1,*p2,*p3;//p1 cmd!, p2, data p3 json
//	u8 waitTime=0;
	u8 res=0;    //retutn res 0,  1,2,10
	Data.id="ShuiPing";
	Data.data=223;
	p1 = mymalloc(20);
	p2 = mymalloc(300);
	p3 = mymalloc(50);
	if(p1==NULL)res= 10;
	if(p2==NULL)res= 10;
	sprintf((char*)p3,"{\"%s\":{\"%s\":%d}}\0",Data.id,Data.time,Data.data);//4λ����
	sprintf((char*)p2,"POST http://api.heclouds.com/devices/4059208/datapoints?type=4 HTTP/1.1\r\nUser-Agent: sim900A\r\napi-key: XgD=K7o=EhKrAbiWDgr462z1pQU=\r\nHost: api.heclouds.com\r\nContent-Length: %d\r\n\r\n%s\r\n",strlen(p3)+2,p3);//4λ����
	sprintf((char*)p1,"AT+CIPSEND=%d",strlen(p2));//4λ����
	//--------------

	sim900a_send_cmd("AT","OK",100);	//
	sim900a_send_cmd("ATE1","OK",100);	//

 	sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//�ر�����
	sim900a_send_cmd("AT+SAPBR=1,1","OK",100);			//init2  ����
//	sim900a_send_cmd("AT+SAPBR=2,1","XXXXX",100);		//  ��ѯ
	if(sim900a_send_cmd("AT+CIPSTART=\"TCP\",\"api.heclouds.com\",80","OK",100))res= 1;	//link  ERROR
//	if(sim900a_send_cmd("AT+CIPSTART=\"TCP\",\"zyknet.e2.luyouxia.net\",32135","OK",100))res= 1;	//link  ERROR

	if(sim900a_send_cmd('\0',"CONNECT OK",300))res=2;
	//-----------------------
	if (res!=0) goto end;
	
	sim900a_send_cmd(p1,">",100);		//length_all//send  ERROR 
	u2_printf(p2);
//printf(p2);
	delay_ms(50);
	sim900a_check_time();
	sim900a_send_cmd("AT+CIPSHUT","OK",100);
	//-------------------------
	myfree(p1);
	myfree(p2);
printf("\r\n  \r\n  \r\n %d \r\n", res);
	return res;
	end:
	myfree(p1);
	myfree(p2);
printf("\r\n  \r\n  \r\n %d \r\n", res);
	delay_ms(100);
	return res;
	
}

void change(char a,int b)
{
Data.data=b;
}











