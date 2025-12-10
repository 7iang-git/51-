#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "motor.h"
#include "follow.h"

sbit Trig = P2^1;
sbit Echo = P2^0;

// 超声波测量状态机
typedef enum {
    SONAR_IDLE = 0,        // 空闲状态
    SONAR_TRIGGER,         // 发送触发脉冲
    SONAR_WAIT_RISING,     // 等待上升沿
    SONAR_MEASURING,       // 测量中
    SONAR_COMPLETE         // 测量完成
} SonarState;

// 全局变量
unsigned int distance = 0;      // 测量距离
SonarState sonar_state = SONAR_IDLE;  // 当前状态
unsigned long echo_start_time = 0;    // 上升沿时间（毫秒）
unsigned long echo_end_time = 0;      // 下降沿时间（毫秒）
unsigned long sonar_timeout = 0;      // 超时时间点
unsigned char need_trigger = 0;       // 需要触发标志

// Timer2毫秒计数器（在中断中更新）
volatile unsigned long timer2_millis = 0;

////////////////////////////////发射超声波//////////////////////////////////
void Delay20us(void)    //@11.0592MHz
{
    unsigned char data i;
    i = 6;
    while (--i);
}

void SendTrigger(void)
{
    Trig = 1;
    Delay20us();      // 保持至少10us高电平
    Delay20us();      // 再延时20us，确保足够
    Trig = 0;
}

//////////////////////////////Timer2初始化//////////////////////////////
void Timer2_Init(void)
{
    // Timer2作为16位自动重装定时器，用于产生1ms中断
    // 11.0592MHz晶振，12T模式，1ms需要922个机器周期
    // 计数值 = 65536 - 922 = 64614 = 0xFC66
    T2CON = 0x00;      // 16位自动重装模式
    RCAP2H = 0xFC;     // 自动重装值高位
    RCAP2L = 0x66;     // 自动重装值低位
    TH2 = 0xFC;        // 初始值高位
    TL2 = 0x66;        // 初始值低位
    ET2 = 1;           // 开启Timer2中断
    EA = 1;            // 开总中断
    TR2 = 1;           // 启动Timer2
}

/////////////////////////////////超声波状态机更新函数//////////////////////////////
void Sonar_Update(void)
{
    unsigned long current_time = timer2_millis;  // 获取当前时间（毫秒）
    
    switch(sonar_state)
    {
        case SONAR_IDLE:
            // 检查是否需要新的测量
            if(need_trigger)
            {
                // 检查是否满足测量间隔（至少60ms）
                static unsigned long last_measure_time = 0;
                if((current_time - last_measure_time) >= 60)
                {
                    sonar_state = SONAR_TRIGGER;
                    need_trigger = 0;
                    last_measure_time = current_time;
                }
            }
            break;
            
        case SONAR_TRIGGER:
            // 发送触发脉冲
            SendTrigger();
            echo_start_time = 0;
            echo_end_time = 0;
            sonar_state = SONAR_WAIT_RISING;
            // 设置超时：5ms内必须检测到上升沿
            sonar_timeout = current_time + 5;
            break;
            
        case SONAR_WAIT_RISING:
            // 检查是否超时
            if(current_time > sonar_timeout)
            {
                sonar_state = SONAR_IDLE;
                distance = 0;  // 超时返回0
                break;
            }
            
            // 检查上升沿
            if(Echo)
            {
                // 记录上升沿时间
                echo_start_time = current_time;
                sonar_state = SONAR_MEASURING;
                // 设置超时：26ms内必须检测到下降沿（最大测量距离）
                sonar_timeout = current_time + 26;
            }
            break;
            
        case SONAR_MEASURING:
            // 检查是否超时
            if(current_time > sonar_timeout)
            {
                // 超时，按最大距离处理
                echo_end_time = current_time;
                sonar_state = SONAR_COMPLETE;
                break;
            }
            
            // 检查下降沿
            if(!Echo)
            {
                echo_end_time = current_time;
                sonar_state = SONAR_COMPLETE;
            }
            break;
            
        case SONAR_COMPLETE:
            // 计算距离
            if(echo_start_time > 0 && echo_end_time > echo_start_time)
            {
                // 计算高电平时间（毫秒）
                unsigned long time_ms = echo_end_time - echo_start_time;
                
                // 转换为微秒：time_us = time_ms * 1000
                // 距离(cm) = 时间(us) / 58
                // 所以：距离 = (time_ms * 1000) / 58
                // 简化计算避免溢出：距离 = time_ms * 17
                distance = (unsigned int)(time_ms * 17);
                
                // 限制最大距离（约4.5米）
                if(distance > 450) distance = 450;
                // 限制最小有效距离（约2cm）
                if(distance < 2) distance = 0;
            }
            else
            {
                distance = 0;
            }
            
            sonar_state = SONAR_IDLE;
            break;
    }
}

/////////////////////////////////触发一次测量//////////////////////////////
void Sonar_Trigger(void)
{
    if(sonar_state == SONAR_IDLE)
    {
        need_trigger = 1;
    }
    // 如果正在测量中，忽略本次触发
}

/////////////////////////////////获取当前距离//////////////////////////////
unsigned int GetDistance(void)
{
    return distance;
}

////////////////////////////////////////Timer2中断服务///////////////////////////
void Timer2_Routine(void) interrupt 5
{
    TF2 = 0;  // 清除中断标志
    timer2_millis++;  // 毫秒计数器加1
}