#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "communication.h"

int main(void)
{
	usart_init();

	DDRB |= (1 << PD5);
	while (1)
	{
		command_t cmd;

		// Zet de LED aan om aan te geven dat de Arduino
		// klaar is voor een nieuw commando.
		PORTB |= (1 << PD5);

		// Wacht op een nieuw commando
		cmd = usart_get_command();

		// Zet de LED uit om aan te geven dat het commando
		// ontvangen is.
		PORTB &= ~(1 << PD5);

		switch (cmd.code)
		{
		case CTX_NOP:
			// Doe niets
			break;

		case CTX_PING:
			// Stuur een CRX_PONG terug
			cmd.code = CRX_PONG;
			cmd.length = 0;
			usart_put_command(cmd);
			break;
		}
	}
	return 0;
}