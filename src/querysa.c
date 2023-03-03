#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "fileutils.h"
#include "timeutils.h"
#include "serialize.h"
#include "linked_list.h"
#include "bst.h"
#include "sa.h"
#include "preftab.h"
#include "genome.h"

struct query {
    char *name, *sequence;
    int sequence_length;
};

struct result {
    char* name;
    int num_hits;
    struct bst* positions;
};

void free_query(void* ptr) {
    struct query* query = ptr;
    free(query->name);
    free(query->sequence);
    free(query);
}

void free_result(void* ptr) {
    struct result* result = ptr;
    free(result->name);
    bst_free(result->positions, &free);
    free(result);
}

int lt(void* ptr, void* other) {
    return *((size_t*) ptr) < *((size_t*) other);
}

struct linked_list* read_queries(const char* queries_filename) {
    struct linked_list* queries = initialize_linked_list();

    FILE* file = fopen(queries_filename, "r");
    long file_length = fsize(file);

    while (fpeek(file) != EOF) {
        struct query* query = malloc(sizeof(struct query));
        char* name = malloc(file_length + 1);
        char* sequence = malloc(file_length + 1);

        fgetc(file);                                                                // advance past >
        fscanf(file, "%s", name);                                                   // read query name
        readline(file, NULL);                                                       // clear rest of header line
        uint64_t sequence_length = readline_until_sentinel(file, '>', sequence);    // read sequence until next query

        query->name = name;
        query->sequence = sequence;
        query->sequence_length = sequence_length;

        append(queries, query);
    }

    return queries;
}

struct linked_list* process_queries(const char* text, int* sa, int len, struct interval* preftab, int k, 
        int (*binary_search)(const char*, const int*, const char*, int, int), struct linked_list* queries) {
    struct linked_list* results = initialize_linked_list();

    for (struct node* node = queries->head->next; node; node = node->next) {
        struct query* query = node->data;
        char* next_sequence = next_pattern(query->sequence, query->sequence_length);
        struct interval query_interval = {1, len};
        struct interval next_interval = {1, len};
        
        if (preftab) {
            char query_kmer[k + 1], next_kmer[k + 1];
            strncpy(query_kmer, query->sequence, k);
            strncpy(next_kmer, next_sequence, k);
            query_interval = lookup(preftab, query_kmer, k);
            next_interval = lookup(preftab, next_kmer, k);
        }
        
        size_t start = binary_search(text, sa, query->sequence, query_interval.start - 1, query_interval.end);
        size_t end = binary_search(text, sa, next_sequence, next_interval.start - 1, next_interval.end);
        free(next_sequence);
        
        struct result* result = malloc(sizeof(struct result));
        result->name = strdup(query->name);
        result->num_hits = end - start;
        result->positions = NULL;
        for (size_t i = start; i < end; i++) {
            int* position = malloc(sizeof(int));
            *position = sa[i];
            result->positions = bst_insert(result->positions, position, &lt);
        }

        append(results, result);
    }

    return results;
}

void output_positions(struct bst* root, char delimiter, FILE* file) {
    if (root) {
        output_positions(root->left, delimiter, file);
        fprintf(file, "%c%d", delimiter, *((int*) root->data));
        output_positions(root->right, delimiter, file);
    }
}

void output_results(struct linked_list* results, FILE* file) {
    for (struct node* node = results->head->next; node; node = node->next) {
        struct result* result = node->data;
        fprintf(file, "%s\t%d", result->name, result->num_hits);
        output_positions(result->positions, '\t', file);
        fprintf(file, "\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5 || argc > 5) {
        fprintf(stderr, "Usage: %s <index> <queries> <query mode> <output>\n", argv[0]);
        return 1;
    }

    char* index = argv[1];
    char* queries_file = argv[2];
    char* query_mode = argv[3];
    char* output_file = argv[4];

    // Report as many errors as possible before terminating
    errno = 0;
    if (!fexists(index)) perror(index);
    if (!fexists(queries_file)) perror(queries_file);
    int naive = (strcmp(query_mode, "naive") == 0);
    int simpaccel = (strcmp(query_mode, "simpaccel") == 0);
    int valid_mode = naive || simpaccel;
    if (!valid_mode) fprintf(stderr, "Invalid query mode %s\n", query_mode);
    if (errno != 0 || !valid_mode) return 1;

    char* text;
    int* sa;
    int len;
    struct interval* preftab;
    size_t k;
    int (*binary_search)(const char*, const int*, const char*, int, int) = 
        (naive) ? &binary_search_naive : &binary_search_lcp;
   
    deserialize(index, &text, &sa, &len, &preftab, &k);

    struct linked_list* queries = read_queries(queries_file);
    
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    struct linked_list* results = process_queries(text, sa, len, preftab, k, binary_search, queries);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double query_time = time_delta(begin, end);
    printf("SA Query Time: %.4f s\n", query_time);

    FILE* output = fopen(output_file, "w");
    output_results(results, output);
    fclose(output);

    free(text);
    free(sa);
    free(preftab);
    free_linked_list(queries, &free_query);
    free_linked_list(results, &free_result);

    return 0;
}
