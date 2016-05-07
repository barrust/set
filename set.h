/*******************************************************************************
***
***	 Author: Tyler Barrus
***	 email:  barrust@gmail.com
***
***	 Version: 0.1.3
***	 Purpose: Simple, yet effective, set implementation
***
***	 License: MIT 2016
***
***	 URL: https://github.com/barrust/set
***
***	 Usage:
***
***
***	Required Compile Flags:
***
*******************************************************************************/

#ifndef __SIMPLE_SET_H__
#define __SIMPLE_SET_H__

#include <inttypes.h>       /* PRIu64 */

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

int set_init(SimpleSet *set);
int set_init_alt(SimpleSet *set, HashFunction hash);
int set_destroy(SimpleSet *set);
int set_add(SimpleSet *set, char *key);
int set_remove(SimpleSet *set, char *key);
int set_contains(SimpleSet *set, char *key);
int set_union(SimpleSet *res, SimpleSet *s1, SimpleSet *s2);
int set_intersection(SimpleSet *res, SimpleSet *s1, SimpleSet *s2);
int set_difference(SimpleSet *res, SimpleSet *s1, SimpleSet *s2);   /* TODO: implement */
int set_is_subset(SimpleSet *test, SimpleSet *against);
int set_is_superset(SimpleSet *test, SimpleSet *against);
//char** set_to_array(SimpleSet *set);                              /* TODO: implement */

#define SET_TRUE 0
#define SET_FALSE -1
#define SET_MALLOC_ERROR -2
#define SET_CIRCULAR_ERROR -3
#define SET_OCCUPIED_ERROR -4
#define SET_ALREADY_PRESENT 1


#endif /* END SIMPLE SET HEADER */
