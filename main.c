#include <stdio.h>
#include "stribog.h"

int main(int argc, char **argv)
{
	char buf[4096];
	uint8_t res512[64], res256[32];
	struct stribog_ctx ctx512, ctx256;
	FILE *f;
	size_t r, i;

	if (argc != 2)
		return 1;
	f = fopen(argv[1], "r");
	if (!f)
		return 1;

	stribog_init(&ctx512, 0);
	//stribog_init(&ctx256, 1);
	while ((r = fread(buf, 1, sizeof(buf), f)) > 0) {
		stribog_add_data(&ctx512, buf, r);
		//stribog_add_data(&ctx256, buf, r);
	}
	stribog_close(&ctx512, res512);
	//stribog_close(&ctx256, res256);

	for (i = 63; i < 64; i--)
		printf("%02x", res512[i]);
	printf("\n");

	/*for (i = 31; i < 32; i--)
		printf("%02x", res256[i]);
	printf("\n");*/

	return 0;
}
