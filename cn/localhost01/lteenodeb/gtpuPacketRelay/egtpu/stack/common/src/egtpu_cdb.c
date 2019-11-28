/***********************************************************************
**  FUNCTION:
**	GTP-U runtime database implementation
************************************************************************
**
**  FILE NAME:
**	egtpu_cdb.c
**
**  DESCRIPTION:
**	Contains routines to manage the GTP-U runtime database.
**
**  DATE	    NAME				REFERENCE	REASON
**  ----	    ----				---------	------
**  Oct 2009    Vipul Aggarwal        			Initial
**  14Jan,10    Vipul Aggarwal        			SPR FIX 7447
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
/*#include "ylib.h"*/
#include <egtpu_glob.h>
#include <egtpu_ietypes.h>
#include <egtpu_error.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_trace.h>
#include <egtpu_tmr.h>
#include <egtpu_util.h>

#include <egtpu_list.h>

#include <egtpu_pmm.h>

/********************************************************************************
 * Function Name  : pool_alloc
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  pool,
 *                  p_size - Pointer to egtpu_size_struct_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion alloc buffer from dynamic memory pool. If no
 *                  buffer is available, it extends the memory pool and returns
 *                  a buffer from it.
 ********************************************************************************/
static egtpu_void_t *pool_alloc (egtpu_global_t *p_global,
							QMPOOL pool, egtpu_size_struct_t *p_size)
{
    egtpu_void_t *p_buf;
    U32bit      incr;

    if (!(p_buf = egtpu_pool_alloc(pool)))
	{
        if (p_size->curr_size == p_size->max_size)
		{
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_INSERT_DB_FAILURE,
                        egtpu_entity_name_arr[EGTPU_EI_GTPU]));

            /* SPR 17747 FIX START */
            LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                    relayGtpuCurrentTick_g, __LINE__, p_size->curr_size,
                    0,0,0,0,0, __func__, "MemFailed");
            /* SPR 17747 FIX END */
			LOG_ID_MSG("replaced id is:%d,logId is:%d, logLev is %d,logCat is:%d, globalTick is:%d,
			intValOne is:%d, intValTwo is %d,intValThree is %d, intValFour is %d,
			intValFive is:%d, floatValOne is %f,floatValTwo is %f, strValOne is %s,
			strValTwo is %s\n",1,logId, logLev, logCat, relayGtpuCurrentTick_g,__LINE__,p_size->curr_size, 
			0,0,0,0,0,__func__,"MemFailed");
			


            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_INSERT_DB_FAIL);
        }
		else
		{
            incr = (p_size->curr_size +
                    p_size->step_size > p_size->max_size) ?
                p_size->max_size - p_size->curr_size :
                p_size->step_size;

            if (!egtpu_pool_extend (pool, incr))
			{
                EGTPU_ERROR_REPORTER (p_global,
									(EGTPU_TS_SERR_STATIC_MEM_NOT_AVAIL,
                            egtpu_entity_name_arr[EGTPU_EI_GTPU], incr));

                /* SPR 17747 FIX START*/
                LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                        relayGtpuCurrentTick_g, __LINE__, incr,0,0,0,0,0,
                        __func__, "MemExtendFail");
                /* SPR 17747 FIX END */

                EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_STATIC_MEMGET_FAIL);
            }
			else
			{
                p_size->curr_size += incr;
                p_buf = egtpu_pool_alloc (pool);
            }
        }
    }
    return p_buf;
}

/********************************************************************************
 * Function Name  : pool_free
 * Inputs         : p_buf - Pointer to egtpu_void_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion frees buffer back to dynamic memory pool.
 ********************************************************************************/
static egtpu_void_t pool_free (egtpu_void_t *p_buf)
{
    egtpu_pool_free (p_buf);
}

/*SPR_20405_START*/
/* rab context list node declaration moved to hdr file*/
/*SPR_20405_END*/

/* Peer context list node */
struct __peer_ctxt_rec__ {
    XLIST            __header__;
    U8bit            __hashed__; /* indicates if node is allocated */
    YTREE            __teid_tree__; /* tree of tunnels with the peer */
    egtpu_peer_ctxt_t ctxt;
};

/***********************************************************************
** DATA FUNCTION:
**     table for egtpu stack databases
***********************************************************************/
STATIC struct {
    egtpu_size_struct_t       rab_pool_size;

    struct __peer_ctxt_rec__ *peer_ctxt_pool;
    XLIST                    alloc_list;
    XLIST                    free_list;
} __egtpu_pool__;

/********************************************************************************
 * Function Name  : egtpu_db_init_rab_ctxt_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS
 * Description    : This funtion Initializes the RAB Context Table/Pool,
 *                  the size of the table is configured using the INIT_DB API.
 ********************************************************************************/
