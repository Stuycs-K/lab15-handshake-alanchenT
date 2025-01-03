#include "pipe_networking.h"
#include "signal.h"

static void handle_sigint(int signo) {
    if (signo != SIGINT) {
        return;
    }

    remove(WKP);
    
    printf("\n***Interrupt! Shutting down server\n");

    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, handle_sigint);

    // When the client disconnects, the pipe breaks, which sends SIGPIPE and terminates the server
    signal(SIGPIPE, SIG_IGN);

    int to_client;
    int from_client;

    srand(time(NULL));

    while (1) {
        from_client = server_handshake(&to_client);

        printf("=================================\n");
        send_loop(to_client, "PERSISTENT SERVER");
        printf("=================================\n");

        close(to_client);
        close(from_client);
    }

    return 0;
}
