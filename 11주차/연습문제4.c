#include <avr/io.h>
#include <avr/interrupt.h>
//
#define LCD_CONTROL (*(volatile unsigned char *)0x8001)
#define LCD_DATA (*(volatile unsigned char *)0x8000)
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
volatile char BANNER[16] = "Seoultech EIE  ";
volatile char location[16] = {0x8f, 0xce, 0x8d, 0xcc, 0x8b, 0xca, 0x89, 0xc8, 0x87, 0xc6, 0x85, 0xc4, 0x83, 0xc2, 0x81, 0xc0};
volatile float sec = 0.01; // Interrupt Period
volatile int count = 0;	   // 카운트 저장 변수
volatile int index = 0;	   // 문자열 인덱스
//
void port_init(void)
{
	MCUCR = 0x80;
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
	delay(1000);			 // local initialization
	lcd_control_write(0x38); // fuction set
	lcd_control_write(0x01); // clear display
	lcd_control_write(0x0c); // display on, cursor off, cursor blink-off
}
//
ISR(TIMER0_OVF_vect)
{
	if (count++ > 100) // 1초
	{
		for (int i = 0; i <= index; i++)
		{
			lcd_control_write(location[index - i]);
			lcd_data_write(BANNER[i]);
		}
		if (index == 15)
		{
			index = 0;
			lcd_control_write(0x01);
		}
		else
		{
			index++;
		}
		count = 0;
	}
	TCNT0 = 255 - ((unsigned int)(sec * 16000.0));
}
//
int main(void)
{
	port_init();
	lcd_init();
	while (1)
		;
}