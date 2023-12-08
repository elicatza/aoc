#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./core.h"

#define BUFSZ 1 << 12
#define ALPHAB_LEN 26
#define HMSIZE ALPHAB_LEN * ALPHAB_LEN * ALPHAB_LEN + ALPHAB_LEN * ALPHAB_LEN + ALPHAB_LEN

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

    size_t starts_hash_len = 0;
    char starts_hash[BUFSZ][3];
    HashMap hm = { 0 };
    while (fgets(fbuf, BUFSZ, fp)) {
        char key[3] = { *fbuf, *(fbuf + 1), *(fbuf + 2) };
        char val[2][3] = { { *(fbuf + 7), *(fbuf + 8), *(fbuf + 9) }, { *(fbuf + 12), *(fbuf + 13), *(fbuf + 14) } };
        if (key[2] == 'A') {
            starts_hash[starts_hash_len][0] = key[0];
            starts_hash[starts_hash_len][1] = key[1];
            starts_hash[starts_hash_len][2] = key[2];
            ++starts_hash_len;
        }
        hm_put(hm, key, val);
    }

    size_t matches[starts_hash_len];
    size_t uniq_matches = 0;
    memset(matches, 0, sizeof(*matches) * starts_hash_len);
    for (i = 0; ; ++i) {
        size_t starts;
        for (starts = 0; starts < starts_hash_len; ++starts) {
            char *tmp = hm_get(hm, starts_hash[starts])[lr[i % lr_len]];
            starts_hash[starts][0] = tmp[0];
            starts_hash[starts][1] = tmp[1];
            starts_hash[starts][2] = tmp[2];

            if (tmp[2] == 'Z') {
                if (matches[starts] == 0) {
                    ++uniq_matches;
                    matches[starts] = (i + 1) / lr_len;
                    if (uniq_matches >= starts_hash_len) {
                        goto end;
                    }
                }
            }
        }
    }
end:;
    size_t sum = 271;
    for (i = 0; i < starts_hash_len; ++i) {
        sum  *= matches[i];
    }
    printf("%zu\n", sum);
    

    return 0;
}

#else // TEST


int main(void)
{
    test_result_print();
}
#endif // TEST
