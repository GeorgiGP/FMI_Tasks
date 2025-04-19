#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(void) {
    int pfd[2];

    if ( pipe(pfd) < 0) {
        err(1, "Couldn't pipe");
    }

    int pid = fork();

    if (pid < 0) {
        err(1, "Couldn't fork!");
    } else if (pid > 0) {
        close(pfd[1]);
        int cpyStdIn = dup(0);
        dup2(pfd[0], 0);
        close(pfd[0]);
        int pid = fork();
        if (pid == 0) {
            if ( 0 > execlp("wc", "wc", "-l", (char*) NULL) ) {
                err(1, "Couldn't exec wc");
            }
        } else {
            dup2(cpyStdIn, 0);
            execlp("wc", "wc", "-l", (char*) NULL);
        }
    } else {
        close(pfd[0]);
        const char msg[128] = "Some\nrand|\nwow   textttt!\n";
        write(pfd[1], msg, strlen(msg));
        close(pfd[1]);
    }

}
