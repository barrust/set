#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #include <openssl/md5.h>

#include "minunit.h"
#include "../src/set.h"


// the basic set to use!
SimpleSet s;

// static int calculate_md5sum(const char* filename, char* digest);


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
}

MU_TEST(test_alt_setup) {
    SimpleSet t;
    set_init_alt(&t, 500, NULL);  /* use default hash */
    mu_assert_int_eq(500, t.number_nodes);  /* 1024 is the INITIAL_NUM_ELEMENTS definition! */
    mu_assert_int_eq(0, t.used_nodes);
    set_destroy(&t);
}


MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    /* setup */
    MU_RUN_TEST(test_default_setup);
    MU_RUN_TEST(test_alt_setup);

}


int main() {
    // we want to ignore stderr print statements
    freopen("/dev/null", "w", stderr);

    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}


/* private functions */
// static int calculate_md5sum(const char* filename, char* digest) {
//     FILE *file_ptr;
//     file_ptr = fopen(filename, "r");
//     if (file_ptr == NULL) {
//         perror("Error opening file");
//         fflush(stdout);
//         return 1;
//     }
//
//     int n;
//     MD5_CTX c;
//     char buf[512];
//     ssize_t bytes;
//     unsigned char out[MD5_DIGEST_LENGTH];
//
//     MD5_Init(&c);
//     do {
//         bytes = fread(buf, 1, 512, file_ptr);
//         MD5_Update(&c, buf, bytes);
//     } while(bytes > 0);
//
//     MD5_Final(out, &c);
//
//     for (n = 0; n < MD5_DIGEST_LENGTH; n++) {
//         char hex[3] = {0};
//         sprintf(hex, "%02x", out[n]);
//         digest[n*2] = hex[0];
//         digest[n*2+1] = hex[1];
//     }
//
//     fclose(file_ptr);
//
//     return 0;
// }
