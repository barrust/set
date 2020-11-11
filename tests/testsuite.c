#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "minunit.h"
#include "../src/set.h"


// the basic set to use!
SimpleSet s;


void test_setup(void) {
    set_init(&s);
}

void test_teardown(void) {
    set_destroy(&s);
}


/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    mu_assert_int_eq(1024, s.number_nodes);  /* 1024 is the INITIAL_NUM_ELEMENTS definition! */
    mu_assert_int_eq(0, s.used_nodes);
    mu_assert_int_eq(0, set_length(&s));
}

MU_TEST(test_alt_setup) {
    SimpleSet t;
    set_init_alt(&t, 500, NULL);  /* use default hash */
    mu_assert_int_eq(500, t.number_nodes);  /* 1024 is the INITIAL_NUM_ELEMENTS definition! */
    mu_assert_int_eq(0, t.used_nodes);
    set_destroy(&t);
}


/*******************************************************************************
*   Test add, contains, remove elements
*******************************************************************************/
MU_TEST(test_add_key) {
    mu_assert_int_eq(SET_TRUE, set_add(&s,"test"));
    mu_assert_int_eq(1, set_length(&s));
}

MU_TEST(test_add_key_twice) {
    mu_assert_int_eq(SET_TRUE, set_add(&s, "test"));
    mu_assert_int_eq(SET_ALREADY_PRESENT, set_add(&s, "test"));
    mu_assert_int_eq(1, set_length(&s));
}

MU_TEST(test_remove_key) {
    mu_assert_int_eq(SET_TRUE, set_add(&s, "test"));
    mu_assert_int_eq(1, set_length(&s));

    mu_assert_int_eq(SET_TRUE, set_remove(&s, "test"));
    mu_assert_int_eq(0, set_length(&s));
    mu_assert_int_eq(SET_FALSE, set_remove(&s, "test"));  // no longer present, return false!
}

MU_TEST(test_set_contains) {
    mu_assert_int_eq(SET_TRUE, set_add(&s, "test"));
    mu_assert_int_eq(1, set_length(&s));

    mu_assert_int_eq(SET_TRUE, set_contains(&s, "test"));
    mu_assert_int_eq(SET_FALSE, set_contains(&s, "blah"));
}


MU_TEST(test_grow_set) {
    for (int i = 0; i < 3000; ++i) {
        char key[5] = {0};
        sprintf(key, "%d", i);
        set_add(&s, key);
    }
    mu_assert_int_eq(3000, set_length(&s));
    mu_assert_int_eq(16384, s.number_nodes);  /* grew 4x */
}



/*******************************************************************************
*   Testsuite
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    /* setup */
    MU_RUN_TEST(test_default_setup);
    MU_RUN_TEST(test_alt_setup);

    /* add & remove */
    MU_RUN_TEST(test_add_key);
    MU_RUN_TEST(test_add_key_twice);
    MU_RUN_TEST(test_remove_key);
    MU_RUN_TEST(test_set_contains);
    MU_RUN_TEST(test_grow_set);
}


int main() {
    // we want to ignore stderr print statements
    freopen("/dev/null", "w", stderr);

    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
