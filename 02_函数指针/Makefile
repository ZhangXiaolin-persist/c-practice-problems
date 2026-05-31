CC = clang
CFLAGS = -Wall -Wextra -std=c11

.PHONY: test

test: FuncPtr.c FuncPtr.h test.c
	$(CC) $(CFLAGS) -o test FuncPtr.c test.c
	./test

clean:
	rm -f test
