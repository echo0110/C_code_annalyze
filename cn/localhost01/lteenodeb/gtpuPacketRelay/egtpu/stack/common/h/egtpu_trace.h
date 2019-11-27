/***********************************************************************
**  FILE :
**      Trace related declarations
**
************************************************************************
**  DESCRIPTION :
**      Contains the declaration of the trace flag array and function
**      prototypes for trace related functionality
**
**  DATE             NAME              REFERENCE             REASON
**  ----            ------             ---------             ------
**  Oct,2009        Vipul Aggarwal                           Original
**
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/

#ifndef _EGTPU_TRC_DECL_
#define _EGTPU_TRC_DECL_

#if defined (EGTPU_ENABLE_TRACE)
extern char const *egtpu_trace_array[];
extern char const *egtpu_error_array[];

extern char const *egtpu_entity_name_arr[];

extern char const **egtpu_in_api_name_arr[];
extern char const **egtpu_out_api_name_arr[];
#endif

/* the print function should have the same prototype as printf */
extern int STACK_PRINT (const char *, ...);

/* #defines for trace strings */
#define EGTPU_TS_IN_API                     egtpu_trace_array[0]
#define EGTPU_TS_IN_MSG                     egtpu_trace_array[1]
#define EGTPU_TS_OUT_API                    egtpu_trace_array[2]
#define EGTPU_TS_OUT_MSG                    egtpu_trace_array[3]
#define EGTPU_TS_START_TIMER                egtpu_trace_array[4]
#define EGTPU_TS_STOP_TIMER                 egtpu_trace_array[5]
#define EGTPU_TS_ENTERING_FUNCTION          egtpu_trace_array[6]
#define EGTPU_TS_EXITING_FUNCTION           egtpu_trace_array[7]
#define EGTPU_TS_ENTERING_HANDLER           egtpu_trace_array[8]
#define EGTPU_TS_EXITING_HANDLER            egtpu_trace_array[9]


/* trace strings for parse errors */
#define EGTPU_TS_PERR_UNKNOWN_MSG           egtpu_error_array[0]
#define EGTPU_TS_PERR_UNKNOWN_API           egtpu_error_array[1]
#define EGTPU_TS_PERR_UNEXPECTED_MSG        egtpu_error_array[2]
#define EGTPU_TS_PERR_UNEXPECTED_API        egtpu_error_array[3]
#define EGTPU_TS_PERR_MSG_TOO_SHORT         egtpu_error_array[4]
#define EGTPU_TS_PERR_VERSION_NOT_SUPP      egtpu_error_array[5]
#define EGTPU_TS_PERR_PROTO_TYPE_INCORRECT  egtpu_error_array[6]
#define EGTPU_TS_PERR_INV_MSG_FORMAT        egtpu_error_array[7]
#define EGTPU_TS_PERR_MAND_IE_MISSING       egtpu_error_array[8]
#define EGTPU_TS_PERR_MAND_IE_INCORRECT     egtpu_error_array[9]
#define EGTPU_TS_PERR_ECHO_TIMER_EXP        egtpu_error_array[10]
#define EGTPU_TS_PERR_INV_TIMER_TYPE        egtpu_error_array[11]
#define EGTPU_TS_PERR_UNEXPECTED_TIMER_EXP  egtpu_error_array[12]
#define EGTPU_TS_PERR_EXT_HDR_NOT_SUPP      egtpu_error_array[13]

#define EGTPU_TS_PERR_INV_API_FORMAT        egtpu_error_array[14]
#define EGTPU_TS_PERR_INV_OPCODE_RCVD	    egtpu_error_array[15]
#define EGTPU_TS_PERR_INV_TRAFFIC_CLASS     egtpu_error_array[33]
#define EGTPU_TS_PERR_INV_ADDR              egtpu_error_array[34]



