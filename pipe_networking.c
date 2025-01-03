#include "pipe_networking.h"

void receive_loop(int receive_from, char *name) {
    char message[256];
    while (1) {
        ssize_t bytes_read = read(receive_from, &message, sizeof(message));

        if (bytes_read <= 0) {
            break;
        }

        printf("[%s]: Received message: %s", name, message);
    }

    printf("[%s]: Connection terminated\n", name);
}

void send_loop(int send_to, char *name) {
    char message[256];

    while (1) {
        snprintf(message, sizeof(message), "This is my cool random number: %d\n", (rand() % 100) + 1);

        ssize_t bytes_written = write(send_to, &message, sizeof(message));

        if (bytes_written <= 0) {
            break;
        }

        printf("[%s]: Sent message: %s", name, message);

        sleep(1);
    }

    printf("[%s]: Connection terminated\n", name);
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

    printf("[SERVER]: Received SYN: %s\n", syn);

    int downstream = open(syn, O_WRONLY, 0);
    ASSERT(downstream, "Server open PP")
    *to_client = downstream;

    int syn_ack_value = rand();
    ssize_t bytes = write(downstream, &syn_ack_value, sizeof(syn_ack_value));
    ASSERT(bytes, "Server write SYN-ACK")

    printf("[SERVER]: Sent SYN-ACK: %d\n", syn_ack_value);
    printf("[SERVER]: Waiting for ACK...\n");

    int ack_value;
    bytes = read(from_client, &ack_value, sizeof(ack_value));
    ASSERT(bytes, "Server read ACK")

    printf("[SERVER]: Received ACK: %d\n", ack_value);

    if (ack_value != syn_ack_value + 1) {
        printf("[SERVER]: Invalid ACK received. Handshake failed\n");
        exit(EXIT_FAILURE);
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
    ssize_t bytes = write(upstream, pid_string, sizeof(pid_string));
    ASSERT(bytes, "Client write SYN")

    printf("[CLIENT]: Sent SYN: %d\n", client_pid);
    printf("[CLIENT]: Waiting for SYN-ACK...\n");

    int from_server = open(pid_string, O_RDONLY, 0);
    ASSERT(from_server, "Client open PP")

    remove(pid_string);

    int syn_ack_value;
    bytes = read(from_server, &syn_ack_value, sizeof(syn_ack_value));
    ASSERT(bytes, "Client read SYN-ACK")

    printf("[CLIENT]: Received SYN-ACK: %d\n", syn_ack_value);

    int ack_value = syn_ack_value + 1;
    bytes = write(upstream, &ack_value, sizeof(ack_value));
    ASSERT(bytes, "Client write ACK")

    printf("[CLIENT]: Sent ACK: %d\n", ack_value);
    printf("[CLIENT]: Handshake complete\n");

    return from_server;
}

int server_handshake_half(int from_client) {
    printf("[SERVER]: Waiting for SYN...\n");

    char syn[HANDSHAKE_BUFFER_SIZE];
    read(from_client, syn, sizeof(syn));

    printf("[SERVER]: Received SYN: %s\n", syn);

    int downstream = open(syn, O_WRONLY, 0);
    ASSERT(downstream, "Server open PP")

    int syn_ack_value = rand();
    ssize_t bytes = write(downstream, &syn_ack_value, sizeof(syn_ack_value));
    ASSERT(bytes, "Server write SYN-ACK")

    printf("[SERVER]: Sent SYN-ACK: %d\n", syn_ack_value);
    printf("[SERVER]: Waiting for ACK...\n");

    int ack_value;
    bytes = read(from_client, &ack_value, sizeof(ack_value));
    ASSERT(bytes, "Server read ACK")

    printf("[SERVER]: Received ACK: %d\n", ack_value);

    if (ack_value != syn_ack_value + 1) {
        printf("[SERVER]: Invalid ACK received. Handshake failed\n");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER]: Handshake complete\n");

    return downstream;
}

/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================
int server_connect(int from_client) {
    int to_client = 0;
    return to_client;
}
*/
