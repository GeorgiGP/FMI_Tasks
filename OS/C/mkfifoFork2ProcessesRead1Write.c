#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

int main(void) {
    int res = mkfifo("file", 0755);
    if (res < 0) {
        err(1, "Couldn't mkfifo \"file\"");
    }

    int fd = open("file", O_RDWR | O_TRUNC, 0755);
    unlink("file");
    if (fd < 0) {
        err(1, "No fd");
    }
    int pid = fork();
    if (pid < 0) {
        err(1, "Couldn't fork!");
    } else if (pid == 0) {
        char buf[128] = "Some text to\n be read!\n";
        while (true) {
            sleep(1);
            write(fd, buf, strlen(buf));
        }
    } else {
        int pid2 = fork();
        if (pid2 == 0) {
            sleep(2);
        }
        char buf[128];
        char buf2[150];
        int s = 0;
        while (true) {
            sleep(4);
            int bytes = read(fd, buf, sizeof(buf) - 1);
            if ( bytes >= 0 ) {
                buf[bytes] = '\0';
            }
            snprintf(buf2, sizeof(buf2), "%s\n", buf);
            write(1, buf2, strlen(buf2));
            printf("%d\n", s++);
        }
    }
}
