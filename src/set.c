/*******************************************************************************
***
***     Author: Tyler Barrus
***     email:  barrust@gmail.com
***
***     Version: 0.2.0
***
***     License: MIT 2016
***
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "set.h"

#define MAX_FULLNESS_RATIO 0.25       /* arbitrary */

// We always want to check for errors when dealing with private functions, so
// generate warnings if we don't on GCC-compatible compilers.
#ifdef __GNUC__
#define warn_unused_result __attribute__((warn_unused_result))
#else
#define warn_unused_result
#endif

/* PRIVATE FUNCTIONS */
static warn_unused_result uint64_t __default_hash(const char *key, key_size_tt len);
static warn_unused_result int __get_index(const SimpleSet *set, const char *key, key_size_tt len, uint64_t hash, uint64_t *index);
static warn_unused_result int __assign_node(SimpleSet *set, const char *key, key_size_tt len, uint64_t hash, uint64_t index);
static void __free_index(SimpleSet *set, uint64_t index);
static warn_unused_result int __set_contains(const SimpleSet *set, const char *key, key_size_tt len, uint64_t hash);
static warn_unused_result int __set_add(SimpleSet *set, const char *key, key_size_tt len, uint64_t hash);
static warn_unused_result int __relayout_nodes(SimpleSet *set, uint64_t start, short end_on_null);

/*******************************************************************************
***        FUNCTIONS DEFINITIONS
*******************************************************************************/

int set_init_alt(SimpleSet *set, uint64_t num_els, set_hash_function hash) {
    set->nodes = malloc(num_els * sizeof(simple_set_node*));
    if (set->nodes == NULL) {
        return SET_MALLOC_ERROR;
    }
    set->number_nodes = num_els;
    uint64_t i;
    for (i = 0; i < set->number_nodes; ++i) {
        set->nodes[i] = NULL;
    }
    set->used_nodes = 0;
    set->hash_function = (hash == NULL) ? &__default_hash : hash;
    return SET_TRUE;
}

int set_clear(SimpleSet *set) {
    uint64_t i;
    for(i = 0; i < set->number_nodes; ++i) {
        if (set->nodes[i] != NULL) {
            __free_index(set, i);
        }
    }
    set->used_nodes = 0;
    return SET_TRUE;
}

int set_destroy(SimpleSet *set) {
    set_clear(set);
    free(set->nodes);
    set->number_nodes = 0;
    set->used_nodes = 0;
    set->hash_function = NULL;
    return SET_TRUE;
}

int set_add(SimpleSet *set, const char *key, key_size_tt len) {
    uint64_t hash = set->hash_function(key, len);
    return __set_add(set, key, len, hash);
}

int set_add_str(SimpleSet *set, const char *key) {
    return set_add(set, key, strlen(key));
}

int set_contains(const SimpleSet *set, const char *key, key_size_tt len) {
    uint64_t index, hash = set->hash_function(key, len);
    return __get_index(set, key, len, hash, &index);
}

int set_contains_str(const SimpleSet *set, const char *key) {
    return set_contains(set, key, strlen(key));
}

int set_remove(SimpleSet *set, const char *key, key_size_tt len) {
    uint64_t index, hash = set->hash_function(key, len);
    int res = __get_index(set, key, len, hash, &index);
    if (res != SET_TRUE) {
        return res;
    }
    // remove this node
    __free_index(set, index);
    // re-layout nodes
    if ((res = __relayout_nodes(set, index, 0)) < 0)
        return res;
    --set->used_nodes;
    return SET_TRUE;
}

int set_remove_str(SimpleSet *set, const char *key) {
    return set_remove(set, key, strlen(key));
}

uint64_t set_length(const SimpleSet *set) {
    return set->used_nodes;
}

char** set_to_array(const SimpleSet *set, uint64_t *size) {
    char** results = calloc(set->used_nodes, sizeof(char*));

    if (results) {
        uint64_t len, i, j = 0;
        for (i = 0; i < set->number_nodes; ++i) {
            if (set->nodes[i] != NULL) {
                len = (uint64_t) set->nodes[i]->_len;

                if (!(results[j] = calloc(len, sizeof(char)))) {
                    // Free all previous allocations before returning after an
                    // error.
                    for (uint64_t k = 0; k < j; k++) {
                        free(results[k]);
                    }

                    free(results);
                    results = NULL;
                    break;
                }

                memcpy(results[j], set->nodes[i]->_key, len);
                ++j;
            }
        }

        *size = set->used_nodes;
    }

    return results;
}

