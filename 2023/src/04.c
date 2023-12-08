#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>

#define NOB_IMPLEMENTATION
#include "./nob.h"

#include "core.h"


#define BUFSZ 0xff

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
    char *ptr;
    for (ptr = cstr; *ptr != ':'; ++ptr);
    ++ptr;

    char *endptr;

    int lhs[BUFSZ] = { 0 };
    size_t lhs_len = 0;
    int rhs[BUFSZ] = { 0 };
    size_t rhs_len = 0;

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

    return (matches > 0) ? 1 << (matches - 1) : 0;
}


#ifndef TEST


int main(void)
{
    char fbuf[BUFSZ];
    FILE *fp = fopen("./input/04.txt", "r");
    assert(fp != NULL);

    size_t sum = 0;
    while (fgets(fbuf, BUFSZ, fp) != NULL) {
        sum += parse_card(fbuf);
    }

    printf("Sum: %zu\n", sum);

    return 0;
}

#else  // TEST

void test_cards()
{
	ASSERT(parse_card("Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53\n") == 8);
	ASSERT(parse_card("Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19\n") == 2);
	ASSERT(parse_card("Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1\n") == 2);
	ASSERT(parse_card("Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83\n") == 1);
	ASSERT(parse_card("Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36\n") == 0);
	ASSERT(parse_card("Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11\n") == 0);
}

int main(void)
{

    test_cards();
    printf("[%d/%d] Tests passed!\n", passed, tests);
    return 0;
}
#endif
