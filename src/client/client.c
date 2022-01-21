#include "client.h"
#include "../trprotocol/trprotocol.h"


int main() {
    int sd = do_connect();  // Client connection

    char *username = get_send_usrname(sd);  // Username prompt and processing

    // Is this client the host?
    struct TRPacket *host_pkt = recv_urhost_pkt(sd);
    int am_host = host_pkt->host;
    free(host_pkt);

    struct TRPacket *text_pkt = recv_typetext_pkt(sd);  // Receive Text to be typed
    char *type_text = text_pkt->text;
    free(text_pkt);  // free the packet, but not the text

    if (am_host) { // if is host prompt start game
        char line[10];
        printf("Start Game? [Y/N]: "); // Prompt
    	fgets(line, 10, stdin); // Read from STDIN

        if (strcmp(line,"N\n")==0 || strcmp(line,"Y\n")!=0) return 0; // If N, end client

        struct TRPacket *rstart_pkt = calloc(1, sizeof(struct TRPacket));
        rstart_pkt->type = 4;
        send_rstart_pkt(sd, rstart_pkt); // Send race start packet
        free(rstart_pkt);
    }

    // receive game start packet run this shit below
    struct TRPacket *rstart = recv_rstart_pkt(sd);


    setup_curses();
    draw_pregame();

    // Set up screen
    int row, col;
    getmaxyx(stdscr, row, col);

    attron(COLOR_PAIR(1));
    mvprintw(0, 2, "TypeRacer");

    attron(COLOR_PAIR(2));
    mvprintw(0, col-12, "Accuracy: 85");
    mvprintw(0, col-22, "WPM 65");

    attron(COLOR_PAIR(3));
    mvprintw(3, 2, "%s", type_text);

    attron(COLOR_PAIR(3));
    mvprintw(row-2, 2, "%s", username);

    refresh();




    getch();
    endwin();

    // free(username);
    // free(TEXT);
    // free(_host);

    return 0;
}

char * get_send_usrname(int sockfd) {
    char *username = calloc(1024, sizeof(char));
    struct TRPacket *uname_pkt = calloc(1, sizeof(struct TRPacket));

    // Prompt USERNAME
    printf("Username: "); // Prompt
    fgets(username, 1024, stdin); // Read from STDIN
    *strrchr(username, '\n') = 0;
    uname_pkt->type = 0;
    uname_pkt->uname_length = strlen(username);
    uname_pkt->username = username;
    send_usr_pkt(sockfd, uname_pkt);

    free(uname_pkt);

    return username;
}

int do_connect() {
    printf("Welcome to TypeRacer!\n\n");

    printf("Hostname (leave blank for localhost): ");
    char *hostname = calloc(6, sizeof(char));  // see https://stackoverflow.com/questions/8724954/what-is-the-maximum-number-of-characters-for-a-host-name-in-unix
    fgets(hostname, 256, stdin);
    *strrchr(hostname, '\n') = 0;
    if (strlen(hostname) == 0) {
        memcpy(hostname, "localhost", 10);
    }

    printf("Port (leave blank for default): ");
    char *port = calloc(6, sizeof(char));
    fgets(port, 6, stdin);
    *strrchr(port, '\n') = 0;
    if (strlen(port) == 0) {
        memcpy(port, "9001", 5);
    }

    int sd = client_connect(hostname, port);
    free(hostname);
    free(port);

    return sd;
}

int client_connect(char *host, char *port) {
    struct addrinfo *hints, *results;

    hints = calloc(1,sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;

    getaddrinfo(host, port, hints, &results);

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

void setup_curses() {
    // Initialize Curses
    initscr();
    noecho();  // don't echo typed characters
    curs_set(FALSE);  // no cursor
    // nodelay(stdscr, TRUE);  // non-blocking getch

    // Initialize Colors
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
}

void draw_pregame() {
    ;
}
