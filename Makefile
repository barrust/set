all: clean
	gcc set.c set_test.c -o ./dist/set -g
clean:
	if [ -e ./dist/set ]; then rm ./dist/set; fi;
