#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const *a, char const *b)
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
            return (*a ^ 42) - (*b ^ 42); // found difference, return
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
    return frobcmp(*(char const **)a, *(char const **)b);
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

int main()
{
    size_t count = 0, size = 0, i, j;
    int curr, next;
    char *word = (char *)malloc(sizeof(char));
    char **words = (char **)malloc(sizeof(char *));
    if (word == NULL || words == NULL)
        exitMemoryError(word, words, size);

    curr = getchar();
    if (ferror(stdin))
        exitInputError(word, words, size);
    next = getchar();
    if (ferror(stdin))
        exitInputError(word, words, size);

    while (curr != EOF)
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
            char **tempWords = (char **)realloc(words, (size + 2) *
                                                           sizeof(char *));
            if (tempWords != NULL)
            {
                words = tempWords;
                size++;
                // successfully added word, reset
                word = NULL;
                word = (char *)malloc(sizeof(char));
                if (word == NULL)
                    exitMemoryError(word, words, size);
                count = 0;
            }
            else
                exitMemoryError(word, words, size);

            // next char is EOF, end loop
            if (next == EOF)
                break;
            // multiple spaces, skip them
            if (next == ' ')
            {
                while (curr == ' ')
                {
                    curr = getchar();
                    if (ferror(stdin))
                        exitInputError(word, words, size);
                }
                next = getchar();
                if (ferror(stdin))
                    exitInputError(word, words, size);
                continue;
            }
        }
        // ended without space
        else if (next == EOF)
        {
            curr = ' ';
            continue;
        }
        // continue adding char
        curr = next;
        next = getchar();
        if (ferror(stdin))
            exitInputError(word, words, size);
    }

    // sort word
    qsort(words, size, sizeof(char *), frobcmp2);

    // print words
    for (i = 0; i < size; i++)
    {
        for (j = 0; words[i][j] != ' '; j++)
        {
            printf("%c", words[i][j]);
        }
        printf(" ");
    }

    // free words and its elements
    freeAll(word, words, size);
    exit(0);
}