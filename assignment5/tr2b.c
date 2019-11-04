#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    if (argc == 3) {
        long i, j;
        char c;
        const char *from = argv[1];
        const char *to = argv[2];
        long fromLen = strlen(from);
        long toLen = strlen(to);

        // check length
        if (fromLen != toLen) {
            fprintf(stderr, "Arguments have different lengths");
            exit(1);
        }

        // check for duplicates
        for (i = 0; i < fromLen-1; i++) {
            for (j = i+1; j < fromLen; j++) {
                if (from[i] == from[j]) {
                    fprintf(stderr, "Duplicate found in first argument");
                    exit(1);
                }
            }
        }

        // read stdin and write to stdout, replace characters if found in from
        for (c = getchar(); c != EOF; c = getchar()) {
            for (i = 0; i < fromLen; i++) {
                if (from[i] == c) {
                    c = to[i];
                    break;
                }
            }
            putchar(c);
        }
        putchar('\n');
        return 0;
    }
    printf("Input error");
    return 1;
}