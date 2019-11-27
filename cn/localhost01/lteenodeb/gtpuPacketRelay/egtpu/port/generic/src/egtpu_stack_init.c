/***********************************************************
**  FILE NAME:
**       egtpu_stack_init.c
**       
**       
*************************************************************
**       
**  DESCRIPTION:
**       This file provides Initialization Routines for
**       E-GTPU stack.
**       
**      DATE       NAME          REFERENCE    REASON
**      ----       ----          ---------    ------
**
**
**  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
************************************************************/

#include <port.h>

#include <egtpu_msgtypes.h>
#include <egtpu_glob.h>
#include <egtpu_ietypes.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_util.h>
#include <egtpu_apihdr.h>
#include <egtpu_msgtypes.h>
#include <egtpu_os_proto.h>
#include <egtpu_trace.h>
#include "lteRelay.h"
#include <egtpu_pmm.h>
#include "lteMsgPool.h"
/* CA changes start */
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

unsigned short ei_to_srvname_map[] = {
    SRVNAME_GTPU,
    SRVNAME_UDP,
    SRVNAME_RELAY,
    SRVNAME_CP,
    SRVNAME_LM,
    SRVNAME_TS,
    SRVNAME_UDP6
};

char dstId_to_srcIp4Add_map[MAX_SOURCE_IP_ADDS+SRVNAME_UDP6][32];
char dstId_to_srcIp6Add_map[MAX_SOURCE_IP_ADDS+SRVNAME_UDP6][50];

egtpu_global_t *p_global = NULL;
egtpu_return_t egtpu_init_reorder_timer_buf(egtpu_global_t *p_global, egtpu_rab_ctxt_t *p_rab_ctxt);
extern void lteRelayMsgHandler(void *);
egtpu_void_t egtpu_check_echo_timer_expiry(void);
/*-SPR #3456*/


/****************************************************************************
 * Function Name  : egtpu_init 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion initialise the gtpu global memory
 ****************************************************************************/
void egtpu_init ()
{
   p_global = egtpu_malloc (sizeof(egtpu_global_t));
   egtpu_bzero ((egtpu_void_t *)p_global, sizeof(egtpu_global_t)); 
   p_global->max_window_size[0] =   EGTPU_WINDOW_SZ_SUBSCRIPTION_TRAFFIC_CLASS ;
   p_global->max_window_size[1] =	EGTPU_WINDOW_SZ_CONVERSATIONAL_TRAFFIC_CLASS ;
   p_global->max_window_size[2] =	EGTPU_WINDOW_SZ_STREAMING_TRAFFIC_CLASS ;
   p_global->max_window_size[3] =	EGTPU_WINDOW_SZ_INTERACTIVE_TRAFFIC_CLASS ;
   p_global->max_window_size[4] =	EGTPU_WINDOW_SZ_BKGROUND_TRAFFIC_CLASS ;
   p_global->reorder_timer_drtn[0] = 	EGTPU_REORDER_TMR_DRTN_SUBSCRIPTION_TRAFFIC_CLASS ;
   p_global->reorder_timer_drtn[1] =	EGTPU_REORDER_TMR_DRTN_CONVERSATIONAL_TRAFFIC_CLASS ;
   p_global->reorder_timer_drtn[2] =	EGTPU_REORDER_TMR_DRTN_STREAMING_TRAFFIC_CLASS ;
   p_global->reorder_timer_drtn[3] =	EGTPU_REORDER_TMR_DRTN_INTERACTIVE_TRAFFIC_CLASS ;
   p_global->reorder_timer_drtn[4] =	EGTPU_REORDER_TMR_DRTN_BKGROUND_TRAFFIC_CLASS ; /*granularity is in ms*/
   
   if (!(p_global->mpool_out = egtpu_msg_pool_create 
               (PORT_MSG_POOL_MSG_SIZE, PORT_MSG_MAX)))
   {
       LOG_GTPU_MSG(GTPU_NO_MEM_TO_CREATE_POOL, LOGFATAL, PR_GTPU_COMMON,
               relayGtpuCurrentTick_g,__LINE__,
               0,0,0,0, 0,0, __func__, "");
   }
   else
   {
       LOG_GTPU_MSG(GTPU_INIT_DONE, LOGINFO, PR_GTPU_COMMON,
               relayGtpuCurrentTick_g,__LINE__,
               0,0,0,0, 0,0, __func__, "");
   }

}

