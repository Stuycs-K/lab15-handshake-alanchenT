#include "pipe_networking.h"
#include "signal.h"

int main() {
    signal(SIGPIPE, SIG_IGN);

    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);

    printf("=================================\n");

#ifdef SEND
    send_loop(to_server, "CLIENT");
#else
    receive_loop(from_server, "CLIENT");
#endif
}
