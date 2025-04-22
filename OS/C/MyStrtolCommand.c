#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char** argv) {
    if ( argc < 2 ) {
        errx(1, "Missing argument!");
    }

    char* msg;

    char num[128] = "";
    for (int i = 1; i < argc; ++i) {
        strcat(num, argv[i]);
        strcat(num, " ");
    }
    num[strlen(num) - 1] = '\0';

    long res = strtol(num, &msg, 10);
    if (msg == num) {
        char ex[128];
        snprintf(ex, sizeof(ex), "Not a number: %s\n", num);
        errx(2, ex);
    }
    char buf[128];
    snprintf(buf, sizeof(buf), "%ld\n", res);
    write(1, buf, strlen(buf));

    if (*msg != '\0') {
        snprintf(buf, sizeof(buf), "Other args after this: %s\n", msg);
        write(1, buf, strlen(buf));
    }
}
