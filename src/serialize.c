#include <stdio.h>
#include "serialize.h"
#include "fileutils.h"
#include "genome.h"

void serialize(const char* index, char* text, int* sa, int len, struct interval* preftab, size_t k) {
    FILE* serialized = fopen(index, "wb");

    fwrite(&len, sizeof(int), 1, serialized);
    fwrite(text, sizeof(char), len + 1, serialized);
    fwrite(sa, sizeof(int), len, serialized);
    if (preftab) {
        fwrite(&k, sizeof(size_t), 1, serialized);
        fwrite(preftab, sizeof(struct interval), pow4(k), serialized);
    }

    fclose(serialized);
}

void deserialize(const char* index, char** text, int** sa, int* len, struct interval** preftab, size_t* k) {
    FILE* serialized = fopen(index, "rb");

    fread(len, sizeof(int), 1, serialized);
    *text = malloc(*len + 1);
    *sa = malloc(*len * sizeof(int));
    fread(*text, sizeof(char), *len + 1, serialized);
    fread(*sa, sizeof(int), *len, serialized);
    if (fpeek(serialized) != EOF) {
        fread(k, sizeof(size_t), 1, serialized);
        int pow4k = pow4(*k);
        *preftab = malloc(pow4k * sizeof(struct interval));
        fread(*preftab, sizeof(struct interval), pow4k, serialized);
    }

    fclose(serialized);
}
