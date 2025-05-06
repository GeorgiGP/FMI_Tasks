#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>

int openRead(const char* name) {
    int res = open(name, O_RDONLY);
    if ( res < 0) {
        err(2, "Error: Cannot open: %s for reading!\n", name);
    }
    return res;
}

int openWrite(const char* name) {
    int res = open(name, O_WRONLY | O_EXCL | O_CREAT, 0744);
    if ( res < 0) {
        err(3, "Error: Cannot open: %s for writing or already exists!\n", name);
    }
    return res;
}

int readFD(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(4, "Error: Coudnt read from file!\n");
    }
    return res;
}

void writeFD(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(5, "Error: Coudnt write in file!\n");
    }
    if (res != size) {
        errx(6, "Couldnt write all bytes in file!\n");
    }
}

int comp(const void* lhs, const void* rhs) {
    return *(const uint16_t*)lhs - *(const uint16_t*)rhs;
}

int main(int argc, char* argv[]) {
    if ( argc < 3 ) {
        err(1, "2 args required!\n");
    }

    int fd = openRead(argv[1]);
    int fdW = openWrite(argv[2]);

    uint16_t buf[65535];
    int bytes;
    if ((bytes = readFD(fd, buf, sizeof(buf))) % 2 == 1) {
        errx(7, "Invalid file format!\n");
    }

    printf("%ld %d %d\n", sizeof(buf), bytes, fdW);
    qsort(buf, bytes / sizeof(uint16_t), sizeof(uint16_t), comp);
    writeFD(fdW, buf, bytes);
    close(fd);
    close(fdW);
}