/* trace strings for database errors */
#define EGTPU_TS_DBERR_INSERT_DB_FAILURE    egtpu_error_array[16]
#define EGTPU_TS_DBERR_KEY_NOT_PRESENT      egtpu_error_array[17]
#define EGTPU_TS_DBERR_INSERT_INHASH_TBL_FAILURE egtpu_error_array[18]
/* trace strings for system errors */
#define EGTPU_TS_SERR_STATIC_MEM_NOT_AVAIL  egtpu_error_array[19]
#define EGTPU_TS_SERR_DYN_MEM_NOT_AVAIL     egtpu_error_array[20]
#define EGTPU_TS_SERR_START_TIMER_FAILURE   egtpu_error_array[21]
#define EGTPU_TS_SERR_INV_INDEX_RCVD		egtpu_error_array[22]	
/*reordering*/
#define EGTPU_TS_SERR_CONTEXT_POOL_MEM_NOT_AVAIL_FOR_HASH  egtpu_error_array[23]
#define EGTPU_TS_SERR_CONTEXT_POOL_MEM_NOT_AVAIL_FOR_TIMER egtpu_error_array[24]
/* trace strings for common errors */
#define EGTPU_TS_CERR_INIT_NOT_DONE         egtpu_error_array[25]
#define EGTPU_TS_CERR_TRACES_DISABLED       egtpu_error_array[26]
#define EGTPU_TS_CERR_INV_TRACE_LEVEL       egtpu_error_array[27]
#define EGTPU_TS_CERR_STATS_DISABLED        egtpu_error_array[28]
#define EGTPU_TS_CERR_INV_STATS_TYPE        egtpu_error_array[29]
#define EGTPU_TS_CERR_SEND_MSG_FAILURE      egtpu_error_array[30]
#define EGTPU_TS_CERR_PATH_FAILURE          egtpu_error_array[31]
#define EGTPU_TS_CERR_INIT_FAILURE          egtpu_error_array[32]


/***********************************************************************
Macro :    To print all traces. 
************************************************************************
Parameters :
    arr         : array of 2 S32bit elements (Global for a stack)
    trace_level : BRIEF_TRACE_LEVEL / DETAILED_TRACE_LEVEL
    bitmap      : Combination of various flags like EGTPU_INCOMING_TRACE,
                  EGTPU_FSM_TRACE, EGTPU_INIT_TRACE,
                  EGTPU_OUTGOING_TRACE etc.
    trace_str   : Standard "printf" type string
***********************************************************************/
#if defined EGTPU_ENABLE_TRACE
#define EGTPU_STACK_TRACE(p_global,trace_level, bitmap, trace_str)               \
    do {                                                               \
        if ((p_global->egtpu_trace_arr[trace_level - 1] & (bitmap)) ||            \
            ((trace_level == EGTPU_BRIEF_TRACE_LEVEL) &&                \
             (p_global->egtpu_trace_arr[EGTPU_DETAILED_TRACE_LEVEL - 1] &          \
              (bitmap))))                                              \
                STACK_PRINT trace_str;                                 \
    } while (0)
#else
#define EGTPU_STACK_TRACE(p_global,trace_level, bitmap, trace_str) \
    /* GCC_4.9_WARNINGS_FIX_START */ \
    (void)p_global; \
    /* GCC_4.9_WARNINGS_FIX_END */
#endif

/***********************************************************************
Macro :	 To report stack errors
************************************************************************
Parameters :
    str         : string to be displayed
***********************************************************************/
#define EGTPU_ERROR_REPORTER(p_global,str)                                       \
    EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL, EGTPU_ERROR_TRACE, str)

/***********************************************************************
Macro :    To print the hex dump of a mesg
************************************************************************
Parameters :
    arr         : array of 2 S32bit elements (Global for a stack)
    bitmap      : Combination of various flags like INCOMING_TRACE,
                  OUTGOING_TRACE etc.
    p_mesg      : pointer to mesg
    n_octets    : Length of the mesg
***********************************************************************/
#if defined (EGTPU_ENABLE_TRACE)
extern egtpu_return_t egtpu_hex_dump (U8bit *ptr, U16bit noctets);

#define EGTPU_HEX_DUMP(p_global,trace_level, bitmap, p_mesg, n_octets)           \
    do {                                                               \
        if (p_global->egtpu_trace_arr[trace_level - 1] & (bitmap))                \
            egtpu_hex_dump (p_mesg, n_octets);                          \
    } while (0)
#else
#define EGTPU_HEX_DUMP(p_global,trace_level, bitmap, p_mesg, n_octets)
#endif       

/***********************************************************************
                macros to print entering/exiting function ...
***********************************************************************/
#define EGTPU_TRACE_FN_ENTR(p_global,fname)                                      \
    EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL, EGTPU_FSM_TRACE,          \
            (EGTPU_TS_ENTERING_FUNCTION,                                \
             egtpu_entity_name_arr[EGTPU_EI_GTPU],                       \
             fname))

