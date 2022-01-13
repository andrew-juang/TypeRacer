#ifndef TRPROTOCOL_H
#define TRPROTOCOL_H

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
    };
};

// Functions:
// Sending functions take in a pointer to a struct
//   TRPacket and returns 0 on success, -1 on failure.
// Recieving functions return a pointer to a struct
//   TRPacket. Returns NULL on failure.

// Generic functions (wrapper for appropriate send/recv funcs)
int send_packet(struct TRPacket *packet);
struct TRPacket * recv_packet();

int send_usr_pkt(struct TRPacket *packet);
struct TRPacket * recv_usr_pkt();

int send_pjoined_pkt(struct TRPacket *packet);
struct TRPacket * recv_pjoined_pkt();

int send_typetext_pkt(struct TRPacket *packet);
struct TRPacket * recv_typetext_pkt();

int send_cntdwn_pkt(struct TRPacket *packet);
struct TRPacket * recv_cntdwn_pkt();

int send_rstart_pkt(struct TRPacket *packet);
struct TRPacket * recv_rstart_pkt();

int send_progress_pkt(struct TRPacket *packet);
struct TRPacket * recv_progress_pkt();

#endif
