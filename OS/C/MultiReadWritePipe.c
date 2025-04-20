#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
int main(void) {
    int pfd[2];

    if ( pipe(pfd) < 0 ) {
        err(1, "Couldn't pipe!");
    }

    int pid = fork();
    if (pid < 0) {
        err(1, "Couldn't fork");
    }
    pid = fork();
    if (pid < 0) {
        err(1, "Couldn't fork2");
    }
    if ( pid == 0 ) {
        close(pfd[0]);
        char buf[128] = "Child!\n";
        write(pfd[1], buf, strlen(buf));
        close(pfd[1]);
    } else {
        close(pfd[1]);
        char buf[128];
        int n = read(pfd[0], buf, sizeof(buf) - 1);
        if (n < 0) {
            err(1, "read");
            return 1;
        }
        buf[n] = '\0';

        close(pfd[0]);

        char cpy[130];

        snprintf(cpy, sizeof(cpy), "%s \n", buf);
        write(1, buf, strlen(buf));
    }
}
