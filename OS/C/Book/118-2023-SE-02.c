//issue: When the middle process gets killed -> the leaves children are zombies
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void pipeWrapper(int pfd[2]) {
    if (pipe(pfd) < 0) {
        err(26, "Couldnt pipe!\n");
    }
}

int readfd(int fd, void* content, long size) {
    int bytes = read(fd, content, size);
    if ( bytes < 0) {
        err(26, "Couldnt read!\n");
    }
    return bytes;
}

void writefd(int fd, const void* content, long size) {
    int bytes = write(fd, content, size);
    if ( bytes < 0) {
        err(26, "Couldnt write!\n");
    }
    if (bytes != size) {
        errx(26, "Couldnt write all bytes!\n");
    }
}

void dup2Wrapper(int old, int new) {
    if ( dup2(old, new) < 0) {
        err(26, "Couldnt dup2!\n");
    }
}

int dupWrapper(int fd) {
    int newFd = dup(fd);
    if (newFd < 0 ) {
        err(26, "Couldnt dup!\n");
    }
    return newFd;
}

void pushBack(char* str, long strLen, char ch) {
    for(int i = 1; i < strLen; ++i) {
        str[i - 1] = str[i];

    }
    str[strLen - 1] = ch;
}

int forkWrapper(void) {
    int res = fork();
    if (res < 0) {
        err(26, "Couldnt fork!\n");
    }
    return res;
}
int main(int argc, char* argv[]) {

    int pfd[2];
    pipeWrapper(pfd);

    int pids[1024];
    for(int i = 1; i < argc; ++i) {
        int pid = pids[i - 1] = forkWrapper();
        if (pid == 0) {
            close(pfd[0]);
            int syncWriterFd = dupWrapper(pfd[1]);
            close(pfd[1]);

            int pfdInner[2];
            pipeWrapper(pfdInner);
            int pidInner = forkWrapper();

            if (pidInner == 0) {
                close(syncWriterFd);
                close(pfdInner[0]);
                dup2Wrapper(pfdInner[1], 1);
                close(pfdInner[1]);

                execlp(argv[i], argv[i], (char*)NULL);
                err(4, "Couldnt exec: %s!\n", argv[i]);
            }

            close(pfdInner[1]);
            const char look[] = "found it!";
            char toLookFor[] = "found it!";
            char dummy = '1';
            if (readfd(pfdInner[0], toLookFor, sizeof(toLookFor) - 1) == (sizeof(look) - 1)) {
                if ( strcmp(look, toLookFor) == 0) {
                    writefd(syncWriterFd, &dummy, sizeof(dummy));
                    close(syncWriterFd);
                    close(pfdInner[0]);
                    exit(0);
                }
                while(readfd(pfdInner[0], &dummy, sizeof(dummy)) == sizeof(dummy)) {
                    pushBack(toLookFor, sizeof(toLookFor) - 1, dummy);
                    if ( strcmp(look, toLookFor) == 0) {
                        writefd(syncWriterFd, &dummy, sizeof(dummy));
                        close(syncWriterFd);
                        close(pfdInner[0]);
                        exit(0);
                    }
                }
            }
            close(syncWriterFd);
            close(pfdInner[0]);

            int status;
            if (waitpid(pidInner, &status, 0) < 0) {
                err(6, "Error while waiting other pid!\n");
            }
            exit(0);
        }
    }

    close(pfd[1]);

    char ch;
    int bytes = readfd(pfd[0], &ch, sizeof(ch));
    close(pfd[0]);
    if (bytes == sizeof(ch)) {
        char msg[] = "We found it!\n";
        write(1, msg, sizeof(msg));

        int status;
        for(int i = 0; i < argc - 1; ++i) {
            kill(pids[i], SIGTERM);
            if (waitpid(pids[i], &status, 0) < 0) {
                err(26, "Error while waiting other pid!\n");
            }
        }
        exit(0);
    }

    int status;
    for(int i = 0; i < argc - 1; ++i) {
        if (waitpid(pids[i], &status, 0) < 0) {
            err(26, "Error while waiting other pid!\n");
        } else if(!WIFEXITED(status)) {
            errx(26, "One of the childs didnt exit in a normal way with status code!\n");
        } else if (WEXITSTATUS(status)) {
            errx(26, "Process %s exited with status code: %i\n", argv[i + 1], WEXITSTATUS(status));
        }
    }
    if (bytes == 0) {
        errx(1, "We have not found what we were looking for!\n");
    }
}
