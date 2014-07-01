#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#if defined(__unix__)
# include <unistd.h>
# include <fcntl.h>
# include <termios.h>
# include <stdbool.h>
# include <sys/time.h>
# include <sys/select.h>
#endif

#define CTX_NOP		0
#define CTX_PING	1

#define CRX_NOP		0
#define CRX_PONG	1

static int serial_fd;
static fd_set read_fds, write_fds, except_fds;
static struct timeval timeout;


bool serial_open();
void serial_close();

void comm_send_command(uint16_t code, char *data, uint16_t length);
int comm_read_command(uint16_t *code, char *data);
bool comm_ping();

#endif