/* egtpu_return_t egtpu_init_reorder_timer_buf(egtpu_global_t *p_global, egtpu_rab_ctxt_t *p_rab_ctxt); */

/****************************************************************************
 * Function Name  : egtpu_bzero 
 * Inputs         : p - pointer to the memory to be initalised
 *                  len - size of memory
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion initialise the memory with zero 
 ****************************************************************************/
egtpu_void_t egtpu_bzero (egtpu_void_t *p, U32bit len)
{
    memSet (p, 0, len);
}

/****************************************************************************
 * Function Name  : egtpu_pool_create 
 * Inputs         : s - size of the buffer to be created
 *                  n - number of the buffer blocks
 * Outputs        : None
 * Returns        : Pointer to the buffer
 * Description    : This funtion creates the memory pool and returns the pointer
 ****************************************************************************/
egtpu_pool_t egtpu_pool_create (U32bit s, U32bit n)
{
   /* return qvPoolCreate (s, n);*/
    return createAndReturnMemPool (s, n);
}

/****************************************************************************
 * Function Name  : egtpu_msg_pool_create 
 * Inputs         : s - size of the buffer to be created
 *                  n - number of the buffer blocks
 * Outputs        : None
 * Returns        : Pointer to the buffer
 * Description    : This funtion creates the message pool and returns the pointer
 ****************************************************************************/
egtpu_pool_t egtpu_msg_pool_create (U32bit s, U32bit n)
{
     return qvMsgPoolCreate (s, n);
    /*return msgPoolCreate (s, n);*/
}

/****************************************************************************
 * Function Name  : egtpu_pool_delete 
 * Inputs         : p - pointer to the memory to be freed 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion delete the pool
 ****************************************************************************/
egtpu_void_t egtpu_pool_delete (egtpu_pool_t p)
{
    /* qvPoolDelete (p);*/
    deletePool (p);
}

/****************************************************************************
 * Function Name  : egtpu_pool_alloc 
 * Inputs         : p - pointer to tho the pool 
 * Outputs        : None
 * Returns        : Pointer to the buffer
 * Description    : This funtion creates the memory from pool p  and returns the pointer
 ****************************************************************************/
egtpu_void_t *egtpu_pool_alloc (egtpu_pool_t p)
{
    /* return qvPoolAlloc (p);*/
    return allocateMemFromPool (p);
}

/****************************************************************************
 * Function Name  : egtpu_pool_sequence 
 * Inputs         : buf - pointer to the buffer 
 * Outputs        : None
 * Returns        : sequence 
 * Description    : This funtion find the sequence number of buffer
 ****************************************************************************/
U16bit egtpu_pool_sequence (egtpu_void_t *buf)
{
    /*return qvPoolSequence (buf);*/
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
    return getPoolSequence (buf);
}

/****************************************************************************
 * Function Name  : egtpu_pool_extend 
 * Inputs         : p - pointer to the memory to be extended 
 *                  len - size of memory
 * Outputs        : None
 * Returns        : S32bit 
 * Description    : This funtion extends the memory with size 
 ****************************************************************************/
S32bit egtpu_pool_extend (egtpu_pool_t p, U32bit s)
{
    /*return qvPoolExtend (p, s);*/
    return extendPool (p, s);
}

/****************************************************************************
 * Function Name  : egtpu_pool_free 
 * Inputs         : p - pointer to the memory to be freed 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion free the memory pointed in p
 ****************************************************************************/
egtpu_void_t egtpu_pool_free (egtpu_void_t *p)
{
    /*qvPoolFree (p);*/
    freeMemPool(p);
}

/****************************************************************************
 * Function Name  : egtpu_pool_index 
 * Inputs         : p - pointer to the memory pool 
 *                  s - buffer 
 * Outputs        : None
 * Returns        : U32bit 
 * Description    : This funtion gets the index of buffer in a memory pool
 ****************************************************************************/
U32bit egtpu_pool_index (egtpu_pool_t p, egtpu_void_t *s)
{
    /*return qvPoolIndex (p, s);*/
    return getPoolIndex (p, s);
}

/****************************************************************************
 * Function Name  : egtpu_pool_locate 
 * Inputs         : p - pointer to the memory pool 
 *                  s - buffer 
 * Outputs        : None
 * Returns        : locationof pool 
 * Description    : This funtion gets the pointer of buffer in a memory pool
 ****************************************************************************/
