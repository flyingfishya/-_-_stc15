#include <STC15F2K60S2.H>
#include "intrins.h"
#include "iic.h"
#define sendcon P10
#define getcon P11
unsigned int n=0,light=0;
unsigned char shumaguanduan[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00};
unsigned char shumaguan[8]={10,10,10,10, 10,10,10,10};
unsigned int display_time=0,key_time=0,L3_time=0;
unsigned char rb2;
unsigned char L3_flag=0;
unsigned int time = 0,juli = 0,ware_flag = 0,ware_time=0;
unsigned int L10_state=0,L10_time=0,stop_flag=0,stop_time=0,L4_state=0,L4light_time=0;
unsigned char turn_flag=0,turn1=2,turn2=4,shumaguanlight_time = 0,shumaguanlight_flag=0;
	
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L10 = P0^4;

void Timer0Init(void)		//1毫秒@12MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x20;				//设置定时初始值
	TH0 = 0xD1;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
		
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x00;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	
	ET0 = 1;
	EA = 1;

}

void else_init()
{
	P0=0X00; P2&=0X1F; P2|=0Xa0; P0=0X00; P2&=0X1F; P0=0X00;
	P0=0XFF; P2&=0X1F; P2|=0X80; P0=0XFF; P2&=0X1F; P0=0XFF;
}

void Delay13us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 36;
	while (--i);
}

void display()
{
	P0=0XFF; P2&=0X1F; P2|=0XC0; P0=0x01<<n; P2&=0X1F; P0=0XFF; //0xc0//wei
	P0=0XFF; P2&=0X1F; P2|=0Xe0; P0=~shumaguanduan[shumaguan[n]]; P2&=0X1F; P0=0XFF;//0xe0//duan
	n++;
	if(n>=8)n=0;
}

void read_rb2()
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_WaitAck();
	IIC_Stop();
	rb2 = temp/5.1;
}
	
	

void key()
{
	P30=1;P31=1;P32=1;P33=1;
	if(P30==0)
	{
		if(rb2<10)
		{
			switch(turn_flag)
			{
				case 0:
					
				break;
				case 1:
					turn1++;
					if(turn1>10)turn1=0;
				break;
				case 2:
					turn2++;
					if(turn2>10)turn2=0;
				break;
			}
		}//tiaozheng
	}
	if(P31==0)
	{
		if(rb2<10)
		{
			turn_flag++;
			turn_flag=turn_flag%3;
		}//shezhi
	}
	if(P32==0)
	{
		if((stop_flag==1)|(L10_state==1))
		{
			stop_flag++;
			stop_flag = stop_flag%2;
			if(stop_flag==1)//ting
			{
				P0=0X00; P2&=0X1F; P2|=0Xa0; P0&=~0X10; P2|=0Xa0; L10_state=0; P2&=0X1F; P0=0X00;//jidianqi
				stop_time = L10_time;
			}
			else if(stop_flag==0)//jixu
			{
				L10_time = stop_time;
				P0=0X00; P2&=0X1F; P2|=0Xa0; P0|=0X10; P2|=0Xa0; L10_state=1; P2&=0X1F; P0=0X00;//jidianqi
			}
		}
	}
	if(P33==0)
	{
		if((rb2>=10)&&(rb2<40))
		{
			P0=0X00; P2&=0X1F; P2|=0Xa0; P0|=0X10; P2|=0Xa0; L10_state=1; P2&=0X1F; P0=0X00;//jidianqi
		}
	}
}

void ware_send()
{
	unsigned char i=8;
	while(i--)
	{
		sendcon=1;
		Delay13us();
		sendcon=0;
		Delay13us();		
	}
	TR1 = 1;		//定时器1开始计时
}

void ware_get()
{
	while((TF1==0)&&(getcon==1));
	TR1 = 0;
	if(getcon==0)
	{
		juli = ((TH1*256+TL1)*0.017)/12;
		TH1 = 0;
		TL1 = 0;
		TF1 = 0;
	}
	else
	{
		TF1 = 0;
		TH1 = 0;
		TL1 = 0;
	}
	
	if(juli<=30)
	{		
			shumaguan[0]=1;
			shumaguan[1]=10;
			shumaguan[2]=10;
			shumaguan[3]=juli%100/10;
			shumaguan[4]=juli%10;
			shumaguan[5]=10;
			shumaguan[6]=10;
			shumaguan[7]=1;
			if((L10_state==0)&&(stop_flag==0))L10_time = turn1*1000;
	}
	else if(juli>30)
	{
			shumaguan[0]=1;
			shumaguan[1]=10;
			shumaguan[2]=10;
			shumaguan[3]=juli%100/10;
			shumaguan[4]=juli%10;
			shumaguan[5]=10;
			shumaguan[6]=10;
			shumaguan[7]=2;
		  if((L10_state==0)&&(stop_flag==0))L10_time = turn2*1000;
	}
}


