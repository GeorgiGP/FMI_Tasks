#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
int readBytes(int fd, void* arg, long bytes) {
    int readbytes;
    if ( (readbytes = read(fd, arg, bytes)) < 0 ) {
        err(5, "Error while reading bytes!\n");
    } else if (readbytes != bytes) {
        errx(6, "Couldnt read all bytes\n");
    }
    return readbytes;
}

void readPeekBytes(int fd, void* arg, long bytes) {
    int readbytes;
    int pos;
    if ( (pos = lseek(fd, 0, SEEK_CUR)) < 0) {
        err(11, "Error while seeking in readPeekBytes");
    }

    if ( (readbytes = pread(fd, arg, bytes, pos)) < 0 ) {
        err(9, "Error while peek reading bytes!\n");
    } else if (readbytes != bytes) {
        errx(10, "Couldnt peek read all bytes\n");
     }
}

void writeBytes(int fd, void* arg, long bytes) {
    int writebytes;
    if ( (writebytes = write(fd, arg, bytes)) < 0 ) {
        err(5, "Error while writing bytes!\n");
    } else if (writebytes != bytes) {
        errx(6, "Couldnt write all bytes\n");
    }
}
int main(int argc, char* argv[]) {
    if ( argc < 5 ) {
        err(1, "4 args needed!\n");
    }

    int f1 = open(argv[1], O_RDONLY);
    if ( f1 < 0 ) {
        err(2, "First file cannot be opened!\n");
    }
    int f2 = open(argv[2], O_RDONLY);
    if ( f2 < 0 ) {
        err(3, "Second file cannot be opened!\n");
    }
    int fW1 = open(argv[3], O_WRONLY | O_EXCL | O_CREAT, 0744);
    if ( fW1 < 0) {
        err(11, "Cannot open first file for writing!\n");
    }
    int fW2 = open(argv[4], O_WRONLY | O_EXCL | O_CREAT, 0744);
    if ( fW1 < 0) {
        err(12, "Cannot open second file for writing!\n");
    }

    uint16_t pos;
    int bytes;
    char buf[128];
    while ( ( bytes = read(f2, &pos, sizeof(uint16_t) ) ) == sizeof(uint16_t) ) {
        uint8_t len;
        readBytes(f2, &len, sizeof(len));
        uint8_t ignored;
        readBytes(f2, &ignored, sizeof(ignored));
        if ( lseek(f1, pos, SEEK_SET) < 0 ) {
            err(8, "Couldnt seek in the %s\n", argv[1]);
        }
        uint8_t peek1;
        readPeekBytes(f1, &peek1, sizeof(peek1));
        if (!((uint8_t)'A' <= peek1 && peek1 <= (uint8_t)'Z' )) {
            break;
        }
        bytes = readBytes(f1, buf, len);
        buf[bytes] = '\0';
        writeBytes(fW1, buf, bytes);
        writeBytes(fW2, &pos, sizeof(pos));
        writeBytes(fW2, &len, sizeof(len));
        writeBytes(fW2, &ignored, sizeof(ignored));
    }
    if (bytes < 0) {
        err(4, "Error while reading first var from the 3 elements!\n");
    }
    close(f1);
    close(f2);
    close(fW1);
    close(fW2);
}
