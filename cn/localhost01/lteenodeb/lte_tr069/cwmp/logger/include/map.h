/*
 * Generic map manipulation functions
 */
#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */


// We need to keep keys and values
typedef struct _map_element{
         char * key;
         int data;
} map_element;

// A map has some maximum size and current size,
// as well as the data to hold.
typedef struct _map_map{
        int table_size;
        int size;
        map_element *data;
} map_map;

/*
 * any_t is a pointer.  This allows you to put arbitrary structures in
 * the map.
 */
typedef void *any_t;

/*
 * PFany is a pointer to a function that can take two any_t arguments
 * and return an integer. Returns status code..
 */
typedef int (*PFany)(any_t, any_t);

/*
 * map_map is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how maps are
 * represented.  They see and manipulate only map_map's.
 */
typedef any_t map_t;

/*
 * Return an empty map. Returns NULL if empty.
*/
 map_map* map_new(void);

/*
 * Iteratively call f with argument (item, data) for
 * each element data in the map. The function must
 * return a map status code. If it returns anything other
 * than MAP_OK the traversal is terminated. f must
 * not reenter any map functions, or deadlock may arise.
 */
 int map_iterate(map_map* in, PFany f, any_t item);

/*
 * Add an element to the map. Return MAP_OK or MAP_OMEM.
 */
 int map_put(map_map* in, char * key, int value);

/*
 * Get an element from the map. Return MAP_OK or MAP_MISSING.
 */
int map_get(map_map* in, const char * key, int* arg);

/*
 * Remove an element from the map. Return MAP_OK or MAP_MISSING.
 */
 int map_remove(map_map* in, char *  key);

/*
 * Get any element. Return MAP_OK or MAP_MISSING.
 * remove - should the element be removed from the map
 */
 int map_get_one(map_map* in, any_t *arg, int remove);

/*
 * Free the map
 */
 void map_free(map_map* in);

/*
 * Get the current size of a map
 */
 int map_length(map_map* in);

#endif
