#include <REGX52.H>
#include <Delay.h>

void ExInt0_Init()   //外部中断0对应P3_2引脚  即可以是第三个按键
{
	IT0=1;
	IE0=0;
	EX0=1;
	EA=1;
	PX0=0;
}

void main()
{
	ExInt0_Init();
	while(1)
	{
		
	}
}

void ExInt0_Routine() interrupt 0
{
  P2_0=0;
}