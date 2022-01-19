#include "server.h"
#include "../trprotocol/trprotocol.h"

static void sighandler(int signo);
int sd;

int main() {
	srand(time(NULL)); // Seed RNG
	signal(SIGINT, sighandler); // Handle Signals

	sd = server_setup(); // Binds to a port and listens for incoming connections

	while (1) {
		int to_client = server_connect(sd);
		printf("[server] connected to client!\n");
		char start[10];

		// Receive USERNAME Packet
		struct TRPacket *USERNAME = recv_usr_pkt(to_client);
		print_packet(USERNAME);

		// Receive packet to start
		recv(to_client, start, sizeof(start),0);

		if (strcmp(start,"Y\n")==0) { // Received Message to Start Game
			char * text = generate_text();
			struct TRPacket *TEXT = calloc(1, sizeof(struct TRPacket));
			TEXT->type = 2;
		    TEXT->text_length = strlen(text);
		    TEXT->text = text;
			send_typetext_pkt(to_client,TEXT);
		} else if (strcmp(start,"N\n")==0) {
			char * text = "N\n";
			send(to_client, text, 4032, 0);
		}
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

char * generate_text(){
	int r = rand()%4;
	char * text = calloc(2000, sizeof(char));
	if (r == 0){
		text = "Four score and seven years ago our fathers brought forth on this continent, a new nation, conceived in Liberty, and dedicated to the proposition that all men are created equal. \0";
	}
	else if (r == 1) {
		text = "To be, or not to be, that is the question: Whether 'tis nobler in the mind to suffer The slings and arrows of outrageous fortune, Or to take arms against a sea of troubles And by opposing end them.\0";
	}
	else if (r == 2){
		text = "The history of all hitherto existing society is the history of class struggles. Freeman and slave, patrician and plebeian, lord and serf, guild-maste and journeyman, in a word, oppressor and oppressed\0"
	} else if(r == 3) {
		text = "Maya hii Maya hoo Maya haaah Maya haaah haah Maya hoo Maya haah Maya haah haah Maya hiiMaya hoo Maya haah Maya haah haaah \0";
	}
	return text;
}
