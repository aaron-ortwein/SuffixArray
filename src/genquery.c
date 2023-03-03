#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "fileutils.h"
#include "genome.h"

size_t rand_size_t() {
    size_t random = 0, size_t_size = sizeof(size_t);

    for (int i = 0; i < size_t_size * CHAR_BIT; i++) {
        random |= rand() % 2 << i;
        
    }

    return random;
}

char* random_substring(const char* str, size_t strlen, size_t substrlen) {
    return strndup(str + (rand_size_t() % (strlen - substrlen + 1)), substrlen);
}

void generate_query_file(FILE* query_file, const char* genome, uint64_t len, size_t query_len, size_t nqueries) {
    for (int i = 0; i < nqueries; i++) {
        char* query = random_substring(genome, len, query_len);
        fprintf(query_file, ">Query%d\n", i);
        fprintf(query_file, "%s\n", query);
        free(query);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <reference> <query length> <# queries> <output>\n", argv[0]);
        return 1;
    }

    char* reference = argv[1];
    int query_length = atoi(argv[2]);
    int nqueries = atoi(argv[3]);
    char* output = argv[4];

    if (!fexists(reference)) {
        perror(reference);
        return 1;
    }
    else if (!(query_length || nqueries)) {
        fprintf(stderr, "query length or number of queries 0 is invalid\n");
        return 1;
    }

    int len;
    char* genome = read_reference_genome(reference, &len);

    FILE* file = fopen(output, "w");
    generate_query_file(file, genome, len, query_length, nqueries);
    fclose(file);

    free(genome);

    return 0;
}