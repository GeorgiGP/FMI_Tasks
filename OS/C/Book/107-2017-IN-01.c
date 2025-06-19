#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/wait.h>
void dup2fd(int old, int new) {
    int res = dup2(old, new);
    if ( res  < 0) {
        err(3, "Invalid dup2!\n");
    }
}

void closeUseless(int pfd[4][2]) {
    for(int i = 0 ; i < 4; ++i) {
        close(pfd[i][0]);
        close(pfd[i][1]);
    }
}

void waitp(int pid) {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
        errx(6, "Child didnt exit normally!\n");
    } else {
        printf("%i\n", status);
    }
}

int main(void) {
    int pfd[4][2];
    for (int i = 0; i < 4; ++i) {
        if (pipe(pfd[i]) < 0) {
            err(1, "Invalid pipe!\n");
        }
    }

    int pids[5];
    pids[0] = fork();

    if (pids[0] < 0) {
        err(2, "Invalid fork!\n");
    }
    if (pids[0] == 0) {
        dup2fd(pfd[0][1], 1);
        closeUseless(pfd);

        execlp("cat", "cat", "/etc/passwd", (char*) NULL);
        err(4, "Couldnt exec cat!\n");
    }

    pids[1] = fork();
    if (pids[1] < 0) {
        err(2, "Invalid fork!\n");
    }
    if (pids[1] == 0) {
        dup2fd(pfd[0][0], 0);
        dup2fd(pfd[1][1], 1);
        closeUseless(pfd);

        execlp("cut", "cut", "-d:", "-f7" , (char*) NULL);
        err(4, "Couldnt exec cut!\n");
    }

    pids[2] = fork();
    if (pids[2] < 0) {
        err(2, "Invalid fork!\n");
    }
    if (pids[2] == 0) {
        dup2fd(pfd[1][0], 0);
        dup2fd(pfd[2][1], 1);
        closeUseless(pfd);

        execlp("sort", "sort", (char*) NULL);
        err(4, "Couldnt exec sort!\n");
    }

    pids[3] = fork();
    if (pids[3] < 0) {
        err(2, "Invalid fork!\n");
    }
    if (pids[3] == 0) {
        dup2fd(pfd[2][0], 0);
        dup2fd(pfd[3][1], 1);
        closeUseless(pfd);

        execlp("uniq", "uniq", "-c", (char*) NULL);
        err(4, "Couldnt exec uniq!\n");
    }

    pids[4] = fork();
    if (pids[4] < 0) {
        err(2, "Invalid fork!\n");
    }
    if (pids[4] == 0) {
        dup2fd(pfd[3][0], 0);
        closeUseless(pfd);

        execlp("sort", "sort", "-n", (char*) NULL);
        err(4, "Couldnt exec sort -n!\n");
    }

    closeUseless(pfd);

    for(int i = 0 ; i < 5; ++i) {
        waitp(pids[i]);
    }
}
