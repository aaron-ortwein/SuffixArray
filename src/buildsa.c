#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fileutils.h"
#include "timeutils.h"
#include "serialize.h"
#include "sa.h"
#include "preftab.h"
#include "genome.h"

// https://www.codingninjas.com/codestudio/library/generic-linked-list-in-c

// replace all uint64_t with size_t?

int main(int argc, char* argv[]) {
    if (!(argc == 3 || argc == 5)) {
        fprintf(stderr, "Usage: %s [--preftab <k>] <reference> <output>\n", argv[0]);
        return 1;
    }

    char* reference;
    char* output;

    int len;
    char* text; 
    int* sa;
    int k = 0;
    struct interval* preftab = NULL;
    struct timespec begin, end;
    
    if (strcmp(argv[1], "--preftab") == 0) {
        k = atoi(argv[2]);
        if (!k) {
            fprintf(stderr, "invalid k-mer size\n");
            return 1;
        }

        reference = argv[3];
        output = argv[4];
    }
    else {
        reference = argv[1];
        output = argv[2];
    }

    if (!fexists(reference)) {
        perror(reference);
        return 1;
    }
    
    text = read_reference_genome(reference, &len);
    printf("%d\n", len);
    
    clock_gettime(CLOCK_MONOTONIC, &begin);
    sa = build_sa(text, len);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double sa_time = time_delta(begin, end);
    printf("SA Construction Time: %.4f s\n", sa_time);

    if (k > 0) {
        clock_gettime(CLOCK_MONOTONIC, &begin);
        preftab = build_preftab(text, sa, len, k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double preftab_time = time_delta(begin, end);
        printf("Preftab Construction Time: %.4f s\n", preftab_time);
    }
    
    serialize(output, text, sa, len, preftab, k);
    
    free(text);
    free(sa);
    free(preftab);

    return 0;
}
