#include <stdlib.h>
#include "preftab.h"

void serialize(const char* index, char* text, int* sa, int len, struct interval* preftab, size_t k);
void deserialize(const char* index, char** text, int** sa, int* len, struct interval** preftab, size_t* k);