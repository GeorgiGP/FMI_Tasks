#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
int openRead(const char* name) {
    int fd = open(name, O_RDONLY);
    if (fd < 0) {
        err(3, "Cannot open file for read!\n");
    }
    return fd;
}

void readBuf(int fd, void* content, long size){
    int res = read(fd, content, size);
    if ( res < 0) {
        err(5, "Error, Couldnt read bytes!\n");
    }
    if ( res != size ) {
        errx(6, "Couldnt read all bytes!\n");
    }
}

void writeBuf(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(8, "Error, Couldnt write bytes!\n");
    }
    if ( res != size ) {
        errx(9, "Couldnt write all bytes!\n");
    }
}

char replace(char ch, const char* chToReplace, const char* chReplacing) {
    for ( size_t i = 0; i < strlen(chToReplace); ++i ) {
        if ( chToReplace[i] == ch) {
            return chReplacing[i];
        }
    }
    return ch;
}

bool deleting(char ch, const char* chToDelete) {
    for ( size_t i = 0; i < strlen(chToDelete); ++i ) {
        if ( chToDelete[i] == ch) {
            return true;
        }
    }
    return false;
}

bool trim(char ch, char* lastCh, bool* started, const char* trimChars) {
    if (!*started) {
        *lastCh = ch;
        *started = true;
        return false;
   }
    for ( size_t i = 0; i < strlen(trimChars); ++i) {
        if ( trimChars[i] == ch ) {
            if (ch == *lastCh) {
                return true;
            }
            *lastCh = ch;
            return false;
        }
    }
    *lastCh = ch;
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        errx(1, "Invalid args count!");
    }
    char buf[4096];
    int bytes;
    if (strcmp(argv[1], "-d") == 0) {
        while ((bytes = read(0, buf, sizeof(buf))) > 0) {
            for( int i = 0; i < bytes; ++i) {
                if ( !deleting(buf[i], argv[2]) ) {
                    writeBuf(1, buf + i, sizeof(char));
                }
            }
        }
    } else if ( strcmp(argv[1], "-s") == 0) {
        char last;
        bool started = false;
        while ((bytes = read(0, buf, sizeof(buf))) > 0) {
            for( int i = 0; i < bytes; ++i) {
                if ( !trim(buf[i], &last, &started, argv[2]) ) {
                    writeBuf(1, &last, sizeof(last));
                }
            }
        }
    } else {
        if ( strlen(argv[1]) != strlen(argv[2]) ) {
            errx(7, "Char count in both strings should match!\n");
        }
        while ((bytes = read(0, buf, sizeof(buf))) > 0) {
            for( int i = 0; i < bytes; ++i) {
                char ch = replace(buf[i], argv[1], argv[2]);
                writeBuf(1, &ch, sizeof(ch));
            }
        }
    }
    if (bytes < 0) {
        err(4, "Couldnt read bytes in bufer!\n");
    }
}
