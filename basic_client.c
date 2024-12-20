#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);

    printf("=================================\n");
    printf("To server: %d, from server: %d\n", to_server, from_server);

    ssize_t bytes;

    // Send int
    srand(time(NULL));
    int x = rand();
    bytes = write(to_server, &x, sizeof(x));
    ASSERT(bytes, "Client send fail")

    printf("[CLIENT TEST]: Sent %d\n", x);

    // Read int
    int y;
    bytes = read(from_server, &y, sizeof(y));
    ASSERT(bytes, "Client receive fail")

    printf("[CLIENT TEST]: Received %d\n", y);
}
