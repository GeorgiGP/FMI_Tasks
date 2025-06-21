#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
int openRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if ( fd < 0 ) {
        err(2, "Couldnt open %s for reading!\n", name);
    }
    return fd;
}

void closeAllPipes(int count, int pipes[][2]) {
    for(int i = 0; i < count; ++i) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

void readfd(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(9, "Error reading bytes!\n");
    }
    if ( res != size) {
        errx(10, "Couldnt read all bytes!\n");
    }
}

void writefd(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(9, "Error writing bytes!\n");
    }
    if ( res != size) {
        errx(10, "Couldnt write all bytes!\n");
    }
}
void lseekfd(int fd, int offset, int mode) {
    int res = lseek(fd, offset, mode);
    if ( res < 0) {
        err(11, "Couldnt lseek!\n");
    }
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "Should take 2 args!\n");
    }

    struct stat s;
    if ( stat(argv[1], &s) < 0) {
        err(3, "Couldnt stat!\n");
    }


    int elSize = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 2;
    if ( s.st_size % elSize != 0) {
        errx(4, "Illegal file logic!\n");
    }
    int fdR = openRead(argv[1]);

    int countFiles = s.st_size / elSize;
    if ( countFiles > 8 ) {
        errx(7, "The files should be 8 max!\n");
    }

    int pfd[8][2];

    for(int i = 0; i < countFiles; ++i) {
        if (pipe(pfd[i]) < 0) {
            err(5, "Couldnt pipe!\n");
        }
    }

    int pids[8];
    for (int i =0; i < countFiles; ++i) {
        char fileName[8];
        readfd(fdR, fileName, sizeof(fileName));
        if (fileName[7] != '\0') {
            errx(8, "The filename length should be max 7 bytes + terminating 0!\n");
        }
        uint32_t offset;
        readfd(fdR, &offset, sizeof(offset));
        uint32_t count;
        readfd(fdR, &count, sizeof(count));

        pids[i] = fork();
        if ( pids[i] < 0) {
            err(6, "Couldnt fork!\n");
        }
        if ( pids[i] == 0) {
            int fdWrite = dup(pfd[i][1]);
            closeAllPipes(countFiles, pfd);

            uint16_t curResult = 0;
            int fd = openRead(fileName);
            lseekfd(fd, offset * 2, SEEK_SET);

            for(long j = 0; j < count; ++j) {
                uint16_t cur;
                readfd(fd, &cur, sizeof(cur));
                curResult ^= cur;
            }
            writefd(fdWrite, &curResult, sizeof(curResult));
            close(fdR);
            close(fdWrite);
            close(fd);
            exit(0);
        }
    }

    for(int i = 0 ; i< countFiles; ++i) {
        close(pfd[i][1]);
    }

    for(int i = 0 ; i < countFiles;++i) {
        int status;
        waitpid(pids[i], &status, 0);
        if (!WIFEXITED(status)) {
            errx(12, "The child didnt exit normally!\n");
        } else if (WEXITSTATUS(status) != 0) {
            errx(13, "The child exited with status: %i!\n", WEXITSTATUS(status));
        }
    }

    uint16_t res = 0;
    for (int i = 0; i< countFiles; ++i) {
        uint16_t cur;
        readfd(pfd[i][0], &cur, sizeof(cur));
        res ^= cur;
    }

    for(int i =0; i < countFiles; ++i) {
        close(pfd[i][0]);
    }
    char msg[50];
    snprintf(msg, sizeof(msg), "%x\n", res);
    writefd(1, msg, strlen(msg));
    close(fdR);
}
