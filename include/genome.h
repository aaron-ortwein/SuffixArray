#include <stdlib.h>

char* read_reference_genome(char* reference_filename, int* len);
int pow4(unsigned int k);
int to_digit(char c);
int pattern_to_index(const char* pattern, int k);
char* index_to_pattern(int index, int k);
char* next_pattern(const char* pattern, int k);