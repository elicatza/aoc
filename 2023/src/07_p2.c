#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "./core.h"

#define BUFSZ 1 << 12
#define HAND_LEN 5

typedef struct {
    u32 bid;
    u8 cards[5];
} Hand;

typedef enum {
    HighCard = 0,
    OnePair,
    TwoPair,
    ThreeOfKind,
    FullHouse,
    FourOfKind,
    FiveOfKind,
} HandType;

Hand parse_hand(char *line)
{
    Hand hand;
    hand.bid = strtoul(line + 6, NULL, 10);
    size_t i;
    for (i = 0; i < 5; ++i) {
        if (isdigit(*(line + i))) {
            hand.cards[i] = *(line + i) - '0';
            continue;
        }

        switch (*(line + i)) {
            case ('T'):
                hand.cards[i] = 10;
                break;
            case ('J'):
                hand.cards[i] = 1;
                break;
            case ('Q'):
                hand.cards[i] = 12;
                break;
            case ('K'):
                hand.cards[i] = 13;
                break;
            case ('A'):
                hand.cards[i] = 14;
                break;
            default:
                fprintf(stderr, "ERROR: This should not happen! Have fun!!\n");
                exit(1);
        }
    }
    return hand;
}

int contains_u8(u8 *hs, size_t hs_len, u8 needle)
{
    size_t i;
    for (i = 0; i < hs_len; ++i) {
        if (hs[i] == needle) return i;
    }
    return -1;
}

int max_u8_id(u8 *arr, size_t arr_len)
{
    assert(arr_len > 0);
    size_t i;
    u8 max = 0;
    for (i = 1; i < arr_len; ++i) {
        if (arr[i] >= arr[max]) {
            max = i;
        }
    }
    return max;
}

HandType eval_hand(Hand hand)
{
    u8 num[HAND_LEN] = { 0 };
    u8 count[HAND_LEN] = { 0 };
    u8 num_len = 0;

    size_t i;
    size_t j_count = 0;
    for (i = 0; i < HAND_LEN; ++i) {
        if (hand.cards[i] == 1) {
            ++j_count;
            continue;
        }
        if (contains_u8(num, num_len, hand.cards[i]) < 0) {
            num[num_len] = hand.cards[i];
            count[num_len] = 1;
            ++num_len;
        } else {
            ++count[contains_u8(num, num_len, hand.cards[i])];
        }
    }

    if (j_count == 5) return FiveOfKind;
    count[max_u8_id(count, num_len)] += j_count;

    switch (num_len) {
        case 1:
            return FiveOfKind;
        case 2:
            if (count[0] == 4 || count[1] == 4) return FourOfKind;
            return FullHouse;
        case 3:
            if (count[0] == 3 || count[1] == 3 || count[2] == 3) return ThreeOfKind;
            return TwoPair;
        case 4:
            return OnePair;
        case 5:
            return HighCard;
        default:
            fprintf(stderr, "ERROR: This should not happen again! Have more fun!!\n");
            exit(1);
    }
}

int compr_hands(const void *hand_a, const void *hand_b)
{
    Hand a = *(Hand *) hand_a;
    Hand b = *(Hand *) hand_b;

    i32 diff = eval_hand(a) - eval_hand(b);
    if (diff != 0) return diff;

    size_t i;
    for (i = 0; i < HAND_LEN; ++i) {
        diff = a.cards[i] - b.cards[i];
        if (diff != 0) return diff;
    }

    fprintf(stderr, "Equal? Sholud not happen\n");

    return 0;
}

#ifndef TEST

int main(void)
{
    char fbuf[BUFSZ];
    FILE *fp = fopen("./input/07.txt", "r");
    assert(fp != NULL);

    Hand hands[BUFSZ];
    size_t hands_len = 0;

    size_t i;
    for (i = 0; fgets(fbuf, BUFSZ, fp) != NULL; ++i) {
        hands[i] = parse_hand(fbuf);
    }
    hands_len = i;

    qsort(hands, hands_len, sizeof(*hands), compr_hands);

    size_t sum = 0;
    for (i = 0; i < hands_len; ++i) {
        sum += (i + 1) * hands[i].bid;
    }

    printf("Sum: %zu\n", sum);
    return 0;
}

#else // TEST

void test_eval_hand(void)
{
	ASSERT(eval_hand(parse_hand("32T3K 765")) == OnePair);
	ASSERT(eval_hand(parse_hand("T55J5 684")) == FourOfKind);
	ASSERT(eval_hand(parse_hand("KK677 28")) == TwoPair);
	ASSERT(eval_hand(parse_hand("KTJJT 220")) == FourOfKind);
	ASSERT(eval_hand(parse_hand("QQQJA 483")) == FourOfKind);
	ASSERT(eval_hand(parse_hand("QQQJJ 360")) == FiveOfKind);
	ASSERT(eval_hand(parse_hand("QQQQJ 360")) == FiveOfKind);
	ASSERT(eval_hand(parse_hand("QQQQQ 448")) == FiveOfKind);
	ASSERT(eval_hand(parse_hand("62345 553")) == HighCard);
	ASSERT(eval_hand(parse_hand("8TTQQ 10")) == TwoPair);
}

int main(void)
{
    test_eval_hand();
    test_result_print();
}
#endif // TEST
