#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define CTX_NOP		0
#define CTX_PING	1
#define CRX_NOP		0
#define CRX_PONG	1

#define BAUD 19200

#include <avr/io.h>
#include <util/setbaud.h>
#include <stdlib.h>

#define usart_getword() ((uint16_t)usart_getchar() | (uint16_t)usart_getchar() << 8)
#define usart_putword(__x__) usart_putchar(__x__); usart_putchar(__x__ >> 8)

typedef struct
{
	uint16_t code;
	uint16_t length;
	char *data;
} command_t;

void usart_init(void);
void usart_putchar(char c);
void usart_puts(char *s);
void usart_write(char *data, int length);
char usart_getchar();
char *usart_gets(char *buf, int count);
char *usart_read(char *buf, int count);

void usart_put_command(command_t cmd);
command_t usart_get_command();

#endif