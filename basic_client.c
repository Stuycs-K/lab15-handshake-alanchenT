#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);

    printf("=================================\n");

    ssize_t bytes;

    int random_number;
    while (1) {
        bytes = read(from_server, &random_number, sizeof(random_number));
        ASSERT(bytes, "Client receive fail")

        printf("[CLIENT]: Received %d from server\n", random_number);

        sleep(1);
    }
}
