/********************************************************************************
 *
 *      FILE NAME   : map.c
 *
 *      DESCRIPTION : This file is used for different function call
 *
 *      REVISION HISTORY:
 *
 *      NAME             DATE            REFERENCE               REASON
 *_______________________________________________________________________________
 *
 *   Vikas Upadhyay    Aug 17,2012   TR-069 Amendment 4     Initial Creation
 *   Anant K Sharma    Mar 29,2013                          Added HAVE_IPSEC
 *   Anant K Sharma    Apr 08,2013                          Added check for connectivity of servers after tunnel establishment
 *
 *
 *       All Rights Reserved. Copyright @ Aricent Group, 2012.
 *
 ********************************************************************************/

/* Generic map implementation. This class is thread-safe.
 * free() must be invoked when only one thread has access to the map.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
/* SPR 16448 Fix Start */
#include    <cspl.h>
/* SPR 16448 Fix End */
#include <ipr_cwmp_enb_wrapper.h>
#define INITIAL_SIZE 50
#define MAX_MODULE_NAME 25


/*
 * Return an empty map, or NULL on failure.
 */
map_map* map_new(void) {
    /* Coverity 96631 + */
	map_map* m = (map_map*) ALLOCATE(sizeof(map_map) );
    /* Coverity 96631 - */
	if(!m) goto err;

    /* Coverity 96631 + */
	m->data = (map_element*) ALLOCATE(( INITIAL_SIZE * sizeof(map_element)) );
    /* Coverity 96631 - */
	if(!m->data) goto err;
	//m->data = 0;	

	//m->key = (char *) calloc(1,MAX_MODULE_NAME);
	//if(!m->data) goto err;

	m->table_size = INITIAL_SIZE;
	m->size = 0;
	return m;
	err:
		if (m)
			map_free(m);
		return NULL;
}


/*
 * Add a pointer to the map with some key
 */
int map_put(map_map *in, char * key, int value){
	int index;
	map_map* m;
	int i;

	/* Cast the map */
	m = (map_map *) in;
	
	  for(i = 0; i< m->size; i++){

         if (key != NULL)
         {
                if(strcmp(m->data[i].key ,key)==0){
                         m->data[i].data = value;
                        return MAP_OK;
                }
         }
        else
         return -3;
}
//coverity fix

	/* Find a place to put our value */
	index = m->size;

	if(m->size == m->table_size) return MAP_FULL;

	/* Set the data */

	m->data[index].data = value;

	m->data[index].key = (char *) ALLOCATE(MAX_MODULE_NAME);
	strncpy (m->data[index].key, key, MAX_MODULE_NAME);
	m->size++; 

	return MAP_OK;
}

/*
 * Get your pointer out of the map with a key
 */
int map_get(map_map *in, const char * key, int *arg){
	 //const char * curr_key;
 
	int i;
	map_map* m;

	/* Cast the map */
	m = (map_map *) in;


	/* Find data location */
	//curr_key = key;

	/* Linear probing, if necessary */
	for(i = 0; i< m->size; i++){

	 if (key != NULL)	
	 {
		if(strcmp(m->data[i].key ,key)==0){
			*arg = (m->data[i].data);
			return MAP_OK;
		}
	 }
	}
/*  coverity 86601 fix start */
/* code removed */
/*  coverity 86601 fix end */

	/* Not found */
	return MAP_MISSING;
}



/*
 * Remove an element with that key from the map
 */
int map_remove(map_map *in, char * key){
	int i;
	//char * curr_key;
	map_map* m;

	/* Cast the map */
	m = (map_map *) in;


	/* Find key */
	//curr_key = key;

	/* Linear probing, if necessary */
	for(i = 0; i< m->size; i++){
		if( strcmp(m->data[i].key, key) == 0 ){
			/* Blank out the fields */
			m->data[i].data = 0;
			m->data[i].key = NULL;

			/* Reduce the size */
			m->size--;
			if (m->size < 0 ) m->size = 0;
			return MAP_OK;
		}
	}

	/* Data not found */
	return MAP_MISSING;
}

/* Deallocate the map */
void map_free(map_map* in){

	if (in == NULL)
		return;
	map_map* m = (map_map*) in;

	if (m->data)
	{
/* SPR 16448 Fix Start */
		DEALLOCATE(m->data); 
/* SPR 16448 Fix End */
		m->data = NULL;
	}
	//ENB_FREE (m->key);
/* SPR 16448 Fix Start */
/*  coverity 96631 fix start */
/* MEMORY LEAK 31 DEC FIXED START */
	 DEALLOCATE(m); 
/* MEMORY LEAK 31 DEC FIXED END */
/*  coverity 96631 fix end */

/* SPR 16448 Fix End */
/*  coverity 86654 fix start */
/* code removed */
/*  coverity 86654 fix end */    
}

/* Return the length of the map */
int map_length(map_map* in){
	map_map* m = (map_map *) in;
	if(m != NULL) return m->size;
	else return 0;
}
