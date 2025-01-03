#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifndef NETWORKING_H
#define NETWORKING_H

#define WKP "mario"

#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000
#define MAX_CLIENTS 100

#define SYN 0
#define SYN_ACK 1
#define ACK 2
#define MESSAGE 3
#define EXIT 4

#define ASSERT(value, error_prefix)                                                                \
    if ((value) == -1) {                                                                           \
        fprintf(stderr, "[ASSERTION FAILED]: ");                                                   \
        perror(error_prefix);                                                                      \
        exit(EXIT_FAILURE);                                                                        \
    }

void receive_loop(int receive_from, char *name);
void send_loop(int send_to, char *name);

// for forking server
int server_setup();

int server_handshake(int *to_client);
int client_handshake(int *to_server);

int server_handshake_half(int from_client);

// for basic & persistent servers
// int server_connect(int from_client);

#endif
