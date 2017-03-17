#include "stribog.h"
#include <stdio.h>

int main()
{
	FILE *f = fopen("stribog_fast.c", "w");
	size_t i, j, k;
	uint64_t t;
	const char *fmt;

	fputs("#include \"stribog_fast.h\"\n", f);
	fputs("#include <stdint.h>\n\n", f);
#if 1
	fputs("uint64_t lbox_fast[8][256] = {\n", f);

	for (i = 0; i < 8; ++i) {
		fputs("\t{\n", f);
		for (j = 0; j < 256; ++j) {
			t = 0; 
			for (k = 0; k < 8; ++k)
				if (sbox[j] & (1 << k))
					t ^= lbox[63 - (i * 8 + k)];
			if (j % 8 == 0)
				fmt = "\t\t0x%016llx, ";
			else if (j % 8 == 7)
				fmt = "0x%016llx,\n";
			else
				fmt = "0x%016llx, ";
			fprintf(f, fmt, t);
		}
		fputs("\t},\n", f);
	}
#else
	fputs("uint64_t lbox_fast[4][65536] = {\n", f);

	for (i = 0; i < 4; ++i) {
		fputs("\t{\n", f);
		for (j = 0; j < 65536; ++j) {
			t = 0; 
			for (k = 0; k < 16; ++k)
				if (j & (1 << k))
					t ^= lbox[63 - (i * 16 + k)];
			if (j % 8 == 0)
				fmt = "\t\t0x%016llx, ";
			else if (j % 8 == 7)
				fmt = "0x%016llx,\n";
			else
				fmt = "0x%016llx, ";
			fprintf(f, fmt, t);
		}
		fputs("\t},\n", f);
	}
#endif

	fputs("};\n", f);
	fclose(f);
	return 0;
}
