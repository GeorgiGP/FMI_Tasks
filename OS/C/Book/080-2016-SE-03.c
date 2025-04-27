#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
int compare(const void* lhs, const void* rhs) {
    return *(const uint32_t*)lhs - *(const uint32_t*)rhs;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        errx(1, "Too few args, Must have 1 valid path to file\n");
    }
    char* fileName = argv[1];
    int fd = open(fileName, O_RDWR);
    if (fd < 0) {
        err(2, "Invalid file!\n");
    }
    struct stat info;
    if ( stat(fileName, &info) < 0 ) {
        err(3, "Couldnt stat!\n");
    }
    uint64_t size = info.st_size;
    if ( ( size > 100000000 * sizeof(uint32_t)) || (size == 0)) {
        errx(4, "Invalid file format: %s\n", fileName);
    }
    int half = size / 2;
    if( half % sizeof(uint32_t) != 0) {
        half += 2;
    }
    int otherHalf = size - half;
    otherHalf -= (otherHalf % sizeof(uint32_t));

    uint32_t* buf = malloc(half);
    if ( buf == NULL ) {
        err(6, "Error allocating memory!\n");
    }
    if ( read(fd, buf, half) < half ) {
        err(5, "Error while reading bytes from %s\n", fileName);
    }

    int fdWrite = open("helpFile", O_CREAT | O_EXCL | O_RDWR, 0744);
    if (fdWrite < 0) {
        err(7, "HelpFile cant be opened, because already exists one with the same name!\n");
    }
    qsort(buf, half / sizeof(uint32_t), sizeof(uint32_t), compare);

    if ( write(fdWrite, buf, half) != half ) {
        err(8, "Couldnt write all bytes in the file!\n");
    }

    if (lseek(fdWrite, 0, SEEK_SET) < 0) {
        err(9, "Couldnt seek in the help file!\n");
    }

    if ( read(fd, buf, otherHalf) != otherHalf) {
        err(10, "Couldnt read all bytes for other half of the file!\n");
    }

    if (lseek(fd, 0, SEEK_SET) < 0) {
        err(11, "Couldnt seek int the content file: %s\n", fileName);
    }
    qsort(buf, otherHalf / sizeof(uint32_t), sizeof(uint32_t), compare);

    int cnt = 0;
    int sizeBuf = otherHalf / sizeof(uint32_t);
    uint32_t curNum;
    int bytes;
    int pos = 0;
    while (cnt < sizeBuf && (bytes = pread(fdWrite, &curNum, sizeof(curNum), pos)) == sizeof(curNum) ) {
        if (buf[cnt] < curNum) {
            curNum = buf[cnt++];
        } else {
            if (lseek(fdWrite, sizeof(curNum), SEEK_CUR) < 0) {
                err(12, "Couldnt seek by %ld bytes\n", sizeof(curNum));
            }
            pos += sizeof(curNum);
        }
        if (write(fd, &curNum, sizeof(curNum) ) != sizeof(curNum) ) {
            err(13, "Couldnt write the number in file\n");
        }
    }
    if ( bytes < 0) {
        err(15, "Error while reading bytes from help file!\n");
    }
    while(cnt < sizeBuf) {
        if (write(fd, buf + cnt, sizeof(buf[cnt]) ) != sizeof(buf[cnt]) ) {
            err(14, "Couldnt write the number in file from remaining\n");
        }
        ++cnt;
    }
    free(buf);
    while(true) {
        if( ( bytes = read(fdWrite, &curNum, sizeof(curNum) ) ) < 0) {
            err(17, "Error while reading remaining bytes from help file!\n");
        } else if (bytes != sizeof(curNum)) {
            break;
        }
        if ( write(fd, &curNum, sizeof(curNum) ) != sizeof(curNum)) {
            err(18, "Error while writing remaining bytes in result file!\n");
        }
    }
    close(fd);
    close(fdWrite);
    unlink("helpFile");
}
