#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "communication.h"

int main(int argc, char **argv)
{
	wdt_enable(WDTO_2S);

	while (1)
	{
		usart_puts("Hello world!\n");
		_delay_ms(1000);
		wdt_reset();
	}

	return 0;
}