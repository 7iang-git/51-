#include <REGX52.H>

//变量声明区域
unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	
//延时函数
void Delay(unsigned int xms)
{	unsigned char data i, j;
  while(xms--)
	{
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
  }
}

//静态数码管单个显示函数
void Nixie(unsigned char Location,unsigned char Number)
{  //位选  38译码器
	switch(Location)		//注意led编号小的在右侧 而习惯上从左到右编号
  {
		case 1:P2_2=1;P2_3=1;P2_4=1;break;
		case 2:P2_2=0;P2_3=1;P2_4=1;break;
		case 3:P2_2=1;P2_3=0;P2_4=1;break;
	  case 4:P2_2=0;P2_3=0;P2_4=1;break;
		case 5:P2_2=1;P2_3=1;P2_4=0;break;
		case 6:P2_2=0;P2_3=1;P2_4=0;break;
		case 7:P2_2=1;P2_3=0;P2_4=0;break;
		case 8:P2_2=0;P2_3=0;P2_4=0;break;  //注意P2_4才是高位
	}
	//段选  共阴极连接用阳码 即1亮0灭 写出1 0转为8位二进制时注意从P0_7开始为高位
	P0=NixieTable[Number]; 	
	//为后面消影
	Delay(1);
	P0=0x00;
}


void main()
{
	while(1)
	{
	  //动态数码管  多个位显示不同数字
		Nixie(1,1);
		Nixie(2,2);
		Nixie(3,3);
	
	
	}


}
