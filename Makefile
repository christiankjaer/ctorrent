CC=gcc
CLIBS=-lcurl
CFLAGS= -Wall $(CLIBS)

all: ctorrent

ctorrent:
	$(CC) $(CFLAGS) ctorrent.c bencoding.c -o ctorrent

clean:
	rm ctorrent
