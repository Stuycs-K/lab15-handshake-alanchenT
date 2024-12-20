#include "pipe_networking.h"

int main() {
    int to_client;
    int from_client;

    from_client = server_handshake(&to_client);

    printf("=================================\n");
    printf("To client: %d, from client: %d\n", to_client, from_client);

    ssize_t bytes;

    // Receive
    int x;
    bytes = read(from_client, &x, sizeof(x));
    ASSERT(bytes, "Server receive fail")

    printf("[SERVER TEST]: Received %d\n", x);

    int y = x + 1;
    bytes = write(to_client, &y, sizeof(y));
    ASSERT(bytes, "Server send fail")

    printf("[SERVER TEST]: Sent %d\n", y);
}
