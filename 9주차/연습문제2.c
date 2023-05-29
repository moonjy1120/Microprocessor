#include <avr/io.h>
#include <avr/interrupt.h>

volatile float sec = 0.001; // Interrupt Period

ISR(TIMER0_OVF_vect) 
{
	static unsigned char speaker = 0xff;
	static unsigned char timer_count = 0;
	
	timer_count++;
	if (timer_count == 1) 
	{
		PORTG = speaker;
		speaker ^= 0xff;
		timer_count=0;
	}
	TCNT0 = 255-((unsigned int)(sec*64000.0));
}

int main(void)
{
	DDRG = 0xff; // Set PORTG as output
	TCCR0 = 0x06; //   0000_0110 - Normal Mode, Prescaler : CPU_CLOCK / 256 = 64Khz
	TIMSK = 0x01; // Enable overflow interrupt
	TCNT0 = 255-((unsigned int)(sec*64000.0));
	sei(); // Enable global interrupts
	while (1); // main loop
}
