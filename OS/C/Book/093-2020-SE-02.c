#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int openRead(const char* fileName) {
    int fd = open(fileName, O_RDONLY);
    if ( fd < 0 ) {
        err(2, "Couldnt open %s for reading \n", fileName);
    }
    return fd;
}
int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if ( fd < 0 ) {
        err(2, "Couldnt open %s for writing \n", fileName);
    }
    return fd;
}

int readNonStrict(int fd, void* content, long size) {
    int bytes = read(fd, content, size);
    if ( bytes < 0 ) {
        err(3, "Couldnt read from fd!\n");
    }
    return bytes;
}
void readStrict(int fd, void* content, long size) {
    int bytes = read(fd, content, size);
    if ( bytes < 0 ) {
        err(6, "Couldnt read from fd!\n");
    }
    if ( bytes != size ) {
        errx(7, "Couldnt read all bytes from fd!\n");
    }
}
void writefd(int fd, const void* content, long size) {
     int bytes = write(fd, content, size);
    if ( bytes < 0 ) {
        err(4, "Couldnt write in fd!\n");
    }
    if (bytes != size) {
        errx(5, "Couldnt write all bytes in fd!\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) { errx(1, "args must be 4!\n"); }

    int scl = openRead(argv[1]);
    int sdl = openRead(argv[2]);
    int result = openWrite(argv[3]);

    uint8_t byte;
    uint16_t element;
    while( readNonStrict(scl, &byte, sizeof(byte)) == sizeof(byte)) {
        for (int i = 7; i >= 0; --i) {
            readStrict(sdl, &element, sizeof(element));
            if ( (byte >> i) & 1 ) {
                writefd(result, &element, sizeof(element));
            }
        }
    }
}
