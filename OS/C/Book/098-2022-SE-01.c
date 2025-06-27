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

int openReadWrite(const char* name) {
    int fd = open(name, O_RDWR);
    if ( fd < 0) {
        err(4, "Couldnt open %s for reading and writing!\n", name);
    }
    return fd;
}
int openRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if ( fd < 0) {
        err(4, "Couldnt open %s for reading!\n", name);
    }
    return fd;
}
long mylseek(int fd, long pos, int mode) {
    long posR = lseek(fd, pos, mode);
    if ( posR < 0) {
        err(6, "Couldnt lseek at position!\n");
    }
    return posR;
}

struct headerData_t {
    uint32_t magic;
    uint32_t count;
};

struct headerComp_t {
    uint32_t magic1;
    uint16_t magic2;
    uint16_t reserved;
    uint64_t count;
};

struct elementComp_t {
    uint16_t type;
    uint16_t reserved[3];
    uint32_t offset1, offset2;
};

int main(int argc, char* argv[]) {

    if ( argc != 3 ) {
        errx(1, "Args must be 3!\n");
    }

    int fdData = openReadWrite(argv[1]);
    int fdComp = openRead(argv[2]);

    struct headerData_t hData;
    struct headerComp_t hComp;

    readStrict(fdData, &hData, sizeof(hData));
    readStrict(fdComp, &hComp, sizeof(hComp));

    const uint32_t theMagicData = 0x21796F4A;
    const uint32_t theMagicComp1 = 0xAFBC7A37;
    const uint16_t theMagicComp2 = 0x1C27;
    if ( hData.magic != theMagicData || hComp.magic1 != theMagicComp1 || hComp.magic2 != theMagicComp2) {
        errx(7, "Logic error with input data magic numbers!\n");
    }

    for(size_t i = 0; i < hComp.count; ++i) {
        struct elementComp_t elComp;
        readStrict(fdComp, &elComp, sizeof(elComp));
        for (int j = 0; j < 3; ++j) {
            if ( elComp.reserved[j] != 0) {
                errx(8, "Logic error, the reserved values for Comparator elements must be 0!\n");
            }
        }
        if ( elComp.type != 0 && elComp.type != 1) {
            errx(9, "Logic error, type of comparator value should be 1 or 0!\n");
        }
        if (elComp.offset1 >= hData.count || elComp.offset2 >= hData.count) {
            errx(12, "Logic error, this position goes out of allowed elements for data file!\n");
        }

        uint64_t pos1 = mylseek(fdData, sizeof(struct headerData_t) + sizeof(uint64_t) * elComp.offset1, SEEK_SET);
        uint64_t el1;
        readStrict(fdData, &el1, sizeof(el1));
        uint64_t pos2 = mylseek(fdData, sizeof(struct headerData_t) + sizeof(uint64_t) * elComp.offset2, SEEK_SET);
        uint64_t el2;
        readStrict(fdData, &el2, sizeof(el2));
        if ( elComp.type == 0 ) {
            if ( el1 > el2) {
                printf("Swapped values to be lower - v1: %lu, v2: %lu, pos1: %lu, pos2: %lu!\n", el1, el2, pos1, pos2);
                mylseek(fdData, pos1, SEEK_SET);
                writefd(fdData, &el2, sizeof(el2));
                mylseek(fdData, pos2, SEEK_SET);
                writefd(fdData, &el1, sizeof(el1));
            }
        } else {
            if ( el1 < el2 ) {
                printf("Swapped values to be bigger - v1: %lu, v2: %lu, pos1: %lu, pos2: %lu!\n", el1, el2, pos1, pos2);
                mylseek(fdData, pos1, SEEK_SET);
                writefd(fdData, &el2, sizeof(el2));
                mylseek(fdData, pos2, SEEK_SET);
                writefd(fdData, &el1, sizeof(el1));
            }
        }
    }

}
