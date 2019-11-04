#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    if (argc == 3) {
        size_t i, j;
        char c;
        const char *from = argv[1];
        const char *to = argv[2];
        size_t fromLen = strlen(from);
        size_t toLen = strlen(to);

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
        for (read(stdin, c, 1); c != EOF; read(stdin, c, 1)) {
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