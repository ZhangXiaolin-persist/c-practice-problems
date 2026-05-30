CC = clang
CFLAGS = -Wall -Wextra -std=c11

.PHONY: test

test: BitOps.c BitOps.h test.c
	$(CC) $(CFLAGS) -o test BitOps.c test.c
	./test

clean:
	rm -f test
