#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int openFDRead(char* path) {
    int fd = open(path, O_RDONLY);
    if ( fd < 0 ) {
        err(2, "Cannot open file for reading: %s\n", path);
    }
    return fd;
}

void writeFD(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0 ) {
        err(4, "Cannot write in file!\n");
    } else if ( res != size ) {
        errx(5, "Cannot write all bytes in file!\n");
    }
}

const uint16_t MAX = ((1 << 16) - 1);

void plusAndValidate(uint16_t* pos) {
    if (*pos == MAX) {
        errx(6, "Limit of position reached and overflows, cannot work with that big file!\n");
    }
    ++(*pos);
}

int main(int argc, char* argv[]) {
    if ( argc < 4) {
        err(1, "3 args needed!\n");
    }
    int fd1 = openFDRead(argv[1]);
    int fd2 = openFDRead(argv[2]);

    int bytes1, bytes2;
    uint8_t ch1, ch2;
    uint16_t pos = 0;
    int fdW = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, 0744);
    if ( fdW < 0 ) {
        err(3, "Cannot open file for wrinting: %s\n", argv[3]);
    }
    while( ((bytes1 = read(fd1, &ch1, sizeof(ch1))) == sizeof(ch1)) && ((bytes2 = read(fd2, &ch2, sizeof(ch2))) == sizeof(ch2))) {
        if ( ch1 == ch2 ) {
            plusAndValidate(&pos);
            continue;
        }
        writeFD(fdW, &pos, sizeof(pos));
        writeFD(fdW, &ch1, sizeof(ch1));
        writeFD(fdW, &ch2, sizeof(ch2));
        plusAndValidate(&pos);
    }
    if (bytes1 < 0) {
       err(7, "Couldnt read from: %s\n", argv[1]);
    } else if (bytes2 < 0) {
        err(8, "Couldnt read from %s\n", argv[2]);
    }
}
