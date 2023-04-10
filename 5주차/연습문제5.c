#include <avr/io.h>
//
#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)
//
int word[8][10] = {
	{
		0b0000000000,	//H = 0
		0b0010000100,
		0b0010000100,
		0b0010000100,
		0b0011111100,
		0b0010000100,
		0b0010000100,
		0b0010000100,
		0b0010000100,
		0b0000000000
		},{
		0b0000000000,	//E = 1
		0b0011111100,
		0b0010000000,
		0b0010000000,
		0b0011111100,
		0b0010000000,
		0b0010000000,
		0b0010000000,
		0b0011111100,
		0b0000000000
		},{
		0b0000000000,	//L = 2
		0b0010000000,
		0b0010000000,
		0b0010000000,
		0b0010000000,
		0b0010000000,
		0b0010000000,
		0b0010000000,
		0b0011111100,
		0b0000000000
		},{
		0b0000000000,	//O = 3
		0b0000110000,
		0b0001001000,
		0b0010000100,
		0b0010000100,
		0b0010000100,
		0b0010000100,
		0b0001001000,
		0b0000110000,
		0b0000000000
		},{
		0b0000000000,	//W = 4
		0b0100000010,
		0b0100000100,
		0b0100000100,
		0b0010100100,
		0b0010101000,
		0b0010101000,
		0b0001101000,
		0b0000110000,
		0b0000000000
		},{
		0b0000000000,	//R = 5
		0b0111110000,
		0b0100010000,
		0b0100010000,
		0b0111110000,
		0b0101000000,
		0b0100100000,
		0b0100010000,
		0b0100001000,
		0b0000000000
		},{
		0b0000000000,	//D = 6
		0b0111110000,
		0b0110011000,
		0b0110001100,
		0b0110001100,
		0b0110001100,
		0b0110001100,
		0b0110011000,
		0b0111110000,
		0b0000000000
		},{
		0b0000000000,	//X = 7
		0b0000000000,
		0b0000000000,
		0b0000000000,
		0b0000000000,
		0b0000000000,
		0b0000000000,
		0b0000000000,
		0b0000000000,
		0b0000000000
		}
};
//
int character_index = 7;
//
void delay(int n)
{
	volatile int i, j;
	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < n; j++);
	}
}
//
void display_character(int character_index)
{
	for(int k = 0; k < 10; k++)
	{
		DM_SEL = (1<<(k));
		DM_DATA = (word[character_index][k]);
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
				return i;
			}
		}
	}
	return 0xff;
}
//
int main(void)
{
	MCUCR = 0x80;
	DDRB = 0x00;

	while (1)
	{
		int input = get_switch_input();
		
		if (input != 0xff)	//스위치 눌림
		{
			if (input==7)
			{
				character_index=0;
				display_character(character_index); // H 출력
			}
			
			else if (input==6)
			{
				character_index=1;
				display_character(character_index); // E 출력
			}
			
			else if (input==5)
			{
				character_index=2;
				display_character(character_index); // L 출력
			}
			
			else if (input==4)
			{
				character_index=3;
				display_character(character_index); // O 출력
			}
			
			else if (input==3)
			{
				character_index=4;
				display_character(character_index); // W 출력
			}
			
			else if (input==2)
			{
				character_index=5;
				display_character(character_index); // R 출력
			}
			else if (input==1)
			{
				character_index=6;
				display_character(character_index); // D 출력
			}
			else if (input==0)
			{
				character_index=7;
				display_character(character_index); // 출력없음
			}
		}
		display_character(character_index);
	}

	return 0;
}
