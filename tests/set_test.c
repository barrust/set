
#include "../set.h"

#include <stdio.h>

#define KEY_LEN 25

void check_string(SimpleSet *set, char *key) {
    if (set_contains(set, key) == SET_TRUE) {
        printf("Set contains [%s]!\n", key);
    } else {
        printf("Set does not contains [%s]!\n", key);
    }
}

void missing_key_check(SimpleSet *set, char *key) {
    if (set_contains(set, key) != SET_TRUE) {
        printf("Missing key! [%s]\n", key);
    }
}

int main(int argc, char **argv) {
    SimpleSet set;
    set_init(&set);
    set_add(&set, "test");
    int i;
    for (i = 0; i < 50000000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        int res = set_add(&set, key);
        if (res != SET_TRUE) {
            printf("Error inserting: %s\tres: %d\n", key, res);
        }
    }
    check_string(&set, "foo");
    check_string(&set, "test");
    check_string(&set, "bar");

    printf("Remove [test]....\n");
    set_remove(&set, "test");
    check_string(&set, "test");

    printf("Test inclusion of other elements...\n");
    for (i = 0; i < 50000000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        missing_key_check(&set, key);
    }
    printf("Completed testing other elements in set\n");

    printf("Test clearing set...\n");
    printf("Total Elements (pre): %" PRIu64 "\n", set.used_nodes);
    printf("Total Nodes (pre): %" PRIu64 "\n", set.number_nodes);
    set_clear(&set);
    printf("Total Elements (post): %" PRIu64 "\n", set.used_nodes);
    printf("Total Nodes (post): %" PRIu64 "\n", set.number_nodes);
    set_destroy(&set);


    return 0;
}
