#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

void writefd(int fd, void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0 ) {
        err(5, "Couldnt write in fd!\n");
    }
    if ( res != size ) {
        errx(6, "Couldnt write all bytes in fd!\n");
    }
}
int readNonStrict(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0 ) {
        err(2, "Couldnt read from fd!\n");
    }
    return res;
}

int openRead(const char* fileName) {
    int fd = open(fileName, O_RDONLY);
    if ( fd < 0 ) {
        err(3, "Couldnt open %s for reading!\n", fileName);
    }
    return fd;
}
int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if ( fd < 0 ) {
        err(4, "Couldnt open %s for writing!\n", fileName);
    }
    return fd;
}

void swap(uint8_t* a, uint8_t* b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char* argv[]) {
    if ( argc != 3 ) {
        errx(1, "Required 3 args!\n");
    }
    int fdread = openRead(argv[1]);
    int fdwrite = openWrite(argv[2]);

    uint8_t res = 0;
    uint8_t buf[2];
    while ( readNonStrict(fdread, buf, sizeof(buf)) > 0 ) {
        swap(buf, buf + 1);
        uint16_t* sh = (uint16_t*)buf;
        for (int i = 7; i >= 0; --i) {
            res = (res << 1);
            if ( ( (*sh >> (i * 2) ) & 3) == 2) {
                res = res | 1;
            } else if (( (*sh >> (i * 2) ) & 3) != 1){
                errx(7, "Invalid logic in file!\n");
            }
        }
        writefd(fdwrite, &res, sizeof(res));
    }
}
