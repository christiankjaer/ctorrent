CC=gcc
CLIBS=-lcurl
CFLAGS= -Wall $(CLIBS) -g
DEPS=bencoding.h

all: ctorrent

ctorrent:
	$(CC) $(CFLAGS) ctorrent.c bencoding.c -o ctorrent

.PHONY: clean

clean:
	rm -f ctorrent
