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
* Set Compare
* Print statistics about the set
* Make thread safe when compiled with **OpenMP**


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

## Required Compile Flags:
   None
