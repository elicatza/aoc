#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./core.h"

#define BUFSZ 1 << 8

#ifndef TEST

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

size_t parse_i64s(char *line, i64 *out, size_t sz)
{
    size_t i;
    char *tmp = line;
    char *last = tmp;
    for (i = 0; i < sz; ++i) {
        out[i] = strtoll(tmp, &last, 10);
        if (tmp == last) break;
        tmp = last;
    }
    return i;
}

size_t first_column(i64 *in, size_t inlen, i64 *out, size_t outsize)
{
    assert(outsize + 1 >= inlen);

    i64 tmp[inlen];
    memcpy(tmp, in, inlen * sizeof(i64));

    size_t i, row;


    out[0] = tmp[0];
    for (row = 1; row < inlen; ++ row) {
        for (i = 0; i < inlen - 1 - row; ++i) {
            tmp[i] = tmp[i + 1] - tmp[i];
        }
        out[row] = tmp[0];
    }
    return row;
}

i64 prev_num(i64 *first_col, size_t first_col_len)
{
    i64 rv = 0;
    size_t i; 
    for (i = 0; i < first_col_len; ++i) {
        if (i % 2 == 1) {
            rv -= first_col[i];
        } else {
            rv += first_col[i];
        }
    }
    return rv;
}

int main(void)
{
    // char fbuf[BUFSZ];
    FILE *fp = fopen("./input/09.txt", "r");
    assert(fp != NULL);

    size_t sum = 0;
    i64 nums[BUFSZ];
    char fbuf[BUFSZ];
    while (fgets(fbuf, BUFSZ, fp) != NULL) {
        size_t nums_len = parse_i64s(fbuf, nums, BUFSZ);
        nums_len = first_column(nums, nums_len, nums, nums_len);
        sum += prev_num(nums, nums_len);
    }

    printf("Sum: %zu\n", sum);

    return 0;
}

#else // TEST


int main(void)
{
    test_result_print();
}
#endif // TEST
