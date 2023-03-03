My solution is written in C. I use the libdivsufsort library for constructing the suffix array itself.

The project can be built simply by running `make` from the top-level directory, provided that
libdivsufsort is installed and can be linked by compiling with -ldivsufsort. The top-level
Makefile simply invokes the Makefile in src and moves the executables to the top-level directory.

The executables are:
* buildsa
* querysa
* genquery -- used to generate queries by randomly selecting substrings of a user-specified fixed length from a reference genome.

I consulted the following resources for guidance:
* https://rob-p.github.io/CMSC701_S23/static_files/presentations/CMSC701_S23_3.pdf (class slides)

* https://linux.die.net/man/3/, for documentation on C library functions

* Various StackOverflow posts for small file operations
* https://stackoverflow.com/questions/3319717/is-there-a-bit-equivalent-of-sizeof-in-c, for CHAR_BITS
* https://stackoverflow.com/questions/2082743/c-equivalent-to-fstreams-peek, for fpeek
* https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c, for fsize

* https://www.prepbytes.com/blog/linked-list/generic-linked-list-in-c/, for generic linked list