#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int readBytes(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(4, "Error: Cannot read from file!\n");
    }
    return res;
}

void readBytesStrict(int fd, void* content, long size) {
    int res = read(fd, content, size);
    if ( res < 0) {
        err(2, "Error: Cannot read from file!\n");
    }
    if ( res != size) {
        errx(3, "Cannot read all bytes!\n");
    }
}

int openFDRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if ( fd < 0) {
        err(5, "Couldnt open file %s for reading!\n", name);
    }
    return fd;
}

long lseekFD(int fd, long offset, int pos) {
    long res = lseek(fd, offset, pos);
    if ( res < 0 ) {
        err(6, "Couldnt seek in file!\n");
    }
    return res;
}

long fileSize(int fd) {
    long curPos = lseekFD(fd, 0, SEEK_CUR);
    long size = lseekFD(fd, 0, SEEK_END);
    lseekFD(fd, curPos, SEEK_SET);
    return size;
}

typedef struct {
    uint32_t uid;
    uint32_t duration;
} User;

int main(int argc, char* argv[]) {
    if ( argc < 2 ) {
        errx(1, "1 arg required!\n");
    }

    uint32_t uid;
    uint16_t ignore;
    uint32_t start, end;
    uint64_t sum = 0;
    int fd = openFDRead(argv[1]);
    if ( fileSize(fd) == 0 ) {
        errx(11, "File is empty!\n");
    }

    User users[16384];
    uint16_t counter = 0;

    while ( readBytes(fd, &uid, sizeof(uid)) > 0 ) {
        lseekFD(fd, sizeof(ignore) * 2, SEEK_CUR);
        readBytesStrict(fd, &start, sizeof(start));
        readBytesStrict(fd, &end, sizeof(end));

        uint32_t dur = end - start;
        sum += dur;
        User cur = {uid, dur};
        users[counter++] = cur;
    }
    double avg = (double)sum / counter;

    double dispertion = 0;
    for (int i = 0; i < counter; ++i ) {
        dispertion+= (users[i].duration - avg)*(users[i].duration - avg);
    }
    dispertion /= counter;
    printf("AVG: %f , DISPERTION: %f\n", avg, dispertion);
    User* maxDurPerUser[2048];
    uint16_t sizeMax = 0;

    for( int i = 0; i < counter; ++i ) {
        bool found = false;
        printf("UID: %u, Dur: %u\n", users[i].uid, users[i].duration);
        for ( int j = 0; j < sizeMax; ++j) {
            if ( maxDurPerUser[j]->uid == users[i].uid ) {
                if (users[i].duration > maxDurPerUser[j]->duration) {
                    maxDurPerUser[j] = users + i;
                }
                found = true;
                break;
            }
        }
        if (!found) {
            maxDurPerUser[sizeMax++] = users + i;
        }
    }

    char msg[128];
    for (int i = 0; i < sizeMax; ++i ) {
        if ((long)(maxDurPerUser[i]->duration)*(maxDurPerUser[i]->duration) > dispertion ) {
            snprintf(msg, sizeof(msg), "User: %u , With max Duration above Dispertion: %u\n", maxDurPerUser[i]->uid, maxDurPerUser[i]->duration);
            write(1, msg, strlen(msg));
        }
    }
}
