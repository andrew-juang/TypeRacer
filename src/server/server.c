#include "server.h"

static void sighandler(int signo);
int sd;

int main() {
	signal(SIGINT, sighandler);

	// Binds to a port and listens for incoming connections
	sd = server_setup();

	while (1) {
		int to_client = server_connect(sd);
		printf("[server] connected to client!\n");
	}

	return 0;
}


int server_setup() {
	printf("[server] creating socket\n");
	struct addrinfo *hints, *results;
	hints = calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	getaddrinfo("localhost", "9001", hints, &results);

	int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	int bresult = bind(sd, results->ai_addr, results->ai_addrlen);
	if (bresult) {
		printf("[server] couldn't bind to socket\n");
		exit(EXIT_FAILURE);
	}

	printf("[server] created and bound to socket\n");

	listen(sd, 10);
	printf("[server] now listening on socket\n");

	return sd;
}

int server_connect(int from_client) {
	int client_socket;
	socklen_t sock_size;
	struct sockaddr_storage client_address;
	sock_size = sizeof(client_address);

	client_socket = accept(from_client, (struct sockaddr *) &client_address, &sock_size);

	return client_socket;
}

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n[server] SIGINT recieved, exiting!\n");

    close(sd);
    printf("\n");
    exit(EXIT_SUCCESS);
  }
}
