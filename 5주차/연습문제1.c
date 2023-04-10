#include <avr/io.h>
//
#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)
//
void delay(int n)
{
	volatile int i,j;
	for (i=0;i<100;i++)
	{
		for (j=0;j<n;j++);
	}
}
//
int main(void)
{
	MCUCR=0x80;

	while(1)
	{
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < i; j++)
			{
				for(int k = 0; k < 10; k++)
				{
					DM_SEL = (1<<(k));
					DM_DATA = (0b0111111111>>(k));
					delay(10);
				}
			}
		}

		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < i; j++)
			{
				for(int k = 0; k < 10; k++)
				{
					DM_SEL = (1<<(k));
					DM_DATA = (~(0b1111111111>>(k)));
					delay(10);
				}
			}
		}
	}
	return 1;
}