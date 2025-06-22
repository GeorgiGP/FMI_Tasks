#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void dup2fd(int old, int new) {
    if ( dup2(old, new) < 0) {
        err(4, "Couldnt dup2!\n");
    }
}
void waitPid(int pid) {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
        errx(9, "Child Couldnt exit in a normal way!\n");
    } else if (WEXITSTATUS(status)) {
        errx(10, "The child exited with code %i!\n", WEXITSTATUS(status));
    }
}
void readfd(int fd, void* content, long size) {
    int r = read(fd, content, size);
    if ( r < 0) {
        err(5, "Error while reading bytes!\n");
    }
    if ( r != size) {
        err(6, "Couldnt read all bytes\n");
    }
}

int readfdNonStrict(int fd, void* content, long size) {
    int r = read(fd, content, size);
    if ( r < 0) {
        err(5, "Error while reading bytes!\n");
    }
    return r;
}

int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (fd < 0) {
        err(8, "Couldnt open %s for writing!\n", fileName);
    }
    return fd;
}
int main(int argc, char* argv[]) {
    if ( argc != 2) {
        errx(1, "2 args required!\n");
    }

    int pfd[2][2];
    for(int i = 0; i < 2; ++i) {
        if (pipe(pfd[i]) < 0) {
            err(2, "Couldnt pipe!\n");
        }
    }

    int pid = fork();
    if (pid < 0) {
        err(3, "Coulnt fork!\n");
    }
    if ( pid == 0 ) {
        dup2(pfd[0][1], 1);
        close(pfd[0][0]);
        close(pfd[0][1]);
        close(pfd[1][0]);
        close(pfd[1][1]);

        execlp("find", "find", argv[1], "-type", "f", (char*)NULL);
        err(4, "Couldnt exec find!\n");
    }

    int pidG = fork();

    if (pidG < 0) {
        err(3, "Coulnt fork!\n");
    }
    if ( pidG == 0 ) {
        dup2(pfd[0][0], 0);
        dup2(pfd[1][1], 1);
        close(pfd[0][0]);
        close(pfd[0][1]);
        close(pfd[1][0]);
        close(pfd[1][1]);

        execlp("egrep", "egrep", "-v", ".hash$", (char*)NULL);
        err(4, "Couldnt exec egrep!\n");
    }

    close(pfd[0][0]);
    close(pfd[0][1]);
    close(pfd[1][1]);


    char ch;
    char curFile[2048];
    unsigned long curSize = 0;

    int pids[2048];
    int countChilds = 0;
    while( readfdNonStrict(pfd[1][0], &ch, sizeof(ch)) == sizeof(ch)) {
        if (ch == '\n') {
            if ( countChilds >= 2048 ) {
                errx(11, "Too many files! - max 2048!\n");
            }
            curFile[curSize] = '\0';
            curSize = 0;

            int pidH = pids[countChilds++] = fork();
            if (pidH == 0) {
                close(pfd[1][0]);

                char curFileHash[2053];
                snprintf(curFileHash, sizeof(curFileHash), "%s%s", curFile, ".hash");
                int hashFile = openWrite(curFileHash);

                dup2(hashFile, 1);
                close(hashFile);

                execlp("md5sum", "md5sum", curFile, (char*) NULL);
            }
        } else {
            if ( curSize < sizeof(curFile) - 1) {
                curFile[curSize++] = ch;
            } else {
                errx(10, "The files should be with name max lenght 2047!\n");
            }
        }
    }
    close(pfd[1][0]);

    if (curSize != 0) {
        if ( countChilds >= 2048 ) {
            errx(11, "Too many files! - max 2048!\n");
        }
        curFile[curSize] = '\0';
        curSize = 0;
        int pidH = pids[countChilds++] = fork();
        if (pidH == 0) {
            close(pfd[1][0]);

            char curFileHash[2053];
            snprintf(curFileHash, sizeof(curFileHash), "%s%s", curFile, ".hash");
            int hashFile = openWrite(curFileHash);
            dup2(hashFile, 1);
            close(hashFile);

            execlp("md5sum", "md5sum", curFile, (char*) NULL);
        }
    }

    waitPid(pid);
    waitPid(pidG);

    for(int i = 0; i < countChilds; ++i) {
        waitPid(pids[i]);
    }

}
