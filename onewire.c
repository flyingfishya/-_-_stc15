#include "onewire.h"
unsigned int k;
//�������ڲ���ʱ����
void Delay_OneWire(unsigned int t)  
{
	while(t--)
	{
		for(k=0;k<=11;k++);
	}
}

//������д����
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//�����߶�����
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20��ʼ��
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

unsigned int DS18B20_Read()
{
	volatile unsigned char high,low;
	volatile unsigned int date;
	
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0X44);//启动温度转换
	Delay_OneWire(400);
	
	init_ds18b20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);//读取温度数据命令
	
	low = Read_DS18B20();
	high = Read_DS18B20();
//	date = high<<4;
//	date |= (low>>4);
	date = (unsigned int)high<<8;
	date = date|low;
	date = date*6.25;
	return date;
}
