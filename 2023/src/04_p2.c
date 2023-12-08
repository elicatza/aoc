#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>


#define BUFSZ 1 << 8


bool contains(int *hs, int needle, size_t hs_len)
{
    size_t i;
    for (i = 0; i < hs_len; ++i) {
        if (needle == hs[i]) {
            return true;
        }
    }
    return false;
}

int parse_card(char *cstr)
{
    char *ptr, *endptr;
    for (ptr = cstr; *ptr != ':'; ++ptr);
    ++ptr;

    int lhs[BUFSZ] = { 0 }, rhs[BUFSZ] = { 0 };
    size_t lhs_len = 0, rhs_len = 0;

    bool islhs = true;
    while (*ptr != '\0' && *ptr != '\n') {
        if (*ptr == '|') {
            ptr += 2;
            islhs = false;
        }

        unsigned int num = strtoul(ptr, &endptr, 10);
        if (ptr == endptr) break;
        ptr = endptr;

        if (islhs) {
            lhs[lhs_len] = num;
            ++lhs_len;
        } else {
            rhs[rhs_len] = num;
            ++rhs_len;
        }
        ++ptr;
    }

    size_t matches = 0;
    size_t i;
    for (i = 0; i < rhs_len; ++i) {
        if (contains(lhs, rhs[i], lhs_len)) ++matches;
    }

    return matches;
}

int main(void)
{
    char fbuf[BUFSZ];
    FILE *fp = fopen("./input/04.txt", "r");
    if (fp == NULL) return 1;

    size_t i;
    int cards[BUFSZ] = { 0 };
    for (i = 0; i < BUFSZ; ++i) cards[i] = 1;


    size_t sum = 0;
    size_t tmp;
    for (i = 0; fgets(fbuf, BUFSZ, fp) != NULL; ++i) {
        int matches = parse_card(fbuf);
        for (tmp = matches;tmp != 0; --tmp) {
            cards[i + tmp] += cards[i];
        }
        sum += cards[i];
    }

    printf("Sum: %zu\n", sum);

    return 0;
}
