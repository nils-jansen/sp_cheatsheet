#include <stdlib.h>
#include <signal.h>

#define MY_SIGNAL SIGCHLD
#define MY_HANDLER SIG_IGN

// TODO: sigprocmask

void handle_signal()
{
    struct sigaction action = {
        .sa_handler = MY_HANDLER,
        .sa_flags = SA_RESTART | SA_NOCLDWAIT,
    };
    if (sigemptyset(&action.sa_mask) == -1)
    {
        // error
    }
    if (sigaction(MY_SIGNAL, &action, NULL) == -1)
    {
        // error
    }
}