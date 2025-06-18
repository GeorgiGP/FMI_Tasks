#include <stdio.h>
#include <sys/wait.h>
#include <err.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
void writefd(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(4, "Couldnt write, error!\n");
    }
    if ( res != size) {
        errx(5, "Couldnt write all bytes!\n");
    }
}

int readfd(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(6, "Couldnt read, error!\n");
    }
    return res;
}

int main(void) {

    while(true) {

        int pfd[2];
        if (pipe(pfd) < 0) {
            err(5, "Couldnt pipe!\n");
        }

        int pid = fork();

        if ( pid < 0) {
            err(1, "Couldn't fork!\n");
        }
        if (pid == 0) {
            close(pfd[0]);
            dup2(pfd[1], 1);
            close(pfd[1]);
            execlp("pwd", "pwd", (char*) NULL);
            err(2, "Couldnt execute pwd!\n");
        }

        close(pfd[1]);

        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            errx(3, "the child didnt exit normally for visualization prompt!\n");
        }

        int pid2 = fork();

        if ( pid2 < 0 ) {
            err(4, "Couldn't fork!\n");
        }
        if (pid2 == 0) {
            dup2(pfd[0], 0);
            close(pfd[0]);
            execlp("tr", "tr", "-d", "\n", (char*) NULL);
            err(3, "Couldnt tr new line in cmd prompt visualization!\n");
        }

        close(pfd[0]);

        waitpid(pid2, &status, 0);
        if (!WIFEXITED(status)) {
            errx(3, "the child didnt exit normally for visualization prompt!\n");
        }

        writefd(1, ">>", 2);
        char msgInput[4096];
        for(int i = 0; i < 4096; ++i) {
            if (readfd(0, msgInput + i, 1) && msgInput[i] == '\n') {
                msgInput[i] = '\0';
                break;
            }
        }

        pid = fork();
        if (pid < 0) {
            err(1, "Couldn't fork!\n");
        }

        if (pid == 0) {
            char comm[5000];
            snprintf(comm, sizeof(comm), "/bin/%s", msgInput);
            execl(comm, msgInput, (char*) NULL);
            err(2, "Couldnt execute %s!\n", msgInput);
        }

        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            errx(3, "the child didnt exit normally for command execution!\n");
        }
    }
}
