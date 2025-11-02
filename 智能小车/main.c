#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "motor.h"

////////////变量声明//////////

//////////////////pwm//////////////
unsigned int PWML=20;
//unsigned int PWM_Low_TimeL=500; 
unsigned int PWMR=20; 
//unsigned int PWM_Low_TimeR=500; 
unsigned int t=0; 
///////////////////电机////////////
sbit ENAL=P3^2;
sbit ENAR=P3^7;
/////////////////红外/////////////
sbit IRN1=P2^7;
sbit IRN2=P2^6;
sbit IRN3=P2^5;
sbit IRN4=P2^4;
////////////////蓝牙/////////////
unsigned int receive_data;

////////////////////////蓝牙接收函数///////////////////////
void Receive_Data(unsigned char m)
{
 switch(m)
	{
	 case '2': 
	 {
		Car_Back();break;
	 }
	 case '8': 
	 {
		Car_Go();break;
	 }
	 case '4': 
	 {
		Car_SharpLeft();break;
	 }
	 case '6':
	 {
	 Car_SharpRight();break;
	 }
	 case '0':
	 {
		Car_SharpStop();break;
	 }
	}
}
////////////////////////串口中断初始化函数///////////////////////
void Uart_Init()
{
	PCON=0x00;   //关倍频
	SCON=0x50;   
	//用定时器1做波特率发生器 下面是其寄存器的配置
	TMOD=0x20;   //8倍重载模式
	TL1=0xFd;
	TH1=0xFd;
	ET1=0;   //不让定时器1中断
	TR1=1;
	EA=1;
	ES=1;    //让定时器1通过串口中断
}
	
////////////////////////自动转向循迹函数///////////////////////
void Turn()
{ 
//////////////////////////////////////////////////////////////
  if(IRN1==0&&IRN2==0&&IRN3==0&&IRN4==0)   //未检测到黑线 即已出线
	{
		PWML=20;
		PWMR=19;
		Car_Back();
		Delay(10000);
		Car_Stop();
	}
	
/////////////////////////////////////////////////////////////////
	if(IRN1==0&&IRN2==1&&IRN3==0&&IRN4==0)   //缓慢情况的左转
	{
		Delay(100);
		if(IRN1==0&&IRN2==1&&IRN3==0&&IRN4==0)
		{
			PWML=7;
			PWMR=25; 
			Car_Go();
		}	

	}	
//////////////////////////////////////////////////////////////
	if(IRN1==0&&IRN2==0&&IRN3==1&&IRN4==1)   //右急转
  {
		Delay(100);
		if(IRN1==0&&IRN2==0&&IRN3==1&&IRN4==1)
		{  
			Car_SharpRight();		
		}	
	
	}		
//////////////////////////////////////////////////////////////
	if(IRN1==1&&IRN2==1&&IRN3==0&&IRN4==0)    //左急转
	{
		Delay(100);
		if(IRN1==1&&IRN2==1&&IRN3==0&&IRN4==0)
		{  
			Car_SharpLeft();		
		}	
	}
//////////////////////////////////////////////////////////////	
	if(IRN1==0&&IRN2==0&&IRN3==1&&IRN4==0)   //缓慢情况的右转
	{
		Delay(100);
		if(IRN1==0&&IRN2==0&&IRN3==1&&IRN4==0)
		{
			PWML=25;
			PWMR=5; 
			Car_Go();
		}	

	}	
////////////////////////////////////////////////////////////////
	if(IRN1==0&&IRN2==1&&IRN3==1&&IRN4==0)    //直走
	{
	Delay(100);
		if(IRN2==1&&IRN3==1)
		{
		PWML=20;
		PWMR=19;
		Car_Go();
		
		}
	}

}	


//////////////////////////////main函数///////////////////////////
void main()
{ 
	Timer0_Init();
	Uart_Init();
	while(1)
	{
		Turn();
	}

}
/////////////////////////////定时器0pwm//////////////////////////////
void Timer0_Routine() interrupt 1   
{
 TL0 = (65535-200)%256;				//设置定时初始值
	TH0 = (65535-200)/256;				//设置定时初始值
  t++;
	if(t<=PWML)
	{
		ENAL=1;
	}
	else{ENAL=0;}	
	
	if(t<=PWMR)
	{
		ENAR=1;
	}
	else{ENAR=0;}	
	
	if(t>100) 
	{
		t=0;
	}
	//周期为200微秒乘以100  PWM为百分比 范围0到100
}

////////////////////////串口中断服务函数///////////////////////
void Uart_Routine() interrupt 4
{  
	Car_Stop();
	RI=0;
	receive_data=SBUF;
	Receive_Data(receive_data);
}





