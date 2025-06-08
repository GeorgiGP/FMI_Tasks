#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <err.h>

int seekfd(int fd, int offset, int pos) {
    int res = lseek(fd, offset, pos);
    if ( res < 0 ) { err(2, "Couldnt seek"); }
    return res;
}

void readfd(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(3, "Coulnt read bytes");
    }
    if ( res != size ) {
        errx(4, "Couldnt read all bytes!");
    }
}

void writefd(int fd, void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0 ) {
        err(7, "Coulnt write bytes");
    }
    if ( res != size ) {
        errx(8, "Couldnt write all bytes!");
    }
}

uint16_t extractCount(int fd) {
    int pos = seekfd(fd, 0, SEEK_CUR);
    seekfd(fd, 4, SEEK_SET);
    uint16_t res;
    readfd(fd, &res, sizeof(res));
    seekfd(fd, pos, SEEK_SET);
    return res;
}

int openRead(const char* fileName) {
    int res = open(fileName, O_RDONLY);
    if ( res < 0) {
        err(5, "Couldnt open %s for reading!\n", fileName);
    }
    return res;
}

int openWrite(const char* fileName) {
    int res = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if ( res < 0 ) {
        err(6, "Couldnt open %s for writing!\n", fileName);
    }
    return res;
}

void readWrite(int affix, int crucifixus, int fdRead, int bytesPerElement) {
    uint16_t from, count;
        readfd(affix, &from, sizeof(from));
        readfd(affix, &count, sizeof(count));
        seekfd(fdRead, 16 + from * bytesPerElement, SEEK_SET);
        uint8_t buffer[8];
        for( int j = 0; j < count; ++j ) {
            readfd(fdRead, buffer, bytesPerElement);
            writefd(crucifixus, buffer, bytesPerElement);
        }
}

int main(int argc, char* argv[]) {
    if (argc != 7) { errx(1, "Args must be 7!"); }
    int fdAffix = openRead(argv[1]);
    int fdPostfix = openRead(argv[2]);
    int fdPrefix = openRead(argv[3]);
    int fdInfix = openRead(argv[4]);
    int fdSuffix = openRead(argv[5]);

    int crucifixus = openWrite(argv[6]);

    uint16_t countElements = extractCount(fdAffix);
    uint16_t countGroups = countElements / 8;
    seekfd(fdAffix, 16, SEEK_SET);
    for (int i = 0; i < countGroups; ++i) {
        readWrite(fdAffix, crucifixus, fdPostfix, 4);
        readWrite(fdAffix, crucifixus, fdPrefix, 1);
        readWrite(fdAffix, crucifixus, fdInfix, 2);
        readWrite(fdAffix, crucifixus, fdSuffix, 8);
    }
    printf("Count: %u\n", countElements);
}
