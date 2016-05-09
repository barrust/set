all: clean
	gcc set.c ./tests/set_test.c -o ./dist/s -g
	gcc set.c ./tests/set_union_test.c -o ./dist/su -g
	gcc set.c ./tests/set_intersection_test.c -o ./dist/si -g
	gcc set.c ./tests/set_sub_super_test.c -o ./dist/sst -g
	gcc set.c ./tests/set_differences_test.c -o ./dist/sd -g
clean:
	if [ ! -d ./dist/ ]; then mkdir ./dist/; fi;
	-rm ./dist/*
