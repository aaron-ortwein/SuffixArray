#include "fileutils.h"

#define FASTA_LINE_LENGTH 80 

int fexists(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file) {
        fclose(file);
        return 1;
    }

    return 0;
}

long fsize(FILE* file) {
    long pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, pos, SEEK_SET);
    return size;
}

int fpeek(FILE* file) {
    int c = fgetc(file);
    ungetc(c, file);
    return c;
}

size_t readline(FILE* file, char* dest) {
    size_t bytes_read = 0;
    char buffer[FASTA_LINE_LENGTH + 1];

    while (fgets(buffer, sizeof(buffer), file)) {
        size_t eob = strcspn(buffer, "\n");
        buffer[eob] = 0;
        if (dest) strcpy(dest + bytes_read, buffer);
        bytes_read += eob;
        if (eob < sizeof(buffer) - 1) return bytes_read;
    }

    return EOF;
}

size_t readline_until_sentinel(FILE* file, int sentinel, char* dest) {
    size_t bytes_read = 0;

    while (!(fpeek(file) == sentinel || fpeek(file) == EOF)) {
        bytes_read += readline(file, dest + bytes_read);
    }

    return bytes_read;
}

size_t readline_until_eof(FILE* file, char* dest) {
   return readline_until_sentinel(file, EOF, dest);
}