int set_union(SimpleSet *out, const SimpleSet *s1, const SimpleSet *s2) {
    int res;
    if (out->used_nodes != 0) {
        return SET_OCCUPIED_ERROR;
    }
    // loop over both s1 and s2 and get keys and insert them into out
    uint64_t i;
    for (i = 0; i < s1->number_nodes; ++i) {
        if (s1->nodes[i] != NULL) {
            if ((res = __set_add(out, s1->nodes[i]->_key, s1->nodes[i]->_len, s1->nodes[i]->_hash)) < 0)
                return res;
        }
    }
    for (i = 0; i < s2->number_nodes; ++i) {
        if (s2->nodes[i] != NULL) {
            if ((res = __set_add(out, s2->nodes[i]->_key, s2->nodes[i]->_len, s2->nodes[i]->_hash)) < 0)
                return res;
        }
    }
    return SET_TRUE;
}

int set_intersection(SimpleSet *out, const SimpleSet *s1, const SimpleSet *s2) {
    if (out->used_nodes != 0) {
        return SET_OCCUPIED_ERROR;
    }
    // loop over both one of s1 and s2: get keys, check the other, and insert them into out if it is
    uint64_t i;
    int res;
    for (i = 0; i < s1->number_nodes; ++i) {
        if (s1->nodes[i] != NULL) {
            res = __set_contains(s2, s1->nodes[i]->_key, s1->nodes[i]->_len, s1->nodes[i]->_hash);
            if (res < 0)
                return res;

            if (res == SET_TRUE) {
                res = __set_add(out, s1->nodes[i]->_key, s1->nodes[i]->_len, s1->nodes[i]->_hash);

                if (res < 0)
                    return res;
            }
        }
    }
    return SET_TRUE;
}

/* difference is s1 - s2 */
int set_difference(SimpleSet *out, const SimpleSet *s1, const SimpleSet *s2) {
    if (out->used_nodes != 0) {
        return SET_OCCUPIED_ERROR;
    }
    // loop over s1 and keep only things not in s2
    uint64_t i;
    int res;
    for (i = 0; i < s1->number_nodes; ++i) {
        if (s1->nodes[i] != NULL) {
            res = __set_contains(s2, s1->nodes[i]->_key, s1->nodes[i]->_len, s1->nodes[i]->_hash);
            if (res < 0)
                return res;
            if (res != SET_TRUE) {
                res = __set_add(out, s1->nodes[i]->_key, s1->nodes[i]->_len, s1->nodes[i]->_hash);

                if (res < 0)
                    return res;
            }
        }
    }
    return SET_TRUE;
}

int set_symmetric_difference(SimpleSet *out, const SimpleSet *s1, const SimpleSet *s2) {
    if (out->used_nodes != 0) {
        return SET_OCCUPIED_ERROR;
    }
    uint64_t i;
    int res;
    // loop over set 1 and add elements that are unique to set 1
    for (i = 0; i < s1->number_nodes; ++i) {
        if (s1->nodes[i] != NULL) {
            res = __set_contains(s2, s1->nodes[i]->_key, s1->nodes[i]->_len, s1->nodes[i]->_hash);
            if (res < 0)
                return res;

            if (res != SET_TRUE) {
                res = __set_add(out, s1->nodes[i]->_key, s1->nodes[i]->_len, s1->nodes[i]->_hash);
                if (res < 0)
                    return res;
            }
        }
    }
    // loop over set 2 and add elements that are unique to set 2
    for (i = 0; i < s2->number_nodes; ++i) {
        if (s2->nodes[i] != NULL) {
            res = __set_contains(s1, s2->nodes[i]->_key, s2->nodes[i]->_len, s2->nodes[i]->_hash);
            if (res < 0)
                return res;
            if (res != SET_TRUE) {
                res = __set_add(out, s2->nodes[i]->_key, s2->nodes[i]->_len, s2->nodes[i]->_hash);
                if (res < 0)
                    return res;
            }
        }
    }
    return SET_TRUE;
}

int set_is_subset(const SimpleSet *test, const SimpleSet *against) {
    uint64_t i;
    for (i = 0; i < test->number_nodes; ++i) {
        if (test->nodes[i] != NULL) {
            int res =__set_contains(against, test->nodes[i]->_key, test->nodes[i]->_len, test->nodes[i]->_hash);
            if (res < 0 || res == SET_FALSE)
                return res;
        }
    }
    return SET_TRUE;
}

int set_is_subset_strict(const SimpleSet *test, const SimpleSet *against) {
    if (test->used_nodes >= against->used_nodes) {
        return SET_FALSE;
    }
    return set_is_subset(test, against);
}

