#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/wait.h>
#include <time.h>
#include <err.h>
#include <sys/time.h>
#include <string.h>
int openWrite(const char* fileName) {
    int fd = open(fileName, O_WRONLY | O_CREAT | O_APPEND, 0600);
    if ( fd < 0) {
        err(1, "Couldnt create %s !\n", fileName);
    }
    return fd;
}

void writefd(int fd, const void* content, long size) {
    int res = write(fd, content, size);
    if ( res < 0) {
        err(3, "Couldnt write, error!\n");
    }
    if ( res != size) {
        errx(4, "Couldnt write all bytes!\n");
    }
}

void waitChildPid(int pid) {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
        errx(7, "The child didnt exit in normal way!\n");
    }
    if (WEXITSTATUS(status) != 0) {
        errx(8, "The child exited with status: %i.\n", WEXITSTATUS(status));
    }
}
//chmod u+s but i should be root to be able to lock and kill his processes, due to u+s and Logfile 0600 no body will be able to read it, delete it etc,
//only the process who is using the program will have access to it and of course the root
//also we should export PATH="pathtoSudo":$PATH
//there will allays be way to access the real sudo on the filesystem, we should also put it in a dir without permitions for reading and execution or the file itself should be for group and others.
int main(int argc, char* argv[]) {
    char rowLog[2048];
    int offset = 0;

    struct timeval t;
    int r =gettimeofday(&t, NULL);
    if (r < 0) {
        err(2, "Couldnt get time of day!\n");
    }

    time_t ti = time(NULL);
    struct tm* time = localtime(&ti);

    offset += snprintf(rowLog + offset, sizeof(rowLog) - offset, "%i-%i-%i %i:%i:%i.%li ",
        time->tm_year + 1900, time->tm_mon + 1,
        time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec, t.tv_usec);

    struct passwd* user = getpwuid(getuid());
    offset += snprintf(rowLog + offset, sizeof(rowLog) - offset, "%s ", user->pw_name);

    for(int i = 1 ; i < argc - 1; ++i) {
        offset += snprintf(rowLog + offset, sizeof(rowLog) - offset, "%s ", argv[i]);
    }

    offset += snprintf(rowLog + offset, sizeof(rowLog) - offset, "%s\n", argv[argc - 1]);

    int log = openWrite("/home/students/s0600299/tasks/04-c/2021-SE-01/LogSudo.log");
    writefd(log, rowLog, strlen(rowLog));

    int pid = fork();
    if ( pid < 0) {
        err(5, "Couldnt fork!\n");
    }
    if ( pid == 0) {
        execlp("passwd", "passwd", "-l", user->pw_name, (char*) NULL);
        err(6, "Couldnt exec passwd!\n");
    }

    int pidKill = fork();
    if (pidKill < 0) {
        err(7, "Couldnt fork!\n");
    }
    if (pidKill == 0) {
        execlp("killall", "killall", "-u", user->pw_name, (char*) NULL);
        err(8, "Couldnt kill all processes for user: %s\n", user->pw_name);
    }

    waitChildPid(pid);
    waitChildPid(pidKill);
}
