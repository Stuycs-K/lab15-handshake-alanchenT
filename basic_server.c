#include "pipe_networking.h"

int main() {
    int to_client;
    int from_client;

    from_client = server_handshake(&to_client);

    printf("To client: %d, from client: %d\n", to_client, from_client);

    // Receive
    int x;
    read(from_client, &x, sizeof(x));

    printf("[SERVER TEST]: Received %d\n", x);

    int y = x + 1;
    write(to_client, &y, sizeof(y));

    printf("[SERVER TEST]: Sent %d\n", y);
}
