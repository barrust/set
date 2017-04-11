CC=gcc
CFLAGS= -Wall -Wpedantic -Wextra
SRCDIR=src
DISTDIR=dist
TESTDIR=tests


all: clean set
	$(CC) ./$(DISTDIR)/set.o $(CFLAGS) ./$(TESTDIR)/set_test.c -o ./$(DISTDIR)/s

set:
	$(CC) -c ./$(SRCDIR)/set.c -o ./$(DISTDIR)/set.o $(CFLAGS)

clean:
	rm -rf ./$(DISTDIR)/*
