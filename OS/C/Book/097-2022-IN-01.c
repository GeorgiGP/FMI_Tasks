#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>

void writefd(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(8, "Invalid writing error!\n");
    }
    if ( res != size ) {
        errx(9, "Couldnt write all bytes!\n");
    }
}

void readStrict(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(2, "Invalid reading error!\n");
    }
    if ( res != size ) {
        errx(3, "Couldnt read all bytes!\n");
    }
}

int openRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if ( fd < 0) {
        err(4, "Couldnt open %s for reading!\n", name);
    }
    return fd;
}

int openWrite(const char* name) {
    int fd = open(name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( fd < 0) {
        err(5, "Couldnt open %s for writing!\n", name);
    }
    return fd;
}

void mylseek(int fd, long pos, int mode) {
    if ( lseek(fd, pos, mode) < 0) {
        err(6, "Couldnt lseek at position!\n");
    }
}

struct header_t {
    uint16_t magic;
    uint16_t filetype;
    uint32_t count;
};

typedef struct {
    uint8_t first;
    uint8_t second;
} pair_t ;

int main(int argc, char* argv[]) {

    if ( argc != 4 ) {
        errx(1, "Args must be 4!\n");
    }

    int fdList = openRead(argv[1]);
    int fdData = openRead(argv[2]);
    int fdW = openWrite(argv[3]);

    struct header_t h1, h2;

    readStrict(fdList, &h1, sizeof(h1));
    readStrict(fdData, &h2, sizeof(h2));

    const uint16_t theMagic = 0x5A4D;
    if ( h1.magic != theMagic || h2.magic != theMagic) {
        errx(7, "Logic error with input data magic numbers!\n");
    }

    if ( h1.filetype != 1 || h2.filetype != 2) {
        errx(10, "Logic error with file types!\n");
    }

    uint32_t countRes = h1.count;
    struct header_t h3 = { theMagic, 3, countRes };
    writefd(fdW, &h3, sizeof(h3));

    for(long i = 0; i < countRes; ++i) {
        pair_t pos;
        readStrict(fdList, &pos, sizeof(pos));
        printf("%u %u positions!\n", pos.first, pos.second);

        if (pos.first >= h2.count) {
            printf("%u data file count els, %u pos from!\n", h2.count, pos.first);
            errx(12, "Logic error, this position goes out of allowed elements for data file!\n");
        }
        if (pos.second >= countRes) {
            errx(13, "Logic error, this position goes out of allowed elements for the result!\n");
        }

        lseek(fdData, sizeof(struct header_t) + sizeof(uint32_t) * pos.first, SEEK_SET);
        uint32_t toMove;
        readStrict(fdData, &toMove, sizeof(toMove));
        printf("%x toMove data!\n", toMove);
        lseek(fdW, sizeof(struct header_t) + sizeof(uint64_t) * pos.second, SEEK_SET);
        uint64_t toMoveBig = (uint64_t) toMove;
        writefd(fdW, &toMoveBig, sizeof(toMoveBig));
    }
}
