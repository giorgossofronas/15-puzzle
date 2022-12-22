#include "heuristic.h"

// returns the absolute value of given int
static inline uint abs_int(int x) {
    return x >= 0 ? x : -1 * x;
}

// sum of the manhanttan distances of each tile from its goal position 
uint manhattan_distance(State state) {
    uint sum_manh = 0; 
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (state->puzzle[i][j])
               sum_manh += abs_int((state->puzzle[i][j] - 1) / N - i) + abs_int((state->puzzle[i][j] - 1) % N - j);

    return sum_manh;
}

/* converts a puzzle into a hexadecimal number
   example:
       puzzle: 12 5 9 0
               14 2 3 4
               11 10 6 1
               8 7 15 13
   would be converted into this hex number:
       0xC590E234BA6187FD                    */
static unsigned long state_to_hex(State state) {
    unsigned long result = 0;
    unsigned long power16 = 1; // power of 16 from 0-15

    for (int i = 15, pow = 0; i >= 0; i--, pow++, power16 <<= 4)
        result += state->puzzle[i/4][i%4] * power16;

    return result;
}

// goal state in hexadecimal form
#define GOAL_STATE_HEX 0x123456789ABCDEF0L

// The next two functions calculate the linear conflicts of a given 15-puzzle
// source code from @asarandi | link: https://github.com/asarandi/n-puzzle
static uint count_conflicts(int candidate, int solved, int result) {
    int i, j, ti, tj, f, k, counts[4];

    for (i = 0; i < 4; i++)
        counts[i] = 0;

    for (i = 0; i < 4; i++) {
        if (!(k = (candidate >> ((3 - i) << 2)) & 15))
            continue;

        for (ti = f = 0; ti < 4; ti++)
            if ((f = (k == ((solved >> ((3 - ti) << 2)) & 15))))
                break;

        if (!f)
            continue;

        for (j = 0; j < 4; j++) {
            if (i == j)
                continue;
            if (!(k = (candidate >> ((3 - j) << 2)) & 15))
                continue;

            for (tj = f = 0; tj < 4; tj++)
                if ((f = (k == ((solved >> ((3 - tj) << 2)) & 15))))
                    break;

            if (!f)
                continue;

            counts[i] += ((i > j) && (ti < tj));
            counts[i] += ((i < j) && (ti > tj));
        }
    }
    for (i = k = 0; i < 4; i++)
        k = counts[i] > k ? counts[i] : k;

    if (!k)
        return (result << 1);
    for (i = 0; i < 4; i++)
        if (counts[i] == k)
            break;
    candidate &= ~(15 << ((3 - i) << 2));
    return count_conflicts(candidate, solved, ++result);
}

uint linear_conflicts(State current_state) {
    uint lc = 0;
    int candidate, solved;

    unsigned long state = state_to_hex(current_state);

    for (int i = 0; i < 4; i++) {
        candidate = (int)((state >> ((3 - i) << 4)) & 0xFFFF);
        solved = (int)((GOAL_STATE_HEX >> ((3 - i) << 4)) & 0xFFFF);
        lc += count_conflicts(candidate, solved, 0);

        for (int j = candidate = solved = 0; j < N; j++) {
            candidate = (candidate << 4) | ((state >> (60 - ((i + (j << 2)) << 2))) & 15);
            solved = (solved << 4) | ((GOAL_STATE_HEX >> (60 - ((i + (j << 2)) << 2))) & 15);
        }
        lc += count_conflicts(candidate, solved, 0);
    }
    return lc;
}