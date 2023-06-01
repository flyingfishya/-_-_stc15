#include <STC15F2K60S2.H>

void Delayms(unsigned int xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms--)
	{
			i = 11;
			j = 190;
			do
			{
				while (--j);
			} while (--i);
	}
}