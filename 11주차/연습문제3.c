#include <avr/io.h>
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
//
void port_init(void)
{
	MCUCR = 0x80;
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
void scr_r_shift(int cnt, int delay_time)
{
	while (cnt--)
	{
		lcd_control_write(0x1c);
		delay(delay_time);
	}
}
//
void scr_l_shift(int cnt, int delay_time)
{
	while (cnt--)
	{
		lcd_control_write(0x18);
		delay(delay_time);
	}
}
//
void lcd_str_write(char *Str)
{
	while (*Str)
	{
		lcd_data_write(*Str++);
	}
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
int main(void)
{
	port_init();
	lcd_init();

	while (1)
	{
		lcd_str_write("Seoultech EIE");
		delay(400);
		scr_r_shift(3, 400);
		scr_l_shift(3, 400);
		lcd_control_write(0x01);
		cursor_move(0, 0);
		lcd_str_write("Seoultech EIE");
		delay(400);
		scr_r_shift(3, 400);
		scr_l_shift(3, 400);
		lcd_control_write(0x01);
	}
}