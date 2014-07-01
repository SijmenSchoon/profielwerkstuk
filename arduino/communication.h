#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define CTX_NOP		0
#define CTX_PING	1
#define CRX_NOP		0
#define CRX_PONG	1

#define BAUD 19200

#include <avr/io.h>
#include <util/setbaud.h>

void usart_init(void);
void usart_putchar(char c);
void usart_puts(char *s);
void usart_write(char *data, int length);

#endif