void main()
{
	else_init();
	Timer0Init();
	while(1)
	{
		read_rb2();
		//shumaguan[0]=rb2%10000/1000;
		//shumaguan[1]=rb2%1000/100;
		//shumaguan[2]=rb2%100/10;
		//shumaguan[3]=rb2%10;
		if(rb2<10)
		{
				L3_flag=0;
		   	L3_time=0;
			  P0=0X00; P2&=0X1F; P2|=0Xa0; P0&=~0x40; L10=L10_state; P2&=0X1F; P0=0X00;//3 off
				if(turn_flag==0)
				{
					shumaguanlight_time = 0;
					shumaguanlight_flag=0;
					shumaguan[0]=10;
					shumaguan[1]=10;
					shumaguan[2]=10;
					shumaguan[3]=10;
					shumaguan[4]=10;
					shumaguan[5]=10;
					shumaguan[6]=10;
					shumaguan[7]=10;
				}
				else if(turn_flag==1)
				{
						shumaguanlight_flag=1;
						if(shumaguanlight_time<=100)
						{
								shumaguan[0]=3;
								shumaguan[1]=10;
								shumaguan[2]=10;
								shumaguan[3]=turn1%100/10;
								shumaguan[4]=turn1%10;
								shumaguan[5]=10;
								shumaguan[6]=turn2%100/10;
								shumaguan[7]=turn2%10;
						}
						else
						{
								shumaguan[0]=3;
								shumaguan[1]=10;
								shumaguan[2]=10;
								shumaguan[3]=10;
								shumaguan[4]=10;
								shumaguan[5]=10;
								shumaguan[6]=turn2%100/10;
								shumaguan[7]=turn2%10;
						}
				}	
				else if(turn_flag==2)	
				{
						shumaguanlight_flag=1;
						if(shumaguanlight_time<=100)
						{		
									shumaguan[0]=3;
									shumaguan[1]=10;
									shumaguan[2]=10;
									shumaguan[3]=turn1%100/10;
									shumaguan[4]=turn1%10;
									shumaguan[5]=10;
									shumaguan[6]=turn2%100/10;
									shumaguan[7]=turn2%10;
						}
						else
						{					
									shumaguan[0]=3;
									shumaguan[1]=10;
									shumaguan[2]=10;
									shumaguan[3]=turn1%100/10;
									shumaguan[4]=turn1%10;
									shumaguan[5]=10;
									shumaguan[6]=10;
									shumaguan[7]=10;
						}
				}					
				P0=0XFF; P2&=0X1F; P2|=0X80; L1=0; P2&=0X1F; P0=0XFF;		
		}
		else if(rb2<40)
		{
				L3_flag=0;
			  L3_time=0;
			  P0=0X00; P2&=0X1F; P2|=0Xa0; P0&=~0x40;  L10=L10_state; P2&=0X1F; P0=0X00;//3 off
			
				P0=0XFF; P2&=0X1F; P2|=0X80; L2=0; L4=~L4_state; P2&=0X1F; P0=0XFF;		

			if(ware_flag==1)
			{
				ware_send();
				ware_get();//huowu
				ware_flag = 0;
			}
			if((L10_state==1)|(stop_flag==1))
			{
					shumaguan[0]=2;
					shumaguan[1]=10;
					shumaguan[2]=10;
					shumaguan[3]=10;
					shumaguan[4]=10;
					shumaguan[5]=10;
					shumaguan[6]=(L10_time/1000)%100/10;
					shumaguan[7]=(L10_time/1000)%10;
			}
		}
		else
		{
				L3_flag=1;
			  P0=0X00; P2&=0X1F; P2|=0Xa0; P0|=0x40; L10=L10_state; P2&=0X1F; P0=0X00;				
		}
	}
}


void timer1() interrupt 1
{
	display_time++;
	if(display_time>=2)
	{
		display_time=0;
		display();
	}
	
	
	key_time++;
	if(key_time>=200)
	{
		key_time=0;
		key();
	}
	
	if(ware_flag==0)ware_time++;
	if(ware_time>=500)
	{
		ware_time = 0;
		ware_flag = 1;
	}
	
	
	if(L3_flag==1)
	{
		L3_time++;
	}
	if(L3_time>=500)
	{
		L3_time=0;	
		P0=0XFF; P2&=0X1F; P2|=0X80; L3=light; P2&=0X1F; P0=0XFF;	
		light = ~light;
	}
	
	if(L10_state==1)
	{
		L10_time--;
	}
	if(L10_time<=0)
	{
		L10_state=0;
		P0=0X00; P2&=0X1F; P2|=0Xa0; P0&=~0x40;  L10=L10_state; P2&=0X1F; P0=0X00;//3 off
	}
	
	
	
  if(stop_flag==1)L4light_time++;
	if(L4light_time>=500)
	{
		L4light_time=0;
		L4_state=~L4_state;
		P0=0XFF; P2&=0X1F; P2|=0X80; L4=L4_state; P2&=0X1F; P0=0XFF;
	}
	
	if(shumaguanlight_flag==1)
	{
		shumaguanlight_time++;
		if(shumaguanlight_time>=500)shumaguanlight_time=0;
	}
	
}