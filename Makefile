CC=gcc
CFLAGS= -Wall -Wpedantic -Wextra -O3
SRCDIR=src
DISTDIR=dist
TESTDIR=tests


all: set
	$(CC) ./$(DISTDIR)/set.o $(CFLAGS) ./$(TESTDIR)/set_test.c -o ./$(DISTDIR)/s

set:
	$(CC) -c ./$(SRCDIR)/set.c -o ./$(DISTDIR)/set.o $(CFLAGS)

clean:
	if [ -f "./$(DISTDIR)/set.o" ]; then rm -r ./$(DISTDIR)/set.o; fi
	if [ -f "./$(DISTDIR)/s" ]; then rm -r ./$(DISTDIR)/s; fi
