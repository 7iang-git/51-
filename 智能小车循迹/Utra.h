#ifndef __UTRA_H__
#define __UTRA_H__
void Timer2_Init();
unsigned int GetDistance();
void Sonar_Update(void);    // 新增：状态机更新函数
void Sonar_Trigger(void);   // 新增：触发测量函数
#endif