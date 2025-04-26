#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
int main(int argc, char* argv[]) {
    if (argc < 4) {
        errx(1, "3 args required!\n");
    }

    int fdF1 = open(argv[1], O_RDONLY);
    if ( fdF1 < 0 ) {
        err(2, "First file cannot be opened for reading!\n");
    }
    int fdF2 = open(argv[2], O_RDONLY);
    if ( fdF2 < 0 ) {
        err(3, "Second file cannot be opened for reading!\n");
    }
    int fdF3 = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, 0744);
    if ( fdF3 < 0 ) {
        err(12, "Third file cannot be opened for writing!\n");
    }
    uint32_t first, second;
    int bytes;
    while ( ( bytes = read(fdF1, &first, sizeof(first)) ) == sizeof(first) ) {
        if ( ( bytes = read(fdF1, &second, sizeof(second)) ) < 0 ) {
            err(10, "Error while reading from file: %s\n", argv[1]);
        } else if ( bytes != sizeof(second) ) {
            errx(11, "Invalid file format for file: %s\n", argv[1]);
        }

        if ( lseek(fdF2, first * sizeof(uint32_t), SEEK_SET) < 0) {
            err(6, "Couldnt lseek!\n");
        }

        for (uint32_t i = 0; i < second; ++i ) {
            uint32_t curNum;
            if ( ( bytes = read(fdF2, &curNum, sizeof(curNum)) ) < 0 ) {
                err(7, "Error while reading from file: %s\n", argv[2]);
            } else if ( bytes != sizeof(curNum) ) {
                errx(8, "Couldnt read all bytes at position: %ld\n", (first + i) * sizeof(first));
            }
            if ( ( bytes = write(fdF3, &curNum, sizeof(curNum)) ) < (int)sizeof(curNum) ) {
                err(9, "Couldnt write all bytes!");
            }
            printf("%d\n", curNum);
        }
    }
    if ( bytes < 0 ) {
        err(5, "Error while reading from file: %s\n", argv[1]);
    }

    close(fdF1);
    close(fdF2);
    close(fdF3);

}
