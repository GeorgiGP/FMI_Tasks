#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
void dup2fd(int old, int new){
    if(dup2(old, new) < 0) {
        err(3, "Couldnt dup!\n");
    }
}
int main(int argc, char* argv[]) {
    int pfd[2];
    int prevPipe = -1;
    int pids[100];
    for(int i = 1; i < argc; ++i) {
        if (i < argc - 1 ) {
            if (pipe(pfd) < 0) {
                err(1, "Invalid pipe!\n");
            }
        }

        int pid = fork();
        if (pid < 0) {
            err(2, "Couldnt fork!\n");
        }

        if (pid == 0) {
            if (i < argc - 1) {
                close(pfd[0]);
                dup2fd(pfd[1], 1);
                close(pfd[1]);
            }

            if (prevPipe != -1) {
                dup2fd(prevPipe, 0);
                close(prevPipe);
            }

            execlp(argv[i], argv[i], (char*) NULL);
            err(3, "Couldnt exec %s !\n", argv[i]);
        }

        if (prevPipe != -1) {
            close(prevPipe);
        }

        if (i < argc - 1) {
            close(pfd[1]);
            prevPipe = pfd[0];
        }
        pids[i - 1] = pid;
    }

    int status;
    for (int i =1 ; i < argc; ++i) {
        waitpid(pids[i-1], &status, 0);
    }
}
