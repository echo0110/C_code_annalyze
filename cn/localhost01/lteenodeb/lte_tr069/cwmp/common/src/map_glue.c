 /* Generic map implementation. This class is thread-safe.
 * free() must be invoked when only one thread has access to the map.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include        <cspl.h>
#include <map_glue.h>
#include <logger.h>
#include <ipr_cwmp_enb_wrapper.h>
//#include <glue_layer_def.h>

#define INITIAL_SIZE 50
#define MAX_MODULE_NAME 256

typedef int SInt32;
typedef unsigned char UINT8;
/* SPR 13359 Fix */
/******************************************************************************
  Function Declarations
******************************************************************************/
extern void map_free(map_map* in);

/****************************************************************************
** ** Function Name  : map_new_value
** ** Inputs         : none
** ** Outputs        : none
** ** Returns        : Return an map pointer on Success and empty map, or NULL on failure
** ** Description    : This function creates new map 
** *****************************************************************************/
map_map* map_new_value(void) 
{
		map_map* m = (map_map*) ALLOCATE(sizeof(map_map) );
		if(!m) goto err;

		m->data = (map_element*) ALLOCATE(( INITIAL_SIZE * sizeof(map_element)) );
		if(!m->data) goto err;

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
/****************************************************************************
** ** Function Name  : map_put_value
** ** Inputs         : map_map * in, UINT8 * parent_key, UINT8 *child_key, UINT8 * value
** ** Outputs        : none
** ** Returns        : SUCCESS/FAILURE
** ** Description    : This function inserts new value in map
** *****************************************************************************/
Sint32 map_put_value(map_map * in, Uint8 * parent_key, Uint8 *child_key, Uint8 * value)
{
		Sint32 index;
		Sint32 count = 0;
		map_map* m;

		data_t *data_1 = (data_t *)ALLOCATE(sizeof(data_t ) );
		if(NULL == data_1)
		{
			return MAP_FULL;
		}
		data_1->child_key = child_key;
		data_1->data = value;
		/* Cast the map */
		m = (map_map *) in;


		/* Find a place to put our value */
		index = m->size;

		if(m->size == m->table_size)
		{
          /* MEMORY LEAK 31 DEC FIXED START */
			DEALLOCATE(data_1); //V9D

           /* MEMORY LEAK 31 DEC FIXED END */
			return MAP_FULL;
		}
		/* Set the data */


		for(count =0;count<m->size;count++){
				if(strcmp(parent_key, m->data[count].parent_key) == 0 )
				{

						ylPushTail(&m->data[count].mylist,(YLNODE*)(data_1));
						return  0;
				}
		} 
		/*  parent_key */
		m->data[index].parent_key = (Uint8 *) ALLOCATE(MAX_MODULE_NAME );
		strncpy (m->data[index].parent_key, parent_key, MAX_MODULE_NAME);
        /* SPR 17880 START */
        if(child_key)
        {
		    m->data[index].child_key = (Uint8 *) ALLOCATE(MAX_MODULE_NAME );
		    strncpy (m->data[index].child_key, child_key, MAX_MODULE_NAME);
        }
        else
        {       
            m->data[index].child_key = NULL;
        }
        /* SPR 17880 END */
		ylInit(&m->data[index].mylist);
		ylPushTail(&m->data[index].mylist,(YLNODE*)(data_1));

		m->size++; 

		return MAP_OK;
}

/* Memory Leak 18 Jan changes start */
void map_free_1(map_map* in)
{
     if (in == NULL)
        return;
    int count = 0;
   
    map_map* m = (map_map*) in;
    if(m->size)
    {

        for(count=0 ; count< m->size;count++)
        {
            YLIST *plist = &m->data[count].mylist;
            data_t *node = NULL;

            if(NULL != m->data[count].parent_key)
            {
                DEALLOCATE(m->data[count].parent_key);
            }
            if(NULL != m->data[count].child_key)
            {
                DEALLOCATE(m->data[count].child_key);
            }

            /* SPR 22314 Start */
            while ((node = (data_t *)ylPopTail(plist))!= NULL)
                /* SPR 22314 End */
            {
                DEALLOCATE(node);
            }
        }
        if(m->data)
        {
            DEALLOCATE(m->data);
        }

    }        
    DEALLOCATE(m);

}

/* Memory Leak 18 Jan changes end */

/****************************************************************************
** ** Function Name  : map_get_value
** ** Inputs         : map_map* in ,char * key
** ** Outputs        : none
** ** Returns        : map pointer on success and Null n failure
** ** Description    : This function is used to retrieve values from map
** *****************************************************************************/
YLIST*  map_get_value(map_map* in ,Uint8 * key)
{
		map_map* m;
		SInt32 count = 0;
		m = (map_map *) in;
		for(count =0;count<m->size;count++){
				if(strcmp(key,m->data[count].parent_key)==0){

						YLIST *plist = &m->data[count].mylist;
						data_t *node  = NULL;
						while((node = (data_t *)ylPopTail(plist))!=NULL)
                               TRACE(LOG_CRITICAL, GLUE_LAYER, "child key %s,  data : %s", node->child_key, node->data);

						return &m->data[count].mylist;
				}
		}

		return NULL;
}



/* Return the length of the map */
/****************************************************************************
** ** Function Name  : map_length_value
** ** Inputs         : map_map* in
** ** Outputs        : none
** ** Returns        : map length on success and zero in failure
** ** Description    : This function is used to retrieve values from map
** *****************************************************************************/
SInt32 map_length_value(map_t in){
		map_map* m = (map_map *) in;
		if(m != NULL) return m->size;
		else return 0;
}

/*
   int compare_list_set ( const void *pa, const void *pb )
   {
   char * a =  ((transData_t *)pa)->trans_id;
   char *b = ((transData_t *)pb)->trans_id; 
   int val = strcmp(a,b);

   if( val == 0 ) {
   return 0;
   } else if (val > 0) {
   return 1;
   } else {
   return -1;
   }
   }
   */

/****************************************************************************
** ** Function Name  : compare_list_set_int
** ** Inputs         : const void *pa, const void *pb
** ** Outputs        : none
** ** Returns        : Success/Failure
** ** Description    : This function compares map values
** *****************************************************************************/
SInt32 compare_list_set_int (const void *pa, const void *pb )
{

		SInt32 a = ((transData_t *)pa)->trans_id;
		SInt32 b = ((transData_t *)pb)->trans_id;

		if( a == b ) {
				return 0;
		} else if (a > b) {
				return 1;
		} else {
				return -1;
		} 



}

const void *keyof(const YLNODE *Z)
{
		//return &(((const transData_t  *)Z)->trans_id);
		return (((const transData_t  *)Z));
}



/****************************************************************************
** ** Function Name  : init_list
** ** Inputs         : YLIST *yList 
** ** Outputs        : none
** ** Returns        : none
** ** Description    : This function Initializes the list 
** *****************************************************************************/
void init_list(YLIST *yList )
{
		ylInit(yList);
}

/****************************************************************************
** ** Function Name  : addNodeinSet
** ** Inputs         : YLIST *yList , transData_t *p_transD
** ** Outputs        : none
** ** Returns        : none
** ** Description    : This function add node in the list 
** *****************************************************************************/
void addNodeinSet ( YLIST *yList , transData_t *p_transD)
{

		ADD_IN_LIST ( yList , (YLNODE *) p_transD);

}

/****************************************************************************
** ** Function Name  : remove_node_from_Set
** ** Inputs         : YLIST *yList , transData_t *p_transD
** ** Outputs        : none
** ** Returns        : none
** ** Description    : This function remove node from the list 
** *****************************************************************************/
void * remove_node_from_Set (YLIST *yList , transData_t *p_transD)
{

		transData_t *l_fetch = NULL;


		l_fetch = ( transData_t *)ylFind (yList , p_transD, keyof , compare_list_set_int );


		return l_fetch;

}

/****************************************************************************
** ** Function Name  : free_glue_map
** ** Inputs         : map_map* in 
** ** Outputs        : none
** ** Returns        : none
** ** Description    : This function is used to Deallocate the mpa
** *****************************************************************************/
void free_glue_map(map_map* in)
{

		if (in == NULL)
				return;
		map_map* m = (map_map*) in;
		SInt32 count = 0;  

		if (m->data)
		{

            /* SPR 16448 Fix Start */   
				for(count = m->size-1;count >=0 ;count--)
				{
						DEALLOCATE(m->data[count].parent_key);
				}
				DEALLOCATE(m->data);
		}
		DEALLOCATE(m);
            /* SPR 16448 Fix End */   
        /*  coverity 86633 fix start */
        /* code removed */
        /*  coverity 86633 fix end */
}

