#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
void writeBytes(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(2, "Error while writing bytes!\n");
    }
    if ( res != size ) {
        errx(3, "Couldnt write all bytes!\n");
    }
}
void cat(int fd) {
    uint8_t buf[4096];
    int bytes;
    while ((bytes = read(fd, buf, sizeof(buf))) > 0) {
        writeBytes(1, buf, bytes);
    }
    if (bytes < 0) {
        err(1, "Invalid reading!\n");
    }
}

int openFDRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if ( fd < 0) {
        err(4, "Couldnt open file %s for reading!\n", name);
    }
    return fd;
}

int main(int argc, char* argv[]) {
    if ( argc < 2 ) {
        cat(0);
        exit(0);
    }

    for(int i = 1; i < argc; ++i ) {
        if ( strcmp(argv[i], "-") == 0) {
            cat(0);
        } else {
            int fd = openFDRead(argv[i]);
            cat(fd);
        }
    }
}
