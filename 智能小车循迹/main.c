#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "motor.h"
#include "follow.h"
#include "Utra.h"

////////////变量声明////////////
/////////////////pwm//////////////
unsigned int PWML=25;
//unsigned int PWM_Low_TimeL=500; 
unsigned int PWMR=59; 
//unsigned int PWM_Low_TimeR=500; 
unsigned int t=0; 
///////////////////电机////////////
sbit ENAL=P3^2;
sbit ENAR=P3^7;
////////////////蓝牙/////////////
unsigned int receive_data;
////////////// 避障状态变量/////////////////////////
unsigned char avoid_state = 0;     // 避障状态
unsigned int avoid_timer = 0;      // 避障计时器
unsigned int last_distance = 0;    // 上次测量的距离

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
	 case '1':
	 {
		if(PWML<=100&&PWMR<=100)
		{ PWML+=20;
		PWMR+=20;break;}
	 }
	 case '3':
	 {
		 if(PWML>20)
		 {PWML-=20;
			PWMR-=20;
		 }break;
	 }
	 default: {break;}
	}
}

////////////////////////串口中断初始化函数///////////////////////
void Uart_Init()
{
	PCON=0x00;   //关倍频
	SCON=0x50;   
	//用定时器1做波特率发生器 下面是其寄存器的配置
	TMOD &= 0x0F;  // 清零高4位（定时器1相关位）
    TMOD |= 0x20;  // 设置定时器1为模式2（8位自动重载）
	TL1=0xFd;
	TH1=0xFd;
	ET1=0;   //不让定时器1中断
	TR1=1;
	EA=1;
	ES=1;    //让定时器1通过串口中断
}
//////////////////////避障处理函数//////////////////////////////
void Obstacle_Avoidance(unsigned int dist)
{
    if(dist < 30 && dist > 0)  // 检测到障碍物（30cm内）
    {
        switch(avoid_state)
        {
            case 0:  // 立即停止
                Car_SharpStop();
                avoid_timer++;
                if(avoid_timer > 50)  // 停止500ms
                {
                    avoid_state = 1;
                    avoid_timer = 0;
                }
                break;
                
            case 1:  // 后退
                Car_Back();
                avoid_timer++;
                if(avoid_timer > 30)  // 后退300ms
                {
                    avoid_state = 2;
                    avoid_timer = 0;
                }
                break;
                
            case 2:  // 右转
                Car_SharpRight();
                avoid_timer++;
                if(avoid_timer > 80)  // 右转800ms
                {
                    avoid_state = 3;
                    avoid_timer = 0;
                }
                break;
                
            case 3:  // 短暂停止
                Car_Stop();
                avoid_timer++;
                if(avoid_timer > 20)  // 停止200ms
                {
                    avoid_state = 0;  // 完成避障，返回初始状态
                    avoid_timer = 0;
                }
                break;
        }
    }
    else
    {
        // 没有障碍物，重置避障状态
        avoid_state = 0;
        avoid_timer = 0;
    }
}

//////////////////////main函数//////////////////////////////
void main()
{ 	unsigned int dis = 0;
    static unsigned int measure_counter = 0;
    
    Timer0_Init();
    Timer2_Init();
    Uart_Init();
    
    // 等待系统稳定
    Delay(1000);
    
    while(1)
    {
       // 更新超声波状态机（非阻塞）
        Sonar_Update();
        
        // 每50次循环触发一次测量（控制测量频率）
        measure_counter++;
        if(measure_counter >= 50)
        {
            measure_counter = 0;
            Sonar_Trigger();  // 触发测量
        }
        
        // 获取当前距离
        dis = GetDistance();
        
        // 避障处理
        if(dis < 30 && dis > 0)
        {
            Obstacle_Avoidance(dis);
        }
        else
        {
            // 没有障碍物时执行巡线
            Turn();
        }
        
        // 短延时，避免循环过快
        {
            unsigned char i;
            for(i = 0; i < 10; i++);
        }
    }
}
////////////////////////串口中断服务函数///////////////////////
void Uart_Routine() interrupt 4
{  
 if(RI==1)
 {receive_data=SBUF;
	Receive_Data(receive_data);
	RI=0;
 }
 if(TI==1)
 {
 TI=0;
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





