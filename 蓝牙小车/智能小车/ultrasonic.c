#include <REGX52.H>
#include "Delay.h"
#include "motor.h"

// 超声波引脚定义
sbit Trig = P2^1;
sbit Echo = P2^0;

// 全局变量定义
unsigned int distance = 0;

void Delay20us(void)
{
    unsigned char data i;
    
    // 11.0592MHz下，20μs需要约18.5个机器周期
    // 调整为8，经过测试大约18-20μs
    i = 8;  // 调整为8
    while(--i);
}

// 获取距离函数
unsigned int GetDistance(void)
{
    unsigned long time_cnt = 0;
    unsigned int temp_distance = 0;
    unsigned int timeout = 0;
    
    // 发送触发脉冲（10us高电平）
    Trig = 1;
    Delay20us();  // 延时20us
    Delay20us();  // 再延时20us，总共40us确保可靠
    Trig = 0;
    
    // 等待回声开始（带超时保护）
    while(!Echo)
    {
        Delay20us();
        timeout++;
        if(timeout > 500)  // 约10ms超时
        {
            return 0;  // 返回0表示超时
        }
    }
    
    // 开始计时（简单循环计数方式，不依赖定时器）
    time_cnt = 0;
    while(Echo)
    {
        Delay20us();  // 每次延时约20us
        time_cnt++;
        
        // 超时保护（最大测量距离约4.5米，对应约26ms）
        if(time_cnt > 1300)  // 1300 * 20us = 26ms
        {
            return 450;  // 返回最大距离
        }
    }
    
    // 计算距离
    // time_cnt的单位是20us，所以总时间 = time_cnt * 20 us
    // 距离(cm) = 时间(us) / 58
    temp_distance = (unsigned int)((time_cnt * 20) / 58);
    
    // 限制最大距离
    if(temp_distance > 450)
    {
        temp_distance = 450;
    }
    
    return temp_distance;
}

// 超声波避障处理函数
void Ultrasonic_Avoidance(void)
{
    static unsigned int measure_counter = 0;
    
    // 控制测量频率（每100ms测量一次）
    measure_counter++;
    if(measure_counter >= 100)
    {
        measure_counter = 0;
        
        // 测量距离
        distance = GetDistance();
        
        // 避障判断（距离小于30cm时停止）
        if(distance < 10 && distance > 0)
        {
            Car_SharpStop();  // 立即停止
        }
    }
}