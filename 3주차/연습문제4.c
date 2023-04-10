#include <avr/io.h>
//
#define EX_LED (*(volatile unsigned char *) 0x8008)  //외부LED 주소
//
unsigned char password[8]= {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};	//비밀번호 76543210
unsigned char flag=0,index=0,sw;
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
				delay(300);
				if(index==8)	//다 맞춘 상태
				{
					while(1)
					{
						EX_LED=0xff;
						delay(1000);
						EX_LED=0x00;
						delay(1000);
						index=0;
					}
				}
					
			} 
			else	//중간에 틀린경우
			{
				PORTD=0xc0;
				delay(1000);
				PORTD=0x00;
				delay(1000);
				PORTD=0xc0;
				delay(1000);
				PORTD=0x00;
				break;
			}
			
		} 
	}
}