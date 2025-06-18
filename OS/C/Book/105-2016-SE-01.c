#include <err.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/wait.h>
void dup2fd(int old, int new) {
    int res = dup2(old, new);
    if (res < 0) {
        err(7, "Couldnt dup!\n");
    }
}
int main(int argc, char* argv[]) {
    if ( argc != 2 ) {
        errx(1, "Should have 1 param!\n");
    }

    int pfd[2];
    if (pipe(pfd) < 0) {
        err(2, "Couldnt pipe!\n");
    }

    int pid = fork();
    if (pid < 0 ) {
        err(3, "Couldnt fork!\n");
    }

    if (pid == 0) {
        close(pfd[0]);
        dup2fd(pfd[1], 1);
        close(pfd[1]);

        execlp("cat", "cat", argv[1], (char*)NULL);
        err(4, "Couldnt exec cat!\n");
    }

    int pid_sort = fork();
    if (pid_sort < 0) {
        err(5, "Couldnt fork for sort!\n");
    }

    if (pid_sort == 0) {
        close(pfd[1]);
        dup2fd(pfd[0], 0);
        close(pfd[0]);

        execlp("sort", "sort", (char*) NULL);
        err(6, "Couldnt exec sort!\n");
    }
    close(pfd[0]);
    close(pfd[1]);
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
        errx(8, "Child didnt exit normally!\n");
    }
    waitpid(pid_sort, &status, 0);
    if (!WIFEXITED(status)) {
        errx(9, "Child didnt exit normally!\n");
    }
}
