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
#define SECTIONS 7

typedef struct {
    u64 dest;
    u64 src;
    u64 range;
} RangeTrans;

typedef struct {
    u64 start;
    u64 range;
} RangeSeed;

typedef struct {
    RangeSeed seeds[BUFSZ];
    size_t seeds_len;
    RangeTrans trans[SECTIONS][BUFSZ];
    size_t trans_len[SECTIONS];
} Almanac;


u64 map(RangeTrans *t, u64 in)
{
    if (in >= t->src && in < t->src + t->range) {
        return in - t->src + t->dest;
    }
    return in;
}

u64 walk(Almanac *alm, u64 seed)
{
    size_t i, sect;
    for (sect = 0; sect < SECTIONS; ++sect) {
        for (i = 0; i < SECTIONS; ++i) {
            u64 num = map(&alm->trans[sect][i], seed);
            if (seed != num) {
                seed = num;
                break;
            }
        }
    }

    return seed;
}

size_t parse_u64s(char *line, u64 *out, size_t sz)
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

size_t print_u64s(u64 *nums, size_t len)
{
    size_t i;
    putchar('[');
    for (i = 0; i < len; ++i) {
        printf(" %2ld,", nums[i]);
    }
    printf("\b ]\n");
    return i;
}

u64 parse_seed_nth(char *line, size_t nth)
{
    u64 nums[BUFSZ];
    size_t nums_len = parse_u64s(line, nums, BUFSZ);
    size_t sum = 0;
    size_t i;
    for (i = 0; i < (nums_len / 2); ++i) {
        if (nth >= sum && nth < sum + nums[1 + i * 2]) {
            return nums[i * 2] + nth - sum;
        }
        sum += nums[1 + i * 2];
    }
    return -1;
}

void parse_almanac(FILE *fp, Almanac *alm)
{
    char fbuf[BUFSZ];
    u64 numbuf[BUFSZ];
    size_t i;

    assert(fgets(fbuf, BUFSZ, fp) != NULL);
    size_t len = parse_u64s(fbuf + 7, numbuf, BUFSZ);
    for (i = 0; i + 1 < len; i += 2) {
        alm->seeds[i / 2].start = numbuf[i];
        alm->seeds[i / 2].range = numbuf[i + 1];
    }
    alm->seeds_len = i / 2;


    assert(fgets(fbuf, BUFSZ, fp) != NULL);
    assert(fgets(fbuf, BUFSZ, fp) != NULL);

    size_t section = 0;
    for (i = 0; fgets(fbuf, BUFSZ, fp) != NULL;) {
        if (parse_u64s(fbuf, numbuf, 3) == 0) {
            assert(fgets(fbuf, BUFSZ, fp) != NULL);
            alm->trans_len[section] = i;
            ++section;
            i = 0;
            continue;
        }
        alm->trans[section][i].dest = numbuf[0];
        alm->trans[section][i].src = numbuf[1];
        alm->trans[section][i].range = numbuf[2];
        ++i;
    }
    alm->trans_len[section] = i;
}


int main(void)
{
    FILE *fp = fopen("./input/05.txt", "r");
    assert(fp != NULL);

    Almanac alm;
    parse_almanac(fp, &alm);


    size_t i;
    for (i = 0; i < alm.seeds_len; ++i) {
        printf("Seed: %ld, %ld\n", alm.seeds[i].start, alm.seeds[i].range);
    }

    size_t seedid;
    u64 loc, min;
    min = walk(&alm, alm.seeds[0].start);
    for (seedid = 0; seedid < alm.seeds_len; ++seedid) {
        printf("Starting seedid %lu\n", seedid);
        for (i = 0; i < alm.seeds[seedid].range; ++i) {
            loc = walk(&alm, i + alm.seeds[seedid].start);
            if (min > loc) {
                printf("New min: %lu->%lu\n", i + alm.seeds[seedid].start, loc);
                min = loc;
            }
        }
        printf("Done with seedid %lu\n", seedid);
    }
    printf("Min: %lu\n", min);

    fclose(fp);
    return 0;
}
