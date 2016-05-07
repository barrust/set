/*******************************************************************************
***
***	 Author: Tyler Barrus
***	 email:  barrust@gmail.com
***
***	 Version: 0.1.0
***
***	 License: MIT 2016
***
*******************************************************************************/

#include "set.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>        /* boolean: true false */

#define INITIAL_NUM_ELEMENTS 1024
#define MAX_FULLNESS_PERCENT 0.25       /* arbitrary */

/* PRIVATE FUNCTIONS */
static uint64_t __default_hash(char *key);
static int __get_index(SimpleSet *set, char *key, uint64_t hash, uint64_t *index);
static int __assign_node(SimpleSet *set, char *key, uint64_t hash, uint64_t index);
static void __free_index(SimpleSet *set, uint64_t index);

/* FUNCTION DEFINITIONS */

int set_init(SimpleSet *set) {
    return set_init_alt(set, NULL);
}

int set_init_alt(SimpleSet *set, HashFunction hash) {
    set->nodes = (simple_set_node**) malloc(INITIAL_NUM_ELEMENTS * sizeof(simple_set_node*));
    // should add check to make sure it is not NULL
    set->number_nodes = INITIAL_NUM_ELEMENTS;
    uint64_t i;
    for (i = 0; i < set->number_nodes; i++) {
        set->nodes[i] = NULL;
    }
    set->used_nodes = 0;
    set->hash_function = (hash == NULL) ? &__default_hash : hash;
    return 0;
}

int set_destroy(SimpleSet *set) {
    uint64_t i;
    for(i = 0; i < set->number_nodes; i++) {
        if (set->nodes[i] != NULL) {
            __free_index(set, i);
        }
    }
    free(set->nodes);
    set->number_nodes = 0;
	set->used_nodes = 0;
	set->hash_function = NULL;
}

int set_add(SimpleSet *set, char *key) {
    if (set_contains(set, key) == 0) {
        return 1; // this is already present in the set
    }
    // Expand nodes if we are close to our desired fullness
    if ((set->used_nodes * 1.0) / set->number_nodes > MAX_FULLNESS_PERCENT) {
        uint64_t num_els = set->number_nodes * 2; // we want to double each time
        simple_set_node** tmp = realloc(set->nodes, num_els * sizeof(simple_set_node*));
        if (set->nodes == NULL) { // malloc failure
            return -2;
        }
        set->nodes = tmp;
		uint64_t orig_num_els = set->number_nodes;
        uint64_t i;
        for (i = orig_num_els; i < num_els; i++) {
			set->nodes[i] = NULL;
		}
		set->number_nodes = num_els;
        // re-layout all nodes
    	for (i = 0; i < set->number_nodes; i++) {
    		if(set->nodes[i] != NULL) {
    			uint64_t index;
                __get_index(set, set->nodes[i]->_key, set->nodes[i]->_hash, &index);
                if (i != index) { // we are moving this node
                    __assign_node(set, set->nodes[i]->_key, set->nodes[i]->_hash, index);
                    __free_index(set, i);
                }

    		}
    	}
    }
    uint64_t index, hash = set->hash_function(key);
    int res = __get_index(set, key, hash, &index);
    if (res == -1) { // this is the first open slot
        __assign_node(set, key, hash, index);
        set->used_nodes++;
        return 0;
    } else {
        return res;
    }
}

int set_contains(SimpleSet *set, char *key) {
    uint64_t index, hash = set->hash_function(key);
    return __get_index(set, key, hash, &index);
}



/*******************************************************************************
***		PRIVATE FUNCTIONS
*******************************************************************************/
static uint64_t __default_hash(char *key) {
    // FNV-1a hash (http://www.isthe.com/chongo/tech/comp/fnv/)
	int i, len = strlen(key);
	char *p = calloc(len + 1, sizeof(char));
	strncpy(p, key, len);
	uint64_t h = 14695981039346656073ULL; // FNV_OFFSET 64 bit
	for (i = 0; i < len; i++){
		h = h ^ (unsigned char) p[i];
		h = h * 1099511628211ULL; // FNV_PRIME 64 bit
	}
	free(p);
	return h;
}

static int __get_index(SimpleSet *set, char *key, uint64_t hash, uint64_t *index) {
    uint64_t i, idx;
    idx = hash % set->number_nodes;
    i = idx;

    while (true) {
        if (set->nodes[i] == NULL) {
            *index = i;
            return -1; // not here
        } else if (hash == set->nodes[i]->_hash && strlen(key) == strlen(set->nodes[i]->_key) && strncmp(key, set->nodes[i]->_key, strlen(key)) == 0) {
            *index = i;
            return 0;
        } else {
            i++;
            if (i > set->number_nodes) {
                i = 0;
            }
            if (i == idx) { // this means we went all the way around and the set is full
                return -2;
            }
        }
    }
}

static int __assign_node(SimpleSet *set, char *key, uint64_t hash, uint64_t index) {
    set->nodes[index] = malloc(sizeof(simple_set_node));
    set->nodes[index]->_key = calloc(strlen(key) + 1, sizeof(char));
    strncpy(set->nodes[index]->_key, key, strlen(key));
    set->nodes[index]->_hash = hash;
    return 0;
}

static void __free_index(SimpleSet *set, uint64_t index) {
    free(set->nodes[index]->_key);
    free(set->nodes[index]);
    set->nodes[index] = NULL;
}
