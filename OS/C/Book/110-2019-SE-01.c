#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
void writefd(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(5, "Error while writitng in fd!\n");
    }
    if ( res != size) {
        errx(6, "Couldnt write all bytes!\n");
    }
}
int mytime(void) {
    int res = time(NULL);
    if ( res < 0) {
        err(3, "Error when getting time!\n");
    }
    return res;
}
int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if (fd < 0) {
        err(2, "Couldnt open %s for writing!\n", fileName);
    }
    return fd;
}

int main(int argc, char* argv[]) {
    if ( argc < 3 ) {
        errx(1, "Must have at least 3 args!\n");
    }
    int dur = atoi(argv[1]);
    int log = openWrite("run.log");

    int cnt = 0;
    while(true) {
        int pid = fork();
        if (pid < 0) {
            err(4, "Couldnt fork!\n");
        }
        if (pid == 0) {
            execvp(argv[2], argv + 2);
            err(6, "Couldnt execute %s\n", argv[2]);
        }

        bool code = false;
        int status;

        int startTime = mytime();
        waitpid(pid, &status, 0);
        int endTime = mytime();
        if (WIFEXITED(status)) {
            if (status != 0) {
                code = true;
            }
            char msg[250];
            snprintf(msg, sizeof(msg), "%d %d %i\n", startTime, endTime, status);
            writefd(log, msg, strlen(msg));
        } else if (WIFSIGNALED(status)) {
            err(129, "The child killed by signal %d\n", WTERMSIG(status));
        }
        if ( (endTime - startTime < dur) && code) {
            if(++cnt == 2) {
                break;
            }
        } else {
            cnt = 0;
        }
    }

    close(log);
}
