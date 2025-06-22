#include <err.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void closeAllPipes(int pip[8][2], int count,  int noRead, int noWrite) {
    for(int i =0; i< count; ++i) {
        if ( i != noRead ) {
            close(pip[i][0]);
        }
        if ( i != noWrite ) {
            close(pip[i][1]);
        }
    }
}
void readfd(int fd, void* content, long size) {
    int r = read(fd, content, size);
    if (r < 0) {
        err(8, "Error, couldnt read bytes!\n");
    }
    if (r != size) {
        errx(9, "Couldnt read all bytes!\n");
    }
}
void writefd(int fd, const void* content, long size) {
    int r = write(fd, content, size);
    if (r < 0) {
        err(8, "Error, couldnt write bytes!\n");
    }
    if (r != size) {
        errx(9, "Couldnt write all bytes!\n");
    }
}

int main(int argc, char* argv[]) {
    if ( argc != 3 ) {
        errx(1, "Args must be 3!\n");
    }

    int NC = atoi(argv[1]);
    int WC = atoi(argv[2]);
    if ( NC <= 0 ||  WC <= 0) {
        errx(2, "The args must be valid positive digits!\n");
    }
    if ( NC > 7 || WC > 35 ) {
        errx(3, "NC <= 7 and WC <= 35!\n");
    }

    int pfd[8][2];
    for (int i = 0; i < (NC + 1); ++i) {
        if (pipe(pfd[i]) < 0) {
            err(4, "Couldnt pipe at %i iteration!\n", i);
        }
    }

    int pids[7];

    char msgs[3][4] = { "tic ", "tac ", "toe\n" };

    for(int i = 1; i <= NC; ++i) {
        int pid = pids[i - 1] = fork();
        if (pid < 0) {
            err(5, "Couldnt fork!\n");
        }
        if (pid == 0) {
            closeAllPipes(pfd, NC + 1, i, (i + 1) % (NC+1));
            int cnt;
            while(true) {
                readfd(pfd[i][0], &cnt, sizeof(cnt));
                if ( cnt >= WC ) {
                    ++cnt;
                    if ( cnt - WC != NC + 1) {
                        writefd(pfd[(i+1)%(NC+1)][1], &cnt, sizeof(cnt));
                    }
                    close(pfd[(i+1)%(NC+1)][1]);
                    close(pfd[i][0]);
                    exit(0);
                }
                char msg[3];
                snprintf(msg, sizeof(msg), "%i", i);
                writefd(1, msg, strlen(msg));
                writefd(1, *(msgs + (cnt % 3)), 4);
                ++cnt;
                writefd(pfd[(i+1)%(NC+1)][1], &cnt, sizeof(cnt));
            }
        }
    }

    closeAllPipes(pfd, NC + 1, 0, 1);

    writefd(1, msgs[0], 4);
    int cnt = 1;
    write(pfd[1][1], &cnt, sizeof(cnt));
    while(true) {
        readfd(pfd[0][0], &cnt, sizeof(cnt));
        if ( cnt >= WC ) {
            ++cnt;
            if ( cnt - WC != NC + 1) {
                writefd(pfd[1][1], &cnt, sizeof(cnt));
            }
            close(pfd[1][1]);
            close(pfd[0][0]);

            int status;
            for ( int i = 0 ; i< NC; ++i) {
                waitpid(pids[i], &status, 0);
                if (!WIFEXITED(status)) {
                    errx(9, "The child didnt exit in a normal way + %i!\n", status);
                } else if (WEXITSTATUS(status) != 0) {
                    errx(10, "The child exited with code %i !\n", WEXITSTATUS(status));
                }
            }
            exit(0);
        }
        writefd(1, *(msgs + (cnt % 3)), 4);
        ++cnt;
        writefd(pfd[1][1], &cnt, sizeof(cnt));
    }

}
