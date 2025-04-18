#include <string.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int num_tasks = 10;
    int child_pid = fork();
    if (child_pid < 0) {
        err(1, "could not fork");
    }

    if (child_pid == 0) {
        write(1, "Here is some child work!");
        exit(0); // The child does its work and exits
    }

    int status;
    child_pid = wait(&status);
    if (child_pid < 0) {
        err(1, "could not wait for child");
    }
    if (!WIFEXITED(status))
    {
        errx(1, "task failed: child was killed!");
    } else if (WEXITSTATUS(status) != 0) {
        errx(1, "task failed (exit status != 0)!");
    }

    char msg[128] = "task completed successfully with pid: %d\n";
    snprintf(msg, sizeof(msg), msg, child_pid);
    write(1, msg, strlen(msg));

    for(int i = 0; i < num_tasks; i++) {
        int child_pid = fork();
        if (child_pid < 0) {
            err(1, "could not fork");
        }
        if (child_pid == 0) {
            snprintf(msg, sizeof(msg),
                "I am child, Doing stuff - my pid: %d, with parent: %d\n",
                    getpid(),
                    getppid());
            write(1, msg, strlen(msg));
            exit(0); // The child does its work and exits
        }
    }
    for (int i = 0; i < num_tasks; i++) {
        int status;
        int child_pid = wait(&status);
        snprintf(msg, sizeof(msg),
            "Completed successfully - child pid: %d, original: %d, with his parent: %d\n",
            child_pid, getpid(), getppid());
        write(1, msg, strlen(msg));
        if (child_pid < 0) {
            err(1, "could not wait for child");
        }
        if (!WIFEXITED(status)) {
            warnx(1, "a task failed: child was killed!");
        } else if (WEXITSTATUS(status) != 0) {
            warnx(1, "a task failed (exit status != 0)!");
        }
    }
    snprintf(msg, sizeof(msg),"all tasks completed successfully - %d %d\n", getpid(), getppid());
    write(1, msg, strlen(msg));
}
