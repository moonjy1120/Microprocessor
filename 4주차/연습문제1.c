#include <avr/io.h>
//
#define SS_DATA (*(volatile unsigned char *)0x8002)
#define SS_SEL (*(volatile unsigned char *)0x8003)
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
int main()
{
	MCUCR = 0x80;

	int count = 0;	//카운트 저장 변수
	int num = 0;  // fnd 표시값 저장 변수

	while (1) 
	{
		int number[4];  // 카운트 값을 저장할 배열
		
		count++;	
		if (count==50)	//카운트 속도조절
		{
			count=0;
			num++;
		}
		
		if (num >= 10000)
		{
			num = 0;	  // 출력값을 초기화하여 0부터 다시 시작
		}
		
		// fnd 표시값을 계산
		number[3] = (num / 1000) % 10;	
		number[2] = (num / 100) % 10;
		number[1] = (num / 10) % 10;
		number[0] = num % 10;

		for (int i = 0; i < 4; i++)	// 7세그먼트에 값을 출력
		{
			SS_SEL = fnd_sel[i];
			SS_DATA = digit[number[i]];
			delay(1);
		}
	}
}



