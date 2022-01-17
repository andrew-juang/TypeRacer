#include "client.h"
#include "../trprotocol/trprotocol.h"


int main() {
 	char line[10];
    char username[1024];
    struct TRPacket *USERNAME = calloc(1, sizeof(struct TRPacket));

    // Connect to Server
	int sd = client_connect();

    // Prompt USERNAMEkk
    printf("Username: "); // Prompt
	fgets(username, 1024, stdin); // Read from STDIN
    USERNAME->type = 0;
    USERNAME->uname_length = strlen(username);
    USERNAME->username = username;
    send_usr_pkt(sd, USERNAME);

    // Prompt Start of Game
    printf("Start Game? [Y/N]: "); // Prompt
	fgets(line, 10, stdin); // Read from STDIN
    if (strcmp(line,"N\n")==0 || strcmp(line,"Y\n")!=0) return 0; // If, end client
	send(sd, line, sizeof(line), 0); // send Y/N to server

    // Receive Text to be typed
    char text[2000];
    recv(sd, text, sizeof(text), 0);

    // Initialize Curses
	initscr();
    noecho();
    curs_set(FALSE);

    // Initialize Colors
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    // Set up screen
    int row, col;
    getmaxyx(stdscr, row, col);

    attron(COLOR_PAIR(1));
	mvprintw(0, 0, "TypeRacer");

    attron(COLOR_PAIR(2));
    mvprintw(0, col-12, "Accuracy: 85");
    mvprintw(0, col-22, "WPM 65");

    attron(COLOR_PAIR(3));
    mvprintw(2, 0, "%s", text);

	refresh();




	getch();
	endwin();

	return 0;
}

int client_connect() {
	struct addrinfo *hints, *results;

	hints = calloc(1,sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;

	getaddrinfo("localhost", "9001", hints, &results);

	//create socket
	int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

	int conn = connect(sd, results->ai_addr, results->ai_addrlen);
	if (conn) {
		printf("[client] couldn't connect\n");
		exit(EXIT_FAILURE);
	}

	free(hints);
	freeaddrinfo(results);

	return sd;
}
