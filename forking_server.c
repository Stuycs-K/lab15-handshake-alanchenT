#include "pipe_networking.h"
#include "signal.h"

static void handle_sigint(int signo) {
    if (signo != SIGINT) {
        return;
    }

    remove(WKP);
    exit(EXIT_SUCCESS);
}

void run_subserver(int from_client) {
    int to_client = server_handshake_half(from_client);

    pid_t pid = getpid();
    char message[256];

    while (1) {
        read(from_client, &message, sizeof(message));
        printf("[SUBSERVER %d]: Received message: %s\n", pid, message);
    }
}

int main() {
    signal(SIGINT, handle_sigint);

    int from_client;

    srand(time(NULL));

    while (1) {
        from_client = server_setup();

        pid_t pid = fork();
        if (pid == 0) {
            run_subserver(from_client);
        }
    }

    return 0;
}