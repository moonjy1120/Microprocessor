#include <avr/io.h>
//
#define EX_LED (*(volatile unsigned char *) 0x8008)  //외부LED 주소
//
void delay(int n)	//딜레이 함수
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
	DDRB = 0x00;   //스위치 입력
	MCUCR = 0x80;    //외부 led 출력세팅
	DDRD = 0xc0;	//내부 led 출력세팅
	unsigned char password[4] = {0x80, 0x40, 0x20, 0x10}; //7654
	unsigned char match = 0;	//비밀번호 맞춘수
	unsigned char state = 0x00;	//LED 상태변수
	
	while (1)
	{
		switch(match)
		{
			case 0:	//초기상태 맞춘수 0
			if(PINB && (PINB == password[0]))
			{
				match++;
				state |= password[0];
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			else if(PINB == 0x00)
			{
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			break;
			
			
			case 1:	//맞춘수 1
			if(PINB && (PINB == password[1]))
			{
				match++;
				state |= password[1];
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			else if(PINB == 0x00)
			{
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			break;
			
			
			case 2:	//맞춘수 2
			if(PINB && (PINB == password[2]))
			{
				match++;
				state |= password[2];
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			else if(PINB == 0x00)
			{
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			break;
			
			
			case 3:	//맞춘수 3
			if(PINB && (PINB == password[3]))
			{
				match++;
				state |= password[3];
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			else if(PINB == 0x00)
			{
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			break;
			
			
			case 4:	//다맞춤
			if(PINB && (PINB == 0x01))
			{
				PORTD=0x80;
				match++;
				delay(1000);
				continue;
			}
			else if(PINB == 0x00)
			{
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				continue;
			}
			continue;
			
			
			case 5:	//비번설정 1
			if(PINB)
			{
				PORTD=0xc0;
				match++;
				state=0x00;	//LED 상태변수 초기화
				password[0] = PINB;
				delay(300);
				continue;
			}
			continue;
			
			
			case 6:	//비번설정 2
			if(PINB)
			{
				match++;
				password[1] = PINB;
				delay(300);
				continue;
			}
			continue;
			
			case 7:	//비번설정 3
			if(PINB)
			{
				match++;
				password[2] = PINB;
				delay(300);
				continue;
			}
			continue;
			
			
			case 8:	//비번설정 4
			if(PINB)
			{
				password[3] = PINB;
				for(int i=0;i<4;i++)
				{
					state|=password[i];
				}
				EX_LED = state;
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				match=0;	//CASE 0로 돌아가기
				state=0x00;	//LED 상태변수 초기화
				continue;
			}
			
			default:
			continue;
			
		}
		break;
	}
	//중간에 틀린경우
	PORTD =0xc0;
	delay(1000);
	PORTD =0x00;
	delay(1000);
}

