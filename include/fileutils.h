#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FILEUTILS_H
#define FILEUTILS_H

int fexists(const char* path);
long fsize(FILE* file);
int fpeek(FILE* file);
size_t readline(FILE* file, char* dest);
size_t readline_until_sentinel(FILE* file, int sentinel, char* dest);
size_t readline_until_eof(FILE* file, char* dest);

#endif