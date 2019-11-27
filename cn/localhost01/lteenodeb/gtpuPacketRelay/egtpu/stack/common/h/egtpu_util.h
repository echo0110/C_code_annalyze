/***********************************************************************
**  FUNCTION:
**	Common Utility Functions
************************************************************************
**
**  FILE NAME:
**	egtpu_util.h
**
**  DESCRIPTION:
**	Contains the Common Utility Functions
**
**  DATE	    NAME			REFERENCE	REASON
**  ----	    ----			---------	------
**  Oct 2009    Vipul Aggarwal        		Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_UTIL_DECL__
#define __EGTPU_UTIL_DECL__

extern U16bit const egtpu_in_api_base_arr[];
extern U16bit const egtpu_in_last_api[];
extern U16bit const egtpu_out_api_base_arr[];
/*+ SPR 21550 fix start*/
/*+ SPR 21550 fix end*/

/* macro to create dynamic memory pool */
#define EGTPU_CREATE_MEM_POOL(p_global,pool, size, nbuf)                         \
    do {                                                               \
        if (((pool) = egtpu_pool_create (size, nbuf)) == EGTPU_NULL) {       \
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_SERR_DYN_MEM_NOT_AVAIL,      \
                egtpu_entity_name_arr[EGTPU_EI_GTPU],                    \
                ((size) * (nbuf))));                                   \
            /* SPR 17747 FIX START*/\
            LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON, \
                    relayGtpuCurrentTick_g, __LINE__, EGTPU_SERR_DYN_MEM_NOT_AVAIL,\
                    ((size) * (nbuf)),0,0,0,0, __func__, \
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]);\
            /* SPR 17747 FIX END */\
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_STATIC_MEMGET_FAIL);          \
            return EGTPU_FAILURE;                                       \
        }                                                              \
    } while (0)

/* macro to delete dynamic memory pool */
#define EGTPU_DELETE_MEM_POOL(pool)                                     \
    do {                                                               \
        if (pool) {                                                    \
            egtpu_pool_delete (pool);                                       \
            pool = EGTPU_NULL;                                          \
        }                                                              \
    } while (0)

/* macros to get/free static memory */
#define EGTPU_GET_STATIC_MEM(p_global,type, ptr, n)                              \
    do {                                                               \
	if (((ptr) = (type *) egtpu_memget (p_global,(n) * sizeof (type),        \
                        EGTPU_STATIC_BUFFER)) == EGTPU_NULL) {           \
	    EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_SERR_STATIC_MEM_NOT_AVAIL,   \
                        egtpu_entity_name_arr[EGTPU_EI_GTPU],            \
                        ((n) * sizeof (type))));                       \
            /* SPR 17747 FIX START*/\
            LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON, \
                    relayGtpuCurrentTick_g, __LINE__, EGTPU_SERR_STATIC_MEM_NOT_AVAIL,\
                    ((n) * sizeof (type)),0,0,0,0, __func__, \
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]);\
            /* SPR 17747 FIX END */\
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_STATIC_MEMGET_FAIL);          \
	    /*return EGTPU_FAILURE;                             */          \
	}                                                              \
    } while (0)

#define EGTPU_FREE_STATIC_MEM(ptr)                                      \
    do {                                                               \
	if (ptr) {                                                     \
	    egtpu_memfree (EGTPU_STATIC_BUFFER, ptr);                    \
	    ptr = EGTPU_NULL;                                           \
	}                                                              \
    } while (0)

/* macros to get/free dynamic memory */
#define EGTPU_GET_MSG_BUF(p_global,type_m, ptr_m, n)                             \
    do {                                                               \
        if (((ptr_m) = (type_m *) egtpu_memget (p_global,(n) * sizeof (type_m),  \
                        EGTPU_DYNAMIC_BUFFER)) == EGTPU_NULL) {          \
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_SERR_DYN_MEM_NOT_AVAIL,      \
                        egtpu_entity_name_arr[EGTPU_EI_GTPU],            \
                        ((n) * sizeof (type_m))));                     \
            /* SPR 17747 FIX START*/\
            LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON, \
                    relayGtpuCurrentTick_g, __LINE__, EGTPU_SERR_DYN_MEM_NOT_AVAIL,\
                    ((n) * sizeof (type_m)),0,0,0,0, __func__, \
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]);\
            /* SPR 17747 FIX END */\
                                                                       \
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_DYNAMIC_MEMGET_FAIL);         \
           /* return EGTPU_FAILURE; */                                      \
        }                                                              \
    } while (0)

