#define EX_LED (*(volatile unsigned char *) 0x8008)
#include <avr/io.h>

void delay(int n)
{
	volatile int i,j;
	for(i=0;i<1000;i++){
		for(j=0;j<n;j++);
	}
}

int main()
{
	MCUCR = 0x80;
	
	while(1)
	{
		EX_LED = 0x55;
		delay(1000);
		EX_LED = 0x00;
		delay(1000);
	}
}