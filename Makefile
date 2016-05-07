all: clean
	gcc set.c ./tests/set_test.c -o ./dist/s -g
	gcc set.c ./tests/set_union_test.c -o ./dist/su -g
	gcc set.c ./tests/set_intersection_test.c -o ./dist/si -g
	gcc set.c ./tests/set_sub_super_test.c -o ./dist/sst -g
clean:
	if [ ! -d ./dist/ ]; then mkdir ./dist/; fi;
	if [ -e ./dist/s ]; then rm ./dist/s; fi;
	if [ -e ./dist/su ]; then rm ./dist/su; fi;
	if [ -e ./dist/si ]; then rm ./dist/si; fi;
	if [ -e ./dist/sst ]; then rm ./dist/sst; fi;
