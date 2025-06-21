#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <stdint.h>


void writefd(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(6, "Error, Couldnt write!\n");
    }
    if ( res != size) {
        errx(7, "Coudlnt write all bytes!\n");
    }
}

void readfd(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(8, "Error, Couldnt read!\n");
    }
    if ( res != size) {
        errx(9, "Coudlnt read all bytes!\n");
    }
}

int main(int argc, char* argv[]) {
    if ( argc != 3) {
        errx(1, "3 args!\n");
    }

    int N = atoi(argv[1]);
    int D = atoi(argv[2]);

    if ( N <= 0 || D <= 0) {
        errx(3, "The number should be positive and the args must be numbers!\n");
    }
    if ( N >= 10 || D >= 10) {
        errx(2, "The numbers should be single digits!\n");
    }


    int pfd[2][2];

    for(int j = 0; j < N; ++j) {
        for(int i = 0; i < 2; ++i) {
            if (pipe(pfd[i]) < 0) {
                err(4, "Couldnt pipe!\n");
            }
        }

        int pid = fork();
        if (pid < 0) {
            err(5, "Couldnt fork!\n");
        }
        if (pid == 0) {
            close(pfd[0][1]);
            close(pfd[1][0]);
            uint8_t dummy;
            readfd(pfd[0][0], &dummy, sizeof(dummy));
            close(pfd[0][0]);
            writefd(1, "DONG\n", 5);
            writefd(pfd[1][1], &dummy, sizeof(dummy));
            close(pfd[1][1]);

            exit(0);
        }
        close(pfd[0][0]);
        close(pfd[1][1]);

        writefd(1, "DING\n", 5);
        uint8_t dummy = '0';
        writefd(pfd[0][1], &dummy, sizeof(dummy));
        close(pfd[0][1]);
        readfd(pfd[1][0], &dummy, sizeof(dummy));
        close(pfd[1][0]);

        sleep(D);
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            errx(11, "The child didnt exit!\n");
        } else if (WEXITSTATUS(status) != 0) {
            errx(12, "The child exited with status %i!\n", WEXITSTATUS(status));
        }
    }
}
