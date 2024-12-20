#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);

    printf("To server: %d, from server: %d\n", to_server, from_server);

    // Send int
    int x = 1;
    write(to_server, &x, sizeof(x));

    printf("[CLIENT TEST]: Sent %d\n", x);

    // Read int
    int y;
    read(from_server, &y, sizeof(y));

    printf("[CLIENT TEST]: Received %d\n", y);
}
