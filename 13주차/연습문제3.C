#define F_CPU 16000000UL
#define BAUDRATE 70000

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

void sendChar(char c)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ; // 1<<UDRE0 => 0x20
    UDR0 = c;
}

unsigned char receiveChar(void)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}

void sendStr(char *str)
{
    while (*str)
        sendChar(*str++);
}

void port_init(void)
{
    UCSR0A = 0x00;
    UCSR0B = 0x18; // Tx, Rx enable
    UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit
    UBRR0H = 0x00;
    UBRR0L = (14745600 / (16 * BAUDRATE)) - 1; // 12
}

int main(void)
{
    port_init();
    int seed = rand();
    srand(seed++);
    unsigned char X_10 = rand() % 10;
    unsigned char X_1 = rand() % 10;
    unsigned char count = 100;

    while (1)
    {
        unsigned char input_10 = receiveChar() - 0x30;
        unsigned char input_1 = receiveChar() - 0x30;
        if (input_10 > X_10 || (input_10 == X_10 && input_1 > X_1)) // 입력값이 더 큰 경우
        {
            sendStr("DOWN\n\r");
            count--;
        }
        if (input_10 < X_10 || (input_10 == X_10 && input_1 < X_1)) // 입력값이 더 작은 경우
        {
            sendStr("UP\n\r");
            count--;
        }
        if (input_10 == X_10 && input_1 == X_1) // 입력값이 같은 경우
        {
            sendStr("PASS\n\r");
            break;
        }
        if (count == 0)
        {
            sendStr("FAIL\n\r");
            break;
        }
    }
}
