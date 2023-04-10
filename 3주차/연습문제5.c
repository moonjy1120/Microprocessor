#include <avr/io.h>
//
#define EX_LED (*(volatile unsigned char *) 0x8008)  //외부LED 주소
//
unsigned char password[4]= {0x02,0x02,0x04,0x01}	//1120
;
unsigned char flag=0,index=0,state=0,sw;
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
	DDRD=0xc0;	//내부 led 출력세팅
	DDRB=0x00;	//스위치 입력세팅
	while(1) 
	{
		sw=PINB;
		if(sw) 
		{
			if(sw==password[index]) 
			{
				index++;
				delay(100);
				if(index==3)	//다 맞춘 상태
				{
					delay(900);
					for(int i=0;i<4;i++)
					{
						delay(500);
						EX_LED |= password[i];
					}
					delay(3000);
					EX_LED = 0x00;
				}
					
			} 
			else	//중간에 틀린경우
			{
				state = 0x00;
				for(int i=0;i<4;i++)
					{
						state |= password[i];
					}
				EX_LED = ~state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				EX_LED = ~state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				EX_LED = ~state;
				delay(1000);
				EX_LED = 0x00;
				index=0;
			}
		}
	} 
}