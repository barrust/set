
#include "set.h"

#include <stdio.h>

#define KEY_LEN 25

int main(int argc, char **argv) {
    SimpleSet set;
    set_init(&set);
    set_add(&set, "test");
    int i;
    for (i = 0; i < 50000; i++) {
        char key[KEY_LEN] = {0};
		sprintf(key, "%d", i);
        set_add(&set, key);
    }

    printf("Set contains [%s]: %d\n", "foo", set_contains(&set, "foo"));
    printf("Set contains [%s]: %d\n", "test", set_contains(&set, "test"));
    printf("Set contains [%s]: %d\n", "bar", set_contains(&set, "bar"));

    set_destroy(&set);


    return 0;
}
