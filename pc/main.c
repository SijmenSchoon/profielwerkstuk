#include <stdlib.h>
#include <stdio.h>
#include "communication.h"

int main(int argc, char **argv)
{
	// Open de seriele verbinding
	if (!serial_open())
		exit(EXIT_FAILURE);

	// Wacht even tot de Arduino gereset is
	sleep(3);

	if (!comm_ping())
	{
		printf("main: Geen reactie op CTX_PING.\n");
		serial_close();
		exit(EXIT_FAILURE);
	}

	serial_close();

	return 0;
}