#include "pipe_networking.h"


/*=========================
  server_setup
  args:
  creates the socket (upstream) and opens it, waiting for a
  connection.
  removes the  once a connection has been made
  returns the file descriptor for the socket.
  =========================*/
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

/*=========================
  server_connect
  args: int from_client
  handles the subserver portion of the 3 way handshake
  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);

  client_socket = accept(from_client, (struct sockaddr *) &client_address, &sock_size);

  return client_socket;
}


/*=========================
  server_handshake
  args: int * to_client
  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int b, from_client;
  char buffer[HANDSHAKE_BUFFER_SIZE];

  printf("[server] handshake: making wkp\n");
  b = mkfifo(WKP, 0600);
  if ( b == -1 ) {
    printf("mkfifo error %d: %s\n", errno, strerror(errno));
    exit(-1);
  }
  //open & block
  from_client = open(WKP, O_RDONLY, 0);
  //remove WKP
  remove(WKP);

  printf("[server] handshake: removed wkp\n");
  //read initial message
  b = read(from_client, buffer, sizeof(buffer));
  printf("[server] handshake received: -%s-\n", buffer);


  *to_client = open(buffer, O_WRONLY, 0);
  //create SYN_ACK message
  srand(time(NULL));
  int r = rand() % HANDSHAKE_BUFFER_SIZE;
  sprintf(buffer, "%d", r);

  write(*to_client, buffer, sizeof(buffer));
  //rad and check ACK
  read(from_client, buffer, sizeof(buffer));
  int ra = atoi(buffer);
  if (ra != r+1) {
    printf("[server] handshake received bad ACK: -%s-\n", buffer);
    exit(0);
  }//bad response
  printf("[server] handshake received: -%s-\n", buffer);

  return from_client;
}


/*=========================
  client_handshake
  args: none
  Performs the client side pipe 3 way handshake.
  returns the file descriptor for the downstream socket.
  =========================*/
int client_handshake() {
  struct addrinfo *hints, *results;

  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;

  getaddrinfo("localhost", "9001", hints, &results);

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