#define EGTPU_FREE_MSG_BUF(ptr) egtpu_memfree (EGTPU_DYNAMIC_BUFFER, ptr)

/** p_src_addr is used only when dst is UDP; for all other external dst, it is set to NULL **/
/* +- SPR 17777*/
#define EGTPU_SEND_FROM_GTPU(p_global,p_buf, length, dst_id, ret_val, p_src_addr)            \
    do {                                                               \
        if ((ret_val = egtpu_send_from_egtpu (p_global, p_buf, length,             \
                        EGTPU_TRUE)) == EGTPU_FAILURE) {         \
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_CERR_SEND_MSG_FAILURE,       \
                        egtpu_entity_name_arr[EGTPU_EI_GTPU],            \
                        egtpu_out_api_name_arr [dst_id]                 \
                        [EGTPU_API_GET_API_ID (p_buf) -                 \
                        egtpu_out_api_base_arr [dst_id]]));             \
            /* SPR 17747 FIX START*/\
            LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON, \
                    relayGtpuCurrentTick_g, __LINE__, EGTPU_CERR_SEND_MSG_FAILURE,\
                    dst_id,0,0,0,0, __func__, \
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]);\
            /* SPR 17747 FIX END */\
                                                                       \
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_SEND_FAIL);                   \
        } else {                                                       \
            EGTPU_PRINT_OUT_TRACE (p_global,p_buf);                              \
        }                                                              \
    } while (0)
/* +- SPR 17777*/


#define EGTPU_START_ECHO_TIMER(p_global,data, path, res)                         \
    do {                                                               \
        U8bit tbuf[EGTPU_ECHO_TMR_BUFFER_SIZE];                         \
                                                                       \
    /* SPR 20430 Changes Start */\
        EGTPU_SET_ADDRESS (tbuf, data);                         \
                                                                       \
    /* SPR 20430 Changes Start */\
        if (((res) = egtpu_start_timer (p_global,EGTPU_EI_GTPU,                   \
            p_global->tmr_info[EGTPU_ECHO_RSP_TMR].tmr_dur,EGTPU_FALSE,    \
                        tbuf, EGTPU_ECHO_TMR_BUFFER_SIZE,   \
                        &(path)->tmr_id)) != EGTPU_SUCCESS) {           \
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_SERR_START_TIMER_FAILURE,    \
                        egtpu_entity_name_arr[EGTPU_EI_GTPU],            \
                        "ECHO_TIMER"));                                \
            /* SPR 17747 FIX START */\
            LOG_MSG(GTPU_ECHO_TIMER_START_FAILURE, LOGWARNING, PR_GTPU_DL_DATA, \
                    relayGtpuCurrentTick_g, __LINE__,0,0,0,0,0,0,\
                    __func__, "EchoTimer"); \
            /* SPR 17747 FIX END */\
                                                                       \
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_START_TIMER_FAIL);            \
        } else {                                                       \
            EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL,                  \
                             EGTPU_FSM_TRACE,                           \
                             (EGTPU_TS_START_TIMER,                     \
                              egtpu_entity_name_arr[EGTPU_EI_GTPU],      \
                              "ECHO_TIMER"));                          \
                                                                       \
            (path)->tmr_flag = EGTPU_TIMER_ON;                          \
        }                                                              \
    } while (0)


