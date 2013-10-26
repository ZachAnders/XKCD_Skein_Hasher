CC = gcc
CFLAGS = -g -c -O3
LFLAGS = -lc -Wall -O3 -fno-strict-aliasing

all: hash_checker_systematic 

hash_checker: hash_checker.c skein.o skein_block.o SHA3api_ref.o
	$(CC) $(LFLAGS) $^ -o $@

hash_checker_systematic: hash_checker_systematic.c skein.o skein_block.o SHA3api_ref.o
	$(CC) $(LFLAGS) $^ -o $@
	
skein_block.o: skein_block.c skein_block.c
	$(CC) $(CFLAGS) $^ -o $@

skein.o: skein.c
	$(CC) $(CFLAGS) $^ -o $@

SHA3api_ref.o: SHA3api_ref.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o skein_test hash_checker hash_checker_systematic
