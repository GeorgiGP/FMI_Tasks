#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
int compare(const void* lhs,const void* rhs) {
    return *(const uint8_t*)lhs - *(const uint8_t*)rhs;
}

int main(int argc, char* argv[]) {
    if ( argc < 2 ) {
        errx(1, "Must have 1 arg\n");
    }
    char* fName = argv[1];
    int fd = open(fName, O_RDONLY);
    if (fd < 0) {
        err(2, "File %s does not exist!\n", fName);
    }
    struct stat info;
    if( ( stat(fName, &info) ) < 0) {
        err(3, "Couldnt stat file %s\n", fName);
    }
    long size = info.st_size;
    if( size == 0 ) {
        const char msg[] = "Size is 0\n";
        write(1, msg, strlen(msg));
        exit(0);
    }
    uint8_t* fileContent = malloc( size );
    if (fileContent == NULL) {
        err(7, "Couldn't malloc %ld memory!\n", size);
    }
    long readBytes = read(fd, fileContent, size);
    close(fd);
    if (readBytes != size) {
        err(4, "Couldnt read the whole file!");
    }

    qsort(fileContent, size, sizeof(uint8_t), compare);

    if ( ( fd = open(fName, O_CREAT | O_WRONLY | O_TRUNC) ) < 0) {
        err(6, "Couldnt open %s!", fName);
    }

    long written = write(fd, fileContent, size);
    if (written != size) {
        err(5, "Couldnt write all bytes in the file!");
    }
    close(fd);
    free(fileContent);
}
