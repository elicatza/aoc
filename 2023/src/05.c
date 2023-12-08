#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "./core.h"

#define BUFSZ 1 << 8


i64 map(i64 dest, i64 src, i64 range, i64 in)
{
    if (in >= src && in < src + range) {
        return in - src + dest;
    }
    return in;
}

// Returns amount of seeds (out length)
size_t parse_i64s(char *line, i64 *out, size_t sz)
{
    size_t i;
    char *tmp = line;
    char *last = tmp;
    for (i = 0; i < sz; ++i) {
        out[i] = strtoul(tmp, &last, 10);
        if (tmp == last) break;
        tmp = last;
    }
    return i;
}

size_t print_i64s(i64 *nums, size_t len)
{
    size_t i;
    putchar('[');
    for (i = 0; i < len; ++i) {
        printf(" %2ld,", nums[i]);
    }
    printf("\b ]\n");
    return i;
}

i64 min_i64s(i64 *vals, size_t len)
{
    size_t i;
    assert(len > 0);
    i64 min = vals[0];
    for (i = 1; i < len; ++i) {
        if (vals[i] < min) {
            min = vals[i];
        }
    }
    return min;
}

void unsign_i64s(i64 *vals, size_t len)
{
    size_t i = 0;
    for (i = 0; i < len; ++i) {
        if (vals[i] < 0) {
            vals[i] = -vals[i];
        }
    }
}


#ifndef TEST


int main(void)
{
    char fbuf[BUFSZ];
    FILE *fp = fopen("./input/05.txt", "r");
    assert(fp != NULL);

    assert(fgets(fbuf, BUFSZ, fp) != NULL);

    i64 seeds[BUFSZ];
    size_t seeds_len = parse_i64s(fbuf + 7, seeds, BUFSZ);
    print_i64s(seeds, seeds_len);

    while (fgets(fbuf, BUFSZ, fp) != NULL) {

        i64 map_vals[3];
        if (parse_i64s(fbuf, map_vals, 3) == 0) {
            unsign_i64s(seeds, seeds_len);
            continue;
        }

        size_t i;
        for (i = 0; i < seeds_len; ++i) {
            if (seeds[i] < 0) continue;
            i64 val = map(map_vals[0], map_vals[1], map_vals[2], seeds[i]);
            if (val != seeds[i]) {
                seeds[i] = -val;
            }
        }
    }

    unsign_i64s(seeds, seeds_len);
    print_i64s(seeds, seeds_len);
    printf("Min: %ld\n", min_i64s(seeds, seeds_len));

    return 0;
}

#else  // TEST


void test_maps()
{
    ASSERT(map(39, 0, 15, 14) == 53);
    ASSERT(map(50, 98, 2, 97) == 97);
	ASSERT(map(50, 98, 2, 98) == 50);
	ASSERT(map(50, 98, 2, 99) == 51);
	ASSERT(map(50, 98, 2, 100) == 100);
}

int main(void)
{

    test_maps();
    printf("[%d/%d] Tests passed!\n", passed, tests);
    return 0;
}
#endif
