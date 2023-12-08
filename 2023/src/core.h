#ifndef CORE_H
#define CORE_H

#include <stdint.h>

typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;

typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef int8_t i8;

typedef float f32;
typedef double f64;

#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

#ifdef TEST
#define ASSERT(statement) test_assert(statement, #statement)

#include <stdbool.h>
#include <stdio.h>
static int passed = 0;
static int tests = 0;

void test_assert(bool statement, char *msg)
{
    ++tests;

    if (statement) {
        printf("\x1b[32mPASSED:\x1b[0m %s\n", msg);
        ++passed;
    } else {
        printf("\x1b[31mFAILED:\x1b[0m %s\n", msg);
    }
}
void test_result_print(void)
{
    printf("[%d/%d] Tests passed!\n", passed, tests);
}
#endif

#endif  // CORE_H
