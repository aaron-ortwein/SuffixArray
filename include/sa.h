#include <stdlib.h>

int binary_search_naive(const char* text, const int* sa, const char* pattern, int l, int r);
int binary_search_lcp(const char* text, const int* sa, const char* pattern, int l, int r);
int* build_sa(const char* text, size_t len);