egtpu_return_t egtpu_db_init_rab_ctxt_tbl (egtpu_global_t *p_global)
{
    EGTPU_CREATE_MEM_POOL (p_global,p_global->rab_ctxt_pool,
            sizeof (struct __rab_ctxt_rec__),
            p_global->initdb_info.num_rab_rec.curr_size);

    egtpu_memcpy (&__egtpu_pool__.rab_pool_size,
            &p_global->initdb_info.num_rab_rec,
            sizeof (egtpu_size_struct_t));

    return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_db_destroy_rab_ctxt_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion Deletes the RAB Context Table/Pool,
 *                  This happens when the stack is re-initialized.
 ********************************************************************************/
egtpu_void_t egtpu_db_destroy_rab_ctxt_tbl (egtpu_global_t *p_global)
{
	egtpu_stop_reorder_timers(p_global);
    EGTPU_DELETE_MEM_POOL (p_global->rab_ctxt_pool);
}

/********************************************************************************
 * Function Name  : egtpu_db_get_rab_ctxt
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : p_rec->ctxt - Pointer to RAB context
 * Description    : This funtion finds a free RAB context from the RAB Table/Pool
 *                  and returns a pointer to it.
 ********************************************************************************/
egtpu_rab_ctxt_t *egtpu_db_get_rab_ctxt (egtpu_global_t *p_global)
{
    struct __rab_ctxt_rec__ *p_rec = pool_alloc
        (p_global,p_global->rab_ctxt_pool, &__egtpu_pool__.rab_pool_size);

	if(p_rec) 
	{
        egtpu_init_rab_ctxt_r (&p_rec->ctxt);
        return &p_rec->ctxt;
	}
	else
    	return EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_db_free_rab_ctxt
 * Inputs         : p_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion frees the RAB context i.e. returns the
 *                  RAB context to the pool.
 ********************************************************************************/
egtpu_void_t egtpu_db_free_rab_ctxt (egtpu_rab_ctxt_t *p_ctxt)
{
	struct __rab_ctxt_rec__ *p_rab_rec= 
        (EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__, p_ctxt, ctxt));
    pool_free (p_rab_rec);	
}

/********************************************************************************
 * Function Name  : egtpu_db_get_teid_key_rab_ctxt
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : teid
 * Description    : teid is calculated from the index of the record and an
 *                  internal sequence number.Each time an index is acquired, the
 *                  sequence number (one byte)associated with this index is
 *                  incremented by one.
 ********************************************************************************/
egtpu_teid_t egtpu_db_get_teid_key_rab_ctxt (egtpu_global_t *p_global,
												egtpu_rab_ctxt_t *p_ctxt)
{
    egtpu_teid_t             teid;
    struct __rab_ctxt_rec__ *p_rec =
        EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__, p_ctxt, ctxt);

#ifdef LINUX_PC_TEST
	if ((teid = egtpu_pool_index (p_global->rab_ctxt_pool, p_rec)))
		teid = ((teid & 0x00ffffff) | 0x01000000);
	return teid;
#endif

    if ((teid = egtpu_pool_index (p_global->rab_ctxt_pool, p_rec)))
        teid = (teid & 0x00ffffff) |
            (((U32bit) egtpu_pool_sequence (p_rec)) << 24);

    return teid; /* if teid is null, FSM is playing mischief */
}

/********************************************************************************
 * Function Name  : egtpu_db_sel_teid_key_rab_ctxt
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  teid
 * Outputs        : None
 * Returns        : p_rec->ctxt - RAB Context of provided teid
 * Description    : Given a tunnel endpoint, rab ctxt is located.
 ********************************************************************************/
