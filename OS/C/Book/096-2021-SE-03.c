#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <string.h>

int openRead(const char* file) {
    int fd = open(file, O_RDONLY);
    if ( fd < 1 ) {
        err(2, "Couldnt open %s for reading!\n", file);
    }
    return fd;
}

int openWrite(const char* file) {
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if ( fd < 1 ) {
        err(3, "Couldnt open %s for writing!\n", file);
    }
    return fd;
}

int lseekfd(int fd, int offset, int mode) {
    int res = lseek(fd, offset, mode);
    if ( res < 0 ) {
        err(4, "Couldnt lseek.\n");
    }
    return res;
}

uint32_t getSizeFile(int fd) {
    int curPos = lseekfd(fd, 0, SEEK_CUR);
    uint32_t size = lseekfd(fd, 0, SEEK_END);
    lseek(fd, curPos, SEEK_SET);
    return size;
}

void readfd(int fd, void* content, int size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(9, "Couldnt read bytes from file!\n");
    }
    if ( res != size ) {
        errx(10, "Couldnt read all bytes from file!\n");
    }
}

void writefd(int fd, const void* content, int size) {
    int res = write(fd, content, size);
    if ( res < 0 ) {
        err(5, "Couldnt write bytes in file!\n");
    }
    if ( res != size ) {
        errx(6, "Couldnt write all bytes in file!\n");
    }
}

int main(int argc, char* argv[]) {
    if ( argc != 3 ) {
        errx(1, "Args must be 3!\n");
    }

    int fdRead = openRead(argv[1]);
    int fdWrite = openWrite(argv[2]);

    uint32_t size = getSizeFile(fdRead);
    if ( size % 2 == 1) {
        errx(7, "Logic error, the size should be even number!\n");
    }
    if ( size == 0 ) {
        errx(8, "The file is empty!\n");
    }
    uint32_t count = size / 2;
    {
        const char msg[] = "const uint32_t arrN = ";
        writefd(fdWrite, msg, strlen(msg));
    }

    char msg[50];
    snprintf(msg, sizeof(msg), "%u", count);
    writefd(fdWrite, msg, strlen(msg));
    writefd(fdWrite, ";\n", 2);

    {
        const char msg2[] = "const uint16_t arr[] = {";
        writefd(fdWrite, msg2, strlen(msg2));
    }

    uint16_t curElement;
    for(uint64_t i = 0; i < count - 1; ++i) {
        readfd(fdRead, &curElement, sizeof(curElement));
        snprintf(msg, sizeof(msg), "%u", curElement);
        writefd(fdWrite, msg, strlen(msg));
        writefd(fdWrite, ",", 1);
    }
    readfd(fdRead, &curElement, sizeof(curElement));
    snprintf(msg, sizeof(msg), "%u", curElement);
    writefd(fdWrite, msg, strlen(msg));
    writefd(fdWrite, "};\n", 3);
}
