#include "trprotocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


/**
 * Stolen from Beej's guide to networking
 * @param sockfd socket file descriptor
 * @param buf pointer to buffer to send
 * @param len pointer to the length of the buffer
 */
int sendall(int sockfd, void *buf, int *len) {
    int total = 0;
    int bytesLeft = *len;
    int n;

    while (total < *len) {
        n = send(sockfd, buf+total, bytesLeft, 0);
        if (n == -1) break;
        total += n;
        bytesLeft -= n;
    }

    *len = total;

    return n == -1 ? -1 : 0;
}


/**
 * Recieves a specified number of bytes, or else...
 * @param sockfd socket file descriptor
 * @param buf buffer to read into
 * @param n number of bytes to read
 * @return 0 on success, other values on failure (errno?)
 */
int recv_n_bytes(int sockfd, uint8_t *buf, int n) {
    int total = 0;
    int bytesLeft = n;
    int recvd = 0;

    while (total < n) {
        recvd = recv(sockfd, buf+total, bytesLeft, 0);
        if (recvd == -1) return errno;
        total += recvd;
        bytesLeft -= recvd;
    }

    return n == -1 ? errno : 0;
}


/**
 * Sends a packet of type 0 through a socket
 * @param sockfd socket descriptor
 * @param packet packet to send
 */
int send_usr_pkt(int sockfd, struct TRPacket *packet) {
    unsigned int data_size = 6 + packet->uname_length;
    uint8_t *data = malloc(data_size);

    uint16_t _data_size = htons(data_size);
    uint16_t _packet_type = htons(packet->type);
    uint16_t _packet_uname_length = htons(packet->uname_length);
    
    memcpy(data, &_data_size, 2);                       // size of packet
    memcpy(data+2, &_packet_type, 2);                  // packet type
    memcpy(data+4, &_packet_uname_length, 2);          // length of username
    memcpy(data+6, packet->username, packet->uname_length);  // username

    int sent = sendall(sockfd, data, &data_size);
    free(data);

    return sent;
}


/**
 * Recieves a packet of type 0. Allocates memory for packet
 * and returns a pointer to it.
 * 
 * @return pointer to recieved packet or NULL on error
 */
struct TRPacket * recv_usr_pkt(int sockfd) {
    struct TRPacket *ret = calloc(1, sizeof(struct TRPacket));
    unsigned int data_size = 6;  // could probably just be 2
    uint8_t *data = calloc(1, data_size);

    int _read = recv_n_bytes(sockfd, data, 2);  // read the size of the packet only
    data_size = ntohs(*((uint16_t *) data));  // conversion magic

    data = realloc(data, data_size);
    _read = recv_n_bytes(sockfd, data+2, data_size-2);  // read the rest

    unsigned int type = ntohs(*((uint16_t *) (data+2)));  // get the type
    if (type != 0 && type != 1) {  // wrong type
        free(data);
        return NULL;
    }

    ret->type = type;
    ret->uname_length = ntohs(*((uint16_t *) (data+4)));  // get the size
    ret->username = calloc(1, ret->uname_length);

    memcpy(ret->username, data+6, ret->uname_length);

    free(data);
    return ret;
}


/**
 * Sends a packet of type 1 through a socket
 * @param sockfd socket descriptor
 * @param packet packet to send
 */
int send_pjoined_pkt(int sockfd, struct TRPacket *packet) {
    return send_usr_pkt(sockfd, packet);
}


/**
 * Recieves a packet of type 1. Allocates memory for packet
 * and returns a pointer to it.
 * 
 * @return pointer to recieved packet
 */
struct TRPacket * recv_pjoined_pkt(int sockfd) {
    return recv_usr_pkt(sockfd);
}


/**
 * Prints a TRPacket packet
 * @param packet Packet to be printed
 */
void print_packet(struct TRPacket *packet) {
    printf("TRPacket: type=%d ", packet->type);

    switch (packet->type)
    {
    case 0:
    case 1:
        printf("uname_length=%d username=%s", packet->uname_length, packet->username);
        break;

    case 2:
        printf("text_length=%d text=%s", packet->text_length, packet->text);
        break;

    case 3:
        printf("countdown=%d", packet->countdown);
        break;

    case 5:
        printf("puname_length=%d prog_username=%s progress=%d wpm=%d",
                packet->puname_length, packet->prog_username, packet->progress, packet->wpm);
        break;
    
    default:
        break;
    }

    printf("\n");
}

#if 0
/**
 * Runs tests on the trprotocol library
 */
int main() {
    struct TRPacket *type_0 = calloc(1, sizeof(struct TRPacket));

    type_0->type = 0;
    type_0->uname_length = 11;
    type_0->username = "Hello there";

    print_packet(type_0);
    free(type_0);

    return 0;
}
#endif
