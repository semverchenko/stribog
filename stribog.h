#ifndef STRIBOG_H
#define STRIBOG_H

#include <stdint.h>
#include <string.h>

struct stribog_ctx {
	uint64_t h[8], S[8];
	uint64_t N;

	uint64_t last_block[8];
	size_t last_block_size;
	int mode256;
};

void add512(uint64_t *a, uint64_t *b);
void stribog_hash(void *res, const void *data, size_t size, int mode256);
void stribog_init(struct stribog_ctx *ctx, int is256);
void stribog_add_data(struct stribog_ctx *ctx, const void *data, size_t size);
void stribog_close(struct stribog_ctx *ctx, void *res);
void LPS(uint64_t *a);
void LPSX(uint64_t *a, uint64_t *b);

extern uint8_t sbox[256];
extern uint8_t pbox[64];
extern uint64_t lbox[64];
extern uint64_t C[12][8];

#endif
