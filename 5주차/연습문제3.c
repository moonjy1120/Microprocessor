#include <avr/io.h>
#include <stdlib.h>

#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)

void delay(int n)
{
	volatile int i, j;
	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < n; j++);
	}
}

void display_led_status(int led_status[][10])
{
	for (int i = 0; i < 10; i++)
	{
		int row_data = 0;
		for (int j = 0; j < 10; j++)
		{
			if (led_status[i][j])
			{
				row_data |= (1 << j);
			}
		}
		DM_SEL = (1 << i);
		DM_DATA = row_data;
		delay(10);
	}
}

int main()
{
	MCUCR=0x80;

	int seed = rand();
	int led_status[10][10] = {0};

	while (1)
	{
		int row, column;
		do
		{
			srand(seed);
			row = rand() % 10;
			seed = rand();
			srand(seed);
			column = rand() % 10;
			seed = rand();
		} while (led_status[row][column]);

		led_status[row][column] = 1;

		// 도트 메트릭스 랜덤 on
		for(int i = 0; i < 20; i++)
		{
			for(int j = 0; j < i; j++)
			{
				display_led_status(led_status);
			}
		}
	}

	return 0;
}
