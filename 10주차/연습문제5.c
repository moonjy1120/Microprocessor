#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
//
#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)
//
volatile int seed = 0;
volatile int row = 0;
volatile int column = 0;
volatile float sec = 0.01; // Interrupt Period
volatile int count = 0;	//카운트 저장 변수
//
ISR(TIMER0_OVF_vect)
{
	if(count++ > 100)	//1초
	{
		srand(seed);
		row = rand() % 10;
		seed++;
		srand(seed);
		column = rand() % 10;
		seed++;
		DM_SEL = (1 << (row));
		DM_DATA = (1 << (column));
		count = 0;
	}
	
	else
	{
		DM_SEL = (1 << (row));
		DM_DATA = (1 << (column));
	}
	
	TCNT0 = 255-((unsigned int)(sec*16000.0));
}

int main(void)
{
	DDRB = 0x00;
	MCUCR = 0x80;
	TCCR0 = 0x07; // 0000_0111 - Normal Mode, Prescaler : CPU_CLOCK / 1024 = 16Khz
	TIMSK = 0x01; // Enable overflow interrupt
	TCNT0 = 255-((unsigned int)(sec*16000.0));
	sei(); // Enable global interrupts
	while (1)
	{
		if (PINB)
		{
			TIMSK = 0x00;
		}

		else
		{
			TIMSK = 0x01;
			sei();
		}
	}
}



