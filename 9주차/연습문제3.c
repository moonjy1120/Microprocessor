#include <avr/io.h>
#include <avr/interrupt.h>

#define EX_LED (*(volatile unsigned char *)0x8008)

float sec = 0.01; // Interrupt Period

ISR(TIMER0_OVF_vect)
{
	static unsigned int i = 0;
	static unsigned char state = 0;

	i++;
	if (state)
	{
		EX_LED = (0x80 >> ((i / 100) % 8));
		state = 0;
	}

	else
	{
		EX_LED = 0xff;
		state = 1;
	}
	TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
}

int main(void)
{
	MCUCR = 0x80; // Set MCUCR
	TCCR0 = 0x07; //   0000_0111 - Normal Mode, Prescaler : CPU_CLOCK / 1024 = 16Khz
	TIMSK = 0x01; // Enable overflow interrupt
	TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
	sei(); // Enable global interrupts
	while (1)
		; // main loop
}