egtpu_void_t *egtpu_pool_locate (egtpu_pool_t p, U32bit s)
{
    /*return qvPoolLocate (p, s);*/
    return getPoolLocate (p, s);
}

/****************************************************************************
 * Function Name  : egtpu_sysfree 
 * Inputs         : p - pointer to the memory to be freed 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion free the system memory pointd to p
 ****************************************************************************/
egtpu_void_t egtpu_sysfree (egtpu_void_t *p)
{
    /*qvSysFree (p);*/
    freeMemSys (p);
}

/****************************************************************************
 * Function Name  : egtpu_msgfree 
 * Inputs         : buf - pointer to the memory to be freed 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion free the  memory pointd to buf
 ****************************************************************************/
egtpu_void_t egtpu_msgfree (egtpu_void_t *buf)
{
    msgFree (buf);
}

/****************************************************************************
 * Function Name  : egtpu_malloc 
 * Inputs         : p - pointer to the memory to be assigned 
 * Outputs        : None
 * Returns        : allocated memory pointer 
 * Description    : This funtion get the memory from the system
 ****************************************************************************/
egtpu_void_t *egtpu_malloc (U32bit p)
{
   /*return qvSysMalloc (p);*/
   return getMemFromSys (p);
}

/****************************************************************************
 * Function Name  : egtpu_msg_alloc_from_pool 
 * Inputs         : buf - pointer to the memory to be assigned
 * Outputs        : None
 * Returns        : allocated memory pointer
 * Description    : This funtion allocates the memory from message pool 
 ****************************************************************************/
egtpu_void_t *egtpu_msg_alloc_from_pool (egtpu_pool_t buf)
{
    return qvMsgAllocFromPool (buf);
    /*return msgAlloc (buf,0,0,0);*/
}

/****************************************************************************
 * Function Name  : egtpu_memget 
 * Inputs         : p_global - pointer to the global memory
 *                  len - size of memory
 *                  type - static/dynamic
 * Outputs        : None
 * Returns        : allocated memory pointer 
 * Description    : This funtion allocate thes the memory
 ****************************************************************************/
egtpu_void_t *egtpu_memget (egtpu_global_t *p_global, U32bit len, U8bit type)
{
    egtpu_void_t *buf = 0;

    if (type == EGTPU_STATIC_BUFFER) 
	{
		buf = egtpu_malloc (len);

        if (!buf) {
            /*qvWarning ("egtpu_memget fail for static buffer\n");*/
        }
		
    } 
	else if (type == EGTPU_DYNAMIC_BUFFER) 
	{
        	buf = egtpu_msg_alloc_from_pool (p_global->mpool_out);

	        if (!buf) {
    	        /*qvWarning ("egtpu_memget fail for static buffer\n");*/
            }
    }

    return buf;
}

/****************************************************************************
 * Function Name  : egtpu_memfree 
 * Inputs         : type - static/dynamic 
 *                  buf - memory to be freed 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion free the memory
 ****************************************************************************/
egtpu_void_t egtpu_memfree (U8bit type, egtpu_void_t *buf)
{
    if (type == EGTPU_STATIC_BUFFER)
	egtpu_sysfree (buf);
    else if (type == EGTPU_DYNAMIC_BUFFER) {
	egtpu_msgfree (buf);
    }
}

/****************************************************************************
 * Function Name  : egtpu_memcpy 
 * Inputs         : dst - pointer to the destination
 *                  src - pointer to the source
 *                  num - number of byte
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion copy the memory form source to destination
 ****************************************************************************/
egtpu_void_t egtpu_memcpy (egtpu_void_t *dst, const egtpu_void_t *src, U32bit num)
{
    /*memcpy (dst, src, num);*/
    memCpy (dst, src, num);
}

/****************************************************************************
 * Function Name  : egtpu_memcmp 
 * Inputs         : p1 - pointer to the memory one 
 *                  p2 - pointer to the memory two 
 *                  num - number of byte
 * Outputs        : None
 * Returns        : S32bit 
 * Description    : This funtion  compare the data in memory
 ****************************************************************************/
S32bit egtpu_memcmp (const egtpu_void_t *p1, const egtpu_void_t *p2, U32bit num)
{
    return memCmp (p1, p2, num);
}

/****************************************************************************
 * Function Name  : egtpu_strcmp 
 * Inputs         : p1 - pointer to the string one 
 *                  p2 - pointer to the string two 
 * Outputs        : None
 * Returns        : S32bit 
 * Description    : This funtion compare the two strings
 ****************************************************************************/
