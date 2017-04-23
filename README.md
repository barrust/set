# set
Implementation of sets in C

Sets allow for quick checks for inclusion and exclusion

This implementation provides a simple and generally quick method to get set functionality into a C program quickly. It was developed to provide a basis for testing and benchmarking performance along with providing a purposeful, low overhead library. Currently only supports strings.

## License
MIT 2016

# Main Features

* Union, intersection, difference, and semantic difference
* Standard and Strict subset and superset checks
* Simple method to change the hashing function if desired
* Add, check, and remove elements in a the set

## Future Enhancements

* In place union - add to an already created Set
* Print statistics about the set


## Usage:
``` c
#include "set.h"
#include <stdio.h>

int main(int argc, char** argv) {
    SimpleSet set;
    set_init(&set);
    set_add(&set, "orange");
    set_add(&set, "blue");
    set_add(&set, "red");
    set_add(&set, "green");
    set_add(&set, "yellow");

    if (set_contains(&set, "yellow") == SET_TRUE) {
        printf("Set contains 'yellow'!\n");
    } else {
        printf("Set does not contains 'yellow'!\n");
    }

    if (set_contains(&set, "purple") == SET_TRUE) {
        printf("Set contains 'purple'!\n");
    } else {
        printf("Set does not contains 'purple'!\n");
    }

    set_destroy(&set);
}
```

## Thread safety

Due to the the overhead of enforcing thread safety, it is up to the user to
ensure that each thread has controlled access to the set. For **OpenMP** code,
the following should suffice.

``` c
#include "set.h"
#include <omp.h>

int main(int argc, char** argv) {
    SimpleSet set;
    set_init(&set);
    int i;
    #pragma parallel for private(i)
    for (i = 0; i < 500000; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%d", i);
        #pragma critical (set_lock)
        {
            set_add(&set, key);
        }
    }
    set_destroy(&set);
}
```

All but `set_contains` needs to be guarded against race conditions as the set
will grow as needed. Set comparison functions (union, intersect, etc.) should
be done on non-changing sets.

## Required Compile Flags:
   None
