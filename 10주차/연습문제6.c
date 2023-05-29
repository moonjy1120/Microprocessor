#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
//
#define SS_DATA (*(volatile unsigned char *)0x8002)
#define SS_SEL (*(volatile unsigned char *)0x8003)
#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)
//
unsigned char digit[10] = { // 7세그먼트 출력을 위한 배열
	0b00111111,  // 0
	0b00000110,  // 1 
	0b01011011,  // 2 
	0b01001111,  // 3 
	0b01100110,  // 4 
	0b01101101,  // 5 
	0b01111101,  // 6 
	0b00000111,  // 7 
	0b01111111,  // 8
	0b01101111   // 9
};
//
unsigned char fnd_sel[4] = {	//fnd 선택용 배열
	0b11110111,  // 0
	0b11111011,  // 1
	0b11111101,  // 2
	0b11111110   // 3
};
//
void delay(int n)
{
	volatile int i,j;
	for (i=0;i<1000;i++)
	{
		for (j=0;j<n;j++);
	}
}
//
volatile float sec = 0.01; // Interrupt Period
volatile int count = 0;	//카운트 저장 변수
volatile int num = 10;  // fnd 표시값 저장 변수
int number[4] = {0,0,0,0};  // 카운트 값을 저장할 배열
volatile int seed = 0;
volatile int row = 0;
volatile int column = 0;
volatile int n = 100;
//
ISR(TIMER0_OVF_vect) 
{
	if(count++ > n)	//100분의n초
	{
		num --;
		number[3] = (num / 1000) % 10;	
		number[2] = (num / 100) % 10;
		number[1] = (num / 10) % 10;
		number[0] = num % 10;
	
		count = 0;
	}
	
	TCNT0 = 255-((unsigned int)(sec*16000.0));
}

int main(void)
{
	MCUCR = 0x80;
	DDRB = 0x00;
	srand(seed);
	row = rand() % 10;
	seed++;
	srand(seed);
	column = rand() % 10;
	seed++;
	
	TCCR0 = 0x07; // 0000_0111 - Normal Mode, Prescaler : CPU_CLOCK / 1024 = 16Khz
	TIMSK = 0x01; // Enable overflow interrupt
	TCNT0 = 255-((unsigned int)(sec*16000.0));
	sei(); // Enable global interrupts
	
	while (num)
	{
		if(PINB == (10*row)+(9-column))
		{
			TIMSK = 0x00;	//인터럽트 비활성화
			n=n-10;	//난이도 증가
			num = 10;	//제한시간 초기화
			srand(seed);	//좌표 초기화
			row = rand() % 10;
			seed++;
			srand(seed);
			column = rand() % 10;
			seed++;
			while(!(PINB==0b10000000));	//sw7 눌릴때 까지 기다림
			TIMSK = 0x01;	//인터럽트 재활성화
			sei();	
		}
		
		for (int i = 0; i < 4; i++)	// 7세그먼트에 값을 출력
		{
			SS_SEL = fnd_sel[i];
			SS_DATA = digit[number[i]];
			delay(1);
		}
		
		DM_SEL = (1 << (row));	//DM 출력
		DM_DATA = (1 << (column));
	}
	
	DM_SEL = 0x03ff;
	DM_DATA = 0x03ff;
}



