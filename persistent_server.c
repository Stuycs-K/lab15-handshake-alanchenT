#include "pipe_networking.h"
#include "signal.h"

static void handle_sigint(int signo) {
    if (signo != SIGINT) {
        return;
    }

    remove(WKP);
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, handle_sigint);

    // When the client disconnects, the pipe breaks, which sends SIGPIPE and terminates the server
    signal(SIGPIPE, SIG_IGN);

    int to_client;
    int from_client;
    ssize_t bytes;

    srand(time(NULL));
    int random_number;

    while (1) {
        from_client = server_handshake(&to_client);

        printf("=================================\n");

        // Send int
        while (1) {
            random_number = (rand() % 100) + 1;
            bytes = write(to_client, &random_number, sizeof(random_number));

            if (bytes == -1) {
                break;
            }
            // ASSERT(bytes, "Server send fail")

            printf("[SERVER]: Sent %d to client\n", random_number);

            sleep(1);
        }

        printf("=================================\n");
        printf("[SERVER]: Client disconnect\n\n");

        close(to_client);
        close(from_client);
    }

    return 0;
}