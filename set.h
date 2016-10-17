/*******************************************************************************
***
***     Author: Tyler Barrus
***     email:  barrust@gmail.com
***
***     Version: 0.1.4
***     Purpose: Simple, yet effective, set implementation
***
***     License: MIT 2016
***
***     URL: https://github.com/barrust/set
***
***     Usage:
***
***
***    Required Compile Flags:
***
*******************************************************************************/

#ifndef __SIMPLE_SET_H__
#define __SIMPLE_SET_H__

#include <inttypes.h>       /* uint64_t */

typedef uint64_t (*HashFunction) (char *key);

typedef struct  {
    char* _key;
    uint64_t _hash;
} SimpleSetNode, simple_set_node;

typedef struct  {
    simple_set_node **nodes;
    uint64_t number_nodes;
    uint64_t used_nodes;
    HashFunction hash_function;
} SimpleSet, simple_set;


/*
    Initialize the set with default values
*/
int set_init(SimpleSet *set);

/*
    Initialize the set with a different hash function
*/
int set_init_alt(SimpleSet *set, HashFunction hash);

/*
    Utility function to clear out the set
*/
int set_clear(SimpleSet *set);

/*
    Free memory
*/
int set_destroy(SimpleSet *set);

/*
    Add element to set, returns SET_TRUE if added, SET_FALSE if already
    present, SET_ALREADY_PRESENT, or SET_CIRCULAR_ERROR if set is
    completely full
*/
int set_add(SimpleSet *set, char *key);

/*
    Remove element from the set; Returns SET_TRUE if removed, SET_FALSE if
    not present
*/
int set_remove(SimpleSet *set, char *key);

/*
    Check if key in hash; Returns SET_TRUE if present, SET_FALSE if not
    found, or SET_CIRCULAR_ERROR if set is full and not found
*/
int set_contains(SimpleSet *set, char *key);

/*
    Set res to the union of s1 and s2
    res = s1 ∪ s2

    The union of a set A with a B is the set of elements that are in either
    set A or B. The union is denoted as A ∪ B
*/
int set_union(SimpleSet *res, SimpleSet *s1, SimpleSet *s2);

/*
    Set res to the intersection of s1 and s2
    res = s1 ∩ s2

    The intersection of a set A with a B is the set of elements that are in
    both set A and B. The intersection is denoted as A ∩ B
*/
int set_intersection(SimpleSet *res, SimpleSet *s1, SimpleSet *s2);

/*
    Set res to the difference between s1 and s2
    res = s1∖ s2

    The set difference between two sets A and B is written A ∖ B, and means
    the set that consists of the elements of A which are not elements
    of B: x ∈ A ∖ B ⟺ x ∈ A ∧ x ∉ B. Another frequently seen notation
    for S ∖ T is S − T.
*/
int set_difference(SimpleSet *res, SimpleSet *s1, SimpleSet *s2);

/*
    Set res to the symmetric difference between s1 and s2
    res = s1 △ s2

    The symmetric difference of two sets A and B is the set of elements either
    in A or in B but not in both. Symmetric difference is denoted
    A △ B or A * B
*/
int set_symmetric_difference(SimpleSet *res, SimpleSet *s1, SimpleSet *s2);

/*
    Return SET_TRUE if test is fully contained in s2; returns SET_FALSE
    otherwise
    test ⊆ against

    A set A is a subset of another set B if all elements of the set A are
    elements of the set B. In other words, the set A is contained inside
    the set B. The subset relationship is denoted as A ⊆ B
*/
int set_is_subset(SimpleSet *test, SimpleSet *against);

/*
    Inverse of subset; return SET_TRUE if set test fully contains
    (including equal to) set against; return SET_FALSE otherwise
    test ⊇ against

    Superset Definition: A set A is a superset of another set B if all
    elements of the set B are elements of the set A. The superset
    relationship is denoted as A ⊇ B
*/
int set_is_superset(SimpleSet *test, SimpleSet *against);

/*
    Strict subset ensures that the test is a subset of against, but that
    the two are also not equal.
    test ⊂ against

    Set A is a strict subset of another set B if all elements of the set A
    are elements of the set B. In other words, the set A is contained inside
    the set B. A ≠ B is required. The strict subset relationship is denoted
    as A ⊂ B
*/
int set_is_subset_strict(SimpleSet *test, SimpleSet *against);

/*
    Strict superset ensures that the test is a superset of against, but that
    the two are also not equal.
    test ⊃ against

    Strict Superset Definition: A set A is a superset of another set B if
    all elements of the set B are elements of the set A. A ≠ B is required.
    The superset relationship is denoted as A ⊃ B
*/
int set_is_superset_strict(SimpleSet *test, SimpleSet *against);

// int set_cmp(SimpleSet *test, SimpleSet, *against);                         /* TODO: implement */
// char** set_to_array(SimpleSet *set);                                       /* TODO: implement */
// void set_printf(SimpleSet *set);                                           /* TODO: implement */

#define SET_TRUE 0
#define SET_FALSE -1
#define SET_MALLOC_ERROR -2
#define SET_CIRCULAR_ERROR -3
#define SET_OCCUPIED_ERROR -4
#define SET_ALREADY_PRESENT 1


#endif /* END SIMPLE SET HEADER */
