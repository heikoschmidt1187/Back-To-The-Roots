#include <stdio.h>

#include "hash/md4/md4.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("BACK TO THE ROOTS!\n");
    printf("******************\n\n");

    printf("MD4:\n");
    printf("----\n\n");

    Md4("The quick brown fox jumps over the lazy dog");
    Md4("The quick brown fox jumps over the lazy cog");
    Md4("");
    Md4("a");
    Md4("abc");
    Md4("message digest");
    Md4("abcdefghijklmnopqrstuvwxyz");
    Md4("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    Md4("12345678901234567890123456789012345678901234567890123456789012345678901234567890");

    return 0;
}