int set_cmp(const SimpleSet *left, const SimpleSet *right) {
    if (left->used_nodes < right->used_nodes) {
        return SET_RIGHT_GREATER;
    } else if (right->used_nodes < left->used_nodes) {
        return SET_LEFT_GREATER;
    }
    uint64_t i;
    int res;
    for (i = 0; i < left->number_nodes; ++i) {
        if (left->nodes[i] != NULL) {
            res = set_contains(right, left->nodes[i]->_key, left->nodes[i]->_len);
            if (res < 0)
                return res;
            if (res != SET_TRUE) {
                return SET_UNEQUAL;
            }
        }
    }

    return SET_EQUAL;
}


/*******************************************************************************
***        PRIVATE FUNCTIONS
*******************************************************************************/
static uint64_t __default_hash(const char *key, key_size_tt len) {
    // FNV-1a hash (http://www.isthe.com/chongo/tech/comp/fnv/)
    uint64_t h = 14695981039346656037ULL; // FNV_OFFSET 64 bit
    for (size_t i = 0; i < len; ++i) {
        h = h ^ (unsigned char) key[i];
        h = h * 1099511628211ULL; // FNV_PRIME 64 bit
    }
    return h;
}

static int __set_contains(const SimpleSet *set, const char *key, key_size_tt len, uint64_t hash) {
    uint64_t index;
    return __get_index(set, key, len, hash, &index);
}

static int __set_add(SimpleSet *set, const char *key, key_size_tt len, uint64_t hash) {
    int res;
    uint64_t index;
    if (__set_contains(set, key, len, hash) == SET_TRUE)
        return SET_ALREADY_PRESENT;

    // Expand nodes if we are close to our desired fullness
    if ((float)set->used_nodes / set->number_nodes > MAX_FULLNESS_RATIO) {
        uint64_t num_els = set->number_nodes * 2; // we want to double each time
        simple_set_node** tmp = realloc(set->nodes, num_els * sizeof(simple_set_node*));
        if (tmp == NULL)
            return SET_MALLOC_ERROR;

        set->nodes = tmp;
        uint64_t i, orig_num_els = set->number_nodes;
        for (i = orig_num_els; i < num_els; ++i)
            set->nodes[i] = NULL;

        set->number_nodes = num_els;
        // re-layout all nodes
        if ((res = __relayout_nodes(set, 0, 1)) < 0)
            return res;
    }
    // add element in
    res = __get_index(set, key, len, hash, &index);
    if (res < 0)
        return res;
    if (res == SET_FALSE) { // this is the first open slot
        if (__assign_node(set, key, len, hash, index) == SET_MALLOC_ERROR)
            return SET_MALLOC_ERROR;

        ++set->used_nodes;
        return SET_TRUE;
    }
    return res;
}

static int __get_index(const SimpleSet *set, const char *key, key_size_tt len, uint64_t hash, uint64_t *index) {
    uint64_t i, idx;
    idx = hash % set->number_nodes;
    i = idx;
    while (1) {
        if (set->nodes[i] == NULL) {
            *index = i;
            return SET_FALSE; // not here OR first open slot
        } else if (hash == set->nodes[i]->_hash && len == set->nodes[i]->_len && memcmp(key, set->nodes[i]->_key, len) == 0) {
            *index = i;
            return SET_TRUE;
        }
        ++i;
        if (i == set->number_nodes)
            i = 0;
        if (i == idx) // this means we went all the way around and the set is full
            return SET_CIRCULAR_ERROR;
    }
}

static int __assign_node(SimpleSet *set, const char *key, key_size_tt len, uint64_t hash, uint64_t index) {
    if (!(set->nodes[index] = malloc(sizeof(simple_set_node)))) {
        return SET_MALLOC_ERROR;
    }

    if (!(set->nodes[index]->_key = calloc(len, sizeof(char)))) {
        free(set->nodes[index]);
        set->nodes[index] = NULL;
        return SET_MALLOC_ERROR;
    }

    set->nodes[index]->_len = len;
    memcpy(set->nodes[index]->_key, key, len);
    set->nodes[index]->_hash = hash;
    return SET_TRUE;
}

static void __free_index(SimpleSet *set, uint64_t index) {
    free(set->nodes[index]->_key);
    free(set->nodes[index]);
    set->nodes[index] = NULL;
}

static int __relayout_nodes(SimpleSet *set, uint64_t start, short end_on_null) {
    int res = 0;
    uint64_t index = 0, i;
    for (i = start; i < set->number_nodes; ++i) {
        if(set->nodes[i] != NULL) {
            if ((res = __get_index(set, set->nodes[i]->_key, set->nodes[i]->_len, set->nodes[i]->_hash, &index)) < 0)
                return res;

            if (i != index) { // we are moving this node
                if ((res = __assign_node(set, set->nodes[i]->_key, set->nodes[i]->_len, set->nodes[i]->_hash, index)) < 0)
                    return res;

                __free_index(set, i);
            }
        } else if (end_on_null == 0 && i != start) {
            break;
        }
    }

    return res;
}
