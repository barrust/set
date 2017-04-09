CC=gcc
CFLAGS= -Wall -Wpedantic -Wextra
SRCDUR=src
DISTDIR=dist
TESTDIR=tests


all: clean set
	$(CC) ./$(DISTDIR)/set.o $(CFLAGS) ./$(TESTDIR)/set_test.c -o ./$(DISTDIR)/s

set:
	$(CC) -c ./$(SRCDUR)/set.c -o ./$(DISTDIR)/set.o $(CFLAGS)
clean:
	if [ ! -d ./dist/ ]; then mkdir ./dist/; fi;
	-rm ./dist/*
