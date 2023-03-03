#include <stdlib.h>

#ifndef PREFTAB_H
#define PREFTAB_H

struct interval {
    int start, end;
};

struct interval* build_preftab(char* text, int* sa, size_t len, size_t k);
struct interval lookup(struct interval* preftab, const char* prefix, size_t k);

#endif