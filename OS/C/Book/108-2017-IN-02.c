#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
int readfd(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(4, "Error while reading content!\n");
    }
    return res;
}

int main(int argc, char* argv[]) {
    char command[5];
    if ( argc == 1) {
        strcpy(command, "echo");
    } else if ( argc == 2) {
        if (strlen(argv[1]) > 4) {
            errx(3, "Command must be with max length 4!\n");
        }
        strcpy(command, argv[1]);
    } else {
        errx(1, "Args must be 1 or 2!\n");
    }

    char ch;
    int curLen = 0;
    char msgs[2][5];
    msgs[0][0] = '\0';
    msgs[1][0] = '\0';

    int curCountWords = 0;
    bool startedWord =false;
    while ( readfd(0, &ch, sizeof(ch)) == sizeof(ch) ) {
        if ( ch == '\n' ) {
            int pid = fork();
            if (pid < 0) {
                err(7, "Couldnt fork!\n");
            }
            if (pid == 0) {
                if (curCountWords == 1) {
                    printf("%s\n", "ECHO 1");
                    execlp(command, command, msgs[0], (char*)NULL);
                    err(11, "invalid command execution!\n");
                } else if (curCountWords == 2) {
                    printf("%s\n", "ECHO 2");
                    printf("'%s' '%s' '%s'\n", msgs[0], msgs[1], command);
                    execlp(command, command, msgs[0], msgs[1], (char*)NULL);
                    err(12, "invalid command execution!\n");
                }
            } else {
                int status;
                waitpid(pid, &status, 0);
                if (!WIFEXITED(status)) {
                    errx(10, "The child didnt exit normally!\n");
                }
            }
            msgs[0][0] = '\0';
            msgs[1][0] = '\0';
            curCountWords = 0;
            startedWord = false;
            curLen = 0;
        } else if (ch == ' ') {
            startedWord = false;
            curLen = 0;
        } else {
            if (startedWord) {
                if (curLen == 4) {
                    errx(5, "Words len should be max 4!\n");
                }
                char terminated[2];
                terminated[0] = ch;
                terminated[1] = '\0';
                strcat(msgs[curCountWords - 1], terminated);
                curLen++;
            } else {
                if (curCountWords == 2) {
                    errx(8, "Should have 2 params max per row!\n");
                } else {
                    curCountWords++;
                    startedWord = true;
                    ++curLen;
                    char terminated[2];
                    terminated[0] = ch;
                    terminated[1] = '\0';
                    strcat(msgs[curCountWords - 1], terminated);
                }
            }
        }
    }

    int pid = fork();
    if (pid < 0) {
        err(7, "Couldnt fork!\n");
    }
    if (pid == 0) {
        if (curCountWords == 1) {
            execlp(command, command, msgs[0], (char*)NULL);
            err(14, "invalid command execution!\n");
        } else if (curCountWords == 2) {
            execlp(command, command, msgs[0], msgs[1], (char*)NULL);
            err(15, "invalid command execution!\n");
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            errx(9, "The child didnt exit normally!\n");
        }
    }
}