#define EGTPU_START_REORDER_TIMER(p_global, p_rab_ctxt)                         \
    do {                                                               \
                                                                       \
                                                                       \
        if (( egtpu_start_reorder_timer (p_global,                    \
                        p_rab_ctxt)) != EGTPU_SUCCESS) {           \
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_SERR_START_TIMER_FAILURE,    \
                        egtpu_entity_name_arr[EGTPU_EI_GTPU],            \
                        "REORDER_TIMER"));                                \
            /* SPR 17747 FIX START */\
            LOG_MSG(GTPU_ECHO_TIMER_START_FAILURE, LOGWARNING, PR_GTPU_DL_DATA, \
                    relayGtpuCurrentTick_g, __LINE__,0,0,0,0,0,0,\
                    __func__, "ReorderTimer"); \
            /* SPR 17747 FIX END */\
                                                                       \
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_START_TIMER_FAIL);            \
        } else {                                                       \
            EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL,                  \
                             EGTPU_FSM_TRACE,                           \
                             (EGTPU_TS_START_TIMER,                     \
                              egtpu_entity_name_arr[EGTPU_EI_GTPU],      \
                              "REORDER_TIMER"));                          \
                                                                       \
        }                                                              \
    } while (0)


#define EGTPU_STOP_ECHO_TIMER(p_global,path)                                     \
    do {                                                               \
        /* + SPR 21577 */\
        egtpu_stop_timer (&(path)->tmr_id);                              \
        /* - SPR 21577 */\
        (path)->tmr_flag = EGTPU_TIMER_OFF;                             \
                                                                       \
        EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL,                      \
                EGTPU_FSM_TRACE,                                        \
                (EGTPU_TS_STOP_TIMER,                                   \
                 egtpu_entity_name_arr[EGTPU_EI_GTPU],                   \
                 "ECHO_TIMER"));                                       \
    } while (0)

#define EGTPU_STOP_REORDER_TIMER(p_global, p_egtpu_rab_ctxt)             \
    do {                                                               \
	    if(p_egtpu_rab_ctxt->p_tid != EGTPU_INVALID_TIMER_ID)		       \
	        qvTimerStop((QTIMER) p_egtpu_rab_ctxt->p_tid, 0);       	 	   \
                                                                       \
        /* Mark the stopped timer as invalid */                        \
        p_egtpu_rab_ctxt->p_tid = EGTPU_INVALID_TIMER_ID;                  \
																	   \
        p_rab_ctxt->guard_arr[0] = p_rab_ctxt->guard_arr[1];		   \
		p_rab_ctxt->guard_arr[1] = p_rab_ctxt->guard_arr[2]; 		   \
	  	p_rab_ctxt->guard_arr[2] = p_rab_ctxt->guard_arr[3];		   \
        p_rab_ctxt->guard_arr[3] = p_rab_ctxt->max_seq_num;			   \
																	   \
	    p_rab_ctxt->max_seq_num  =	-1;  							   \
																	   \
        EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL,             \
                EGTPU_FSM_TRACE,                                        \
                (EGTPU_TS_STOP_TIMER,                                   \
                 egtpu_entity_name_arr[EGTPU_EI_GTPU],                   \
                 "REORDER_TIMER"));                                    \
    } while (0)
/* a macro to fill in peer info in outgoing GTPU messages */
/* in addition it updates the pointer value to next input location */
#define EGTPU_FILL_PEER_INFO(buf, portno, ipaddr)                       \
/* Cyclomatic Complexity changes */                                    \
    {                                                               \
        *(buf)++ = EGTPU_IE_PORT_NUM;                                   \
        EGTPU_SET_U16BIT (buf, portno);                                 \
        (buf) += EGTPU_SIZE_OF_PORT_NUM;                                \
                                                                       \
        *(buf)++ = EGTPU_IE_GSN_ADDR;                                   \
        egtpu_memcpy (buf, ipaddr, EGTPU_GET_U16BIT (ipaddr) + 2);       \
        (buf) += (EGTPU_GET_U16BIT (ipaddr) + 2);                       \
    }

