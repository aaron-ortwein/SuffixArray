#include <stdio.h>
#include <string.h>

#include "preftab.h"
#include "genome.h"
#include "sa.h"

struct interval* build_preftab(char* text, int* sa, size_t len, size_t k) {
    struct interval* preftab = calloc(pow4(k), sizeof(struct interval));

    int start = 1;
    for (int i = start; i < len; i++) {
        if (len - sa[i] <= k) continue;
        else if (strncmp(&text[sa[start]], &text[sa[i]], k) != 0) {
            preftab[pattern_to_index(&text[sa[start]], k)] = (struct interval) {start, i};
            start = i;
        }
    }

    preftab[pattern_to_index(&text[sa[start]], k)] = (struct interval) {start, len};

    return preftab;
}

struct interval lookup(struct interval* preftab, const char* prefix, size_t k) {
    return preftab[pattern_to_index(prefix, k)];
}
