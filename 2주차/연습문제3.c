#define EX_LED (*(volatile unsigned char *) 0x8008)
#include <avr/io.h>

unsigned char pattern[14] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

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
	DDRD=0xc0;
	
	while(1)
	{
		for(int i=0;i<14;i++)
		{
			EX_LED = pattern[i];
			if(i==0)	
				PORTD = 0x40;
			else if(i==7)
				PORTD = 0x80;
			else
				PORTD = 0x00;	
			delay(800);
		}
		
	}
}