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
volatile int score = 0;  // fnd에 표시할 점수
volatile int number[4] = {0,0,0,0};  // 카운트 값을 저장할 배열
volatile int bar_arr[10] = {0,0,0,1,1,1,1,0,0,0};
volatile int bar = 0;
volatile int seed = 0;
volatile int ball_row = 0;
volatile int ball_column = 0;
volatile int n = 100;
volatile int temp = 0;	//배열 시프트용
//
ISR(TIMER0_OVF_vect)
{
	if(count++ > n)	//1초
	{
		ball_row = ball_row +1;
		count = 0;
	}
	DM_SEL = (1 << (ball_row));
	DM_DATA = (1 << (ball_column));
	
	TCNT0 = 255-((unsigned int)(sec*16000.0));
}

int main(void)
{
	MCUCR = 0x80;
	DDRB = 0x00;
	TCCR0 = 0x07; // 0000_0111 - Normal Mode, Prescaler : CPU_CLOCK / 1024 = 16Khz
	TIMSK = 0x01; // Enable overflow interrupt
	TCNT0 = 255-((unsigned int)(sec*16000.0));
	sei(); // Enable global interrupts
	
	srand(seed);	//좌표 초기화
	ball_column = rand() % 10;
	seed++;
	
	while (1)
	{
		if((ball_row==9) && ((bar_arr[9-ball_column])))	//점수획득
		{
			score = score +1;
			n = n - 10;
			ball_row = 0;
			srand(seed);	//좌표 초기화
			ball_column = rand() % 10;
			seed++;
		}
		
		else if (ball_row==9)
		{
			break;
		}
		
		if(PINB == 0b00000001)	//우로이동
		{
			temp = bar_arr[9];
			for (int i = 8; i >= 0; i--)
			bar_arr[i + 1] = bar_arr[i];
			bar_arr[0] = temp;
			delay(100);
		}
		
		if(PINB == 0b00000100)	//좌로이동
		{
			temp = bar_arr[0];
			for (int i = 0; i <= 8; i++)
			bar_arr[i] = bar_arr[i+1];
			bar_arr[9] = temp;
			delay(100);
		}
		
		number[3] = (score / 1000) % 10;
		number[2] = (score / 100) % 10;
		number[1] = (score / 10) % 10;
		number[0] = score % 10;
		for (int i = 0; i < 4; i++)	// 7세그먼트에 값을 출력
		{
			SS_SEL = fnd_sel[i];
			SS_DATA = digit[number[i]];
			delay(1);
		}
		
		DM_SEL = (1 << (ball_row));
		DM_DATA = (1 << (ball_column));
		
		DM_SEL = 0x0200;
		for(int i =0; i<10; i++)
		{
			if(bar_arr[i])
			{
				bar |= (1<< (9-i));
			}
		}
		DM_DATA = bar;
		bar = 0;
		
	}
	

}



