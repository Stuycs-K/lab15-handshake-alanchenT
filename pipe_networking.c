#include "pipe_networking.h"

#define ASSERT(value, error_prefix)                         \
    if ((value) == -1) {                                    \
        fprintf(stderr, "[ASSERTION FAILED]: ");            \
        perror(error_prefix);                               \
        exit(EXIT_FAILURE);                                 \
    }

// WKP: Client to server
// PP: Server to client

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

    // Make named pipe
    int mkfifo_ret = mkfifo(WKP, 0644);
    ASSERT(mkfifo_ret, "mkfifo")

    printf("[SERVER]: Created WKP\n");
    printf("[SERVER]: Waiting for connection...\n");

    int from_client = open(WKP, O_RDONLY, 0);
    ASSERT(from_client, "Server open WKP");

    printf("[SERVER]: Client connected\n");

    remove(WKP);

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
    int from_client = server_setup();

    printf("[SERVER]: Waiting for SYN...\n");

    char syn[HANDSHAKE_BUFFER_SIZE];
    read(from_client, syn, sizeof(syn));
    *to_client = atoi(syn);

    printf("[SERVER]: Received SYN: %d\n", *to_client);

    int downstream = open(syn, O_WRONLY, 0);
    ASSERT(downstream, "Server open PP")

    srand(time(NULL));
    int syn_ack_value = rand();
    write(downstream, &syn_ack_value, sizeof(syn_ack_value));

    printf("[SERVER]: Sent SYN-ACK: %d\n", syn_ack_value);
    printf("[SERVER]: Waiting for ACK...\n");

    int ack_value;
    read(from_client, &ack_value, sizeof(ack_value));

    printf("[SERVER]: Received ACK: %d\n", ack_value);

    if (ack_value != syn_ack_value + 1) {
        printf("[SERVER]: Invalid ACK received\n");
        return -1;
    }

    printf("[SERVER]: Handshake complete\n");

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
    // Create PID string, used as name of PP and value of SYN
    pid_t client_pid = getpid();
    char pid_string[HANDSHAKE_BUFFER_SIZE];
    snprintf(pid_string, sizeof(pid_string), "%d", client_pid);

    remove(pid_string); // Just in case
    int mkfifo_ret = mkfifo(pid_string, 0644);
    ASSERT(mkfifo_ret, "Client create PP")

    printf("[CLIENT]: Created PP: %s\n", pid_string);

    // Open WKP
    int upstream = open(WKP, O_WRONLY, 0);
    ASSERT(upstream, "Client open WKP")
    *to_server = upstream;

    // Write SYN
    write(upstream, pid_string, sizeof(pid_string));

    printf("[CLIENT]: Sent SYN: %d\n", client_pid);
    printf("[CLIENT]: Waiting for SYN-ACK...\n");

    int from_server = open(pid_string, O_RDONLY, 0);
    ASSERT(from_server, "Client open PP")

    remove(pid_string);

    int syn_ack_value;
    read(from_server, &syn_ack_value, sizeof(syn_ack_value));

    printf("[CLIENT]: Received SYN-ACK: %d\n", syn_ack_value);

    int ack_value = syn_ack_value + 1;
    write(upstream, &ack_value, sizeof(ack_value));

    printf("[CLIENT]: Sent ACK: %d\n", ack_value);
    printf("[CLIENT]: Handshake complete\n");

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
