#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./core.h"

#define BUFSZ 1 << 12
#define ALPHAB_LEN 26
#define HMSIZE ALPHAB_LEN * ALPHAB_LEN * ALPHAB_LEN + ALPHAB_LEN * ALPHAB_LEN + ALPHAB_LEN

typedef struct {
    char key[3];
    char val[3];
} KV;


typedef char type24[3];
typedef char HashMap[HMSIZE][2][3];


size_t hash(char el[3])
{
    return el[0] - 'A' + (el[1] - 'A') * ALPHAB_LEN + (el[2] - 'A') * ALPHAB_LEN * ALPHAB_LEN;
}

#define hm_put(hm, k, v) \
    do { \
        hm[hash(k)][0][0] = v[0][0]; \
        hm[hash(k)][0][1] = v[0][1]; \
        hm[hash(k)][0][2] = v[0][2]; \
        hm[hash(k)][1][0] = v[1][0]; \
        hm[hash(k)][1][1] = v[1][1]; \
        hm[hash(k)][1][2] = v[1][2]; \
    } while (0)

#define hm_get(hm, k) (hm)[hash(k)]

#define VALFMT "%.3s, %.3s"
#define VALARG(arg) (arg)[0], (arg)[1]


#ifndef TEST

int main(void)
{
    // char fbuf[BUFSZ];
    FILE *fp = fopen("./input/08.txt", "r");
    assert(fp != NULL);

    char fbuf[BUFSZ];
    assert(fgets(fbuf, BUFSZ, fp) != NULL);

    u8 lr[BUFSZ];
    size_t lr_len = strlen(fbuf) - 1;
    size_t i;
    for (i = 0; i < lr_len; ++i) {
        lr[i] = fbuf[i] == 'L' ? 0 : 1;
    }

    assert(fgets(fbuf, BUFSZ, fp) != NULL);

    HashMap hm = { 0 };
    while (fgets(fbuf, BUFSZ, fp)) {
        char key[3] = { *fbuf, *(fbuf + 1), *(fbuf + 2) };
        char val[2][3] = { { *(fbuf + 7), *(fbuf + 8), *(fbuf + 9) }, { *(fbuf + 12), *(fbuf + 13), *(fbuf + 14) } };
        hm_put(hm, key, val);
    }

    char k[3] = { 'A', 'A', 'A' };
    char *tmp;

    for (i = 0; ; ++i) {
        tmp = hm_get(hm, k)[lr[i % lr_len]];
        k[0] = tmp[0];
        k[1] = tmp[1];
        k[2] = tmp[2];
        if (strncmp(k, "ZZZ", 3) == 0) {
            break;
        }
    }
    
    printf("Steps: %zu\n", i + 1);

    return 0;
}

#else // TEST


int main(void)
{
    test_result_print();
}
#endif // TEST
