#include <REGX52.H>
#include "Delay.h"
#include "motor.h"
#include "Timer0.h"

/////////////////pwm//////////////
extern unsigned int PWML;
extern unsigned int PWMR; 
extern unsigned int t; 
/////////////////红外/////////////
sbit IRN1=P2^7;
sbit IRN2=P2^6;
sbit IRN3=P2^5;
sbit IRN4=P2^4;
sbit IRN5=P2^3;

//检测到黑线是1 白色为0
////////////////////////自动转向循迹函数///////////////////////
void Turn()
{ 
//////////////////////////////////////////////////////////////
  if(IRN1==0&&IRN2==0&&IRN3==0&&IRN4==0&&IRN5==0)   //黑线太细了
	{
		Delay(100);
		if(IRN1==0&&IRN2==0&&IRN3==0&&IRN4==0&&IRN5==0)
		{
		PWML=25;
		PWMR=58;
		Car_Go();		
		}
	}
//////////////////////////////////////////////////////////////
 if(IRN1==1&&IRN2==1&&IRN3==1&&IRN4==1&&IRN5==1)   //十字路口
	{
		Delay(100);
		if(IRN1==1&&IRN2==1&&IRN3==1&&IRN4==1&&IRN5==1)
		{
		PWML=25;
		PWMR=58;
		Car_Go();		
		}
	}	
/////////////////////////////////////////////////////////////////
	if(IRN1==0&&IRN2==1&&IRN3==0&&IRN4==0&&IRN5==0)   //缓慢情况的左转
	{
		Delay(100);
		if(IRN1==0&&IRN2==1&&IRN3==0&&IRN4==0&&IRN5==0)
		{ 
			PWML=25;
			PWMR=25;
			Car_Left();
		}	
	}	
//////////////////////////////////////////////////////////////
	if(IRN1==0&&IRN2==0&&IRN3==0&&IRN4==1&&IRN5==1)   //右急转
  {
		Delay(100);
		if(IRN1==0&&IRN2==0&&IRN3==0&&IRN4==1)
		{  
			PWML=25;
			PWMR=25;
			Car_SharpRight();		
		}		
	}		
//////////////////////////////////////////////////////////////
	if(IRN1==1&&IRN2==1&&IRN3==0&&IRN4==0&&IRN5==0)    //左急转
	{
		Delay(100);
		if(IRN2==1&&IRN3==0&&IRN4==0&&IRN5==0)
		{  
			PWML=25;
			PWMR=25;
			Car_SharpLeft();		
		}	
	}
//////////////////////////////////////////////////////////////	
	if(IRN1==0&&IRN2==0&&IRN3==0&&IRN4==1&&IRN5==0)   //缓慢情况的右转
	{
		Delay(100);
		if(IRN1==0&&IRN2==0&&IRN3==0&&IRN4==1&&IRN5==0)
		{
			PWML=25;
			PWMR=25;
			Car_Right();
		}	
	}	
////////////////////////////////////////////////////////////////
	if(IRN1==0&&IRN2==0&&IRN3==1&&IRN4==0&&IRN5==0)    //直走
	{
	 Delay(100);
		if(IRN2==1&&IRN3==1)
		{
		PWML=25;
		PWMR=58;
		Car_Go();
		}
	}

}	

