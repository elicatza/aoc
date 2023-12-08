#include <stdio.h>
#include <stddef.h>
#include <ctype.h>

#define NOB_IMPLEMENTATION
#include "./nob.h"

#ifndef TEST
#define WIDTH 141
#else
#define WIDTH 11
#endif

// TODO: look into math pattern with %


int engine_sum(char *engine, int width)
{
    const int adj[] = {-width - 1, -width, -width + 1, -1, 1, width - 1, width, width + 1};
    const size_t adj_len = sizeof(adj) / sizeof(adj[0]);

    size_t sum = 0;

    size_t i;
    for (i = 0; engine[i] != '\0'; ++i) {
        if (engine[i] != '*') continue;
        size_t num_buf[2] = { 0, 0 };


        size_t adj_i;
        for (adj_i = 0; adj_i < adj_len; ++adj_i) {

            int num_off;
            for (num_off = 0; isdigit(engine[i + adj[adj_i] - num_off]); ++num_off);

            if (num_off > 0) {
                --num_off;
            } else {
                continue;
            }

            char *tmp;
            size_t num = strtoul(&engine[i + adj[adj_i] - num_off], &tmp, 10);
            if (num_buf[0] == 0) {
                num_buf[0] = num;
            } else if (num_buf[1] == 0) {
                num_buf[1] = num;
            } else {
                continue;
            }

            for (; tmp > &engine[i + adj[adj_i + 1]]; ++adj_i);
            sum += num_buf[0] * num_buf[1];
        }
    }
    return sum;
}


#ifndef TEST


int main(void)
{
    Nob_String_Builder input = { 0 };
    nob_read_entire_file("input/03.txt", &input);

    printf("Sum: %d\n", engine_sum(input.items, 141));
    nob_sb_free(input);

    return 0;
}

#else  // TEST
#include <stdbool.h>

#include "core.h"
void test_games()
{
    char *input =
        "467..114..\n"
		"...*......\n"
		"..35..633.\n"
		"......#...\n"
		"617*......\n"
		".....+.58.\n"
		"..592.....\n"
		"......755.\n"
		"...$.*....\n"
		".664.598..\n";

    int sum = engine_sum(input, 11);
	ASSERT(sum == 467835);

}

void test_big()
{
    char *input =
        ".................521........................................................................................................................\n"
        ".............675*...........................................................................................................................\n"
        "............................................................................................................................................\n";
    int insum = 675 * 521;
    int sum = engine_sum(input, 141);
	ASSERT(sum == insum);
}

void test_small()
{
    char *input =
        ".................521..447....940...123.....................................62....855..452...............455.......264....9...........165....\n"
        ".......$.....675*.....*.../.......*............72......../...........*484....*....=................................*......*.....%...........\n"
        "...................302..476.......64.....................159......815......445..........965........................558...824....281......98.\n";
    int insum = 1071319;
    int sum = engine_sum(input, 141);
	ASSERT(sum == insum);
}

int main(void)
{

    test_games();
    test_small();
    test_big();
    printf("[%d/%d] Tests passed!\n", passed, tests);
    return 0;
}
#endif
