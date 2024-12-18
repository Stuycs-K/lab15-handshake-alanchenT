#include "pipe_networking.h"

// UPSTREAM = to the server / from the client
// DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
    remove(WKP);

    int mkfifo_ret = mkfifo(WKP, 0644);
    ASSERT(mkfifo_ret, "mkfifo")

    printf("mkfifo done\n");

    int from_client = open(WKP, O_RDONLY, 0);
    ASSERT(from_client, "Server create WKP")

    printf("open done\n");

    return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
    server_setup();

    int from_client;

    return from_client;
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    // Make PP
    pid_t client_pid = getpid();

    char private_pipe_name[10];
    snprintf(private_pipe_name, sizeof(private_pipe_name), "%d", client_pid);

    int mkfifo_ret = mkfifo(private_pipe_name, 0666);
    ASSERT(mkfifo_ret, "Client create PP")

    // Open WKP
    int wkp = open(WKP, O_WRONLY, 0);
    ASSERT(wkp, "Client access WKP")

    int from_server;
    return from_server;
}

/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
    int to_client = 0;
    return to_client;
}
