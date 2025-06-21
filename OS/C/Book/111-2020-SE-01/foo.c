#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY);
    if ( fd < 0) {
        err(4, "Couldnt open %s for writing!\n", fileName);
    }
    return fd;
}

int main(int argc, char* argv[]) {
    if ( argc != 2 ) {
        errx(1, "Args must be 2 for foo!\n");
    }

    if (mkfifo("mkfifo", 0744) < 0 && errno != EEXIST) {
        err(2, "Couldnt mkfifo!\n");
    }
    int fd = openWrite("mkfifo");
    int d = dup2(fd, 1);
    if ( d < 0) {
        err(5, "Couldnt dup2!\n");
    }

    close(fd);
    execlp("cat", "cat", argv[1], (char*) NULL);
    err(3, "Couldnt exec cat %s", argv[1]);
}
