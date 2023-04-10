#include <avr/io.h>
//
#define EX_LED (*(volatile unsigned char *) 0x8008)
//
int main()	
{
	DDRB = 0x00;	
	MCUCR = 0x80;

	while (1)		
	{
		if(PINB)
			EX_LED = 0x80/PINB;
		else
			EX_LED = 0x00;
	}
}