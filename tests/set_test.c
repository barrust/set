
#include "../src/set.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define KEY_LEN 25

void check_string(SimpleSet *set, char *key) {
    if (set_contains(set, key) == SET_TRUE) {
        printf("Set contains [%s]!\n", key);
    } else {
        printf("Set does not contains [%s]!\n", key);
    }
}

void initialize_set(SimpleSet *set, int start, int elements, int itter, int TEST) {
    int i;
    for (i = start; i < elements; i+=itter) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%d", i);
        int res = set_add(set, key);
        if (res != TEST) {
            printf("Error: %s\tres: %d\n", key, res);
        }
    }
}

int main() {
    int added_elements, res, inaccuraces = 0;
    uint64_t i, elements = 50000;
    uint64_t ui;

    SimpleSet A, B, C;
    /* Initialize Set A to 1/2 the elements in A */
    printf("==== Set A Initialization with %" PRIu64 " Elements ====\n", elements);
    set_init(&A);
    initialize_set(&A, 0, elements, 1, SET_TRUE);
    assert(A.used_nodes == elements);

    char** keys = set_to_array(&A, &ui);
    assert(A.used_nodes == ui);  // in multi-thread, don't want to assume it hasn't changed
    printf("==== Set A has %" PRIu64 " Keys ====\n", ui);
    for (i = 0; i < 15; i++) {
        printf("%" PRIu64 "\t%s\n", i, keys[i]);
    }
    // free the keys memory
    for (i = 0; i < ui; i++) {
        free(keys[i]);
    }
    free(keys);

    /* Initialize Set B to 1/2 the elements in A */
    printf("==== Set B Initialization with %" PRIu64 " Elements ====\n", elements / 2);
    set_init(&B);
    initialize_set(&B, 0, elements / 2, 1, SET_TRUE);
    assert(B.used_nodes == elements / 2);

    /* Test the length of the set */
    printf("==== Set B Length Test ====\n");
    assert(set_length(&B) == elements / 2);
    printf("Set length test: OK!\n");

    /*  Test that inserting the same element into the set does not increase the
        number of nodes. */
    printf("==== Test Double Insertion ===\n");
    added_elements = A.used_nodes;
    initialize_set(&A, 0, elements / 2, 1, SET_ALREADY_PRESENT);
    assert(added_elements = A.used_nodes);
    printf("Double Insetion check: OK!\n");

    /*  Test the contains functions for keys that exist and keys that are not in
        the set */
    printf("\n\n==== Test Set Contains ====\n");
    printf("The symbol ∈ indicates set membership and means “is an element of” so that the statement x ∈ A means that x is an element of the set A. In other words, x is one of the objects in the collection of (possibly many) objects in the set A.\n");

    for (i = 0; i < elements; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&A, key) != SET_TRUE) {
            printf("Missing Key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Missing keyes check: OK!\n");

    inaccuraces = 0;
    for (i = elements; i < elements * 2; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i * -1);
        if (set_contains(&A, key) == SET_TRUE) {
            printf("Non-present key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Non-present keys check: OK!\n");

    /*  Test removing elements from the set */
    printf("\n\n==== Test Set Remove ====\n");
    for (i = elements / 2; i < elements; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_remove(&A, key) != SET_TRUE) {
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    assert(A.used_nodes == elements / 2);

    inaccuraces = 0;
    for (i = 0; i < elements; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (i >= elements / 2) {
            if (set_contains(&A, key) == SET_TRUE) {
                printf("Additional Key: [%s]\n", key);
                inaccuraces++;
            }
        } else {
            if (set_contains(&A, key) != SET_TRUE) {
                printf("Missing Key: [%s]\n", key);
                inaccuraces++;
            }
        }
    }
    assert(inaccuraces == 0);

    printf("Remove keys check: OK!\n");

    /* Test clearing a set and then reset to its original value */
    printf("\n\n==== Reset A ====\n");
    set_clear(&A);
    inaccuraces = 0;
    for(ui=0; ui < A.number_nodes; ui++) {
        if (A.nodes[ui] != NULL) {
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    assert(A.used_nodes == 0);
    initialize_set(&A, 0, elements, 1, SET_TRUE);
    assert(A.used_nodes == elements);
    printf("Reset A: OK!\n");

    /* Test subset functionality. A is a superset of B. B is a subset of A */
    printf("\n\n==== Test Set Subset ====\n");
    printf("A set A is a subset of another set B if all elements of the set A are elements of the set B. In other words, the set A is contained inside the set B. The subset relationship is denoted as A ⊆ B.\n");
    res = set_is_subset(&A, &B);
    printf("A ⊆ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_FALSE);
    res = set_is_subset(&B, &A);
    printf("B ⊆ A: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_TRUE);
    res = set_is_subset(&A, &A);
    printf("A ⊆ A: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_TRUE);
    res = set_is_subset(&B, &B);
    printf("B ⊆ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_TRUE);

    /*  Test strict subset functionality. A is a superset of B. B is a subset
        of A */
    printf("\n\n==== Test Set Strict Subset ====\n");
    printf("Set A is a strict subset of another set B if all elements of the set A are elements of the set B. In other words, the set A is contained inside the set B. A ≠ B is required. The strict subset relationship is denoted as A ⊂ B.\n");
    res = set_is_subset_strict(&A, &B);
    printf("A ⊂ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_FALSE);
    res = set_is_subset_strict(&B, &A);
    printf("B ⊂ A: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_TRUE);
    res = set_is_subset_strict(&A, &A);
    printf("A ⊂ A: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_FALSE);
    res = set_is_subset_strict(&B, &B);
    printf("B ⊂ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_FALSE);

    /* Test superset functionality. A is a superset of B. B is a subset of A */
    printf("\n\n==== Test Set Superset ====\n");
    printf("Superset Definition: A set A is a superset of another set B if all elements of the set B are elements of the set A. The superset relationship is denoted as A ⊇ B.\n");
    res = set_is_superset(&A, &B);
    printf("A ⊇ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_TRUE);
    res = set_is_superset(&B, &A);
    printf("B ⊇ A: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_FALSE);
    res = set_is_superset(&A, &A);
    printf("A ⊇ A: %s\n", res  == 0 ? "yes" : "no");
    assert(res == SET_TRUE);
    res = set_is_superset(&B, &B);
    printf("B ⊇ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_TRUE);

    /*  Test strict superset functionality. A is a superset of B. B is a
        subset of A */
    printf("\n\n==== Test Set Strict Superset ====\n");
    printf("Strict Superset Definition: A set A is a superset of another set B if all elements of the set B are elements of the set A. A ≠ B is required. The superset relationship is denoted as A ⊃ B.\n");
    res = set_is_superset_strict(&A, &B);
    printf("A ⊃ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_TRUE);
    res = set_is_superset_strict(&B, &A);
    printf("B ⊃ A: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_FALSE);
    res = set_is_superset_strict(&A, &A);
    printf("A ⊃ A: %s\n", res  == 0 ? "yes" : "no");
    assert(res == SET_FALSE);
    res = set_is_superset_strict(&B, &B);
    printf("B ⊃ B: %s\n", res == 0 ? "yes" : "no");
    assert(res == SET_FALSE);

    /* Test Set Intersection functionality. */
    printf("\n\n==== Test Set Intersection ====\n");
    printf("The intersection of a set A with a B is the set of elements that are in both set A and B. The intersection is denoted as A ∩ B.\n");
    set_destroy(&A);
    set_destroy(&B);
    set_init(&A);
    set_init(&B);
    set_init(&C);
    assert(A.used_nodes == 0);
    assert(B.used_nodes == 0);
    assert(C.used_nodes == 0);

    initialize_set(&A, 0, elements, 1, SET_TRUE);
    initialize_set(&B, elements / 2, elements * 2, 1, SET_TRUE);
    set_intersection(&C, &A, &B);

    assert(C.used_nodes == (elements / 2));

    inaccuraces = 0;
    for (i = 0; i < elements / 2;  i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) == SET_TRUE) {
            printf("Non-present key: [%s]\n", key);
            inaccuraces++;
        }
    }
    for (i = elements + 1; i < elements * 2;  i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) == SET_TRUE) {
            printf("Non-present key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Non-present keys check: OK!\n");

    inaccuraces = 0;
    for (i = elements / 2; i < elements; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) != SET_TRUE) {
            printf("Missing Key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Missing keys check: OK!\n");


    /*  Test Set Difference: A ∖ B where C contains only those elements from A
        that are not in B. */
    printf("\n\n==== Test Set Difference ====\n");
    printf("The set difference between two sets A and B is written A ∖ B, and means the set that consists of the elements of A which are not elements of B: x ∈ A ∖ B ⟺ x ∈ A ∧ x ∉ B. Another frequently seen notation for S ∖ T is S − T.\n");
    set_destroy(&C);
    set_init(&C);
    set_difference(&C, &A, &B);
    assert(C.used_nodes == (elements / 2));

    inaccuraces = 0;
    for (i = 0; i < elements / 2; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) != SET_TRUE) {
            printf("Missing Key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Missing keys check: OK!\n");

    inaccuraces = 0;
    for (i = elements + 1; i < elements * 2;  i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) == SET_TRUE) {
            printf("Non-present key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Non-present keys check: OK!\n");


    /*  Test Symmetric Difference: C = A △ B is where C contains only elements
        in A not in B and elements from B that are not in A. */
    printf("\n\n==== Test Set Symmetric Difference ====\n");
    printf("The symmetric difference of two sets A and B is the set of elements either in A or in B but not in both. Symmetric difference is denoted A △ B or A * B.\n");
    set_destroy(&C);
    set_init(&C);
    set_symmetric_difference(&C, &A, &B);
    assert(C.used_nodes == ((elements * 2) - (elements / 2)));

    printf("Completed the set_symmetric_difference\n");
    inaccuraces = 0;
    for (i = 0; i < elements / 2; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) != SET_TRUE) {
            printf("Missing Key: [%s]\n", key);
            inaccuraces++;
        }
    }
    for (i = elements; i < elements * 2; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) != SET_TRUE) {
            printf("Missing Key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Missing keys check: OK!\n");

    inaccuraces = 0;
    for (i = elements / 2 + 1; i < elements;  i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) == SET_TRUE) {
            printf("Non-present key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Non-present keys check: OK!\n");


    /*  Test Set Union: C = A ∪ B denotes that all elements from A and B are
        part of C */
    printf("\n\n==== Test Set Union ====\n");
    printf("The union of a set A with a B is the set of elements that are in either set A or B. The union is denoted as A ∪ B.\n");
    set_destroy(&C);
    set_init(&C);
    set_union(&C, &A, &B);
    assert(C.used_nodes == elements * 2);

    for (i = elements; i < elements * 2; i++) {
        char key[KEY_LEN] = {0};
        sprintf(key, "%" PRIu64, i);
        if (set_contains(&C, key) != SET_TRUE) {
            printf("Missing Key: [%s]\n", key);
            inaccuraces++;
        }
    }
    assert(inaccuraces == 0);
    printf("Missing keys check: OK!\n");

    printf("\n\n==== Clean Up Memory ====\n");
    set_destroy(&A);
    set_destroy(&B);
    set_destroy(&C);
    printf("\n\n==== Completed tests! ====\n");

    return 0;
}
