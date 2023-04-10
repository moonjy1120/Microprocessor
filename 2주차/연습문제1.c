#define EX_LED (*(volatile unsigned char *) 0x8008)
#include <avr/io.h>

int main()
{
	MCUCR = 0x80;
	
	while(1)
	{
		EX_LED = 0x80;
	}
}
