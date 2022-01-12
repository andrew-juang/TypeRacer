#include "pipe_networking.h"


static void sighandler(int signo);

int sd = -1;


int main() {
  signal(SIGINT, sighandler);

  int server;

  server = client_handshake();
  sd = server;

  char input[500];

  while (1) {
    printf(">>> ");

    fgets(input, sizeof(input), stdin);
    if (input[0] == '\n') continue;

    input[strlen(input) - 1] = 0;
    write(server, input, strlen(input));

    int r = read(server, input, sizeof(input));
    input[r] = 0;

    printf("%s\n", input);
  }
}


static void sighandler(int signo) {
  if (signo == SIGINT) {
    close(sd);

    printf("\n");
    exit(EXIT_SUCCESS);
  }
}
