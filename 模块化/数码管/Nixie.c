#include <REGX52.H>
#include "Delay.h"
unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//��̬����ܵ�����ʾ����
void Nixie(unsigned char Location,unsigned char Number)
{  //λѡ
	switch(Location)		//ע��led���С�����Ҳ� ��ϰ���ϴ����ұ��
  {
		case 1:P2_2=1;P2_3=1;P2_4=1;break;
		case 2:P2_2=0;P2_3=1;P2_4=1;break;
		case 3:P2_2=1;P2_3=0;P2_4=1;break;
	  case 4:P2_2=0;P2_3=0;P2_4=1;break;
		case 5:P2_2=1;P2_3=1;P2_4=0;break;
		case 6:P2_2=0;P2_3=1;P2_4=0;break;
		case 7:P2_2=1;P2_3=0;P2_4=0;break;
		case 8:P2_2=0;P2_3=0;P2_4=0;break;  //ע��P2_4���Ǹ�λ
	}
	//��ѡ  ���������������� ��1��0�� д��1 0תΪ8λ������ʱע���P0_7��ʼΪ��λ
	P0=NixieTable[Number]; 	
	//Ϊ������Ӱ
	Delay(1);
	P0=0x00;
}