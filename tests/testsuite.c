#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "minunit.h"
#include "../src/set.h"


/* private function(s) */
static int key_in_array(char** arr, int len, const char* key);

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
*   Test set to array
*******************************************************************************/
MU_TEST(test_set_to_array) {
    for (int i = 0; i < 3000; ++i) {
        char key[5] = {0};
        sprintf(key, "%d", i);
        set_add(&s, key);
    }

    uint64_t size;
    char** a = set_to_array(&s, &size);

    mu_assert_int_eq(3000, size);

    int v = 0;
    for (int i = 0; i < 3000; ++i) {
        char key[5] = {0};
        sprintf(key, "%d", i);
        v += key_in_array(a, 3000, key);
    }
    mu_assert_int_eq(0, v);

    /* free the memory */
    for (uint64_t i = 0; i < size; ++i) {
        free(a[i]);
    }
    free(a);
}

/*******************************************************************************
*   Test set compare
*******************************************************************************/
MU_TEST(test_set_cmp) {
    SimpleSet a, b;
    set_init(&a);
    set_init(&b);

    // set them up with things!
    for (int i = 0; i < 3000; ++i) {
        char key[5] = {0};
        sprintf(key, "%d", i);
        set_add(&a, key);
        set_add(&b, key);
    }

    // currently the sets should be equal (same number elements and values)
    mu_assert_int_eq(SET_EQUAL, set_cmp(&a, &b));
    mu_assert_int_eq(SET_EQUAL, set_cmp(&b, &a));

    // make a smaller!
    set_remove(&a, "0");

    mu_assert_int_eq(SET_LEFT_GREATER, set_cmp(&b, &a)); // a has more than b
    mu_assert_int_eq(SET_RIGHT_GREATER, set_cmp(&a, &b)); // a has more than b

    // make each have same number of elements but have at least 1 thing different
    set_add(&a, "test");
    mu_assert_int_eq(SET_UNEQUAL, set_cmp(&a, &b));

    set_destroy(&a);
    set_destroy(&b);
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

    /* set to array */
    MU_RUN_TEST(test_set_to_array);

    /* set compare */
    MU_RUN_TEST(test_set_cmp);
}


int main() {
    // we want to ignore stderr print statements
    freopen("/dev/null", "w", stderr);

    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}


/* private function(s) */
static int key_in_array(char** arr, int len, const char* key) {
    for (int i = 0; i < len; ++i) {
        if (strcmp(arr[i], key) == 0)
            return 0;
    }
    return 1;
}
