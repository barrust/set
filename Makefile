all: clean
	gcc set.c ./tests/set_test.c -o ./dist/s -g
	gcc set.c ./tests/set_union_test.c -o ./dist/su -g
clean:
	if [ -e ./dist/s ]; then rm ./dist/s; fi;
	if [ -e ./dist/su ]; then rm ./dist/su; fi;
