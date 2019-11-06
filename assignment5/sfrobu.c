#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

int frobcmp(char const *a, char const *b, int ignoreCase)
{
    for (; *a != ' ' && *b != ' '; a++, b++)
    {
        // skip null bytes
        while (*a == '\0')
            a++;
        while (*b == '\0')
            b++;
        if ((*a ^ 42) != (*b ^ 42))
        {
            if (ignoreCase == 0)
                return (*a ^ 42) - (*b ^ 42);
            else
                return toupper((*a ^ 42)) - toupper((*b ^ 42));
        }
    }
    // one or both of them have reached the end
    if (*a == ' ' && *b == ' ')
        return 0;
    if (*a == ' ')
        return -1;
    if (*b == ' ')
        return 1;
}

int frobcmp2(void const *a, void const *b)
{
    return frobcmp(*(char const **)a, *(char const **)b, 0);
}

int frobcmp3(void const *a, void const *b)
{
    return frobcmp(*(char const **)a, *(char const **)b, 1);
}

void freeAll(char *word, char **words, int size)
{
    size_t i;
    for (i = 0; i < size; i++)
        free(words[i]);
    free(words);
    free(word);
}

int exitInputError(char *word, char **words, int size)
{
    freeAll(word, words, size);
    fprintf(stderr, "Unable to read input");
    exit(1);
}

int exitMemoryError(char *word, char **words, int size)
{
    freeAll(word, words, size);
    fprintf(stderr, "Unable to allocate memory");
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "Invalid number of arguments");
        exit(1);
    }

    size_t count = 0, size = 0, i, j;
    char curr, next;
    ssize_t currState, nextState;
    char *text = NULL;
    char *word = (char *)malloc(sizeof(char));
    char **words = (char **)malloc(sizeof(char *));
    struct stat file;
    if (word == NULL || words == NULL)
        exitMemoryError(word, words, size);
    if (fstat(0, &file) < 0)
        exitInputError(word, words, size);

    currState = read(0, &curr, 1);
    nextState = read(0, &next, 1);
    if (currState < 0 || nextState < 0)
        exitMemoryError(word, words, size);

    while (currState > 0)
    {
        // has new char, realloc word
        word[count] = curr;
        char *tempWord = (char *)realloc(word, (count + 2) * sizeof(char));
        if (tempWord != NULL)
        {
            word = tempWord;
            count++;
        }
        else
            exitMemoryError(word, words, size);
        // word ended, add to words
        if (curr == ' ')
        {
            words[size] = word;
            char **tempWords = (char **)realloc(words, (size + 2) * sizeof(char *));
            if (tempWords != NULL)
            {
                words = tempWords;
                size++;
                // successfully added word, reset
                word = NULL;
                word = (char *)malloc(sizeof(char));
                count = 0;
            }
            else
                exitMemoryError(word, words, size);
            // next char is EOF, end loop
            if (nextState <= 0)
                break;
            // multiple spaces, skip them
            if (next == ' ')
            {
                while (currState > 0 && curr == ' ')
                    currState = read(0, &curr, 1);
                nextState = read(0, &next, 1);
                if (currState < 0 || nextState < 0)
                    exitInputError(word, words, size);
                continue;
            }
        }
        // ended without space
        else if (nextState <= 0)
        {
            curr = ' ';
            continue;
        }
        // continue adding char
        curr = next;
        currState = nextState;
        nextState = read(0, &next, 1);
        if (currState < 0 || nextState < 0)
            exitInputError(word, words, size);
    }

    // sort words based on "-f" flag
    if (argc == 2 && !strcmp(argv[1], "-f"))
        qsort(words, size, sizeof(char *), frobcmp3);
    else
        qsort(words, size, sizeof(char *), frobcmp2);

    // print words
    for (i = 0; i < size; i++)
    {
        for (j = 0; words[i][j] != ' '; j++)
            write(1, &words[i][j], 1);
        write(1, " ", 1);
    }

    // free words and its elements
    freeAll(word, words, size);
    exit(0);
}