#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ncurses.h>

#ifndef CLIENT_H
#define CLIENT_H

int do_connect();
char * get_send_usrname(int sockfd);
int client_connect(char *host, char *port);
void setup_curses();
void draw_pregame();

#endif
