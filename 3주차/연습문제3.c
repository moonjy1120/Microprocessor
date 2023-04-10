#include <avr/io.h>
//
#define EX_LED (*(volatile unsigned char *) 0x8008)
//
void delay(int n)
{
	volatile int i,j;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<n;j++);
	}
}
//
int main()
{
	DDRB = 0x00;
	MCUCR = 0x80;
	unsigned char state = 0x00;
	unsigned char count = 0;
	unsigned char threshold = 2;	//threshold 설정
	
	while (1)
	{

		if(count<=threshold)
		{
			if(PINB)
			{
				state |= PINB;
				count++;
				if(count>threshold)	//(threshold+1)번째 스위치 눌린경우 바로 반전시키기 이 경우에는 3번째 버튼 누를때
				{
					EX_LED = ~state;
					delay(1000);
					EX_LED = 0x00;
					delay(1000);
				}
				else
				{
					EX_LED = state;
					delay(1000);
					EX_LED = 0x00;
					delay(1000);
				}
			}
			
			else
			{
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
			}
			
		}
		
		else if (count>threshold)
		{
			if(PINB)
			{
				state |= PINB;
				count++;
				EX_LED = ~state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
			}
			
			else
			{
				EX_LED = ~state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
			}
		}
	}
}


