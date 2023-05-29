#define F_CPU 16000000UL
#define BAUDRATE 70000
#define EX_LED (*(volatile unsigned char *)0x8008)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

volatile unsigned char input;

void receiveChar(void)
{
    while (!(UCSR0A & (1 << RXC0)))
    {
        if (input == 0x31)
        {
            EX_LED = 0xff;
        }
        else if (input == 0x30)
        {
            EX_LED = 0xff;
            _delay_ms(500);
            EX_LED = 0x00;
            _delay_ms(500);
        }
    }
    input = UDR0;
}

void port_init(void)
{
    MCUCR = 0x80;
    UCSR0A = 0x00;
    UCSR0B = 0x18; // Tx enable, Rx enable
    UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit
    UBRR0H = 0x00;
    UBRR0L = (14745600 / (16 * BAUDRATE)) - 1; // 12
}

int main(void)
{
    port_init();
    while (1)
    {
        receiveChar();
    }
}