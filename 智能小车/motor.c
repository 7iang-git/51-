#include <REGX52.H>


sbit IN1=P3^3;
sbit IN2=P3^4;
sbit IN3=P3^5;
sbit IN4=P3^6;

//左右电机各有4状态 正，反转；停，急停；
void LeftMotor_Forward()
{IN1=1 ;IN2=0;}
void LeftMotor_Back()
{IN1=0 ;IN2=1;}
void LeftMotor_Stop()
{IN1=0 ;IN2=0;}
void LeftMotor_SharpStop()
{IN1=1 ;IN2=1;}


void RightMotor_Forward()
{IN3=1 ;IN4=0;}
void RightMotor_Back()
{IN3=0 ;IN4=1;}
void RightMotor_Stop()
{IN3=0 ;IN4=0;}
void RightMotor_SharpStop()
{IN3=1 ;IN4=1;}

//小车状态
void Car_Go()
{ LeftMotor_Forward(); RightMotor_Forward();}
void Car_Back()
{	LeftMotor_Back();RightMotor_Back();} 
void Car_Right()
{	LeftMotor_Forward();RightMotor_Stop();}
void Car_Left()
{	LeftMotor_Stop();RightMotor_Forward();}
void Car_SharpLeft()
{	LeftMotor_Back();RightMotor_Forward();}
void Car_SharpRight()
{	RightMotor_Back();LeftMotor_Forward();}
void Car_Stop()
{	LeftMotor_Stop();RightMotor_Stop();} 
void Car_SharpStop()
{	LeftMotor_SharpStop();RightMotor_SharpStop();}
