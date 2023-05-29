#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
//
#define LCD_CONTROL (*(volatile unsigned char *)0x8001)
#define LCD_DATA (*(volatile unsigned char *)0x8000)
#define DM_SEL (*(volatile unsigned int *)0x8004)
#define DM_DATA (*(volatile unsigned int *)0x8006)
//
#define LCD_EN_ON (LCD_CONTROL = (LCD_CON |= 0x04))
#define LCD_EN_OFF (LCD_CONTROL = (LCD_CON &= 0x0b))
#define LCD_RW_ON (LCD_CONTROL = (LCD_CON |= 0x02))
#define LCD_RW_OFF (LCD_CONTROL = (LCD_CON &= 0x0d))
#define LCD_RS_ON (LCD_CONTROL = (LCD_CON |= 0x01))
#define LCD_RS_OFF (LCD_CONTROL = (LCD_CON &= 0x0e))
//
#define DELAY 1
//
volatile char LCD_CON = 0;
volatile int n = 100;                    // 속도 설정
volatile float sec = 0.01;               // Interrupt Period
volatile int count = 0;                  // 카운트 저장 변수
volatile char score = 0x30;              // 점수 저장 변수
volatile char block_x = 0;               // 블록 좌상 X좌표
volatile char block_y = 0;               // 블록 좌상 Y좌표
volatile int seed = 0;                   // 랜덤블록 생성 시드
volatile int block_pat = 0;              // 현재 블록 패턴 번호
volatile int game_board[10][10] = {0};   // 게임판
volatile int moving_block[10][10] = {0}; // 움직이는 블록
volatile unsigned char block[6][2][3] = {
    //[블록종류][행][열]
    {{1, 1, 0}, {0, 0, 0}}, // ㅡ
    {{1, 1, 0}, {1, 1, 0}}, // ㅁ
    {{1, 0, 0}, {1, 0, 0}}, // ㅣ
    {{1, 0, 0}, {1, 1, 0}}, // ㄴ
    {{1, 1, 0}, {0, 1, 0}}, // ㄱ
    {{0, 1, 1}, {1, 1, 0}}  // ㄹ
};
//
void port_init(void)
{
    MCUCR = 0x80;
    DDRB = 0x00;
    TCCR0 = 0x07; // 0000_0111 - Normal Mode, Prescaler : CPU_CLOCK / 1024 = 16Khz
    TIMSK = 0x01; // Enable overflow interrupt
    TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
    sei(); // Enable global interrupts
}
//
void delay(int cnt)
{
    volatile int i, j;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < 1000; j++)
            ;
    }
}
//
void lcd_control_write(unsigned char value)
{
    LCD_EN_OFF;
    delay(DELAY);
    LCD_RS_OFF;
    delay(DELAY);
    LCD_RW_OFF;
    delay(DELAY);
    LCD_EN_ON;
    delay(DELAY);
    LCD_DATA = value;
    delay(DELAY);
    LCD_EN_OFF;
    delay(DELAY);
}
//
void lcd_data_write(unsigned char value)
{
    LCD_EN_OFF;
    delay(DELAY);
    LCD_RS_ON;
    delay(DELAY);
    LCD_RW_OFF;
    delay(DELAY);
    LCD_EN_ON;
    delay(DELAY);
    LCD_DATA = value;
    delay(DELAY);
    LCD_EN_OFF;
    delay(DELAY);
}
//
void cursor_move(int Xpos, int Ypos) // x:0~15, y:0~1
{
    unsigned char cursor_xy = Xpos + (Ypos == 1 ? 0x80 : 0xc0);
    lcd_control_write(cursor_xy);
}
//
void lcd_init()
{
    delay(1000);             // local initialization
    lcd_control_write(0x38); // fuction set
    lcd_control_write(0x01); // clear display
    lcd_control_write(0x0c); // display on, cursor off, cursor blink-off
}
//
void new_block()
{
    srand(seed++);
    block_pat = rand() % 6;
    block_x = 4;
    block_y = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            moving_block[i + block_y][j + block_x] = block[block_pat][i][j];
        }
    }
}
//
int block_move_check(int x) // Player1 떨어진 블록과 움직이는 블록 충돌 체크
{
    if (block_y == 8)
        return 0; // 블록이 바닥에 닿았을 때

    for (int i = 0; i < 3; i++)
    {
        if (game_board[block_y + 2][block_x + i] == 1) // 블록이 블록에 닿아있을 때
            return 0;
    }

    if (block_x + x < 0) // 블록이  왼쪽벽에 닿아있을 때
        return 0;

    if (block_x + x > 7 && (block_pat == 5)) // 블록이  오른쪽벽에 닿아있을 때
        return 0;

    if (block_x + x > 8 && (block_pat == 0 || block_pat == 1 || block_pat == 3 || block_pat == 4))
        return 0;

    if (block_x + x > 9 && (block_pat == 2))
        return 0;

    return 1; // 블록이 움직일 수 있을 때
}
//
void moving_block_clear() // 움직이는 블록 정보 클리어
{
    for (int j = 0; j < 10; j++)
    {
        for (int k = 0; k < 10; k++)
        {
            moving_block[j][k] = 0;
        }
    }
}
//
void block_stack() // 움직이는 블록 정보 복사 (블록 쌓기)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            game_board[i][j] |= moving_block[i][j];
        }
    }
}
//
void moving_block_refresh() // 움직이는 블록 정보 새로고침 (블록 이동)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            moving_block[i + block_y][j + block_x] = block[block_pat][i][j];
        }
    }
}
//
ISR(TIMER0_OVF_vect)
{
    if (count++ > n) // 0.01 * n초 마다 실행
    {
        int down_stop = 0;
        count = 0;
        down_stop = block_move_check(0); // 한 칸 아래 이동 가능한지 조사

        if (down_stop != 0) // 아래로 이동 가능하면 현재 블록 지우고 이동
        {
            moving_block_clear();
            block_y += 1;
            moving_block_refresh();
        }

        if (down_stop == 0) // 블록이 아래로 이동 불가
        {
            block_stack();               // 블록 쌓기
            moving_block_clear();        // 현재 움직이는 블록 지우기
            for (int j = 0; j < 10; j++) // 꽉찬 줄이 있는지 확인
            {
                n = 1;
                for (int k = 1; k < 10; k++)
                {
                    n = n * game_board[j][k];
                }
                if (n == 1) // 한 줄 채워짐
                {
                    for (int m = j; m > 1; m--) // 윗 줄 아래로 당기기
                    {
                        for (int k = 1; k < 10; k++)
                        {
                            game_board[m][k] = game_board[m - 1][k];
                        }
                    }
                    score++;    // 한 줄 지울때마다 점수 플러스
                    n = n - 10; // 게임속도 증가
                }
            }
            new_block(); // 새로운 블록 생성
        }
    }
    TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
}
//
int main(void)
{
    int data = 0x0000;
    port_init();
    lcd_init();
    new_block();
    while (1)
    {
        if (PINB == 0b00000001) // stop
        {
            TIMSK = 0x00;
        }

        else if (PINB == 0b00000010) // move right
        {
            TIMSK = 0x01;
            sei();
            int right_stop = 0;
            right_stop = block_move_check(1);

            if (right_stop != 0)
            {
                moving_block_clear();   // 현재 블록 데이터 클리어
                block_x += 1;           // 블록 X 좌표 +1
                moving_block_refresh(); // 블록 이동
            }
        }

        else if (PINB == 0b00000100) // move left
        {
            TIMSK = 0x01;
            sei();
            int left_stop = 0;
            left_stop = block_move_check(-1);

            if (left_stop != 0)
            {
                moving_block_clear();   // 현재 블록 데이터 클리어
                block_x -= 1;           // 블록 X 좌표 -1
                moving_block_refresh(); // 블록 이동
            }
        }

        // display dot matrix
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (game_board[i][j] || moving_block[i][j])
                {
                    data |= (0x0001 << (9 - j));
                }
            }
            DM_SEL = 0x0001 << i;
            DM_DATA = data;
            delay(1);
            data = 0x0000;
        }

        // display lcd
        for (int i = 0; i < 3; i++)
        {
            cursor_move(i, 1);
            if (block[block_pat][0][i] == 1)
                lcd_data_write('0');
        }
        for (int i = 0; i < 3; i++)
        {
            cursor_move(i, 0);
            if (block[block_pat][1][i] == 1)
                lcd_data_write('0');
        }

        cursor_move(15, 0);
        lcd_data_write(score);
    }
}