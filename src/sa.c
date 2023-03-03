#include <stdio.h>
#include <string.h>
#include <divsufsort.h>
#include "sa.h"

static int lcp(const char* string, const char* other, int* cmp) {
    int i;
    for (i = 0; string[i] == other[i] && string[i]; i++);
    if (cmp) *cmp = string[i] - other[i];
    return i;
}

int binary_search_naive(const char* text, const int* sa, const char* pattern, int l, int r) {
    int c;

    for (;;) {
        c = (l + r) / 2;
        int cmp;
        lcp(pattern, &text[sa[c]], &cmp);

        if (cmp < 0) {
            if (c == l + 1) return c;
            r = c;
        }
        else if (cmp > 0) {
            if (c == r - 1) return r;
            l = c;
        }
    }
}

static int min(int a, int b) {
    return (a < b) ? a : b;
}

int binary_search_lcp(const char* text, const int* sa, const char* pattern, int l, int r) {
    int c, skip, lcp_l = 0, lcp_r = 0;
    int cmp;

    for (;;) {
        c = (l + r) / 2;
        skip = min(lcp_l, lcp_r);
        skip += lcp(pattern + skip, &text[sa[c] + skip], &cmp);
        
        if (cmp < 0) {
            if (c == l + 1) return c;
            r = c;
            lcp_r = skip;
        }
        else if (cmp > 0) {
            if (c == r - 1) return r;
            l = c;
            lcp_l = skip;
        }
    }
}

int* build_sa(const char* text, size_t len) {
    int* sa = malloc(len * sizeof(int));
    divsufsort((unsigned char *) text, sa, len);
    return sa;
}