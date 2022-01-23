#include "server.h"
#include "../trprotocol/trprotocol.h"

static void sighandler(int signo);
int sd;

int main() {
	srand(time(NULL)); // Seed RNG
	signal(SIGINT, sighandler); // Handle Signals

	// Text packet with randomly generated text
	struct TRPacket *text_packet = generate_text_packet();

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

	// Receive Username Packet
	struct TRPacket *_username = recv_usr_pkt(to_client);
	print_packet(_username);

	// Tell the host that they're host
	struct TRPacket *host_pkt = calloc(1, sizeof(struct TRPacket));
	host_pkt->type = 6;
	host_pkt->host = 1;
	send_urhost_pkt(to_client, host_pkt);
	free(host_pkt);

	// Send the text to the host
	send_typetext_pkt(to_client, text_packet);

	// Create a packet to tell non-host clients that they're not
	struct TRPacket *not_host = calloc(1, sizeof(struct TRPacket));
	not_host->type = 6;
	not_host->host = 0;

	while (1) {

		int num_avail = poll(fds, num_users, -1);  // poll forever

		int i;
		int done = 0;

		for (i = 0; i <= num_users; i++) {

			// Listener Socket -> Connect Client
			if (i == 0 && fds[i].revents == POLLIN) {
				num_users++;

				// New client Connects
				int to_client = server_connect(fds[0].fd);

				// Add to array of pollfd structs
				fds[num_users].fd = to_client;
				fds[num_users].events = POLLIN;

				// Receive Username Packet
				struct TRPacket *_username = recv_usr_pkt(to_client);
				print_packet(_username);

				send_urhost_pkt(fds[num_users].fd, not_host);  // they're not host
				send_typetext_pkt(fds[num_users].fd, text_packet);  // Sends Typetext Packet
			}

			// Host Socket
			if (i == 1 && fds[i].revents == POLLIN) {
				struct TRPacket *rstart = recv_rstart_pkt(fds[1].fd); // Receive race start packet

				int j;
				for (j = 1; j <= num_users; j++) { // Send game start to all users
					struct TRPacket *rstart_pkt = calloc(1, sizeof(struct TRPacket));
					rstart_pkt->type = 4;
			        send_rstart_pkt(fds[j].fd, rstart_pkt); // Send race start packet
					free(rstart_pkt);
				}

				done++;
				break;
			}
		}

		if (done) break;
	}

	// clean up stuff before game
	fds[0].fd = -1 * sd;  // stop polling listener socket
	free(text_packet);
	free(not_host);

	// GAME LOOP
	while (1) {
		//
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
	int file = open("test.txt", O_RDONLY);
	if(file==-1){
        printf("Error opening:\n");
        printf("%s\n", strerror(errno));
        return 0;
    }

	// get file size
	struct stat st;
	stat("test.txt", &st);
	int size = st.st_size;
	printf("%d", size);

	int r = rand() % (size-200);

	lseek(file, r, SEEK_SET );

	char * text = calloc(200, sizeof(char));
	int err = read(file, text, 200);
	if(err==-1){
        printf("Error:\n");
        printf("%s\n", strerror(errno));
        return 0;
    }

	return remove_spaces(text);
}


/**
 * Generates a text packet with randomly generated text.
 * Important: free the text field, then the TRPacket.
 *
 * @return a pointer to a text TRPacket
 */
struct TRPacket * generate_text_packet() {
	char *text = generate_text();

	struct TRPacket *text_packet = calloc(1, sizeof(struct TRPacket));
	text_packet->type = 2;
	text_packet->text_length = strlen(text);
	text_packet->text = text;

	return text_packet;
}


/**
 * Takes in a line and removes whitespace
 *
 * @param line pointer to a string
 */
char * remove_spaces(char * line) {
    // Remove from beginning
    while (*line == ' ' || *line == '\t') {
        line++;
    }

    // Remove from end
    char *end = line + strlen(line) - 1;
    while(end > line && (*end == ' '||*end == '\t')) {
        end--;
    }

    // Add terminating null
    *(end + 1) = '\0';
    return line;
}
