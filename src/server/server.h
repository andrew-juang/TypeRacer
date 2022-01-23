#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/poll.h>

#define MAX_PLAYERS 20

int server_setup();
int server_connect(int from_client);

char * generate_text();
struct TRPacket * generate_text_packet();

char * remove_spaces(char * line);

#endif
