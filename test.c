#include <stdio.h>
#include <stdint.h>
#include "stribog.h"

void print_stribog(const void *mess, size_t size, int is256)
{
	uint8_t res[64];
	size_t i;
	struct stribog_ctx ctx;

	stribog_hash(res, mess, size, is256);
	if (is256) {
		for (i = 31; i < 32; --i)
			printf("%02x", res[i]);
		printf("\n");
	} else {
		for (i = 63; i < 64; --i)
			printf("%02x", res[i]);
		printf("\n");
	}
	stribog_init(&ctx, is256);
	stribog_add_data(&ctx, mess, size);
	stribog_close(&ctx, res);
	if (is256) {
		for (i = 31; i < 32; --i)
			printf("%02x", res[i]);
		printf("\n");
	} else {
		for (i = 63; i < 64; --i)
			printf("%02x", res[i]);
		printf("\n");
	}
	stribog_init(&ctx, is256);
	stribog_add_data(&ctx, mess, size / 2);
	stribog_add_data(&ctx, (uint8_t *)mess + size / 2, size / 2 + size % 2);
	stribog_close(&ctx, res);
	if (is256) {
		for (i = 31; i < 32; --i)
			printf("%02x", res[i]);
		printf("\n");
	} else {
		for (i = 63; i < 64; --i)
			printf("%02x", res[i]);
		printf("\n");
	}
}

int main()
{
	uint64_t mess[8] = {
		0x3736353433323130,
		0x3534333231303938,
		0x3332313039383736,
		0x3130393837363534,
		0x3938373635343332,
		0x3736353433323130,
		0x3534333231303938,
		0x32313039383736
	};
	uint64_t mess2[9] = {
		0xe8f0f2e5e220e5d1,
		0xeee1e8f0f2d1202c,
		0xe8f6f3ede220e8e6,
		0x20faf2fee5e2202c,
		0xf120fff0eeec20f1,
		0x20e8ece0ebe5f0f2,
		0xf0e1e0f0f520e0ed,
		0xfbeafaebef20fffb,
		0xfbe2e5f0eee3c820,
	};

	print_stribog(mess, 63, 0);
	print_stribog(mess, 63, 1);
	print_stribog(mess2, 72, 0);
	print_stribog(mess2, 72, 1);
	return 0;
}
