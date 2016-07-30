CFLAGS= -Wall -Wextra

all: clean
	gcc set.c $(CFLAGS) ./tests/set_test.c -o ./dist/s
clean:
	if [ ! -d ./dist/ ]; then mkdir ./dist/; fi;
	-rm ./dist/*