#define EGTPU_TRACE_FN_EXIT(p_global,fname)                                      \
    EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL, EGTPU_FSM_TRACE,          \
            (EGTPU_TS_EXITING_FUNCTION,                                 \
             egtpu_entity_name_arr[EGTPU_EI_GTPU],                       \
             fname))

/***********************************************************************
                macros to print entering/exiting handlers ...
***********************************************************************/
#define EGTPU_TRACE_HLR_ENTR(p_global,name)                                      \
    EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL, EGTPU_FSM_TRACE,          \
            (EGTPU_TS_ENTERING_HANDLER,                                 \
             egtpu_entity_name_arr[EGTPU_EI_GTPU],                       \
             (name)))

#define EGTPU_TRACE_HLR_EXIT(p_global,name)                                      \
    EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL, EGTPU_FSM_TRACE,          \
            (EGTPU_TS_EXITING_HANDLER,                                  \
             egtpu_entity_name_arr[EGTPU_EI_GTPU],                       \
             (name)))

/***********************************************************************
                Macro : To Print in incoming API  Traces    
***********************************************************************/
#if defined (EGTPU_ENABLE_TRACE)
#define EGTPU_PRINT_IN_TRACE(p_global,p_buf, src, api_id)                        \
    do {                                                               \
        EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL, \
				EGTPU_INCOMING_TRACE, \
                (EGTPU_TS_IN_API,                                       \
                 egtpu_entity_name_arr[EGTPU_EI_GTPU],                   \
                 egtpu_in_api_name_arr[src][api_id -                    \
                 egtpu_in_api_base_arr [src]]));                        \
                                                                       \
        if (api_id == EGTPU_UNITDATA_IND) {                             \
            EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL,                  \
                    EGTPU_INCOMING_TRACE,                               \
                    (EGTPU_TS_IN_MSG,                                   \
                     egtpu_entity_name_arr[EGTPU_EI_GTPU],               \
                     egtpu_get_msg_name                                 \
                     (*(EGTPU_API_GET_PAYLOAD_STRT_PTR (p_buf) + 1)))); \
        }                                                              \
                                                                       \
        EGTPU_HEX_DUMP (p_global,EGTPU_DETAILED_TRACE_LEVEL, \
				EGTPU_INCOMING_TRACE, \
                p_buf, EGTPU_API_GET_MSG_LEN (p_buf));                  \
    } while (0)
#else
#define EGTPU_PRINT_IN_TRACE(p_global,p_buf,src,api_id)
#endif       

/***********************************************************************
                Macro : To Print in Outgoing  API  Traces    
***********************************************************************/
#if defined (EGTPU_ENABLE_TRACE)
#define EGTPU_PRINT_OUT_TRACE(p_global,p_buf)                                    \
    do {                                                               \
        egtpu_entity_id_t dst = EGTPU_API_GET_DST_ID (p_buf);            \
        egtpu_api_id_t api_id = EGTPU_API_GET_API_ID (p_buf);            \
                                                                       \
        EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL, \
				EGTPU_OUTGOING_TRACE, \
                (EGTPU_TS_OUT_API,                                      \
                 egtpu_entity_name_arr[EGTPU_EI_GTPU],                   \
                 egtpu_out_api_name_arr [dst] [api_id -                 \
                 egtpu_out_api_base_arr [dst]],                         \
                 egtpu_entity_name_arr[dst]));                          \
                                                                       \
        if (dst == EGTPU_EI_UDP) {                                      \
            EGTPU_STACK_TRACE (p_global,EGTPU_BRIEF_TRACE_LEVEL,       \
                    EGTPU_OUTGOING_TRACE,                               \
                    (EGTPU_TS_OUT_MSG,                                  \
                     egtpu_entity_name_arr[EGTPU_EI_GTPU],               \
                     egtpu_get_msg_name                                 \
                     (*(EGTPU_API_GET_PAYLOAD_STRT_PTR (p_buf) + 1)))); \
        }                                                              \
                                                                       \
        EGTPU_HEX_DUMP (p_global,EGTPU_DETAILED_TRACE_LEVEL, \
				EGTPU_OUTGOING_TRACE, \
                p_buf, EGTPU_API_GET_MSG_LEN (p_buf));                  \
    } while(0)
#else
#define EGTPU_PRINT_OUT_TRACE(p_global,p_buf)
#endif       

/** Function prototype for trace related functions **/
S8bit *egtpu_get_msg_name (U8bit);

egtpu_return_t egtpu_init_trace_level_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_set_trace_level_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_get_trace_level_req (egtpu_global_t *,egtpu_info_t *);

#endif
