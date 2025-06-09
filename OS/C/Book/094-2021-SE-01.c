#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

void writefd(int fd, void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0 ) {
        err(5, "Couldnt write in fd!\n");
    }
    if ( res != size ) {
        errx(6, "Couldnt write all bytes in fd!\n");
    }
}
int readNonStrict(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(2, "Couldnt read from fd!\n");
    }
    return res;
}

int openRead(const char* fileName) {
    int fd = open(fileName, O_RDONLY);
    if ( fd < 0 ) {
        err(3, "Couldnt open %s for reading!\n", fileName);
    }
    return fd;
}
int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if ( fd < 0 ) {
        err(4, "Couldnt open %s for writing!\n", fileName);
    }
    return fd;
}

int main(int argc, char* argv[]) {
    if ( argc != 3 ) {
        errx(1, "Required 3 args!\n");
    }
    int fdread = openRead(argv[1]);
    int fdwrite = openWrite(argv[2]);

    uint8_t ch;
    uint16_t res = 0x0000;
    while ( readNonStrict(fdread, &ch, sizeof(ch)) > 0 ) {
        for (int i = 7; i >= 0; --i) {
            res = (res << 2);
            if ( (ch >> i) & 1) {
                res = res | 2;
            } else {
                res = res | 1;
            }
        }
        uint8_t* buf = (uint8_t*)(&res);
        writefd(fdwrite, buf + 1, sizeof(uint8_t));
        writefd(fdwrite, buf, sizeof(uint8_t));
    }
}
