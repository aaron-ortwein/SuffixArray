#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "genome.h"
#include "fileutils.h"

static const char alphabet[] = "ACGT";

static void format(char* reference, int len);
static void next_pattern_aux(char* pattern, int k, int offset);

char* read_reference_genome(char* reference_filename, int* len) {
    FILE* file = fopen(reference_filename, "r");
    long file_length = fsize(file);

    char* reference = malloc(file_length + 1);
    int genome_length = 0;
    while (fpeek(file) != EOF) {
        readline(file, NULL);
        genome_length += readline_until_sentinel(file, '>', reference + genome_length);
    }
    reference[genome_length++] = '$';
    reference[genome_length] = 0;

    if (len) *len = genome_length;

    format(reference, genome_length); // convert bases to uppercase and replace N's
    
    return reference;
}

static void format(char* reference, int len) {
    for (int i = 0; i < len; i++) {
        reference[i] = toupper(reference[i]);
        if (reference[i] == 'N') {
            switch (rand() % 4) {
                case 0:
                    reference[i] = 'A';
                    break;
                case 1:
                    reference[i] = 'C';
                    break;
                case 2:
                    reference[i] = 'G';
                    break;
                case 3:
                    reference[i] = 'T';
                    break;
            }
        }
    }
}

int pow4(unsigned int k) {
    if (k == 0) return 1;
    return ((int) 4) << (2 * (k - 1));
}

int to_digit(char c) {
    for (size_t i = 0; i < 4; i++) {
        if (c == alphabet[i]) {
            return i;
        }
    }

    return -1;
}

int pattern_to_index(const char* pattern, int k) {
    int n = to_digit(pattern[k - 1]);
    for (int i = 2; i <= k; i++) {
        n += to_digit(pattern[k - i]) * pow4(i - 1);
    }
    return n;
}

char* index_to_pattern(int index, int k) {
    char* pattern = malloc(k + 1);

    for (int i = 1; i <= k; i++) {
        pattern[k - i] = alphabet[index % 4];
        index /= 4;
    }
    pattern[k] = 0;

    return pattern;
}

char* next_pattern(const char* pattern, int k) {
    char* next = strdup(pattern);
    next_pattern_aux(next, k, 0);
    return next;
}

static void next_pattern_aux(char* pattern, int k, int offset) {
    if (offset == k) return;
    pattern[k - 1 - offset] = alphabet[(to_digit(pattern[k - 1 - offset]) + 1) % 4];
    if (pattern[k - 1 - offset] != alphabet[0]) return;
    next_pattern_aux(pattern, k, offset + 1);
}
