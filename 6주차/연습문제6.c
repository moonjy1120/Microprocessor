#include <avr/io.h>

#define EX_LED (*(volatile unsigned char*)0x8008)
#define BRIGHTNESS 500

int ledbrightness[4][8] = {	//0~7번 led
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}
};

int sw_cnt[8] = {0,0,0,0,0,0,0,0};	//0~7번 스위치

int selected_led = 0;	//선택된 led

void delay(long int i)
{
	volatile char k;
	while(i--)
	for(k=0; k<30; k++);
}

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

int main(void)
{
	DDRB = 0x00;    // 스위치 입력 설정
	MCUCR=0x80;
	int row =0;
	int val = 0b00000000;
	while(1)
	{
		int input = get_switch_input();
		if (input != 0xff)	//스위치 눌림
		{
			switch(input)
			{
				case 0:
				ledbrightness[0][selected_led]=1;
				ledbrightness[1][selected_led]=1;
				ledbrightness[2][selected_led]=1;
				ledbrightness[3][selected_led]=1;
				break;
				
				case 1:
				ledbrightness[0][selected_led]=1;
				ledbrightness[1][selected_led]=1;
				ledbrightness[2][selected_led]=1;
				ledbrightness[3][selected_led]=0;
				break;
				
				case 2:
				ledbrightness[0][selected_led]=1;
				ledbrightness[1][selected_led]=1;
				ledbrightness[2][selected_led]=0;
				ledbrightness[3][selected_led]=0;
				break;
				
				case 3:
				ledbrightness[0][selected_led]=1;
				ledbrightness[1][selected_led]=0;
				ledbrightness[2][selected_led]=0;
				ledbrightness[3][selected_led]=0;
				break;
				
				case 4:
				ledbrightness[0][selected_led]=0;
				ledbrightness[1][selected_led]=0;
				ledbrightness[2][selected_led]=0;
				ledbrightness[3][selected_led]=0;
				break;
				
				case 5:
				EX_LED = (1<< selected_led);
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				EX_LED = (1<< selected_led);
				delay(1000);
				EX_LED = 0x00;
				delay(1000);
				break;
				
				case 6:
				selected_led =(selected_led == 0) ? 7 : selected_led - 1;
				break;
				
				case 7:
				selected_led =(selected_led == 7) ? 0 : selected_led + 1;
				break;
			}
			delay(10000);
		}
		
		val = 0b00000000;
		for(int i=8;i>0;i--)
		{
			val |= (ledbrightness[row][i-1] << (i-1));
		}
		EX_LED=val;
		delay(25);
		
		if(row==3)
		row=0;
		else
		row++;
	}

}