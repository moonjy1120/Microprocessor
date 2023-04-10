#define F_CPU 16000000UL
#define SS_DATA (*(volatile unsigned char *)0x8002)
#define SS_SEL (*(volatile unsigned char *)0x8003)
#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

volatile int t = 10; // 두더지가 사라지는 시간 (t초)
volatile int seed = 0;
volatile int player_x = 0;
volatile int player_y = 9;
volatile int mole_x, mole_y;
volatile int score = 0;
volatile int number[4];

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

unsigned char fnd_sel[4] = { // fnd 선택용 배열
	0b11110111,  // 0
	0b11111011,  // 1
	0b11111101,  // 2
	0b11111110   // 3
};

void display_dm(int x, int y) // dot_matrix 표시 함수
{
	DM_SEL = (1 << (9-y));
	DM_DATA = (1 << (9-x));
	_delay_ms(5);
}

void display_fnd(int score) // fnd 표시 함수
{
	number[3] = (score / 1000) % 10;
	number[2] = (score / 100) % 10;
	number[1] = (score / 10) % 10;
	number[0] = score % 10;
	for (int i = 0; i < 4; i++)
	{
		SS_SEL = fnd_sel[i];
		SS_DATA = digit[number[i]];
		_delay_ms(1);
	}
}

void move_player(int input)
{
	if (input == 0x08) player_y = (player_y == 9) ? 0 : player_y + 1; // 위로 이동
	else if (input == 0x04) player_y = (player_y == 0) ? 9 : player_y - 1; // 아래로 이동
	else if (input == 0x02) player_x = (player_x == 0) ? 9 : player_x - 1; // 좌로 이동
	else if (input == 0x01) player_x = (player_x == 9) ? 0 : player_x + 1;// 우로 이동
}

int check_hit()
{
	return ((player_x == mole_x)&(player_y == mole_y));
}

void generate_mole()
{
	srand(seed);
	seed = rand();
	mole_x = rand() % 10;
	mole_y = rand() % 10;
}

int main(void)
{
	MCUCR = 0x80;
	DDRB = 0x00;

	uint32_t start_time = 0;
	uint32_t end_time = t * 10000;
	uint32_t mole_timer = 0;
	
	generate_mole();

	while (start_time < end_time)
	{
		if (mole_timer >= t * 5000)	//5초후 두더지 초기화
		{
			generate_mole();
			display_dm(player_x,player_y);
			display_dm(mole_x,mole_y);
			display_fnd(score);
			mole_timer = 0;
		}

		if (PINB)
		{
			int input = PINB;
			move_player(input);
			_delay_ms(300);
			if (check_hit())
			{
				score++;
				generate_mole();
				display_dm(player_x,player_y);
				display_dm(mole_x,mole_y);
				display_fnd(score);
				if (score >= 7)
				{
					break;
				}
			}
			display_dm(player_x,player_y);
			display_dm(mole_x,mole_y);
			display_fnd(score);
		}
		
		display_dm(player_x,player_y);
		display_dm(mole_x,mole_y);
		display_fnd(score);
		
		_delay_ms(1);
		start_time++;
		mole_timer++;
	}
	return 0;
}

