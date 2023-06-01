/*******************************************************************************  
* 文件名称：时钟芯片驱动程序
* 程序说明：1.    
* 硬件说明：IAP15F2K61S2@11.0592MHz
* 日期版本：2017-12-2   Ver_1.0
*******************************************************************************/
#include <STC15F2K60S2.H>
/********************************************************************/ 
sbit SCK=P1^7;		
sbit SD=P2^3;		
sbit RST=P1^3;
/********************************************************************/ 
/*复位脚*/
#define RST_CLR	RST=0	/*电平置低*/
#define RST_SET	RST=1	/*电平置高*/
/*双向数据*/
#define SDA_CLR	SD=0	/*电平置低*/
#define SDA_SET	SD=1	/*电平置高*/
#define SDA_R	SD	/*电平读取*/	
/*时钟信号*/
#define SCK_CLR	SCK=0	/*时钟信号*/
#define SCK_SET	SCK=1	/*电平置高*/

/********************************************************************/ 
/*单字节写入一字节数据*/
void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	for (i=0;i<8;i++) 
	{ 
//		if (dat & 0x01) 	// 等价于if((addr & 0x01) ==1) 
//		{
//			SDA_SET;		//#define SDA_SET SDA=1 /*电平置高*/
//		}
//		else 
//		{
//			SDA_CLR;		//#define SDA_CLR SDA=0 /*电平置低*/
//		}		 
		SD=dat & 0x01;
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*单字节读出一字节数据*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;

//		if (SDA_R) 	  //等价于if(SDA_R==1)    #define SDA_R SDA /*电平读取*/	
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
/*向DS1302 单字节写入一字节数据*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 

	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;			/*启动DS1302总线,RST=1电平置高 */
//	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是写操作,写之前将最低位置零*/	
	Write_Ds1302_Byte(dat);	 /*写入数据：dat*/
	RST_CLR;				 /*停止DS1302总线*/

	SDA_CLR;
}

/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;	/*启动DS1302总线,RST=1电平置高 */	
//	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是读操作,写之前将最低位置高*/
	temp=Read_Ds1302_Byte(); /*从DS1302中读出一个字节的数据*/		
	RST_CLR;	/*停止DS1302总线*/

	SDA_CLR;
	return temp;
}