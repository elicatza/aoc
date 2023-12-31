#include <stdio.h>

#define NOB_IMPLEMENTATION
#include "./nob.h"

enum Color {
    RED,
    GREEN,
    BLUE,
};

typedef struct {
    enum Color color;
    int amount;
} Cube;

Cube cstr_to_cube(char *cstr, char **endptr)
{
    assert(cstr != NULL);
    assert(endptr != NULL);

    *endptr = cstr;
    Cube cube = { 0 };
    cube.amount = strtoul(cstr, endptr, 10);
    if (*endptr == cstr) return cube;

    ++*endptr;

    if (strncmp(*endptr, "red", strlen("red")) == 0) {
        cube.color = RED;
        *endptr += strlen("red");
    } else if (strncmp(*endptr, "green", strlen("green")) == 0) {
        cube.color = GREEN;
        *endptr += strlen("green");
    } else if (strncmp(*endptr, "blue", strlen("blue")) == 0) {
        cube.color = BLUE;
        *endptr += strlen("blue");
    } else {
        *endptr = cstr;
    }

    return cube;
}

int is_valid_game(char *cstr)
{
    char *ptr = cstr;
    ptr += 5;
    char *tmp;
    strtoul(ptr, &tmp, 10);
    ptr = tmp + 2;

    unsigned int max[3] = { 0 };
    while (*(ptr - 2) != '\0' && *(ptr - 2) != '\n') {
        tmp = ptr;
        Cube cube = cstr_to_cube(ptr, &tmp);
        assert(ptr != tmp && "Invalid cube!");
        ptr = tmp + 2;

        if (max[cube.color] < (unsigned int) cube.amount) {
            max[cube.color] = cube.amount;
        }
    }
    return max[RED] * max[GREEN] * max[BLUE];
}

void sv_shift_to_next(Nob_String_View *sv, char pattern)
{
    while (*sv->data != pattern && sv->count > 1) {
        --sv->count;
        ++sv->data;
    }
    --sv->count;
    ++sv->data;
}

int sb_cube_sum(Nob_String_Builder sb)
{
    Nob_String_View sv = nob_sv_from_parts(sb.items, sb.count);
    size_t sum = 0;
    size_t tmp;

    while (sv.count > 0) {
        tmp = is_valid_game((char *) sv.data);
        sum += tmp;
        sv_shift_to_next(&sv, '\n');
    }
    return sum;
}

#ifndef TEST


int main(void)
{
    Nob_String_Builder input = { 0 };
    nob_read_entire_file("input/02.txt", &input);

    printf("Sum: %d\n", sb_cube_sum(input));
    return 0;
}

#else  // TEST

#include "core.h"
void test_games()
{
    ASSERT(is_valid_game("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green") == 48);
	ASSERT(is_valid_game("Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue") == 12);
	ASSERT(is_valid_game("Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red") == 1560);
	ASSERT(is_valid_game("Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red") == 630);
	ASSERT(is_valid_game("Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green") == 36);
}

void test_game_batch()
{
    Nob_String_Builder sb = { 0 };
    nob_sb_append_cstr(&sb, "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\nGame 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\nGame 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\nGame 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\nGame 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green\n");

    ASSERT(sb_cube_sum(sb) == 2286);
}

int main(void)
{
    test_games();
    test_game_batch();
    printf("[%d/%d] Tests passed!\n", passed, tests);
    return 0;
}
#endif
