#include "trprotocol.h"
#include <stdio.h>
#include <stdlib.h>

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
