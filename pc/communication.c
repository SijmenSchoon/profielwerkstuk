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

void comm_put_command(command_t cmd)
{
	write(serial_fd, &cmd.code, 2);
	write(serial_fd, &cmd.length, 2);
	if (cmd.length > 0)
		write(serial_fd, cmd.data, cmd.length);
}

command_t comm_get_command()
{
	command_t cmd;

	if (!select(serial_fd + 1, &read_fds, &write_fds, &except_fds, &timeout))
	{
		cmd.code = 0;
		cmd.length = 0;
		return cmd;
	}

	assert(read(serial_fd, &cmd.code, 2) == 2);
	assert(read(serial_fd, &cmd.length, 2) == 2);
	if (cmd.length > 0)
	{
		cmd.data = (char *)malloc(cmd.length);
		read(serial_fd, cmd.data, cmd.length);
	}
	return cmd;
}

bool comm_ping()
{
	command_t cmd;

	// Verzend een ping-commando
	cmd.code = CTX_PING;
	cmd.length = 0;
	comm_put_command(cmd);

	// Wacht op antwoord
	cmd = comm_get_command();

	// Return true als het antwoord CRX_PONG is
	return cmd.code == CRX_PONG;
}