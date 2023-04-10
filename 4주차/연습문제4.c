#include <avr/io.h>
#include <stdlib.h>
//
#define SS_DATA (*(volatile unsigned char *)0x8002)
#define SS_SEL (*(volatile unsigned char *)0x8003)
#define EX_LED (*(volatile unsigned char *)0x8008)
//
int seed = 0;
unsigned char led_mapping[8];
unsigned char reference[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
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
void random_mapping()
{
	srand(seed);
	seed++;
	for (int i = 0; i < 8; i++)
	{
		int random_index = rand() % (8 - i);
		led_mapping[i] = reference[random_index];
		reference[random_index] = reference[7 - i];
	}
}
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
int main()
{
	MCUCR=0x80;	//외부 led 제어용
	DDRB=0x00;	//스위치 입력세팅
	EX_LED=0x00;
	
	int sw_count = 0;// 스위치 입력 횟수
	int fnd_index = 0;	// fnd 인덱스
	
	random_mapping();

	while (1)
	{
		if(PINB)	//스위치 눌림
		{
			int input = PINB;
			sw_count++;
			delay(300);
			
			for (int i = 0; i < 8; i++) //버튼 입력에 맞춰 LED 점등
			{
				SS_SEL = fnd_sel[fnd_index];
				SS_DATA = digit[sw_count];
				if (input == (1 << i))
				{
					EX_LED |= led_mapping[i];
				}
			}
			
			if(sw_count>9)	//스위치 10번째 눌렸을때
			{
				random_mapping();
				sw_count=0;
				fnd_index++;
				EX_LED=0x00;
				if(fnd_index >3)	// i>3 인경우 종료
				{
					EX_LED=0x00;
					for (int k = 0; k < 4; k++)
					{
						SS_SEL = fnd_sel[3-k];
						SS_DATA = 0;
						delay(1);
					}
					
					break;
				}
				
				SS_SEL = fnd_sel[fnd_index];
				SS_DATA = digit[sw_count];
			}
		}
		//스위치 안눌림
		SS_SEL = fnd_sel[fnd_index];
		SS_DATA = digit[sw_count];
	}
	return 0;
}
