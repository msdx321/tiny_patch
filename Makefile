CC=gcc
CFLAGS=-std=gnu99 -Wall -Werror -pedantic -O3

tiny_patch: tiny_patch.c 
	$(CC) $(CFLAGS) -o tiny_patch tiny_patch.c

clean:
	rm -rf *.exe
	rm -rf tiny_patch