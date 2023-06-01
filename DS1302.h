#ifndef _DS1302_H_
#define _DS1302_H_

void Write_Ds1302_Byte(unsigned char dat);

/********************************************************************/ 
/*单字节读出一字节数据*/
unsigned char Read_Ds1302_Byte(void);
/********************************************************************/ 
/*向DS1302 单字节写入一字节数据*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat);

/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) ;
#endif