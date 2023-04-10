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
int main(void)
{
	MCUCR=0x80;	//fnd 제어용
	DDRB=0x00;	//스위치 입력세팅
	
	int password[8] = {0x02, 0x20, 0x08, 0x04, 1, 5, 3, 2}; //1532
	int index=0,sw;
	
	while(1)
	{
		sw=PINB;
		if(sw)
		{
			if(sw==password[index])
			{
				index++;
				delay(300);
				if(index==4)	//다 맞춘 상태, 1초 간격 점멸
				{
					while(1)
					{
						for (int i=0;i<10;i++)
						{
							for (int j=0;j<i;j++)
							{
								for (int k = 0; k < 4; k++)
								{
									SS_SEL = fnd_sel[3-k];
									SS_DATA = digit[password[k+4]];
									delay(1);
								}
							}
						}
						
						for (int i = 0; i < 4; i++)	//fnd 끄기
						{
							SS_SEL = fnd_sel[3-i];
							SS_DATA = 0;
							delay(1);
						}
						
						delay(1000);	
					}
				}
				
			}
			
			else if(sw==0b00101110)	//한번에 입력되는 경우
			{
				while(1)
				{
					for (int i = 0; i < 4; i++)
					{
						SS_SEL = fnd_sel[3-i];
						SS_DATA = digit[password[i+4]];
						delay(1);
					}
				}
			}
			
			else	//중간에 틀린경우
			{
				while(1)
				{
					for (int i = 0; i < 4; i++)
					{
						SS_SEL = fnd_sel[3-i];
						SS_DATA = 0;
						delay(1);
					}
				}
			}
		}
	}
}