/******************************************************************************
Macro :    To mark a record as dirty
*******************************************************************************
Description :  To mark a record as dirty 

Parameters  :
    p_rec        : pointer to the structure node, which has to be 
                      removed from the free list
    opcode          : opcode to be changed
    p_dirty_list    : pointer to the head of the dirty list

NOTE : It is assumed that p_rec has "opcode"
       field with the same name
*******************************************************************************/
/* ALGO : for marking the record dirty */
/*
ND + C = NC         where 
ND + U = U          ND = EGTPU_NOT_DIRTY
ND + D = D          NC = EGTPU_DIRTY_NEW_CREATE
NC + U = NC         C  = EGTPU_DIRTY_CREATE
NC + D = ND         U  = EGTPU_DIRTY_UPDATE
C  + U = C          D  = EGTPU_DIRTY_DELETE
C  + D = D
U  + U = U
U  + D = D
D  + C = C
D  + D = D
*/
#define MARK_DIRTY(p_rec, new_opcode)                         \
do {                                                             \
    switch (new_opcode) {                                        \
        case EGTPU_DIRTY_CREATE :                                  \
            if(p_rec->opcode == EGTPU_NOT_DIRTY) {              \
                p_rec->opcode = EGTPU_DIRTY_NEW_CREATE;         \
            } else if(p_rec->opcode == EGTPU_DIRTY_DELETE) {    \
                p_rec->opcode = EGTPU_DIRTY_CREATE;             \
            }                                                    \
            break;                                               \
                                                                 \
        case EGTPU_DIRTY_UPDATE :                                  \
            if(p_rec->opcode == EGTPU_NOT_DIRTY ||              \
               p_rec->opcode == EGTPU_DIRTY_DATA_UPDATE) {      \
                p_rec->opcode = EGTPU_DIRTY_UPDATE;             \
            }                                                    \
            break;                                               \
                                                                 \
        case EGTPU_DIRTY_DELETE :                                  \
            if(p_rec->opcode == EGTPU_DIRTY_NEW_CREATE) {       \
                p_rec->opcode = EGTPU_NOT_DIRTY;                \
            } else {                                             \
                p_rec->opcode = EGTPU_DIRTY_DELETE;             \
            }                                                    \
            break;                                               \
                                                                 \
        case EGTPU_DIRTY_DATA_UPDATE :                             \
           if(p_rec->opcode == EGTPU_NOT_DIRTY) {               \
                p_rec->opcode = EGTPU_DIRTY_DATA_UPDATE;        \
            }                                                    \
            break;                                               \
    }                                                            \
} while(0)

/*  Get the Index from the Record. */
#define EGTPU_GET_INDEX_FROM_REC_ADDR(pool, rec, size) \
    (((U8bit*)(rec) - (U8bit*)(pool))/(size))

/* reordering pool create */
#define EGTPU_GET_HASH_REC(p_global,pool, ptr_m)                         \
    do {                                                               \
    if(!(ptr_m = cvContextAlloc(&pool))) {		\
    	EGTPU_ERROR_REPORTER (p_global, \
		(EGTPU_TS_DBERR_INSERT_INHASH_TBL_FAILURE,	\
                egtpu_entity_name_arr[EGTPU_EI_GTPU]));			\
        /* SPR 17747 FIX START*/\
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON, \
                relayGtpuCurrentTick_g, __LINE__,\
                EGTPU_DBERR_INSERT_DB_FAILURE,\
                0,0,0,0,0, __func__, \
                egtpu_entity_name_arr[EGTPU_EI_GTPU]);\
        /* SPR 17747 FIX END */\
  	    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_INSERT_HASH_TBL_FAILURE);			\
	    return EGTPU_NULL;														\
		}																		\
    } while (0)

#define EGTPU_GET_TIMER_BLOCK(p_global,pool, ptr_m)                         \
    do {                                                               \
    if(!(ptr_m = cvContextAlloc(&pool))) {		\
																		\
	  EGTPU_ERROR_REPORTER(p_global, \
	  (EGTPU_TS_SERR_CONTEXT_POOL_MEM_NOT_AVAIL_FOR_TIMER, \
	  egtpu_entity_name_arr[EGTPU_EI_GTPU], \
	  EGTPU_INIT_TIMER_POOL_SIZE));		\
        /* SPR 17747 FIX START*/\
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON, \
                relayGtpuCurrentTick_g, __LINE__,\
                EGTPU_DBERR_INSERT_DB_FAILURE,\
                EGTPU_INIT_TIMER_POOL_SIZE,0,0,0,0, __func__, \
                egtpu_entity_name_arr[EGTPU_EI_GTPU]);\
        /* SPR 17747 FIX END */\
      													 					\
        EGTPU_UPDATE_STATS(p_global ,EGTPU_STAT_TIMER_CTXT_MEMGET_FAIL);		\
																			\
	    return EGTPU_FAILURE;												\
		}																		\
    } while (0)

