CFLAGS = -c -g -Wall -Wextra -pedantic -O3
LDFLAGS = -g

all: test stribog

test: stribog.o test.o stribog_asm.o stribog_data.o stribog_fast.o
	gcc $^ -o $@ $(LDFLAGS)

stribog: stribog.o stribog_asm.o stribog_data.o main.o stribog_fast.o
	gcc $^ -o $@ $(LDFLAGS)

stribog_fast.o: stribog_fast.c
	gcc $< $(CFLAGS)

stribog_fast.c: gen_opt
	./gen_opt

gen_opt: gen_opt.o stribog_data.o
	gcc $^ -o $@ $(LDFLAGS)

gen_opt.o: gen_opt.c
	gcc $< $(CFLAGS)

main.o: main.c
	gcc $< $(CFLAGS)

stribog.o: stribog.c stribog.h
	gcc $< $(CFLAGS)

stribog_asm.o: stribog_asm.S
	gcc $< $(CFLAGS)

test.o: test.c stribog.h
	gcc $< $(CFLAGS)

stribog_data.o: stribog_data.c stribog.h
	gcc $< $(CFLAGS)

clean:
	rm -f *.o stribog test gen_opt stribog_fast.c