/*
S32bit egtpu_strcmp (const S8bit *p1, const S8bit *p2)
{
    return strCmp(p1, p2);
}

S8bit* egtpu_strcpy(S8bit *p1, const S8bit *p2)
{
    return strCpy(p1, p2);
}
*/

/****************************************************************************
 * Function Name  : egtpu_check_echo_timer_expiry 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion checkst the echo timer expired or not
 ****************************************************************************/
egtpu_void_t egtpu_check_echo_timer_expiry(void)
{
    /* + SPR 21577 */
    UInt8 peer_count = 0;
    egtpu_peer_ctxt_t *p_ctxt = EGTPU_NULL;

    /* Echo request timer shall be maintained for each of the SGW context 
     * available. If any one of the SGW is down, it shall be running for 
     * other SGWs. Therefore, check for timer expiry for each of the contexts
     * available */
    for( peer_count = 0; 
            peer_count < p_global->initdb_info.num_active_peer_rec;
            peer_count++ )
    {
        p_ctxt = (egtpu_peer_ctxt_t *)(p_global->initdb_info.
                peer_ctxt_addr[peer_count]);
        /* Check for each peer context on the expiry of echo timer */
        if( ( EGTPU_NULL != p_ctxt ) && 
                ( 0 != p_ctxt->path_struct.tmr_id ) && 
                ( p_ctxt->path_struct.tmr_id <= relayGtpuCurrentTick_g ) )
        {
            p_ctxt->path_struct.tmr_id = 0;
            egtpu_echo_timeout(p_global, p_ctxt);
        }
    }
    /* - SPR 21577 */
}

/****************************************************************************
 * Function Name  : egtpu_start_timer 
 * Inputs         : p_global - pointer to the global 
 *                  entity,
 *                  dur - duration of timer
 *                  rstr - to be restarted or not
 *                  buf,
 *                  len,
 *                  tid - timer Id
 * Outputs        : None
 * Returns        : EGTPU SUCCESS/EGTPU FAILURE 
 * Description    : This funtion starts the timer 
 ****************************************************************************/