egtpu_rab_ctxt_t *egtpu_db_sel_teid_key_rab_ctxt (egtpu_global_t *p_global,
														egtpu_teid_t teid)
{
    struct __rab_ctxt_rec__ *p_rec =
        egtpu_pool_locate (p_global->rab_ctxt_pool, teid & 0x00ffffff);

#ifdef LINUX_PC_TEST
	return (p_rec) ? &p_rec->ctxt : EGTPU_NULL;

#endif

    return (p_rec && (((U8bit) egtpu_pool_sequence (p_rec)) ==
                ((U8bit) (teid >> 24)))) ? &p_rec->ctxt : EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_db_get_rab_ctxt_by_idx
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 *                  p_idx
 * Outputs        : None
 * Returns        : p_rec->ctxt/EGTPU_NULL
 * Description    : This funtion provides RAB Context corresponding to 
 *                  provided idx such as 1 as idx for first.
 ********************************************************************************/
egtpu_rab_ctxt_t *egtpu_db_get_rab_ctxt_by_idx (egtpu_global_t *p_global,
															U32bit *p_idx)
{
    struct __rab_ctxt_rec__ *p_rec;
    U32bit                  index;

    for(index=*p_idx; index <= __egtpu_pool__.rab_pool_size.curr_size; index++)
        if ((p_rec = egtpu_pool_locate(p_global->rab_ctxt_pool, index))) {
            *p_idx = index + 1;
            return &p_rec->ctxt;
        }

    return EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_stop_reorder_timers
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion is used to stop the reorder timer.
 ********************************************************************************/
egtpu_void_t egtpu_stop_reorder_timers (egtpu_global_t *p_global)
{
    struct __rab_ctxt_rec__ *p_rec;
    U32bit                  index;
    egtpu_rab_ctxt_t         *p_rab_ctxt;


    for (index = 1; index <= __egtpu_pool__.rab_pool_size.curr_size; index++)
        if ((p_rec = egtpu_pool_locate (p_global->rab_ctxt_pool, index)))
		{

    		p_rab_ctxt = &(p_rec->ctxt);
			if ( p_rab_ctxt == EGTPU_NULL)
			{
				return;
			}	
			if( p_rab_ctxt->p_tid != EGTPU_INVALID_TIMER_ID )
			{
				/*EGTPU_STOP_REORDER_TIMER(p_global, p_rab_ctxt);*/	    
			}
        }
}


typedef S32bit (*cmp_t) (const egtpu_void_t *, const egtpu_void_t *);
typedef const egtpu_void_t *(*keyof_t) (const YTNODE *);

/*****************************************************************************
 * Function Name  : cmp_teid_key_rab_ctxt
 * Inputs         : p_key1,
 *                  p_key2
 * Outputs        : None
 * Returns        : S32bit 
 * Description    : This function returns the result after comaparing the keys
 *****************************************************************************/
S32bit cmp_teid_key_rab_ctxt (const egtpu_void_t *p_key1,
								const egtpu_void_t *p_key2)
{
    /* SPR 13280 fix start */
    SInt64 tempKey1 = 0;
    SInt64 tempKey2 = 0;

    /* Calculate the temporary keys from cellId and peer tunnel Id from the 
     * structures passed */
    tempKey1 = (SInt64)(((egtpu_peer_teid_cell_ctxt_t *)p_key1)->teid_peer) | 
               ((SInt64)(((egtpu_peer_teid_cell_ctxt_t *)p_key1)->cellIndex) <<
                NUM_BITS_IN_TEID );
    tempKey2 = (SInt64)(((egtpu_peer_teid_cell_ctxt_t *)p_key2)->teid_peer) |
               ((SInt64)(((egtpu_peer_teid_cell_ctxt_t *)p_key2)->cellIndex) <<
                NUM_BITS_IN_TEID );
               
    /* key1 is the item, key2 is the value returned from keyof */
    
    if( tempKey1 < tempKey2 )
    {
        return -1 ;
    }
    else if( tempKey1 > tempKey2 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
    /* SPR 13280 fix end */
}

/********************************************************************************
 * Function Name  : teid_keyof_rab_ctxt
 * Inputs         : node - Constant pointer to YTNODE
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion return the teid key of rab context 
 ********************************************************************************/
const egtpu_void_t *teid_keyof_rab_ctxt (const YTNODE *node)
{
    /* SPR 10787 fix start */
    return (egtpu_void_t *) &((EGTPU_GET_STR_FROM_ELMT
            (struct __rab_ctxt_rec__, node, __node__))->ctxt.teid_cell);
    /* SPR 10787 fix end */
}

/********************************************************************************
 * Function Name  : egtpu_db_init_peer_ctxt_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion Initializes the Peer Context Table/Pool,
 *                  The size of the table is configured using the INITDB_REQ API.
 ********************************************************************************/
egtpu_return_t egtpu_db_init_peer_ctxt_tbl (egtpu_global_t *p_global)
{
    struct __peer_ctxt_rec__ *p_tmp;
    U32bit                   num_peer = p_global->initdb_info.num_peer_rec;

    EGTPU_GET_MSG_BUF (p_global,struct __peer_ctxt_rec__,
            __egtpu_pool__.peer_ctxt_pool, num_peer);
    /*++klockwork warning */
    if(EGTPU_NULL == __egtpu_pool__.peer_ctxt_pool)
    {
        return EGTPU_FAILURE;
    }
    /*--klockwork warning */
    xlInit (&__egtpu_pool__.alloc_list);
    xlInit (&__egtpu_pool__.free_list);

    for (p_tmp = __egtpu_pool__.peer_ctxt_pool; num_peer; num_peer--, p_tmp++)
	{
        xlPushTail (&__egtpu_pool__.free_list, &p_tmp->__header__);
        p_tmp->__hashed__ = 0;
        ytInit (&p_tmp->__teid_tree__,
                (cmp_t) cmp_teid_key_rab_ctxt,
                (keyof_t) teid_keyof_rab_ctxt);
        egtpu_bzero(p_tmp->ctxt.entity_addr, EGTPU_MAX_IP6_ADDR_SIZE);
    }

    return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_db_init_hash_ctxt_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion initialise the hash table 
 ********************************************************************************/
egtpu_return_t egtpu_db_init_hash_ctxt_tbl (egtpu_global_t *p_global )
{
  	if(!cvContextPoolSetup(&p_global->hash_ctxt_pool ,sizeof(egtpu_hash_tbl),
					EGTPU_INIT_HASH_CTXT_POOL,EGTPU_MAX_HASH_CTXT_POOL * 4,
												EGTPU_INIT_HASH_CTXT_POOL))
    {
        EGTPU_ERROR_REPORTER(p_global,
						(EGTPU_TS_SERR_CONTEXT_POOL_MEM_NOT_AVAIL_FOR_HASH,
						egtpu_entity_name_arr[EGTPU_EI_GTPU],
						sizeof(egtpu_hash_tbl)*EGTPU_INIT_HASH_CTXT_POOL));
       
        /* SPR 17747 FIX START*/
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                sizeof(egtpu_hash_tbl) * EGTPU_INIT_HASH_CTXT_POOL,0,0,0, 
                0,0, __func__, "HashCtxCreateFail");
        /* SPR 17747 FIX END */
       
        EGTPU_UPDATE_STATS(p_global ,EGTPU_STAT_HASH_CTXT_MEMGET_FAIL);
        return EGTPU_FAILURE;
    }
	return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_db_init_hash_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion initialise the hash table 
 ********************************************************************************/
egtpu_return_t egtpu_db_init_hash_tbl (egtpu_global_t *p_global )
{
  	if(!cvContextPoolSetup(&p_global->hash_pool,sizeof(YHASH),
					EGTPU_INIT_HASH_CTXT_POOL,EGTPU_MAX_HASH_CTXT_POOL,
					EGTPU_INIT_HASH_CTXT_POOL))
    {
        EGTPU_ERROR_REPORTER(p_global,
					(EGTPU_TS_SERR_CONTEXT_POOL_MEM_NOT_AVAIL_FOR_HASH,
					egtpu_entity_name_arr[EGTPU_EI_GTPU],
					sizeof(YHASH)*EGTPU_INIT_HASH_CTXT_POOL));
       
        /* SPR 17747 FIX START*/
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                sizeof(YHASH) * EGTPU_INIT_HASH_CTXT_POOL,0,0,0, 
                0,0, __func__, "HashInitFail");
        /* SPR 17747 FIX END */
       
        EGTPU_UPDATE_STATS(p_global ,EGTPU_STAT_HASH_CTXT_MEMGET_FAIL);
        return EGTPU_FAILURE;
    }
	return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_init_hash_table
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion initialise the hash table 
 ********************************************************************************/
egtpu_return_t egtpu_init_hash_table (egtpu_global_t *p_global)
{
  	if(!cvContextPoolSetup(&p_global->table_pool,
							sizeof(YHNODE *) * EGTPU_INIT_WINDOW_SZ,
							EGTPU_INIT_HASH_CTXT_POOL,
							EGTPU_MAX_HASH_CTXT_POOL,
							EGTPU_INIT_HASH_CTXT_POOL))
    {
        EGTPU_ERROR_REPORTER(p_global,
						(EGTPU_TS_SERR_CONTEXT_POOL_MEM_NOT_AVAIL_FOR_HASH,
						egtpu_entity_name_arr[EGTPU_EI_GTPU],
						sizeof(YHNODE *)*EGTPU_INIT_HASH_CTXT_POOL));
       
        /* SPR 17747 FIX START*/
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                sizeof(YHNODE *) * EGTPU_INIT_HASH_CTXT_POOL,0,0,0, 
                0,0, __func__, "HashNodeFail");
        /* SPR 17747 FIX END */
        EGTPU_UPDATE_STATS(p_global ,EGTPU_STAT_HASH_CTXT_MEMGET_FAIL);
        return EGTPU_FAILURE;
    }
	return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_db_init_timer_pool
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion Initializes the timer pool.
 ********************************************************************************/
egtpu_return_t egtpu_db_init_timer_pool (egtpu_global_t *p_global)
{
  	if(!cvContextPoolSetup(&p_global->tpool, EGTPU_TIMER_MSG_SIZE,
					EGTPU_INIT_TIMER_POOL_SIZE, EGTPU_MAX_TIMER_POOL_SIZE,
					EGTPU_INIT_TIMER_POOL_SIZE))
    {
        EGTPU_ERROR_REPORTER(p_global,
				(EGTPU_TS_SERR_CONTEXT_POOL_MEM_NOT_AVAIL_FOR_TIMER,
				egtpu_entity_name_arr[EGTPU_EI_GTPU],
				EGTPU_INIT_TIMER_POOL_SIZE));
       
        /* SPR 17747 FIX START*/
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                EGTPU_INIT_TIMER_POOL_SIZE,0,0,0, 
                0,0, __func__, "InitTimerPoolFail");
        /* SPR 17747 FIX END */
       
        EGTPU_UPDATE_STATS(p_global ,EGTPU_STAT_TIMER_CTXT_MEMGET_FAIL);
        return EGTPU_FAILURE;
    }
	return EGTPU_SUCCESS;
}

/*+SPR #3456*/
/********************************************************************************
 * Function Name  : egtpu_shift_next_peer_ctxt_left
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  index
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion shifts left the entries in peer_ctxt_addr
 *                  table after deleting the candidate peer_ctxt_addr.
 ********************************************************************************/
egtpu_void_t egtpu_shift_next_peer_ctxt_left(egtpu_global_t *p_global,UInt8 index)
{
  UInt8 num_active_peer = p_global->initdb_info.num_active_peer_rec;  
  do
  {
      p_global->initdb_info.peer_ctxt_addr[index] =
          p_global->initdb_info.peer_ctxt_addr[index+1];

      index++;
      p_global->initdb_info.peer_ctxt_addr[index] = NULL;

  }while((index+1) != num_active_peer);

   /*Number of active peer records reduced by 1*/
   p_global->initdb_info.num_active_peer_rec--;   
}

/********************************************************************************
 * Function Name  : egtpu_db_init_peer_ctxt_addr_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion Initializes the peer context table.
 ********************************************************************************/
egtpu_return_t egtpu_db_init_peer_ctxt_addr_tbl(egtpu_global_t *p_global)
{
    UInt8 i;
    /* SPR 4047 Fix Start */
    for(i=0;i < EGTPU_MAX_PEER_CTXT;i++)
    /* SPR 4047 Fix End */
        p_global->initdb_info.peer_ctxt_addr[i]= NULL;

    p_global->initdb_info.num_active_peer_rec = 0;

	return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_free_peer_ctxt_addr_tbl
 * Inputs         : p_ctxt - Pointer to egtpu_peer_ctxt_t,
 *                  p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion deletes the entry in  peer context address
 *                  table when the path gets deleted.
 ********************************************************************************/
egtpu_void_t egtpu_free_peer_ctxt_addr_tbl(egtpu_peer_ctxt_t *p_ctxt,
                                               egtpu_global_t *p_global)

{
   UInt8 i = 0; 

   for(i=0;i<p_global->initdb_info.num_active_peer_rec;i++)
   {
       if(p_global->initdb_info.peer_ctxt_addr[i] == (UInt32*)p_ctxt)
       {
           if(NULL != p_global->initdb_info.peer_ctxt_addr[i+1])
           {
               egtpu_shift_next_peer_ctxt_left(p_global,i);
               break;
           }
           else
           {
               p_global->initdb_info.peer_ctxt_addr[i] = NULL;
               p_global->initdb_info.num_active_peer_rec--;
               break;
           }

       }
   }

}
/*-SPR #3456*/
/********************************************************************************
 * Function Name  : egtpu_db_destroy_hash_ctxt_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion delete the node in the pool
 ********************************************************************************/
egtpu_void_t egtpu_db_destroy_hash_ctxt_tbl(egtpu_global_t *p_global)
{
    cvContextPoolDelete( &p_global->hash_ctxt_pool );
}

/********************************************************************************
 * Function Name  : egtpu_db_destroy_hash_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion delete the node in the pool
 ********************************************************************************/
egtpu_void_t egtpu_db_destroy_hash_tbl(egtpu_global_t *p_global)
{
    cvContextPoolDelete( &p_global->hash_pool );
}

/********************************************************************************
 * Function Name  : egtpu_destroy_hash_table
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion delete the node in the pool
 ********************************************************************************/
egtpu_void_t egtpu_destroy_hash_table(egtpu_global_t *p_global)
{
    cvContextPoolDelete( &p_global->table_pool );
}

/********************************************************************************
 * Function Name  : egtpu_destroy_timer_pool
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion delete the timer node in the pool 
 ********************************************************************************/
egtpu_void_t egtpu_destroy_timer_pool(egtpu_global_t *p_global)
{
    cvContextPoolDelete( &p_global->tpool );
}

/********************************************************************************
 * Function Name  : egtpu_init_hash
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 *                  H - Pointer to YHASH
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS
 * Description    : This funtion initialises the hash table 
 ********************************************************************************/
egtpu_return_t egtpu_init_hash(egtpu_global_t *p_global, YHASH *H )
{
  	YHNODE **table;
	EGTPU_GET_TABLE(p_global, p_global->table_pool, table); 
	yhInit(H, EGTPU_INIT_WINDOW_SZ, egtpu_hash, egtpu_Compare,
							egtpu_search_list, table);
	return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_db_destroy_peer_ctxt_tbl
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion destroys/clears the peer context table/pool.
 ********************************************************************************/
egtpu_void_t egtpu_db_destroy_peer_ctxt_tbl (egtpu_global_t *p_global)
{
    /* play a bit of fsm-role first... cant help */

    /* in case of re-init, previous echo timers shall be stopped */
    /* this is a weird exercise */
    /* good sense will be: re-run the stack */

    if (__egtpu_pool__.peer_ctxt_pool)
	{
        XLIST *L = &__egtpu_pool__.alloc_list;

        while ((L = L->next) != &__egtpu_pool__.alloc_list)
		{
            egtpu_peer_ctxt_t *p_ctxt = &(EGTPU_GET_STR_FROM_ELMT
                    (struct __peer_ctxt_rec__, L, __header__))->ctxt;

            /* assumed that on acquiring peer-ctxt record,
             * fsm will set tmr_flag if timer is being started...
             * otherwise, tmr_flag shall be made 0 by fsm */
            if (p_ctxt->path_struct.tmr_flag == EGTPU_TIMER_ON)
	    {		
                /*EGTPU_STOP_ECHO_TIMER (p_global,&p_ctxt->path_struct);*/
            }
        }
    }
    /* GCC_4.9_WARNINGS_FIX_START */
  LTE_GCC_UNUSED_PARAM(p_global)
    /* GCC_4.9_WARNINGS_FIX_END */
    EGTPU_FREE_MSG_BUF (__egtpu_pool__.peer_ctxt_pool);
}

/********************************************************************************
 * Function Name  : egtpu_db_get_peer_ctxt
 * Inputs         : p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : p_rec->ctxt - Peer context pointer/EGTPU_NULL
 * Description    : This funtion returns peer context pointer.
 ********************************************************************************/
egtpu_peer_ctxt_t *egtpu_db_get_peer_ctxt (egtpu_global_t *p_global)
{
    XLIST *L = xlPopHead (&__egtpu_pool__.free_list);

    if (L)
	{
        struct __peer_ctxt_rec__ *p_rec = EGTPU_GET_STR_FROM_ELMT
            (struct __peer_ctxt_rec__, L, __header__);

        xlPushTail (&__egtpu_pool__.alloc_list, L);

        p_rec->__hashed__ = 1;
        return &p_rec->ctxt;
    }
	EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_INSERT_DB_FAILURE,
                egtpu_entity_name_arr[EGTPU_EI_GTPU]));

    /* SPR 17747 FIX START*/
    LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
            relayGtpuCurrentTick_g, __LINE__, EGTPU_DBERR_INSERT_DB_FAILURE,
            0,0,0,0,0, __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU]);
    /* SPR 17747 FIX END */

    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_INSERT_DB_FAIL);

	return EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_db_free_peer_ctxt
 * Inputs         : p_ctxt - Pointer to egtpu_peer_ctxt_t
 *                  p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion frees peer context pointer.
 ********************************************************************************/
egtpu_void_t egtpu_db_free_peer_ctxt (egtpu_peer_ctxt_t *p_ctxt,
        egtpu_global_t *p_global)
{
    struct __peer_ctxt_rec__ *p_rec =
        EGTPU_GET_STR_FROM_ELMT (struct __peer_ctxt_rec__, p_ctxt, ctxt);

    p_rec->__hashed__ = 0;
    xlDelete (&p_rec->__header__);
    xlPushTail (&__egtpu_pool__.free_list, &p_rec->__header__);
    /* GCC_4.9_WARNINGS_FIX_START */
  LTE_GCC_UNUSED_PARAM(p_global)
    /* GCC_4.9_WARNINGS_FIX_END */
}

/********************************************************************************
 * Function Name  : egtpu_db_sel_addr_key_peer_ctxt
 * Inputs         : p_addr - Peer address
 * Outputs        : None
 * Returns        : p_ctxt/EGTPU_NULL
 * Description    : This funtion searches for the peer context based on the
 *                  address of the peer.
 ********************************************************************************/
egtpu_peer_ctxt_t *egtpu_db_sel_addr_key_peer_ctxt (U8bit *p_addr)
{
    XLIST *L = &__egtpu_pool__.alloc_list;

    while ((L = L->next) != &__egtpu_pool__.alloc_list)
	{
        egtpu_peer_ctxt_t *p_ctxt = &(EGTPU_GET_STR_FROM_ELMT
                (struct __peer_ctxt_rec__, L, __header__))->ctxt;
        /* Coverity 72972 Fix Start*/
        if( (EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH) > EGTPU_SIZE_OF_IPV6_ADDR )
        {
            return EGTPU_NULL;
        }
        /* Coverity 72972 Fix End*/

        if (!egtpu_memcmp (p_ctxt->entity_addr,
                    p_addr, EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH))
		{
            return p_ctxt;
        }
    }

    return EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_db_validate_peer_ctxt
 * Inputs         : p_ctxt - Pointer to egtpu_peer_ctxt_t
 * Outputs        : None
 * Returns        : egtpu_boolean_t 
 * Description    : This funtion is a wrapper to find if context is valid.
 *                  wrapper is being provided as data to tmr for echo is
 *                  peer ctxt ptr itself. so to avoid crashes in
 *                  misbehaviour cases, a shield of check is being provided.
 ********************************************************************************/
egtpu_boolean_t egtpu_db_validate_peer_ctxt (egtpu_peer_ctxt_t *p_ctxt)
{
    return EGTPU_GET_STR_FROM_ELMT
        (struct __peer_ctxt_rec__, p_ctxt, ctxt)->__hashed__;
}

/********************************************************************************
 * Function Name  : egtpu_db_get_first_peer_ctxt
 * Inputs         : None
 * Outputs        : None
 * Returns        : NULL/pointer to context 
 * Description    : This funtion retuns the first peer context 
 ********************************************************************************/
/* iteration support being provided to support Query Interface */
egtpu_peer_ctxt_t *egtpu_db_get_first_peer_ctxt ()
{
    return (xlEmpty (&__egtpu_pool__.alloc_list)) ?
        EGTPU_NULL :
        &(EGTPU_GET_STR_FROM_ELMT (struct __peer_ctxt_rec__,
                    __egtpu_pool__.alloc_list.next, __header__))->ctxt;
}

/********************************************************************************
 * Function Name  : egtpu_db_get_next_peer_ctxt
 * Inputs         : p_ctxt - Pointer to egtpu_peer_ctxt_t
 * Outputs        : None
 * Returns        : NULL/pointer to context 
 * Description    : This funtion retuns the next peer context 
 ********************************************************************************/
/* return next to p_ctxt */
egtpu_peer_ctxt_t *egtpu_db_get_next_peer_ctxt (egtpu_peer_ctxt_t *p_ctxt)
{
    XLIST *node = (EGTPU_GET_STR_FROM_ELMT (struct __peer_ctxt_rec__,
                p_ctxt, ctxt))->__header__.next;

    return (node == &__egtpu_pool__.alloc_list) ?
        EGTPU_NULL :
        &(EGTPU_GET_STR_FROM_ELMT (struct __peer_ctxt_rec__,
                    node, __header__))->ctxt;
}

/********************************************************************************
 * Function Name  : egtpu_db_ins_teid_peer_key_peer_map
 * Inputs         : p_peer - Pointer to egtpu_peer_ctxt_t,
 *                  p_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS
 * Description    : This funtion inserts 'node' present in p_ctxt into 'peer' map.
 ********************************************************************************/
/* teid-peer maintenance */
egtpu_return_t egtpu_db_ins_teid_peer_key_peer_map (egtpu_peer_ctxt_t *p_peer,
													egtpu_rab_ctxt_t *p_ctxt)
{
    YTNODE *node = &(EGTPU_GET_STR_FROM_ELMT
            (struct __rab_ctxt_rec__, p_ctxt, ctxt))->__node__;

    /*SPR_20405_START*/
    ytInsertWithDuplicate (&(EGTPU_GET_STR_FROM_ELMT (struct __peer_ctxt_rec__,
				    p_peer, ctxt))->__teid_tree__, node);
    /*SPR_20405_END*/

    return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_db_del_teid_peer_key_peer_map
 * Inputs         : p_peer - Pointer to egtpu_peer_ctxt_t,
 *                  p_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion deletes 'node' present in p_ctxt into 'peer' map.
 ********************************************************************************/
egtpu_void_t egtpu_db_del_teid_peer_key_peer_map (egtpu_peer_ctxt_t *p_peer,
													egtpu_rab_ctxt_t *p_ctxt)
{
    YTNODE *node = &(EGTPU_GET_STR_FROM_ELMT
            (struct __rab_ctxt_rec__, p_ctxt, ctxt))->__node__;

    ytDelete (&(EGTPU_GET_STR_FROM_ELMT (struct __peer_ctxt_rec__,
                    p_peer, ctxt))->__teid_tree__, node);
}

/*SPR_20405_START*/
/********************************************************************************
 * Function Name  : egtpu_db_check_teid_peer_key_peer_map
 * Inputs         : p_peer - Pointer to egtpu_peer_ctxt_t,
 *                  teid - Tunnel endpoint ID
 *                  prev_node- pointer to the previous matching node
 * Outputs        : None
 * Returns        : matching rab_ctxt node/EGTPU_NULL
 * Description    : This funtion searches 'teid' in 'peer' map,
 *                  returns rab_ctxt node if found.
 ********************************************************************************/
YTNODE *egtpu_db_check_teid_peer_key_peer_map(egtpu_peer_ctxt_t *p_peer,
		egtpu_teid_t teid,
		YTNODE *prev_node)
{
	YTNODE *p_node = EGTPU_NULL;
	if(EGTPU_NULL == prev_node)
	{
		prev_node = ytFirst(&(EGTPU_GET_STR_FROM_ELMT
					(struct __peer_ctxt_rec__, p_peer, ctxt))->__teid_tree__,
				T_INORDER);

		if((prev_node) && (teid == 
					(EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__,prev_node,__node__))->ctxt.teid_cell.teid_peer))
			return prev_node;
	}
	if(EGTPU_NULL != prev_node)
	{
		do
		{
			p_node = ytNext(prev_node,T_INORDER);
			if(p_node)
			{
				if(teid == 
						(EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__,p_node,__node__))->ctxt.teid_cell.teid_peer)
					break;
				else
					prev_node = p_node;
			}

		}while(p_node);
	}

	return p_node;

}
/*SPR_20405_END*/
/********************************************************************************
 * Function Name  : egtpu_db_sel_teid_peer_key_peer_map
 * Inputs         : p_peer - Pointer to egtpu_peer_ctxt_t,
 *                  teid - Tunnel endpoint ID
 * Outputs        : None
 * Returns        : rab_ctxt/EGTPU_NULL
 * Description    : This funtion EGTPU_NULLsearches 'teid' in 'peer' map,
 *                  returns rab_ctxt if found.
 ********************************************************************************/
egtpu_rab_ctxt_t *egtpu_db_sel_teid_peer_key_peer_map(egtpu_peer_ctxt_t *p_peer,
        /* SPR 10787 fix start */
        egtpu_peer_teid_cell_ctxt_t teid_cell)
        /* SPR 10787 fix end */
{
    YTNODE *node = ytFind (&(EGTPU_GET_STR_FROM_ELMT
                (struct __peer_ctxt_rec__, p_peer, ctxt))->__teid_tree__,
            /* SPR 10787 fix start */
            (egtpu_void_t *) &teid_cell);
            /* SPR 10787 fix end */

    if (node)
        return &(EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__,
                    node, __node__))->ctxt;

    return EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_db_get_first_peer_map
 * Inputs         : p_addr - Pointer to Peer's address,
 *                  teid - Tunnel endpoint ID,
 *                  presence_teid
 * Outputs        : None
 * Returns        : rab_ctxt/EGTPU_NULL
 * Description    : In this function iteration being provided for QI if provided
 *                  a teid, search for teid in teid_tree else extracts first
 *                  entry in 'peer' map, returns rab_ctxt if found.
 ********************************************************************************/
egtpu_rab_ctxt_t *egtpu_db_get_first_peer_map
(U8bit *p_addr,
 /* SPR 10787 fix start */
egtpu_peer_teid_cell_ctxt_t teid_cell,
 /* SPR 10787 fix end */
U32bit presence_teid)
{
    struct __peer_ctxt_rec__ *p_rec;
    egtpu_peer_ctxt_t         *p_peer;
    YTNODE                   *node = EGTPU_NULL;

    if ((p_peer = egtpu_db_sel_addr_key_peer_ctxt (p_addr))) {
        p_rec = EGTPU_GET_STR_FROM_ELMT
            (struct __peer_ctxt_rec__, p_peer, ctxt);

        node = (presence_teid) ?
            /* SPR 10787 fix start */
            ytFind (&p_rec->__teid_tree__, (egtpu_void_t *) &teid_cell) :
            /* SPR 10787 fix end */
            ytFirst (&p_rec->__teid_tree__, T_INORDER);
    }

    return (node) ? &(EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__,
                node, __node__))->ctxt : EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_db_get_next_peer_map
 * Inputs         : p_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : rab_ctxt/EGTPU_NULL
 * Description    : This funtion takes in rab-ctxt, extracts next  in 'peer' map,
 *                  returns rab_ctxt if found.
 ********************************************************************************/
egtpu_rab_ctxt_t *egtpu_db_get_next_peer_map (egtpu_rab_ctxt_t *p_ctxt)
{
    YTNODE *node = &(EGTPU_GET_STR_FROM_ELMT
            (struct __rab_ctxt_rec__, p_ctxt, ctxt))->__node__;

    return (node = ytNext (node, T_INORDER)) ?
        &(EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__,
                    node, __node__))->ctxt :
        EGTPU_NULL;
}

