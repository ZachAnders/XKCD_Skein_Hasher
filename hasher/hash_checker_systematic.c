#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "skein.h"

uint8_t target[] = {0x5b, 0x4d, 0xa9, 0x5f, 0x5f, 0xa0, 0x82, 0x80, 0xfc, 0x98, 0x79, 0xdf, 0x44, 0xf4, 0x18, 0xc8, 0xf9, 0xf1, 0x2b, 0xa4, 0x24, 0xb7, 0x75, 0x7d, 0xe0, 0x2b, 0xbd, 0xfb, 0xae, 0x0d, 0x4c, 0x4f, 0xdf, 0x93, 0x17,
		0xc8, 0x0c, 0xc5, 0xfe, 0x04, 0xc6, 0x42, 0x90, 0x73, 0x46, 0x6c, 0xf2, 0x97, 0x06, 0xb8, 0xc2, 0x59, 0x99, 0xdd, 0xd2, 0xf6, 0x54, 0x0d, 0x44, 0x75, 0xcc, 0x97, 0x7b, 0x87, 0xf4, 0x75, 0x7b, 0xe0, 0x23, 0xf1, 0x9b, 0x8f, 0x40,
		0x35, 0xd7, 0x72, 0x28, 0x86, 0xb7, 0x88, 0x69, 0x82, 0x6d, 0xe9, 0x16, 0xa7, 0x9c, 0xf9, 0xc9, 0x4c, 0xc7, 0x9c, 0xd4, 0x34, 0x7d, 0x24, 0xb5, 0x67, 0xaa, 0x3e, 0x23, 0x90, 0xa5, 0x73, 0xa3, 0x73, 0xa4, 0x8a, 0x5e, 0x67, 0x66,
		0x40, 0xc7, 0x9c, 0xc7, 0x01, 0x97, 0xe1, 0xc5, 0xe7, 0xf9, 0x02, 0xfb, 0x53, 0xca, 0x18, 0x58, 0xb6};
int count_bits(uint8_t a1, uint8_t a2) {
	a1 = ~(a1^a2);
	int cntr = 0;
	cntr = a1 & 0x11;
	a1 >>= 1;
	cntr += a1 & 0x11;
	a1 >>= 1;
	cntr += a1 & 0x11;
	a1 >>= 1;
	cntr += a1 & 0x11;
	return (cntr&0xF) + (cntr>>4);
}

int compare(uint8_t *arr1, uint8_t *arr2, uint64_t len) {
    int i;
    for (i=0; i < len; i++) {
        if (arr1[i] != arr2[i])
			return 1;
    }
    return 0;
}
int compare_full(uint8_t *arr1, uint8_t *arr2, uint64_t len) {
    int i, cnt=len*8;
    for (i=0; i < len; i++) {
		cnt -= count_bits(arr1[i], arr2[i]);
    }
    return cnt;
}



void hash_out(FILE* stream, uint8_t *arr, uint64_t len) {
    int i;

    for (i = 0; i < len; i++) {
        fprintf(stream, "%.2x", arr[i]);
    }
    fprintf(stream, "\n");
}

void do_hash(Skein1024_Ctxt_t *ctx, uint8_t *inp, uint64_t len, uint8_t *res) {
	Skein1024_Init(ctx, 1024);
	Skein1024_Update(ctx, inp, len);
	Skein1024_Final(ctx, res);
}

void seed_arr(uint8_t *inp, uint64_t len) {
	inp[random() % len] = (uint8_t) (random()&0xFF);
}

void incr_array(uint8_t *inp, uint64_t len) {
	if (len == 0) return;
	uint8_t *curs = inp;
	curs += len;
	do {
		curs--;
		(*curs)++;
	} while (!(*curs) && curs != inp);
}

void save(int prefix, uint8_t *inp, uint64_t len) {
	char fname[128];
	int i = 0;
	sprintf(fname, "%d.dat", prefix);
	FILE* outf = fopen(fname, "w+b");

	for (i=0; i<len; i++) {
		fputc(inp[i], outf);
	}
	fclose(outf);
}
void attempt_load(int prefix, uint8_t *inp, uint64_t len) {
	char fname[128];
	int i=0;
	sprintf(fname, "%d.dat", prefix);
	FILE * inf = fopen(fname, "r+b");
	if ( inf ) {
		for (i=0; i < len; i++) {
			inp[i] = fgetc(inf);
		}
		fclose(inf);
	}
}

	
#define INP_SIZE 32

int main(int argc, char* argv[]) {
	Skein1024_Ctxt_t ctx;
	int min = INT_MAX;
	//int len = 128;
	int cntr = 0;
	uint8_t result[128];// = {'x', 'k', 'c', 'd'};
	uint8_t inp[INP_SIZE];// = {'x', 'k', 'c', 'd'};
	memset(inp, 42, INP_SIZE);
	uint8_t start[128];
	
	if (argc > 1) {
		int i;
		for (i=0;i<argc-1;i++) {
			inp[i] = (uint8_t) atoi( argv[i+1] );
		}
	}	
	int offset = inp[0];

	attempt_load(offset, inp, INP_SIZE);

	printf("Beginning Hashing at:\n");
	hash_out(stdout, inp, INP_SIZE);

	while (1) {
		cntr++;

		do_hash(&ctx, inp, INP_SIZE, result);
		if (compare_full(result, target, 128) < min) {
			min = compare_full(result, target, 128);
			hash_out(stdout, inp, INP_SIZE);
			printf("prefix: %d, difference: %d\n", offset, min);
			save(offset, inp, INP_SIZE);
		}

		if (!(cntr & 0x1FFFFF)) {
			printf("~2m hashes. Prefix: %d, Minimum: %d\n", offset, min);
			memcpy(start, inp, INP_SIZE);
			save(offset, inp, INP_SIZE);
		}
		incr_array(inp, INP_SIZE);
	}

	printf("Final: %d\n", cntr);
	hash_out(stdout, result, 128);
	hash_out(stdout, start, 128);

	return 1;
}