egtpu_return_t egtpu_start_timer (egtpu_global_t *p_global, egtpu_entity_id_t entity,
        U32bit dur, egtpu_boolean_t rstr,
        U8bit *buf, U16bit len, U32bit *tid)
{
    /*If Echo timer is not running, then only then update it with current tick,
     *if it is already running, the same timer value shall be used for the 
     *new path added, hence both the paths will have timeout at the same tick*/
    /* + SPR 21577 */
    /*A common ECHO_TIMER is used for all existing paths with a single SGW 
     * only if dur is not zero (feature is disabled) */
    if( !(*tid ) && (0 != dur) )
    {
        *tid = relayGtpuCurrentTick_g + ((tickType_t)dur * 1000);
    }
    /* - SPR 21577 */
    LTE_GCC_UNUSED_PARAM(p_global)
        LTE_GCC_UNUSED_PARAM(entity)
        LTE_GCC_UNUSED_PARAM(rstr)
        LTE_GCC_UNUSED_PARAM(buf)
        LTE_GCC_UNUSED_PARAM(len)
    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : egtpu_bzero 
 * Inputs         : tid - timer id 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion stops the timer 
 ****************************************************************************/
/* + SPR 21577 */
egtpu_void_t egtpu_stop_timer (U32bit *tid)
{
    *tid = 0;
}
/* - SPR 21577 */



/****************************************************************************
 * Function Name  : egtpu_hex_dump 
 * Inputs         : buf - pointer to the memory 
 *                  len - size of memory
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS 
 * Description    : 
 ****************************************************************************/
egtpu_return_t egtpu_hex_dump (U8bit *buf, U16bit len)
{
    LTE_GCC_UNUSED_PARAM(buf)
    LTE_GCC_UNUSED_PARAM(len)
    /*qvLogDump (QVLOG_DUMP, buf, len, 0);*/
    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : egtpu_stack_printf 
 * Inputs         : S8bit,fmt,... 
 * Outputs        : None
 * Returns        : S32bit 
 * Description    : wrapper function to printf 
 ****************************************************************************/
S32bit egtpu_stack_printf (const S8bit *fmt, ...)
{
    printf_wrapper(fmt);

    return 0;
}

/****************************************************************************
 * Function Name  : egtpu_send_from_egtpu 
 * Inputs         : p_global,buf,len,free 
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE 
 * Description    : This funtion sends the gtpu data 
 ****************************************************************************/
egtpu_return_t egtpu_send_from_egtpu (egtpu_global_t *p_global, U8bit *buf, U16bit len,
        egtpu_boolean_t free)
{
	U8bit *nbuf = EGTPU_NULL;/*cov 60838 +-*/
	if (!free) {

		nbuf = egtpu_memget (p_global, len, EGTPU_DYNAMIC_BUFFER);
		if (!nbuf){
			qvAbort ();
			return EGTPU_FAILURE;
		}	
		memCpy (nbuf, buf, len);
		buf = nbuf;
	}
	lteRelayMsgHandler(buf);
    if(EGTPU_NULL != nbuf){/*cov 60838 +-*/    
        egtpu_memfree (EGTPU_DYNAMIC_BUFFER, (egtpu_void_t *)nbuf);
        nbuf = EGTPU_NULL;
    }

	return EGTPU_SUCCESS;
}


/****************************************************************************
 * Function Name  : egtpu_search_list 
 * Inputs         : node1 - pointer to the node 
 * Outputs        : None
 * Returns        : 
 * Description    : This funtion search the node from the list 
 ****************************************************************************/
const egtpu_void_t * egtpu_search_list( const YHNODE *node1)
{
 return ( &(YMEMBEROF(egtpu_hash_tbl,lnode,node1)->seq_num));
}


/****************************************************************************
 * Function Name  : egtpu_Compare 
 * Inputs         : id1,id2 
 * Outputs        : None
 * Returns        : S32bit 
 * Description    : This funtion compare the hash id and return the result
 ****************************************************************************/
S32bit egtpu_Compare(const egtpu_void_t *id1,const egtpu_void_t *id2)	
{
	U16bit	  sid1=*(U16bit *)id1;
    U16bit    sid2=*(U16bit *)id2;

    if(sid1>sid2)
			return -1;
	else
		if(sid1==sid2)
		   	return 0;
   	    else
    		return 1;
}

/****************************************************************************
 * Function Name  : egtpu_hash 
 * Inputs         : id
 * Outputs        : None
 * Returns        : hash id 
 * Description    : 
 ****************************************************************************/
unsigned long egtpu_hash(const egtpu_void_t * id)
{
    if (PNULL != id)
    {
        return ((U16bit)*(U16bit *)id);
    }
    else
    {
        return 0;
    }
}

/****************************************************************************
 * Function Name  : egtpu_insert_in_hash_table 
 * Inputs         : p_hash_tbl,H 
 * Outputs        : None
 * Returns        : None 
 * Description    : This funtion insert node into hash table
 ****************************************************************************/
egtpu_void_t egtpu_insert_in_hash_table(egtpu_hash_tbl *p_hash_tbl ,YHASH *H)
{
  yhInsert(H,&p_hash_tbl->lnode);   
}

/****************************************************************************
 * Function Name  : egtpu_find_in_hash_table 
 * Inputs         : H,seq_num,*FOUND
 * Outputs        : FOUND - pointer wheather node found or not
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE 
 * Description    : This funtion find the node in the hash table
 ****************************************************************************/
 egtpu_return_t egtpu_find_in_hash_table(YHASH *H, U16bit seq_num ,YHNODE **FOUND)
{
  *FOUND = EGTPU_NULL;
  
  *FOUND = yhFind(H, (void *)(&seq_num));
  
  if(*FOUND !=EGTPU_NULL)
		return EGTPU_SUCCESS;
  else 
		return EGTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : egtpu_first_pdu_in_hash_table 
 * Inputs         : H 
 * Outputs        : None
 * Returns        : gpdu 
 * Description    : This funtion finds the first pdu in hash table
 ****************************************************************************/
YHNODE *egtpu_first_pdu_in_hash_table(YHASH *H)
{
    YHNODE *gpdu;
    gpdu = yhFirst(H);
    return gpdu;
}
/****************************************************************************
 * Function Name  : egtpu_next_pdu_in_hash_table 
 * Inputs         : H 
 * Outputs        : None
 * Returns        : gpdu 
 * Description    : This funtion finds the first pdu in hash table
 ****************************************************************************/
YHNODE *egtpu_next_pdu_in_hash_table(YHASH *H, YHNODE *N)
{
    YHNODE *gpdu;
    gpdu = yhNext(H ,N );
    return gpdu;
}
