#include "client.h"
#include "../trprotocol/trprotocol.h"


int main() {
 	char line[10];

	int sd = client_connect(); // Connect to Server

	printf("Start Game? [Y/N]: ");
	fgets(line, 10, stdin);
	send(sd, line, sizeof(line), 0); // send Y/N to server

    char text[2000];
    recv(sd, text, sizeof(text), 0);

	initscr();

	printw("%s", text);
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
