#include <avr/io.h>
#include <stdlib.h>
//
#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)
//
void delay(int n)
{
	volatile int i, j;
	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < n; j++);
	}
}
//
int main()
{
	MCUCR=0x80;
	
	int seed = 0;
	
	while (1)
	{
		srand(seed);
		int row = rand() % 10;
		seed++;
		srand(seed);
		int column = rand() % 10;
		seed++;

		// 도트 메트릭스 랜덤 on
		for (int i = 0; i < 100; i++)
		{
			for(int j = 0; j < i; j++)
			{
				DM_SEL = (1 << (row));
				DM_DATA = (1 << (column));
				delay(1);
			}
		}

		// 도트 메트릭스 off
		for (int i = 0; i < 10; i++)
		{
			DM_SEL = (1 << (i));
			DM_DATA = 0;
			delay(1);
		}
		
		delay(100);
	}

	return 0;
}
