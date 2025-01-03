#include "pipe_networking.h"

int main() {
    srand(time(NULL));

    int to_client;
    int from_client;

    from_client = server_handshake(&to_client);

    printf("=================================\n");

    send_loop(to_client, "BASIC SERVER");
}
