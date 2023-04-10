#include <avr/io.h>

#define EX_LED (*(volatile unsigned char*)0x8008)
#define BRIGHTNESS 255

void delay(long int i)
{
	volatile char k;
	while(i--)
	{
		for(k=0; k<30; k++);
	}
}

unsigned char patterns[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

int main(void)
{
	MCUCR=0x80;
	EX_LED=0x00;
	unsigned char led_state=0b00000000;
	
	while(1)
	{
		for(int i=0;i<8;i++)
		{
			for(int j=0;j<BRIGHTNESS;j++)
			{
				led_state|=patterns[i];
				EX_LED=led_state;
				delay(j);
				EX_LED=(~(0b11111111>>(i)));
				delay(BRIGHTNESS-j);
			}
		}
		EX_LED=0x00;
		led_state=0b00000000;
	}
}