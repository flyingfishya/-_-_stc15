/*******************************************************************************  
* �ļ����ƣ�ʱ��оƬ��������
* ����˵����1.    
* Ӳ��˵����IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2017-12-2   Ver_1.0
*******************************************************************************/
#include <STC15F2K60S2.H>
/********************************************************************/ 
sbit SCK=P1^7;		
sbit SD=P2^3;		
sbit RST=P1^3;
/********************************************************************/ 
/*��λ��*/
#define RST_CLR	RST=0	/*��ƽ�õ�*/
#define RST_SET	RST=1	/*��ƽ�ø�*/
/*˫������*/
#define SDA_CLR	SD=0	/*��ƽ�õ�*/
#define SDA_SET	SD=1	/*��ƽ�ø�*/
#define SDA_R	SD	/*��ƽ��ȡ*/	
/*ʱ���ź�*/
#define SCK_CLR	SCK=0	/*ʱ���ź�*/
#define SCK_SET	SCK=1	/*��ƽ�ø�*/

/********************************************************************/ 
/*���ֽ�д��һ�ֽ�����*/
void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	for (i=0;i<8;i++) 
	{ 
//		if (dat & 0x01) 	// �ȼ���if((addr & 0x01) ==1) 
//		{
//			SDA_SET;		//#define SDA_SET SDA=1 /*��ƽ�ø�*/
//		}
//		else 
//		{
//			SDA_CLR;		//#define SDA_CLR SDA=0 /*��ƽ�õ�*/
//		}		 
		SD=dat & 0x01;
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*���ֽڶ���һ�ֽ�����*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;

//		if (SDA_R) 	  //�ȼ���if(SDA_R==1)    #define SDA_R SDA /*��ƽ��ȡ*/	
//		{
//			dat |= 0x80;
//		}
//		else 
//		{
//			dat &= 0x7F;
//		}
		if(SD)
			dat |= 0x80;
		SCK_SET;
		SCK_CLR;
	}
	return dat;
}

/********************************************************************/ 
/*��DS1302 ���ֽ�д��һ�ֽ�����*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 

	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;			/*����DS1302����,RST=1��ƽ�ø� */
//	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤��д����,д֮ǰ�����λ����*/	
	Write_Ds1302_Byte(dat);	 /*д�����ݣ�dat*/
	RST_CLR;				 /*ֹͣDS1302����*/

	SDA_CLR;
}

/********************************************************************/ 
/*��DS1302���ֽڶ���һ�ֽ�����*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;	/*����DS1302����,RST=1��ƽ�ø� */	
//	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤�Ƕ�����,д֮ǰ�����λ�ø�*/
	temp=Read_Ds1302_Byte(); /*��DS1302�ж���һ���ֽڵ�����*/		
	RST_CLR;	/*ֹͣDS1302����*/

	SDA_CLR;
	return temp;
}