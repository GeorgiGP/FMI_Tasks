#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <string.h>
int openRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if ( fd < 0) {
        err(2, "Cannot open file for reading: %s!\n", name);
    }
    return fd;
}
void readBytes(int fd, uint8_t* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(3, "Cannot read bytes!\n");
    }
    if ( res != size ) {
        errx(4, "Cannot read all bytes!\n");
    }
}

void peekBytes(int fd, uint8_t* content, long size, int pos) {
    int res = pread(fd, content, size, pos);
    if ( res < 0 ) {
        err(9, "Cannot peek bytes!\n");
    }
    if ( res != size ) {
        errx(10, "Cannot peek all bytes from position: %d!\n", pos);
    }
}
void writeBytes(int fd, const uint8_t* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0 ) {
        err(12, "Cannot write bytes!\n");
    }
    if ( res != size ) {
        errx(13, "Cannot write all bytes!\n");
    }
}
int main(int argc, char* argv[]) {
    if ( argc < 4) {
        errx(1, "Too few args! Must have 3!\n");
    }
    int f1 = openRead(argv[1]);
    int f2 = openRead(argv[2]);
    int fW = open(argv[3], O_TRUNC | O_CREAT | O_RDWR, 0744);
    if ( fW < 0 ) {
        err(6, "Cannot open file for writing: %s!\n", argv[3]);
    }

    char buf[4096];
    int bytes;
    while ( ( bytes = read(f2, buf, sizeof(buf) - 1)) > 0) {
        buf[bytes] = '\0';
        write(fW, buf, strlen(buf));
    }
    if ( bytes < 0) {
        err(5, "Error while reading bytes!\n");
    }
    uint16_t pos;
    uint8_t orig, newb;

    while ( ( bytes = read(f1, &pos, sizeof(pos))) == sizeof(pos)) {
        readBytes(f1, &orig, sizeof(orig));
        readBytes(f1, &newb, sizeof(newb));

        if ( lseek(fW, pos, SEEK_SET) < 0 ) {
            err(8, "Couldnt seek at pos: %d, in file: %s!\n", pos, argv[3]);
        }
        uint8_t cur;
        peekBytes(fW, &cur, sizeof(cur), pos);
        if ( cur != orig ) {
            errx(11, "Mismatch of old byte with the expected!\n");
        }
        writeBytes(fW, &newb, sizeof(newb));
    }
    if ( bytes < 0 ) {
        err(7, "Cannot read position arg from %s!\n", argv[1]);
    }
}
