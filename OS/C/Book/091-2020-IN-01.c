#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

int openRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if ( fd < 0 ) {
        err(1, "Cannot open %s for reading!\n", name);
    }
    return fd;
}

int openReadWrite(const char* name) {
    int fd = open(name, O_RDWR | O_EXCL | O_CREAT, 0744);
    if ( fd < 0 ) {
        err(2, "Cannot open %s for reading and writing!\n", name);
    }
    return fd;
}

int readFD(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(3, "Error: Couldnt read from file!\n");
    }
    return res;
}

void readFDAll(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(4, "Error: Couldnt read from file!\n");
    }
    if ( res != size) {
        errx(5, "Couldnt read all bytes from file!\n");
    }
}

void peekFDAll(int fd, void* content, long size, int pos) {
    int res = pread(fd, content, size, pos);
    if ( res < 0 ) {
        err(6, "Error: Couldnt peek from file!\n");
    }
    if ( res != size) {
        errx(7, "Couldnt peek all bytes from file!\n");
    }
}

void writeFD(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0 ) {
        err(8, "Error: Couldnt write in file!\n");
    }
    if ( res != size ) {
        errx(9, "Couldnt write all bytes!\n");
    }
}

void copy(int source, int dest) {
    uint8_t buf[4096];
    int bytes;
    while ( (bytes = readFD(source, buf, sizeof(buf))) > 0 ) {
        writeFD(dest, buf, bytes);
    }
}

void lseekFD(int fd, int offset, int pos) {
    int res = lseek(fd, offset, pos);
    if ( res < 0 ) {
        err(10, "Couldnt lseek in file!\n");
    }
}

void replace(int source, int fd, uint16_t count) {
    uint16_t offset;
    uint8_t byte, newByte, cur;
    for(size_t i = 0; i < count; ++i) {
        readFDAll(source, &offset, sizeof(offset));
        readFDAll(source, &byte, sizeof(byte));
        readFDAll(source, &newByte, sizeof(newByte));

        lseekFD(fd, offset, SEEK_SET);
        peekFDAll(fd, &cur, sizeof(cur), offset);
        if ( cur != byte ) {
            errx(11, "Old required word does not match!\n");
        }
        writeFD(fd, &newByte, sizeof(newByte));
    }
}

void replaceV1(int source, int fd, uint16_t count) {
    uint32_t offset;
    uint16_t byte, newByte, cur;
    for(size_t i = 0; i < count; ++i) {
        readFDAll(source, &offset, sizeof(offset));
        readFDAll(source, &byte, sizeof(byte));
        readFDAll(source, &newByte, sizeof(newByte));

        lseekFD(fd, offset, SEEK_SET);
        peekFDAll(fd, &cur, sizeof(cur), offset);
        if ( cur != byte ) {
            errx(12, "Old required word does not match!\n");
        }
        writeFD(fd, &newByte, sizeof(newByte));
    }
}

int main(int argc, char* argv[]) {
    if ( argc < 4 ) {
        errx(13, "Must have 3 args!\n");
    }
    int fd1 = openRead(argv[1]);
    int fd2 = openRead(argv[2]);
    int fRW = openReadWrite(argv[3]);

    uint32_t magic;
    uint8_t headerVersion, dataVersion;
    uint16_t count;

    readFDAll(fd1, &magic, sizeof(magic));

    if( magic != 0xEFBEADDE ) {
        errx(14, "Invalid specification: 0x%x!\n", magic);
    }
    readFDAll(fd1, &headerVersion, sizeof(headerVersion));
    if ( headerVersion != 0x01 ) {
        errx(15, "Unknown Header Version: %d\n", headerVersion);
    }

    readFDAll(fd1, &dataVersion, sizeof(dataVersion));
    readFDAll(fd1, &count, sizeof(count));
    copy(fd2,fRW);
    lseekFD(fd1, sizeof(uint32_t) * 2, SEEK_CUR);
    switch (dataVersion) {
        case 0x00:
            replace(fd1, fRW, count);
            break;
        case 0x01:
            replaceV1(fd1, fRW, count);
            break;
        default:
            errx(16, "Invalid Data Version: %d\n", dataVersion);
    }

    close(fd1);
    close(fd2);
    close(fRW);
}
