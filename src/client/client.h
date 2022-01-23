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
#include <poll.h>

#ifndef CLIENT_H
#define CLIENT_H

#define MAX_PLAYERS 20

struct OtherPlayer {
    char *username;
    int progress;
    int wpm;
};

int do_connect();
char * get_send_usrname(int sockfd);
int client_connect(char *host, char *port);

void setup_curses();
void draw_static_elements(char *username);
void draw_dynamic_elements(char *type_text, char *user_typed, int wpm, int accuracy);

#endif
