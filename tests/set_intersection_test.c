#include "../set.h"

#include <stdio.h>

#define KEY_LEN 25

int main(int argc, char **argv) {
    SimpleSet s1, s2, inter;
    set_init(&s1);
    set_init(&s2);
    set_init(&inter);

    int i, num_missing = 0;

    // fill in the first set with some elements
    for (i = 0; i < 50000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        set_add(&s1, key);
    }
    // fill in the second set with some other elements (some overlap!)
    for (i = 0; i < 100000; i += 2) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        set_add(&s2, key);
    }
    set_intersection(&inter, &s1, &s2);
    printf("Intersect Information: \n");
    printf("Total Elements: %" PRIu64 "\n", inter.used_nodes);
    for (i = 0; i < 50000; i += 2) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        if (set_contains(&inter, key) != SET_TRUE) {
            printf("Element missing: %d\n", i);
            num_missing++;
        }
    }
    printf("Number Missing Elements: %d\n", num_missing);

    printf("Cleaning up the sets\n");
    set_destroy(&s1);
    set_destroy(&s2);
    set_destroy(&inter);

}
