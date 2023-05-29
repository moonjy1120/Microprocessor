#define F_CPU 16000000UL
#define BAUDRATE 70000
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void sendChar(char c)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ; // 1<<UDRE0 => 0x20
    UDR0 = c;
}

void sendStr(char *str)
{
    while (*str)
        sendChar(*str++);
}

void port_init(void)
{
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
        sendStr("2023 microprocessor\n\r");
        _delay_ms(500);
    }
}