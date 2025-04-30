#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>
void writeFD(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if (res < 0) {
        err(3, "Couldnt write bytes!\n");
    } else if ( res != size ) {
        errx(4, "Couldnt write all bytes!\n");
    }
}

void cat(int fd, bool* startLine, uint32_t* counter) {
    char buf[4096];
    int bytes;
    char msg[100];
    while( (bytes = read(fd, &buf, sizeof(buf)) ) > 0) {
        for (int j = 0; j < bytes; ++j) {
            if ( *startLine ) {
                snprintf(msg, sizeof(msg), "\t%d ", (*counter)++);
                writeFD(1, msg, strlen(msg));
                *startLine = false;
            }
            writeFD(1, buf + j, sizeof(char));
            if( buf[j] == '\n') {
                *startLine = true;
            }
        }
    }
    if ( bytes < 0 ) {
        err(2, "Invalid reading!\n");
    }
}

int main(int argc, char* argv[]) {
    if ( argc < 2 || strcmp("-n", argv[1])!= 0) {
        errx(1, "Invalid option!\n");
    }
    uint32_t counter = 1;
    bool startLine = true;

    if ( argc == 2 ) {
        cat(0, &startLine, &counter);
    }

    for(int i = 2; i < argc; ++i) {
        if ( strcmp(argv[i], "-") == 0 ) {
            cat(0, &startLine, &counter);
        } else {
            int fd = open(argv[i], O_RDONLY);
            if (fd < 0) {
                err(5, "Cannot open %s for reading!\n", argv[i]);
            }
            cat(fd, &startLine, &counter);
        }
    }
}
