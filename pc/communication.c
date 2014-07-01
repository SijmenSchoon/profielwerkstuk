#include "communication.h"

bool serial_open()
{
#if defined(__unix__)
	struct termios options;

	// Open the seriele poort
	// TODO: Detecteer de arduino
	serial_fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (serial_fd < 0)
	{
		printf("serial_init: Kon /dev/ttyACM0 niet openen:\n");
		printf("serial_init: %s (%d)\n", strerror(errno), errno);
		return false;
	}
	fcntl(serial_fd, F_SETFL, 0);

	// Haal de attributen
	tcgetattr(serial_fd, &options);

	// Stel de baud-snelheid in
	cfsetispeed(&options, B19200);
	cfsetospeed(&options, B19200);

	// Zet de ontvanger aan en zet hem op "local"-modus
	options.c_cflag |= (CLOCAL | CREAD);

	// Stel de nieuwe attributen in
	tcsetattr(serial_fd, TCSANOW, &options);

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&except_fds);
	FD_SET(serial_fd, &read_fds);

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
#endif

	return true;
}

void serial_close()
{
#if defined(__unix__)
	close(serial_fd);
#endif
}

void comm_send_command(uint16_t code, char *data, uint16_t length)
{
	write(serial_fd, &code, 2);
	write(serial_fd, &length, 2);
	if (length > 0)
		write(serial_fd, data, length);
}

int comm_read_command(uint16_t *code, char *data)
{
	if (!select(serial_fd + 1, &read_fds, &write_fds, &except_fds, &timeout))
	{
		*code = 0;
		return 0;
	}

	uint16_t length;
	assert(read(serial_fd, code, 2));
	assert(read(serial_fd, &length, 2));
	if (length > 0)
	{
		data = (char *)malloc(length);
		read(serial_fd, data, length);
	}
	return length;
}

bool comm_ping()
{
	uint16_t code = 0, size;
	char *data;

	// Verzend een ping-commando
	comm_send_command(CTX_PING, NULL, 0);

	// Wacht op antwoord
	size = comm_read_command(&code, data);

	// Return true als het antwoord CRX_PONG is
	return code == CRX_PONG;
}