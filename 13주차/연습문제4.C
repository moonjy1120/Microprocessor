#define F_CPU 16000000UL
#define BAUDRATE 70000
#define LCD_CONTROL (*(volatile unsigned char *)0x8001)
#define LCD_DATA (*(volatile unsigned char *)0x8000)
#define LCD_EN_ON (LCD_CONTROL = (LCD_CON |= 0x04))
#define LCD_EN_OFF (LCD_CONTROL = (LCD_CON &= 0x0b))
#define LCD_RW_ON (LCD_CONTROL = (LCD_CON |= 0x02))
#define LCD_RW_OFF (LCD_CONTROL = (LCD_CON &= 0x0d))
#define LCD_RS_ON (LCD_CONTROL = (LCD_CON |= 0x01))
#define LCD_RS_OFF (LCD_CONTROL = (LCD_CON &= 0x0e))
#define DELAY 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

volatile char LCD_CON = 0;
volatile float sec = 0.01; // Interrupt Period
volatile int t = 20;       // 속도 설정
volatile int n = 10;       // 범위 설정
volatile int score = 0;    // 점수
volatile int count = 0;    // Interrupt Count

void delay(int cnt)
{
    volatile int i, j;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < 1000; j++)
            ;
    }
}

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

void lcd_str_write(char *Str)
{
    while (*Str)
    {
        lcd_data_write(*Str++);
    }
}

void cursor_move(int Xpos, int Ypos) // x:0~15, y:0~1
{
    unsigned char cursor_xy = Xpos + (Ypos == 1 ? 0x80 : 0xc0);
    lcd_control_write(cursor_xy);
}

void lcd_init()
{
    delay(1000);             // local initialization
    lcd_control_write(0x38); // fuction set
    lcd_control_write(0x01); // clear display
    lcd_control_write(0x0c); // display on, cursor off, cursor blink-off
}

void sendBin(char c)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ; // 1<<UDRE0 => 0x20

    if (c >= 0 && c <= 9)
    {
        UDR0 = c + 0x30; // 숫자를 ASCII 코드로 변환하여 출력
    }
}

void sendChar(char c)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ; // 1<<UDRE0 => 0x20
    UDR0 = c;
}

unsigned char receiveChar(void)
{
    if (t > 0)
    {
        while (!(UCSR0A & (1 << RXC0)))
            ;
        return UDR0;
    }
}

void sendStr(char *str)
{
    while (*str)
        sendChar(*str++);
}

void port_init(void)
{
    MCUCR = 0x80;
    UCSR0A = 0x00;
    UCSR0B = 0x18; // Tx, Rx enable
    UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit
    UBRR0H = 0x00;
    UBRR0L = (14745600 / (16 * BAUDRATE)) - 1; // 12
    TCCR0 = 0x07;                              // 0000_0111 - Normal Mode, Prescaler : CPU_CLOCK / 1024 = 16Khz
    TIMSK = 0x01;                              // Enable overflow interrupt
    TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
    sei(); // Enable global interrupts
}

ISR(TIMER0_OVF_vect)
{
    if (count++ > 50) // 0.5초 마다 실행
    {
        count = 0;
        t--;
    }
    TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
}

int main(void)
{
    port_init();
    lcd_init();
    int seed = 0;
    srand(seed++);
    unsigned char x = rand() % n;
    srand(seed++);
    unsigned char y = rand() % n;
    srand(seed++);
    unsigned char op_sel = rand() % 2;
    unsigned char op[2] = {0x2b, 0x2d}; // +, -
    receiveChar();
    while (t > 0)
    {
        sendBin(x / 100);
        sendBin((x / 10) % 10);
        sendBin(x % 10);
        sendChar(op[op_sel]);
        sendBin(y / 100);
        sendBin((y / 10) % 10);
        sendBin(y % 10);
        sendStr("= \n\r");
        unsigned char sign = (receiveChar() == 0x2d) ? -1 : 1;
        unsigned char input_100 = receiveChar() - 0x30;
        unsigned char input_10 = receiveChar() - 0x30;
        unsigned char input_1 = receiveChar() - 0x30;
        if (op_sel == 0 && input_100 == (x + y) / 100 && input_10 == (((x + y) / 10) % 10) && input_1 == (x + y) % 10 && sign == 1)
        {
            sendStr("PASS\n\r");
            cursor_move(0, 1);
            lcd_str_write("PASS");
            score++;
            t = 20 - score;
            n = n * 2;
            srand(seed++);
            x = rand() % n;
            srand(seed++);
            y = rand() % n;
            srand(seed++);
            op_sel = rand() % 2;
        }
        else if (op_sel == 1 && x >= y && input_100 == (x - y) / 100 && input_10 == (((x - y) / 10) % 10) && input_1 == (x - y) % 10 && sign == 1)
        {
            sendStr("PASS\n\r");
            cursor_move(0, 1);
            lcd_str_write("PASS");
            score++;
            t = 20 - score;
            n = n * 2;
            srand(seed++);
            x = rand() % n;
            srand(seed++);
            y = rand() % n;
            srand(seed++);
            op_sel = rand() % 2;
        }

        else if (op_sel == 1 && x < y && input_100 == (y - x) / 100 && input_10 == (((y - x) / 10) % 10) && input_1 == (y - x) % 10 && sign == -1)
        {
            sendStr("PASS\n\r");
            cursor_move(0, 1);
            lcd_str_write("PASS");
            score++;
            t = 20 - score;
            n = n * 2;
            srand(seed++);
            x = rand() % n;
            srand(seed++);
            y = rand() % n;
            srand(seed++);
            op_sel = rand() % 2;
        }
        else
        {
            sendStr("FAIL\n\r");
            cursor_move(0, 1);
            lcd_str_write("FAIL");
            break;
        }
    }
    if (t <= 0)
    {
        sendStr("TIME OVER\n\r");
        cursor_move(0, 1);
        lcd_str_write("TIME OVER");
    }
}
