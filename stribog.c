#include "stribog.h"
#include "stribog_fast.h"
#include <stdio.h>
#include <stdlib.h>

static inline void LPS2(uint64_t a[8])
{
	size_t i, j;
	uint64_t t[8];
	for (i = 0; i < 64; ++i) {
		((uint8_t *)t)[((i & 0x7) << 3) | (i >> 3)] = sbox[((uint8_t *)a)[i]];
	}
	memset(a, 0, 64);
#if 0
	for (i = 0; i < 8; ++i)
		for (j = 0; j < 64; ++j)
			if (t[i] & (1ULL << (j)))
				a[i] ^= lbox[63 - j];
#else
	for (i = 0; i < 8; ++i)
		for (j = 0; j < 8; ++j)
			a[i] ^= lbox_fast[j][(t[i] >> 8 * j) & 0xff];
#endif
}

static inline void g(uint64_t N, uint64_t h[8], const uint64_t m[8])
{
	uint64_t K[8], mm[8];
	size_t i, j;

	memcpy(K, h, sizeof(K));
	memcpy(mm, m, sizeof(mm));
	K[0] ^= N;
	LPS(K);
	for (i = 0; i < 12; ++i) {
		LPSX(mm, K);
		LPSX(K, C[i]);
	}
	for (j = 0; j < 8; ++j)
		h[j] ^= K[j] ^ mm[j] ^ m[j];
}

void stribog_init(struct stribog_ctx *ctx, int mode256)
{
	ctx->N = 0;
	ctx->mode256 = mode256;
	ctx->last_block_size = 0;
	memset(ctx->h, mode256 ? 1 : 0, sizeof(ctx->h));
	memset(ctx->S, 0, sizeof(ctx->S));
}

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

void stribog_add_data(struct stribog_ctx *ctx, const void *data, size_t size)
{
	size_t a;

	while (size > 0) {
		a = MIN(size, 64 - ctx->last_block_size);
		memcpy((uint8_t *)ctx->last_block + ctx->last_block_size, data, a);
		ctx->last_block_size += a;
		size -= a;
		data = (const void *)((const uint8_t *)data + a);
		if (ctx->last_block_size != 64)
			break;

		g(ctx->N, ctx->h, (uint64_t *)ctx->last_block);
		ctx->N += 512;
		add512(ctx->S, (uint64_t *)ctx->last_block);

		ctx->last_block_size = 0;
	}
}

void stribog_close(struct stribog_ctx *ctx, void *res)
{
	memset((uint8_t *)ctx->last_block + ctx->last_block_size, 0, sizeof(ctx->last_block) - ctx->last_block_size);
	((uint8_t *)ctx->last_block)[ctx->last_block_size] = 0x01;
	g(ctx->N, ctx->h, (uint64_t *)ctx->last_block);
	ctx->N += ctx->last_block_size * 8;
	add512(ctx->S, (uint64_t *)ctx->last_block);
	memset(ctx->last_block, 0, sizeof(ctx->last_block));
	ctx->last_block[0] = ctx->N;
	g(0, ctx->h, (uint64_t *)ctx->last_block);
	g(0, ctx->h, ctx->S);
	if (ctx->mode256)
		memcpy(res, ctx->h + 4, 32);
	else
		memcpy(res, ctx->h, sizeof(ctx->h));
}

void stribog_hash(void *res, const void *data, size_t size, int mode256)
{
	uint64_t h[8], S[8], m[8];
	uint64_t N = 0; /* we do not need 512-bit N, as 64-bit N would be enough to hash ~2^64 bits */

	memset(h, mode256 ? 1 : 0, sizeof(h));
	memset(S, 0, sizeof(S));
	for (; size >= 64; size -= 64, data = (void *)((uint8_t *)data + 64)) {
		memcpy(m, data, sizeof(m));
		g(N, h, m);
		N += 512;
		add512(S, m);
	}
	memset(m, 0, sizeof(m));
	memcpy(m, data, size);
	((uint8_t *)m)[size] = 0x01; //works only on little-endian, but who cares?
	g(N, h, m);
	N += size * 8;
	add512(S, m);
	memset(m, 0, sizeof(m));
	m[0] = N;
	g(0, h, m);
	g(0, h, S);
	if (mode256)
		memcpy(res, h + 4, 32);
	else
		memcpy(res, h, sizeof(h));
}
