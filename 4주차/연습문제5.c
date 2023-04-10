#include <avr/io.h>
//
#define SS_DATA (*(volatile unsigned char *)0x8002)
#define SS_SEL (*(volatile unsigned char *)0x8003)
//
unsigned char digit[11] = {
	0b00111111,  // 0
	0b00000110,  // 1
	0b01011011,  // 2
	0b01001111,  // 3
	0b01100110,  // 4
	0b01101101,  // 5
	0b01111101,  // 6
	0b00000111,  // 7
	0b01111111,  // 8
	0b01101111,  // 9
	0b00000000	 // X
};
//
unsigned char fnd_sel[4] = {
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
int get_switch_input()	//몇번 스위치인지 판정
{
	if(PINB)
	{
		int sw_input = PINB;
		for (int i = 0; i < 8; i++)
		{
			if (sw_input == (1 << i))
			{
				delay(300);
				return i;
			}
		}
	}
	delay(300);
	return 0xff;
}
//   
int main()
{
	DDRB=0x00;	//스위치 입력세팅
	
	int attempts = 0;	//시도
	int secret_number = 1532;	//사전정의 숫자
	int input_number[5] = {10, 10, 10, 10, 0};
	int input_number_index = 0;
	int input_position = 1000;	//천의자리 부터 입력

	while (attempts < 10)
	{
		int input = get_switch_input();
		
		if (input != 0xff)	//스위치 눌림
		{
			input_number[input_number_index] = input;	//입력한 4자리 저장
			input_number_index++;
			input_number[4] += (input*input_position);
			input_position /= 10;
			delay(100);

			if (input_number_index == 4)	//4자리 입력완료
			{
				for (int i=0;i<10;i++)	//1초동안 입력된 숫자 출력
				{
					for (int j=0;j<i;j++)
					{
						for (int k = 0; k < 4; k++)
						{
							SS_SEL = fnd_sel[3-k];
							SS_DATA = digit[input_number[k]];
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
				
				if (input_number[4] == secret_number)
				{
					while(1)	// PASS
					{
						SS_SEL = fnd_sel[3];
						SS_DATA = 0b01110011;	//p
						delay(1);
						
						SS_SEL = fnd_sel[2];
						SS_DATA = 0b01011111;	//a
						delay(1);
						
						SS_SEL = fnd_sel[1];
						SS_DATA = 0b00101101;	//s
						delay(1);
						
						SS_SEL = fnd_sel[0];
						SS_DATA = 0b00101101;	//s
						delay(1);
					}
				}
				
				else
				{
					if (input_number[4] < secret_number)
					{
						for (int i=0;i<10;i++)	//1초동안 DOWN 출력
						{
							for (int j=0;j<i;j++)
							{
								SS_SEL = fnd_sel[3];
								SS_DATA = 0b01011110;	//d
								delay(1);
								
								SS_SEL = fnd_sel[2];
								SS_DATA = 0b01011100;	//o
								delay(1);
								
								SS_SEL = fnd_sel[1];
								SS_DATA = 0b01101010;	//w
								delay(1);
								
								SS_SEL = fnd_sel[0];
								SS_DATA = 0b01010100;	//n
								delay(1);
							}
						}
						
						for (int i = 0; i < 4; i++)	//fnd 끄기
						{
							SS_SEL = fnd_sel[3-i];
							SS_DATA = 0;
							delay(1);
						}
					}
					
					else
					{
						for (int i=0;i<10;i++)	//1초동안 UP 출력
						{
							for (int j=0;j<i;j++)
							{
								SS_SEL = fnd_sel[2];
								SS_DATA = 0b00011100;	//u
								delay(1);
								
								SS_SEL = fnd_sel[1];
								SS_DATA = 0b01110011;	//p
								delay(1);
							}
						}
						
						for (int i = 0; i < 4; i++)	//fnd 끄기
						{
							SS_SEL = fnd_sel[3-i];
							SS_DATA = 0;
							delay(1);
						}
					}
					
					input_position = 1000;
					input_number[0] = 10;
					input_number[1] = 10;
					input_number[2] = 10;
					input_number[3] = 10;
					input_number[4] = 0;
					input_number_index = 0;
					attempts++;
				}
			}
		}
	}
	
	while(1)	// FAIL
	{
		SS_SEL = fnd_sel[3];
		SS_DATA = 0b01110001;	//f
		delay(1);
		
		SS_SEL = fnd_sel[2];
		SS_DATA = 0b01011111;	//a
		delay(1);
		
		SS_SEL = fnd_sel[1];
		SS_DATA = 0b00010001;	//i
		delay(1);
		
		SS_SEL = fnd_sel[0];
		SS_DATA = 0b00111000;	//l
		delay(1);
	}
	
	return 0;
}
