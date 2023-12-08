#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>

#define NOB_IMPLEMENTATION
#include "./nob.h"

#ifndef TEST
#define WIDTH 141
#else
#define WIDTH 11
#endif

// TODO: look into math pattern with %


bool is_symbol(char sym)
{
    if (isdigit(sym) || sym == '\n' || sym == '.') {
        return false;
    }
    return true;
}


int engine_sum(char *engine, int width)
{
    const int adj[] = {-width - 1, -width, -width + 1, -1, 1, width - 1, width, width + 1};
    const size_t adj_len = sizeof(adj) / sizeof(adj[0]);
    size_t i;

    size_t sum = 0;
    size_t adj_i;
    int num_off = 0;
    char *tmp;

    for (i = 0; engine[i] != '\0'; ++i) {
        if (!is_symbol(engine[i])) continue;

        for (adj_i = 0; adj_i < adj_len; ++adj_i) {

            for (num_off = 0; isdigit(engine[i + adj[adj_i] - num_off]); ++num_off);

            if (num_off > 0) {
                --num_off;
            } else {
                continue;
            }

            size_t num = strtoul(&engine[i + adj[adj_i] - num_off], &tmp, 10);
            for (; tmp > &engine[i + adj[adj_i + 1]]; ++adj_i);
            sum += num;
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
    printf("my sum: %d\n", sum);
	ASSERT(sum == 4361);

}

void test_big()
{
    char *input =
        ".................521........................................................................................................................\n"
        ".............675*...........................................................................................................................\n"
        "............................................................................................................................................\n";
    int insum = 675 + 521;
    int sum = engine_sum(input, 141);
    printf("my sum: %d\t%d\n", sum, insum);
	ASSERT(sum == insum);
}

void test_small()
{
    char *input =
        ".................521..447....940...123.....................................62....855..452...............455.......264....9...........165....\n"
        ".......$.....675*.....*.../.......*............72......../...........*484....*....=................................*......*.....%...........\n"
        "...................302..476.......64.....................159......815......445..........965........................558...824....281......98.\n";
    int insum = 675 + 521 + 447 + 302 + 476 + 123 + 64 + 159 + 815 + 484 + 62 + 445 + 855 + 264 + 558 + 9 + 824 + 281;
    //          ^^^                                            ^^^
    int sum = engine_sum(input, 141);
    printf("my sum: %d\t%d\n", sum, insum);
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
