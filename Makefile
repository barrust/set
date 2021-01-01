CC=gcc
COMPFLAGS=-Wall -Wpedantic -Winline -Wextra -Wno-long-long
SRCDIR=src
DISTDIR=dist
TESTDIR=tests


all: set
	$(CC) ./$(DISTDIR)/set.o $(CFLAGS) $(COMPFLAGS) ./$(TESTDIR)/set_test.c -o ./$(DISTDIR)/s

debug: COMPFLAGS += -g
debug: all

release: COMPFLAGS += -O3
release: all

sanitize: COMPFLAGS += -fsanitize=undefined
sanitize: test

test: COMPFLAGS += --coverage
test: set
	$(CC) $(DISTDIR)/set.o $(TESTDIR)/testsuite.c $(CCFLAGS) $(COMPFLAGS) -lcrypto -o ./$(DISTDIR)/test -g

set:
	$(CC) -c ./$(SRCDIR)/set.c -o ./$(DISTDIR)/set.o $(COMPFLAGS) $(CFLAGS)

runtests:
	@ if [ -f "./$(DISTDIR)/test" ]; then ./$(DISTDIR)/test; fi

clean:
	if [ -f "./$(DISTDIR)/set.o" ]; then rm -r ./$(DISTDIR)/set.o; fi
	if [ -f "./$(DISTDIR)/s" ]; then rm -r ./$(DISTDIR)/s; fi
	if [ -f "./$(DISTDIR)/test" ]; then rm -rf ./$(DISTDIR)/*.gcno; fi
	if [ -f "./$(DISTDIR)/test" ]; then rm -rf ./$(DISTDIR)/*.gcda; fi
	if [ -f "./$(DISTDIR)/test" ]; then rm -r ./$(DISTDIR)/test; fi
	rm -f ./*.gcno
	rm -f ./*.gcda
	rm -f ./*.gcov
