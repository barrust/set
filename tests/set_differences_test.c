#include "../set.h"

#include <stdio.h>

#define KEY_LEN 25

int main(int argc, char **argv) {
    SimpleSet s1, s2, diff;
    set_init(&s1);
    set_init(&s2);
    set_init(&diff);

    int i, num_missing = 0;

    // fill in the first set with some elements
    for (i = 0; i < 5000000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        set_add(&s1, key);
    }
    for (i = 0; i < 5000000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        if (set_contains(&s1, key) == SET_FALSE)
            printf("s1 does not contain [%s]\n", key);
    }
    // fill in the second set with some other elements (some overlap!)
    for (i = 2500000; i < 10000000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        int res = set_add(&s2, key);
    }
    for (i = 2500000; i < 10000000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        if (set_contains(&s2, key) == SET_FALSE)
            printf("s2 does not contain [%s]\n", key);
    }

    set_difference(&diff, &s1, &s2);
    printf("Difference Information: \n");
    printf("Total Elements: %" PRIu64 "\n", diff.used_nodes);
    printf("diff nodes: %" PRIu64 "\n", diff.number_nodes);
    for (i = 0; i < 25000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        if (set_contains(&diff, key) != SET_TRUE) {
            printf("Element missing: %d\n", i);
            num_missing++;
        }
    }

    printf("Number Missing Elements: %d\n", num_missing);

    printf("Cleaning up the sets\n");
    set_destroy(&s1);
    set_destroy(&s2);
    set_destroy(&diff);

}