/********************************************************************************
 * Function Name  : egtpu_init_rab_ctxt_r
 * Inputs         : p_rab_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initializes the rab context database.
 ********************************************************************************/
egtpu_void_t egtpu_init_rab_ctxt_r (egtpu_rab_ctxt_t *p_rab_ctxt)
{
    p_rab_ctxt->transaction_id = EGTPU_NULL;
    /* SPR 10787 fix start */
    p_rab_ctxt->teid_cell.cellIndex = INVALID_CELL_INDEX;
    p_rab_ctxt->teid_cell.teid_peer = EGTPU_NULL;
    /* SPR 10787 fix end */
    p_rab_ctxt->teid_peer_fwd = EGTPU_NULL;
    p_rab_ctxt->p_transport_addr = EGTPU_NULL;
    p_rab_ctxt->p_forward_addr = EGTPU_NULL;
    p_rab_ctxt->seq_num_uplk = EGTPU_MIN_SEQ_NUM;
    p_rab_ctxt->seq_num_dnlk = EGTPU_MIN_SEQ_NUM;
    p_rab_ctxt->sap_flag = EGTPU_NULL;
    p_rab_ctxt->seq_disable_flag = EGTPU_SEQ_ENABLE;
    p_rab_ctxt->qos_id = EGTPU_INVALID_QOS_ID;
    p_rab_ctxt->reorder_timer_drtn = EGTPU_NULL;
    p_rab_ctxt->p_tid = EGTPU_INVALID_TIMER_ID;
    p_rab_ctxt->nxt_seq = EGTPU_MIN_SEQ_NUM;
    p_rab_ctxt->max_window_size = EGTPU_NULL;
    p_rab_ctxt->guard_arr[0] = EGTPU_INV_SEQ_NUM;
	p_rab_ctxt->guard_arr[1] = EGTPU_INV_SEQ_NUM;
	p_rab_ctxt->guard_arr[2] = EGTPU_INV_SEQ_NUM;
	p_rab_ctxt->guard_arr[3] = EGTPU_INV_SEQ_NUM;
    p_rab_ctxt->max_seq_num = EGTPU_INV_SEQ_NUM;
    p_rab_ctxt->reordering_reqd =EGTPU_NULL; 
    p_rab_ctxt->ip_traffic = EGTPU_NULL;   
    p_rab_ctxt->p_buf = EGTPU_NULL; 
    p_rab_ctxt->end_marker_flag = EGTPU_NULL; 
    p_rab_ctxt->H = EGTPU_NULL; /* SPR FIX 7447 */
    egtpu_bzero(p_rab_ctxt->src_addr, EGTPU_MAX_IP6_ADDR_SIZE);/* SPR FIX 7447 */
    /* LIPA changes start */
    p_rab_ctxt->p_secondary_transport_addr = EGTPU_NULL; 
    /* LIPA changes end */
}
