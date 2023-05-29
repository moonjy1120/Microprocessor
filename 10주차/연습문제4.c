#include <avr/io.h>
#include <avr/interrupt.h>
//
#define SS_DATA (*(volatile unsigned char *)0x8002)
#define SS_SEL (*(volatile unsigned char *)0x8003)
//
unsigned char digit[10] = {
	// 7세그먼트 출력을 위한 배열
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111	// 9
};
//
unsigned char fnd_sel[4] = {
	// fnd 선택용 배열
	0b11110111, // 0
	0b11111011, // 1
	0b11111101, // 2
	0b11111110	// 3
};
//
void delay(int n)
{
	volatile int i, j;
	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < n; j++)
			;
	}
}
//
volatile float sec = 0.01;	  // Interrupt Period
volatile int count = 0;		  // 카운트 저장 변수
volatile int num = 0;		  // fnd 표시값 저장 변수
int number[4] = {0, 0, 0, 0}; // 카운트 값을 저장할 배열
//
ISR(TIMER0_OVF_vect)
{
	if (count++ > 100)
	{
		num++;
		number[3] = (num / 1000) % 10;
		number[2] = (num / 100) % 10;
		number[1] = (num / 10) % 10;
		number[0] = num % 10;
		count = 0;
	}

	TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
}

int main(void)
{
	MCUCR = 0x80;
	TCCR0 = 0x07; // 0000_0111 - Normal Mode, Prescaler : CPU_CLOCK / 1024 = 16Khz
	TIMSK = 0x01; // Enable overflow interrupt
	TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
	sei(); // Enable global interrupts
	while (1)
	{
		for (int i = 0; i < 4; i++) // 7세그먼트에 값을 출력
		{
			SS_SEL = fnd_sel[i];
			SS_DATA = digit[number[i]];
			delay(1);
		}
	}
}
