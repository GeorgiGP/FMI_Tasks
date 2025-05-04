#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>

void getFromTo(long* from, long* to, const char* str) {
    char* ignore, * ignore2;
    *from = strtol(str, &ignore, 0);
    if ( strcmp(ignore, str) == 0) {
        errx(8, "Invalid field1\n");
    }
    if ( strcmp(ignore, "") == 0) {
        *to = *from;
    } else {
        *to = strtol(ignore + 1, &ignore2, 0);
        if ( strcmp(ignore + 1, ignore2) == 0) {
            errx(7, "Invalid field2!\n");
        }
    } 
}

void writeFD(int fd, const char* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(5, "Error while writing content!\n");
    }
    if (res != size) {
        errx(6, "Couldnt write all bytes!\n");
    }
}

int main(int argc, char* argv[]) {
    if ( argc < 3 ) {
        errx(1, "Option or value missing!\n");
    }
    long from, to;
    char buf[4096];
    int bytes;
    const char newL = '\n';
    if ( strcmp(argv[1], "-c") == 0) {
        getFromTo(&from, &to, argv[2]);
        int posLine = 1;
        const long sizeBuf = sizeof(buf);
        while ((bytes = read(0, buf, sizeBuf)) > 0) {
            for(int i = 0; i < sizeBuf; ++i) {
                if(buf[i] == '\n') {
                    posLine = 1;
                    writeFD(1, buf + i, sizeof(char));
                } else {
                    if( from <= posLine && posLine <= to) {
                        writeFD(1, buf + i, sizeof(char));
                    }
                    ++posLine;
                }
            }
        }
        if (bytes < 0) {
            err(4, "Error while reading from stdin!\n");
        }
        writeFD(1, &newL, sizeof(newL));
    } else if ( strcmp(argv[1], "-d") == 0) {
        if (argc < 5 || strcmp(argv[3], "-f") != 0) {
            errx(3, "List field option required!\n");
        }
        char separator = argv[2][0];
        getFromTo(&from, &to, argv[4]);
        int fieldLine = 1;
        const long sizeBuf = sizeof(buf);
        while ((bytes = read(0, buf, sizeBuf)) > 0) {
            for(int i = 0; i < sizeBuf; ++i) {
                if(buf[i] == '\n') {
                    fieldLine = 1;
                    writeFD(1, buf + i, sizeof(char));
                } else {
                    if (buf[i] == separator) {
                        ++fieldLine;
                        if ( from < fieldLine && fieldLine <= to) {
                            writeFD(1, buf + i, sizeof(char));
                        }
                    } else if (from <= fieldLine && fieldLine <= to) {
                        writeFD(1, buf + i, sizeof(char));
                    }
                }
            }
        }
        if (bytes < 0) {
            err(4, "Error while reading from stdin!\n");
        }
        writeFD(1, &newL, sizeof(newL));
    } else {
        errx(2, "Invalid option!\n");
    }
}
