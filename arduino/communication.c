#include "communication.h"

void usart_init(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0A &= ~(1 << U2X0);

	// 8-bits data
	UCSR0C = 1 << UCSZ01 | 1 << UCSZ00;

	// Zet Rx en Tx aan
	UCSR0B = 1 << RXEN0 | 1 << TXEN0;
}

void usart_putchar(char c)
{
	while (UCSR0A & (1 << UDRE0));
	UDR0 = c;
}

void usart_puts(char *s)
{
	while (*s)
		usart_putchar(*s++);
}

void usart_write(char *buf, int count)
{
	while (--count)
		usart_putchar(*buf++);
}

char usart_getchar()
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

