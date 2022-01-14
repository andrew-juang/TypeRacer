#include "client.h"
#include "../trprotocol/trprotocol.h"


int main() {
 	char line[10];

	int sd = client_connect();
	printf("Start Game? [Y/N]: ");
	fgets(line, 10, stdin);
	int err = write(sd, line, sizeof(line)); // send data to server
	if(err == -1) {
		printf("unable to write to server");
		return 0;
	}

    char text[4000];
    read(sd, text, sizeof(text));

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
