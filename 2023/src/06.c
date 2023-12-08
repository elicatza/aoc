#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#include "./core.h"

#define BUFSZ 1 << 6

size_t parse_u32s(char *line, u32 *out, size_t sz)
{
    size_t i;
    char *tmp = line;
    char *last = tmp;
    for (i = 0; i < sz; ++i) {
        out[i] = strtoull(tmp, &last, 10);
        if (tmp == last) break;
        tmp = last;
    }
    return i;
}

size_t print_u32s(u32 *nums, size_t len)
{
    size_t i;
    putchar('[');
    for (i = 0; i < len; ++i) {
        printf(" %2d,", nums[i]);
    }
    printf("\b ]\n");
    return i;
}

float min_val(f32 time, f32 dist)
{
    float a = -1;
    float b = time;
    float c = -dist;
    return 0.00001 + (-b + sqrtf(b*b - (4 * a * c))) / 2 * a;
}

float max_val(f32 time, f32 dist)
{
    float a = -1;
    float b = time;
    float c = -dist;
    return -0.00001 + (-b - sqrtf(b*b - (4 * a * c))) / 2 * a;
}



int main(void)
{
    FILE *fp = fopen("./input/06.txt", "r");
    assert(fp != NULL);
    char fbuf[BUFSZ];

    assert(fgets(fbuf, BUFSZ, fp) != NULL);
    u32 times[BUFSZ];
    size_t times_len = parse_u32s(fbuf + 10, times, BUFSZ);

    assert(fgets(fbuf, BUFSZ, fp) != NULL);
    u32 dist[BUFSZ];
    size_t dist_len = parse_u32s(fbuf + 10, dist, BUFSZ);
    printf("len: %zu\n", dist_len);

    assert(dist_len == times_len);

    size_t i;
    u32 product = 1;
    for (i = 0; i < dist_len; ++i) {
        putchar('\n');
        f32 max = max_val((f32) times[i], (f32) dist[i]);
        f32 min = min_val((f32) times[i], (f32) dist[i]);
        printf("max: %.2f\tmin = %.2f\n", max, min);
        printf("max: %.2f\tmin = %.2f\n", floorf(max), ceilf(min));
        product *= floorf(max) - ceilf(min) + 1;
    }
    printf("Product: %d\n", product);


    fclose(fp);
    return 0;
}
