#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main(void) {

    int fd = open("file", O_RDWR | O_TRUNC | O_CREAT, 0755);
    int pid = fork();

    if ( pid == 0 ) {
        char msg[] = "Some text I want\nto read\n";
        write(fd, msg, strlen(msg));
        printf("Written\n");
    } else if (pid > 0) {
        sleep(5);
        char msg[] = "Read!\n";
        printf(msg);
        char buf[128];
        lseek(fd, 0, SEEK_SET);
        int bytes = read(fd, buf, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        printf("NumBytes: %d, Content: %s\n", bytes, buf);
    }

}
