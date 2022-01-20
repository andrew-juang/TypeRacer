#include "server.h"
#include "../trprotocol/trprotocol.h"

static void sighandler(int signo);
int sd;

int main() {
	srand(time(NULL)); // Seed RNG
	signal(SIGINT, sighandler); // Handle Signals

	// Generate text and put it into a packet
	char *text = generate_text();
	struct TRPacket *text_packet = calloc(1, sizeof(struct TRPacket));
	text_packet->type = 2;
	text_packet->text_length = strlen(text);
	text_packet->text = text;

	sd = server_setup(); // Binds to a port and listens for incoming connections

	struct pollfd *fds = calloc(20, sizeof(struct pollfd));
	int num_users = 0;

	// add the listener socket to the pollfd array
	fds[0].fd = sd;
	fds[0].events = POLLIN;

	// connect the host and add it to pollfd array
	int to_client = server_connect(sd);
	printf("[server] connected to host!\n");
	fds[1].fd = to_client;
	fds[1].events = POLLIN;
	num_users++;

	while (1) {
		int num_avail = poll(fds, num_users, -1);  // poll forever

		int seen = 0;
		int i;

		for (i = 0; i <= num_users; i++) {
			if (seen == num_avail) break;

			// Listener Socket -> Connect Client
			if (i == 0 && fds[i].revents == POLLIN) {
				seen++;
				num_users++;

				// New client Connects
				int to_client = server_connect(fds[0].fd);

				// Add to array of pollfd structs
				fds[num_users].fd = to_client;
				fds[num_users].events = POLLIN;

				// Receive Username Packet
				struct TRPacket *USERNAME = recv_usr_pkt(to_client);
				print_packet(USERNAME);

				send_typetext_pkt(to_client,text_packet);  // Sends Typetext Packet

				int j;
				for (j = 1; j < num_users; j++) {
					// figure this out later
				}
			}

			// Host Socket
			else if (i == 1 && fds[i].revents == POLLIN) {
				seen++;

				// Receive packet to start
				char start[10];
				recv(to_client, start, sizeof(start),0);

				// IMPLEMENT sending out race start packet
				if (strcmp(start,"Y\n")==0) { // Received Message to Start Game
					// send out race start packet to everyone
				} else if (strcmp(start,"N\n")==0) {
					// restart game
				}
			}
		}
	}

	// clean up stuff before game
	fds[0].fd = -1 * sd;  // stop polling listener socket
	free(text);
	free(text_packet);

	// While Loop to handle the game phase
	while (1) {
		// char start[10];
		//
		// // Receive USERNAME Packet
		// struct TRPacket *USERNAME = recv_usr_pkt(to_client);
		// print_packet(USERNAME);
		//
		// // Receive packet to start
		// recv(to_client, start, sizeof(start),0);
		//
		// if (strcmp(start,"Y\n")==0) { // Received Message to Start Game
		// 	char * text = generate_text();
		// 	struct TRPacket *text_packet = calloc(1, sizeof(struct TRPacket));
		// 	text_packet->type = 2;
		//     text_packet->text_length = strlen(text);
		//     text_packet->text = text;
		// 	send_typetext_pkt(to_client,text_packet);
		// } else if (strcmp(start,"N\n")==0) {
		// 	char * text = "N\n";
		// 	send(to_client, text, 4032, 0);
		// }
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
		text = "The history of all hitherto existing society is the history of class struggles. Freeman and slave, patrician and plebeian, lord and serf, guild-maste and journeyman, in a word, oppressor and oppressed\0";
	} else if(r == 3) {
		text = "Maya hii Maya hoo Maya haaah Maya haaah haah Maya hoo Maya haah Maya haah haah Maya hiiMaya hoo Maya haah Maya haah haaah \0";
	}
	return text;
}
