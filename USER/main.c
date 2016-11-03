#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "led.h"

#include "mpu6050.h"
//ALIENTEK miniSTM32������ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
 int main(void)
 {	
	 //------------int----------------------
	short aacx,aacy,aacz;  //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz; //������ԭʼ����
	short temp;  //�¶�
	 float pitch,roll,yaw; //ŷ����
	 //-----------Init--------------------
	delay_init();	    	 //��ʱ������ʼ��	  
	 uart_init(115200);
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	MPU_Init();
	 while(mpu_dmp_init());
	while(1)
	{
		temp=MPU_Get_Temperature(); //�õ��¶�ֵ
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz); //�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz); //�õ�����������
		printf("aacx is %d\r\n",aacy);

	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{	temp=(int)(roll*10);	printf("roll is %d\r\n",roll);}
		LED1=~LED1;
		delay_ms(300);
	}
 }

