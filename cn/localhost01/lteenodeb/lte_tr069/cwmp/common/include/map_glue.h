/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: map_glue.h $
 *
 *******************************************************************************
******************************************************************************/

/*
 * Generic map manipulation functions
 */
#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */


#include <cspl.h>

#define GLUE_LAYER "Glue_Layer"
#define ADD_IN_LIST ylPushTail 
#define REM_FROM_LIST ylDelete
typedef int Sint32;
typedef char Uint8;
typedef struct
{
 int send_sockfd;
 int recv_sockfd;
}MsgHandle;

    
typedef struct _map_element{
         char * parent_key;
         char * child_key;
         char * data;
         YLIST mylist;
         struct _map_element *next;
} map_element;


typedef struct data
{
/* 3 - Node of type YLNODE added which shall
 *  * be used as anchor*/
 YLNODE anchor;
 char * child_key;
 char * data;
}data_t;

// A map has some maximum size and current size,
// // as well as the data to hold.
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
 * map_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how maps are
 * represented.  They see and manipulate only map_t's.
 */
typedef any_t map_t;

/*
 * Return an empty map. Returns NULL if empty.
 */
map_map* map_new_value(void);



typedef struct trans_data
{
/* 3 - Node of type YLNODE added which shall
 *  * be used as anchor*/
YLNODE anchor;
int trans_id;
}transData_t;


/*
 * Iteratively call f with argument (item, data) for
 * each element data in the map. The function must
 * return a map status code. If it returns anything other
 * than MAP_OK the traversal is terminated. f must
 * not reenter any map functions, or deadlock may arise.
 */
int map_iterate(map_t in, PFany f, any_t item);

/*
 * Add an element to the map. Return MAP_OK or MAP_OMEM.
*/
Sint32 map_put_value (map_map * in, Uint8 * parent_key, Uint8 * child_key,
			 Uint8 * value);

void init_list(YLIST *yList) ;


/* SPR 22314 Start */
#define TR069_FREE_SSETPARAMETERVALUES(ptr) do\
    {\
        tr069_free_ssetparametervalues(ptr);\
        ptr = NULL;\
    }while(TR069_ZERO)
#define TR069_FREE_PARAMMETERVALUELIST(PARAMVALUELIST) do\
    {\
        tr069_free_parameterValueList(PARAMVALUELIST);\
        PARAMVALUELIST = NULL;\
    }while(TR069_ZERO)
/* SPR 22314 End */

#endif
