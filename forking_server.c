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
    // signal(SIGPIPE, SIG_IGN);

    int to_client = server_handshake_half(from_client);

    char server_name[32];
    snprintf(server_name, sizeof(server_name), "SUBSERVER %d", getpid());

    receive_loop(from_client, server_name);

    exit(EXIT_SUCCESS);
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