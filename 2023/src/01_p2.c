#include <stdio.h>
#include <ctype.h>

#define NOB_IMPLEMENTATION
#include "./nob.h"


char cstr_to_digit(char *cstr)
{
    if (cstr[0] >= '1' && cstr[0] <= '9') return cstr[0];
    if (strncmp(cstr, "one", strlen("one")) == 0) return '1';
    if (strncmp(cstr, "two", strlen("two")) == 0) return '2';
    if (strncmp(cstr, "three", strlen("three")) == 0) return '3';
    if (strncmp(cstr, "four", strlen("four")) == 0) return '4';
    if (strncmp(cstr, "five", strlen("five")) == 0) return '5';
    if (strncmp(cstr, "six", strlen("six")) == 0) return '6';
    if (strncmp(cstr, "seven", strlen("seven")) == 0) return '7';
    if (strncmp(cstr, "eight", strlen("eight")) == 0) return '8';
    if (strncmp(cstr, "nine", strlen("nine")) == 0) return '9';
    return '\0';
}

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

        char digit = cstr_to_digit(&input.items[i]);
        if (digit == '\0') continue;

        if (cur_num[0] == '\0') {
            cur_num[0] = digit;
        } else {
            cur_num[1] = digit;
        }
    }

    printf("Sum: %zu\n", sum);
    nob_sb_free(input);
    return 0;
}
