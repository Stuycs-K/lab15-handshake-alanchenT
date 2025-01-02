#include "pipe_networking.h"

int main() {
    int to_client;
    int from_client;

    from_client = server_handshake(&to_client);

    printf("=================================\n");

    ssize_t bytes;

    srand(time(NULL));

    int random_number;

    // Send int
    while (1) {
        random_number = (rand() % 100) + 1;
        bytes = write(to_client, &random_number, sizeof(random_number));
        ASSERT(bytes, "Server send fail")

        printf("[SERVER]: Sent %d to client\n", random_number);

        sleep(1);
    }
}
