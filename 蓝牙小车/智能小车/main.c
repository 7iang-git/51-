#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "motor.h"
#include "ultrasonic.h"

////////////变量声明//////////

//////////////////pwm//////////////
unsigned int PWML=25;
//unsigned int PWM_Low_TimeL=500; 
unsigned int PWMR=56; 
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
    PCON = 0x00;   // 关倍频
    SCON = 0x50;   // 串口模式1，允许接收
    
    // 只设置定时器1，不影响定时器0
    TMOD &= 0x0F;   // 清除高4位（定时器1），保留低4位（定时器0）
    TMOD |= 0x20;   // 设置定时器1为模式2（8位自动重装）
    
    TL1 = 0xFD;     // 9600波特率@11.0592MHz
    TH1 = 0xFD;
    ET1 = 0;        // 禁止定时器1中断
    TR1 = 1;        // 启动定时器1
    
    EA = 1;         // 开总中断
    ES = 1;         // 开串口中断
}

//////////////////////////////main函数///////////////////////////
void main()
{ 
	Timer0_Init();
	Uart_Init();
	while(1)
	{
		Ultrasonic_Avoidance();
    // 短延时，避免循环过快
		Delay(1);
	}

}
/////////////////////////////定时器0pwm//////////////////////////////
void Timer0_Routine() interrupt 1   
{
 TL0 = (65535-184)%256;				//设置定时初始值
	TH0 = (65535-184)/256;				//设置定时初始值
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





