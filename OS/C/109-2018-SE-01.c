#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

void closeAllPipes(int pipes[3][2]) {
    for(int i =0 ; i < 3; ++i) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

void dup2im(int old, int new) {
    int res = dup2(old, new);
    if ( res < 0) {
        err(4, "Couldnt dup2!\n");
    }
}

int main(int argc, char* argv[]) {
    if ( argc != 2 ) {
        errx(1, "Must have 2 args!\n");
    }

    int pipes[3][2];
    int pids[4];

    for(int i = 0; i < 3; ++i) {
        if (pipe(pipes[i]) < 0) {
            err(2, "Couldnt pipe!\n");
        }
    }


    pids[0] = fork();
    if (pids[0] < 0) {
        err(3, "Invalid fork!\n");
    }
    if (pids[0] == 0) {
        dup2im(pipes[0][1], 1);
        closeAllPipes(pipes);
        execlp("find", "find", argv[1], "-mindepth", "1", "-printf", "%T@ %p\n", (char*) NULL);
        err(5, "Couldnt execute find!\n");
    }

    pids[1] = fork();
    if (pids[1] < 0) {
        err(3, "Invalid fork!\n");
    }
    if (pids[1] == 0) {
        dup2im(pipes[0][0], 0);
        dup2im(pipes[1][1], 1);
        closeAllPipes(pipes);
        execlp("sort", "sort", "-n", (char*) NULL);
        err(5, "Couldnt execute sort!\n");
    }

    pids[2] = fork();
    if (pids[2] < 0) {
        err(3, "Invalid fork!\n");
    }
    if (pids[2] == 0) {
        dup2im(pipes[1][0], 0);
        dup2im(pipes[2][1], 1);
        closeAllPipes(pipes);
        execlp("head", "head", "-n1", (char*) NULL);
        err(5, "Couldnt execute head!\n");
    }

    pids[3] = fork();
    if (pids[3] < 0) {
        err(3, "Invalid fork!\n");
    }
    if (pids[3] == 0) {
        dup2im(pipes[2][0], 0);
        closeAllPipes(pipes);
        execlp("cut", "cut", "-d", " ", "-f2", (char*) NULL);
        err(5, "Couldnt execute cut!\n");
    }

    closeAllPipes(pipes);

    for(int i = 0; i < 4; ++i) {
        int status;
        waitpid(pids[i], &status, 0);
        if (!WIFEXITED(status)) {
            errx(6, "The child didnt exit normally!\n");
        } else if (status != 0) {
            errx(7, "The child exited with code %i\n", status);
        }
    }
}
