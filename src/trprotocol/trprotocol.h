#ifndef TRPROTOCOL_H
#define TRPROTOCOL_H

#include <arpa/inet.h>

// Requires C11 or higher support
struct TRPacket {
	unsigned int type;

	union {
		// Packet types 0 and 1
		struct {
			unsigned int uname_length;
			char *username;
		};

		// Packet type 2
		struct {
			unsigned int text_length;
			char *text;
		};

		// Packet type 3
		unsigned int countdown;

		// Packet type 5
		struct {
			unsigned int puname_length;
			char *prog_username;
			unsigned int progress;
			unsigned int wpm;
		};

		// Packet type 6
		unsigned int host;
	};
};


// Helper functions
int sendall(int sockfd, void *buf, int *len);
int recv_n_bytes(int sockfd, uint8_t *buf, int n);

// Functions:
// Sending functions take in a pointer to a struct
//   TRPacket and returns 0 on success, -1 on failure.
// Recieving functions return a pointer to a struct
//   TRPacket. Returns NULL on failure.

// Generic functions (wrapper for appropriate send/recv funcs)
int send_packet(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_packet(int sockfd);
void print_packet(struct TRPacket *packet);
struct TRPacket * recv_types_014(int sockfd);

int send_usr_pkt(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_usr_pkt(int sockfd);

int send_pjoined_pkt(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_pjoined_pkt(int sockfd);

int send_typetext_pkt(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_typetext_pkt(int sockfd);

int send_cntdwn_pkt(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_cntdwn_pkt(int sockfd);

int send_rstart_pkt(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_rstart_pkt(int sockfd);

int send_progress_pkt(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_progress_pkt(int sockfd);

int send_urhost_pkt(int sockfd, struct TRPacket *packet);
struct TRPacket * recv_urhost_pkt(int sockfd);

#endif
