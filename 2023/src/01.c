#include <stdio.h>
#include <ctype.h>

#define NOB_IMPLEMENTATION
#include "./nob.h"


int main(void)
{
    Nob_String_Builder input = { 0 };
    nob_read_entire_file("input/01.txt", &input);
    size_t sum;
    size_t i;

    char cur_num[2];
    for (i = 0; i < input.count; ++i) {
        if (input.items[i] == '\n') {
            if (cur_num[1] == '\0') {
                cur_num[1] = cur_num[0];
            }
            sum += strtoul(cur_num, NULL, 10);
            cur_num[0] = '\0';
            cur_num[1] = '\0';
            continue;
        }

        if (isdigit(input.items[i])) {
            if (cur_num[0] == '\0') {
                cur_num[0] = input.items[i];
            } else {
                cur_num[1] = input.items[i];
            }
        }
    }

    printf("Sum: %zu\n", sum);
    nob_sb_free(input);
    return 0;
}
