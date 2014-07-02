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
	while (!(UCSR0A & (1 << UDRE0)));
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

char *usart_gets(char *buf, int count)
{
	while (--count)
	{
		*buf = usart_getchar();
		if (*buf == '\n')
			break;
		buf++;
	}
	*buf = '\0';
	return buf;
}

char *usart_read(char *buf, int count)
{
	while (--count)
		*buf++ = usart_getchar();
	return buf;
}

void usart_put_command(command_t cmd)
{
	usart_putword(cmd.code);
	usart_putword(cmd.length);
	if (cmd.length > 0)
		usart_write(cmd.data, cmd.length);
}

command_t usart_get_command()
{
	command_t cmd;
	cmd.code = usart_getword();
	cmd.length = usart_getword();
	if (cmd.length > 0)
	{
		cmd.data = (char *)malloc(cmd.length);
		usart_read(cmd.data, cmd.length);
	}
	return cmd;
}