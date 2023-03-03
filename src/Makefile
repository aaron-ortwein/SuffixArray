CC = gcc
CFLAGS = -I'../include/'
LDLIBS = -ldivsufsort

all: buildsa querysa genquery

buildsa: buildsa.c fileutils.c timeutils.c serialize.c sa.c preftab.c genome.c
	$(CC) $(CFLAGS) buildsa.c fileutils.c timeutils.c serialize.c sa.c preftab.c genome.c -o buildsa $(LDLIBS)
	mv buildsa ..

querysa: querysa.c fileutils.c timeutils.c serialize.c linked_list.c bst.c sa.c preftab.c genome.c
	$(CC) $(CFLAGS) querysa.c fileutils.c timeutils.c serialize.c linked_list.c bst.c sa.c preftab.c genome.c -o querysa $(LDLIBS)
	mv querysa ..

genquery: genquery.c fileutils.c genome.c
	$(CC) $(CFLAGS) genquery.c fileutils.c genome.c -o genquery
	mv genquery ..

clean:
	rm -f buildsa querysa genquery
	