#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>

int openRead(const char* fileName) {
    int fd = open(fileName, O_RDONLY);
    if ( fd < 0) {
        err(4, "Couldnt open %s for reading!\n", fileName);
    }
    return fd;
}
int main(int argc, char* argv[]) {
    if ( argc < 2 ) {
        errx(1, "Args must be at least 2 for bar!\n");
    }

    int fd = openRead("mkfifo");
    int d = dup2(fd, 0);
    if ( d < 0) {
        err(5, "Couldnt dup2!\n");
    }
    close(fd);
    execv(argv[1], argv + 1);
    err(3, "Couldnt exec %s", argv[1]);
}
