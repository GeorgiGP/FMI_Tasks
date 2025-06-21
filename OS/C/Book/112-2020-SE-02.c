#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <err.h>

int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if (fd < 0 ) {
        err(4, "Couldnt open %s for writing!\n", fileName);
    }
    return fd;
}
void writefd(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(2, "Error while writing in fd!\n");
    }
    if ( res != size) {
        errx(3, "Couldnt write all data in fd!\n");
    }
}

int readfd(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(9, "Couldnt read from fd, error!\n");
    }
    return res;
}

void dup2fd(int old, int new) {
    int res = dup2(old, new);
    if ( res < 0) {
        err(7, "Couldnt dup2!\n");
    }
}

int main(int argc, char* argv[]) {
    if ( argc != 3) {
        errx(1, "Must have 3 args!\n");
    }

    int pfd[2];
    if (pipe(pfd) < 0) {
        err(5, "Couldnt pipe!\n");
    }

    int pid = fork();
    if ( pid < 0) {
        err(6, "Couldnt fork!\n");
    }
    if ( pid == 0) {
        close(pfd[0]);
        dup2fd(pfd[1], 1);
        close(pfd[1]);

        execlp("cat", "cat", argv[1], (char*)NULL);
        err(8, "Couldnt exec cat!\n");
    }

    close(pfd[1]);

    int fdR = pfd[0];
    int fdW = openWrite(argv[2]);

    uint8_t byte;
    while(readfd(fdR, &byte, sizeof(byte)) == sizeof(byte)) {
        if ( byte == 0x7D ) {
            if ( readfd(fdR, &byte, sizeof(byte)) != sizeof(byte)) {
                err(9, "Invalid escape char!\n");
            } else {
                byte ^= 0x20;
                writefd(fdW, &byte, sizeof(byte));
            }
        } else if ( byte != 0x55 ) {
            writefd(fdW, &byte, sizeof(byte));
        }
    }

    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
        errx(10, "The child couldnt exit in a normal way with code!\n");
    }
    close(fdW);
    close(fdR);
}
