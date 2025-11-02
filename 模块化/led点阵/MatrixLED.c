#include <REGX52.H>
#include <Delay.h>

sbit RCK=P3^5;  //因为rclk已经regx52定义了   rclk
sbit SCK=P3^6;   //srclk口
sbit SER=P3^4;    //ser口
//初始化函数
void MatrixLED_Init()
{  
	SCK=0;
	RCK=0;
}

/**z
  * @brief  74HC595写入一个字节
  * @param  Byte 要写入的字节
  * @retval 无
  */
void _74HC595_WriteByte(unsigned char Byte)
{ unsigned char i; 
	for(i=0;i<8;i++)
	{
		SER=Byte&(0x80>>i);   //取出Byte的第八位  把8位赋给1位遵循非0即1规则
		SCK=1;
		SCK=0;
	}  //循环后把byte全部移入移位寄存器
	RCK=1;
	RCK=0;
}

/**z
  * @brief  led点阵显示
  * @param  Column为要亮的竖列，0为最左 ；Line为要亮的横排 16进制数 高位是上面的排 1为亮0为灭
  * @retval 无
  */
void MatrixLED_Show(unsigned char Column,unsigned char Line)
{  //注意习惯上的第一列被记成第0列
	_74HC595_WriteByte(Line);
	P0=~(0x80>>Column);  //这一步很妙 实现纵向扫描  作用是选哪个整竖排	
	//消影操作  因为选Column之后 马上会选Line 会把数据残留下一个Line
	Delay(1);
	P0=0xFF;
}