#define EGTPU_GET_TABLE(p_global,pool, ptr_m)                         \
    do {                                                               \
    if(!(ptr_m = cvContextAlloc(&pool))) {		\
    	EGTPU_ERROR_REPORTER (p_global, \
		(EGTPU_TS_DBERR_INSERT_INHASH_TBL_FAILURE,	\
                egtpu_entity_name_arr[EGTPU_EI_GTPU]));						\
																			\
        /* SPR 17747 FIX START*/\
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON, \
                relayGtpuCurrentTick_g, __LINE__, \
                EGTPU_DBERR_INSERT_DB_FAILURE,\
                EGTPU_INIT_TIMER_POOL_SIZE,0,0,0,0, __func__, \
                egtpu_entity_name_arr[EGTPU_EI_GTPU]);\
        /* SPR 17747 FIX END */\
  	    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_INSERT_HASH_TBL_FAILURE);			\
        /*coverity 25188 fix*/\
        ltePanic("Unable to Allocate Memory \n");\
        /*coverity 25188 fix*/\
		}																		\
    } while (0)


#ifdef CSPL_LEAK_DEBUG_LITE
#define EGTPU_FREE_TIMER_BLOCK(ptr_m)  cvContextFree(ptr_m,__func__,__LINE__)
#else
#define EGTPU_FREE_TIMER_BLOCK(ptr_m)  cvContextFree(ptr_m)
#endif

/* reordering pool delete */
#define EGTPU_FREE_HASH_REC(pool, ptr_m)                         \
    do {                                                               \
			yhDelete(pool, ptr_m);												\
			EGTPU_FREE_TIMER_BLOCK(ptr_m);											\
																	\
    } while (0)

#define EGTPU_GET_SAP_HASH_REC(p_global,pool, ptr_m)                         \
    do {                                                               \
    if(!(ptr_m = cvContextAlloc(&pool))) {		\
			  /* release the rab-ctxt that was acquired */				\
	            egtpu_db_free_rab_ctxt (p_rab_ctxt);						\
																		\
        		__ERR_RSP_1__ (p_egtpu_info, EGTPU_RESOURCES_NOT_AVAILABLE);	\
	   			/* Cyclomatic Complexity changes */																\
				return EGTPU_FAILURE;														\
		}																		\
    } while (0)

#define EGTPU_FREE_SAP_HASH_REC(ptr_m)                         \
    do {                                                               \
		YHNODE *p;													\
		U16bit index;												\
  		for(index = 0; index < EGTPU_INIT_WINDOW_SZ; index++)	\
  		{															\
		  for(p = ptr_m->table[index]; p; p = yhNext(ptr_m, p) )	\
  		  {															\
			EGTPU_FREE_HASH_REC(ptr_m, p);								\
		  }    													 	\
  		}															 \
	    															 \
	 	EGTPU_FREE_TIMER_BLOCK(ptr_m->table);								 \
		EGTPU_FREE_TIMER_BLOCK(ptr_m);											\
    } while (0)


#define EGTPU_COUNT_PDU_IN_HASH_TABLE(ptr_m)  yhCount(ptr_m)

/** Function prototype for utils related functions **/
/* handler for GET-CTXT-INFO API from LM */
egtpu_return_t egtpu_get_ctxt_info_req (egtpu_global_t *,egtpu_info_t *);

/* the following fn is used to send alarms to LM */
egtpu_return_t egtpu_send_alarm_notification (egtpu_global_t *,
											U8bit, U8bit*);
/** Function prototype for stats related functions **/
egtpu_return_t egtpu_get_stats_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_get_init_stats_req (egtpu_global_t *,egtpu_info_t *);
/*reorder*/
extern egtpu_return_t egtpu_modify_reordering_ie(egtpu_global_t *,
												egtpu_info_t * );

egtpu_return_t local_validate_protocol_hdr (U8bit *p_start,U8bit *p_end,egtpu_error_t *p_error,egtpu_global_t *p_global);
egtpu_return_t local__dec_end_marker__(egtpu_global_t *p_global,
                                                egtpu_info_t *p_egtpu_info,
                                                U8bit *p_start,
                                                U8bit *p_end);


#endif
