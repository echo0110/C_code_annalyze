/******************************************************************************
*
*   FILE NAME:
*       rrc_common_utils.h
*
*   DESCRIPTION:
*       This is the header file of the bridge module. This module provides
*       the bridge between RRC layer and external libraries (CSPL, logger etc).
*
*   DATE            AUTHOR      REFERENCE       REASON
*   31 Mar 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifdef _MSC_VER
#include "pc_test.h"
#else
#ifndef _RRC_COMMON_UTILS_H_
#define _RRC_COMMON_UTILS_H_

#include "rrc_defines.h"
#include "rrc_protocol_events.h"

/* SPR 1643 Start */
#include "rrc_asn_enc_dec.h"
/* SPR 1643 Stop */
#include "rrc_porting.h"
void set_cell_index(U8 cell_index);
void set_ue_index(U16 ue_index);
U8   get_cell_index(void);
U16  get_ue_index(void);
/*SPR 17777+-*/
void set_cell_and_ue_index(U8 cell_index, U16 ue_index);
#define GET_CELL_INDEX()      get_cell_index()
#define GET_UE_INDEX()        get_ue_index()
#define SET_CELL_INDEX(value) set_cell_index(value)
#define SET_UE_INDEX(value)   set_ue_index(value)
#define SET_CELL_AND_UE_INDEX(cell_index, ue_index) set_cell_and_ue_index(cell_index, ue_index)

extern U32 length_read_g;
/* Trace Procedure Code */

#define NORMAL 10 

/* SPR 1643 Start */
#define MAX_NUM_OCTETS             3

/* MCC/MNC Range Check (0-9) */
#define MAX_VAL     0x9

#define IS_VALID_MCC_MNC_DIGIT(x)    ((x) <= MAX_VAL)
/* SPR 1643 Stop */

/*
*   logging
*/

#define RRC_INTERNAL_LOGGER 1
#define RRC_ADV_LOGGER      2

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    /*
    *  Advanced logger
    */

    #include <advlogging.h>
    #include <lteLogger.h>

    AdvL_ContextIndex        g_advl_ctxt;
    extern AdvL_ContextIndex        g_advl_ctxt_rrc;
    extern AdvL_ContextIndex        g_advl_ctxt_s1ap;
    extern AdvL_ContextIndex        g_advl_ctxt_x2ap;
    extern AdvL_ContextIndex        g_advl_ctxt_m2ap;

    #define LogContext_Rrc          LogContext_Default
    #define RRC_LOG_CTXT_CONSOLE    LogContext_Rrc 
    #define S1AP_LOG_CTXT_CONSOLE   LogContext_S1ap 
    #define X2AP_LOG_CTXT_CONSOLE   LogContext_X2ap 


    #define RRC_FATAL               LogFatal
    #define RRC_ERROR               LogError
    #define RRC_WARNING             LogWarning
    #define RRC_INFO                LogInfo
    #define RRC_BRIEF               LogBrief
    #define RRC_DETAILED            LogDetailed
    #define RRC_ASN                 RRC_BRIEF
    #define RRC_PROTOCOL_EVENT      RRC_DETAILED
    /* SPR 21251 Start */
    #define RRC_DETAILEDALL         LogDebug
    /* SPR 21251 End */
/* SPR_17377_Warning_fix + */
    /* SPR 22304 Fix Start */
lgr_log_level_et rrc_map_log_level(AdvL_LogLevel log_level);
    void rrc_trace_dbg(
            const S8* facility_name,U8 log_level,
            const char* format, ...);
    /* SPR 22304 Fix Start */


#define RRC_TRACE_DBG(facility_name,log_level,format, ...)   rrc_trace_dbg(facility_name, log_level, format, ## __VA_ARGS__);
#ifdef ATTRIBUTE_COMP
#define RRC_LOG RRC_TRACE_DBG
#define RRC_LOG_ASN RRC_TRACE_DBG
#define S1AP_LOG RRC_TRACE_DBG
#define X2AP_LOG RRC_TRACE_DBG


#else
/* SPR_17377_Warning_fix - */

#ifdef LINUX_PC_TEST
    #define RRC_LOG(facility_name, log_level, format, ...) \
        {\
        	if (log_level < RRC_DETAILED) \        
				test_module_log(rrc_get_module_name(qvGetModuleId()),rrc_get_loglevel_name((U8)log_level), GET_CELL_INDEX(), GET_UE_INDEX(), \
	            	__FILE__, __LINE__, __func__,(char*)format, ##__VA_ARGS__); \
        }
#else
/* SPR 21275 changes start */
    #define RRC_LOG(facility_name, log_level, format, ...) \
        {\
            AdvL_LogTarget logTarget = LogTarget_None; \
            if(LogSuccess == check_log_level(g_advl_ctxt_rrc, (AdvL_LogLevel)log_level, &logTarget)) {\
                if (LogTarget_Shared_Mem == logTarget) \
                {\
                    lgr_log_level_et logLevel = LGR_TRACE; \
                    logLevel = rrc_map_log_level((AdvL_LogLevel)log_level);\
                    /* SPR 21820 Start */\
                    print_log_message((U32)logLevel, __FILE__, __LINE__, __func__,\
                        (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), LGR_FALSE, (const char*)format, ##__VA_ARGS__); \
                    /* SPR 21820 End */\
                }\
                else \
                    write_log_message(g_advl_ctxt_rrc, (AdvL_LogLevel)log_level, __FILE__, __LINE__, \
                        __func__ , (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), (const char*)format, ##__VA_ARGS__); \              
			}\
        }
#endif

#ifdef LINUX_PC_TEST
    #define S1AP_LOG(facility_name, log_level, format, ...) \
        {\
        	if (log_level < RRC_DETAILED) \        
				test_module_log(rrc_get_module_name(RRC_S1AP_MODULE_ID),rrc_get_loglevel_name((U8)log_level), GET_CELL_INDEX(), GET_UE_INDEX(), \
	            	__FILE__, __LINE__, __func__,(char*)format, ##__VA_ARGS__); \
        }
#else
	    #define S1AP_LOG(facility_name, log_level, format, ...) \
        {\
            AdvL_LogTarget logTarget = LogTarget_None; \
            if(LogSuccess == check_log_level(g_advl_ctxt_s1ap, (AdvL_LogLevel)log_level, &logTarget)) {\
                if (LogTarget_Shared_Mem == logTarget) \
                {\
                    lgr_log_level_et logLevel = LGR_TRACE; \
                    logLevel = rrc_map_log_level((AdvL_LogLevel)log_level);\
                    /* SPR 21820 Start */\
                    print_log_message((U32)logLevel, __FILE__, __LINE__, __func__,\
                        (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), LGR_FALSE, (const char*)format, ##__VA_ARGS__); \
                    /* SPR 21820 End */\
                }\
                else \
                    write_log_message(g_advl_ctxt_s1ap, (AdvL_LogLevel)log_level, __FILE__, __LINE__, \
                        __func__ , (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), (const char*)format, ##__VA_ARGS__); \
            }\
        }
#endif

#ifdef LINUX_PC_TEST
    #define X2AP_LOG(facility_name, log_level, format, ...) \
        {\
           	if (log_level <= RRC_DETAILED) \
	            test_module_log(rrc_get_module_name(RRC_X2AP_MODULE_ID),rrc_get_loglevel_name((U8)log_level), GET_CELL_INDEX(), GET_UE_INDEX(), \
	            	__FILE__, __LINE__, __func__,(char*)format, ##__VA_ARGS__); \
        }
#else
    #define X2AP_LOG(facility_name, log_level, format, ...) \
        {\
            AdvL_LogTarget logTarget = LogTarget_None; \
            if(LogSuccess == check_log_level(g_advl_ctxt_x2ap, (AdvL_LogLevel)log_level, &logTarget)) {\
                if (LogTarget_Shared_Mem == logTarget) \
                {\
                    lgr_log_level_et logLevel = LGR_TRACE; \
                    logLevel = rrc_map_log_level((AdvL_LogLevel)log_level);\
                    /* SPR 21820 Start */\
                    print_log_message((U32)logLevel, __FILE__, __LINE__, __func__,\
                        (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), LGR_FALSE, (const char*)format, ##__VA_ARGS__); \
                    /* SPR 21820 False */\
                }\
                else \
                    write_log_message(g_advl_ctxt_x2ap, (AdvL_LogLevel)log_level, __FILE__, __LINE__, \
                        __func__ , (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), (const char*)format, ##__VA_ARGS__); \
            }\
        }
#endif
    #define M2AP_LOG(facility_name, log_level, format, ...) \
        {\
            AdvL_LogTarget logTarget = LogTarget_None; \
            U32 logLevel = QVLOG_NONE; \
            if(LogSuccess == check_log_level(g_advl_ctxt_m2ap, (AdvL_LogLevel)log_level, &logTarget)) {\
                if (LogTarget_Shared_Mem == logTarget) \
                {\
                    logLevel = ((log_level <= LogWarning) ? (log_level-1) : (log_level-2));\
                    print_log_message((U32)logLevel, __FILE__, __LINE__, __func__,\
                        (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(),LGR_FALSE, (const char*)format, ##__VA_ARGS__); \
                }\
                else \
                    write_log_message(g_advl_ctxt_m2ap, (AdvL_LogLevel)log_level, __FILE__, __LINE__, \
                        __func__ , (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), (const char*)format, ##__VA_ARGS__); \
            }\
        }

#define M2AP_LOG_ASN(facility_name, log_level, format, ...) \
{\
    AdvL_LogTarget logTarget = LogTarget_None; \
    U32 logLevel = QVLOG_NONE; \
    if(LogSuccess == check_log_level(g_advl_ctxt_m2ap, (AdvL_LogLevel)log_level, &logTarget)) {\
        if (LogTarget_Shared_Mem == logTarget) \
        {\
            logLevel = ((log_level <= LogWarning) ? (log_level-1) : (log_level-2));\
            print_log_message((U32)logLevel, __FILE__, __LINE__, __func__,\
                    (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(),LGR_TRUE, (const char*)format, ##__VA_ARGS__); \
        }\
        else \
        write_log_message_asn(g_advl_ctxt_m2ap, (AdvL_LogLevel)log_level, __FILE__, __LINE__, \
                __func__ , (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), (const char*)format, ##__VA_ARGS__); \
    }\
}

	/* SPR 13700 Fix start */
	
    #define RRC_LOG_ASN(facility_name, log_level, format, ...) \
        {\
            AdvL_LogTarget logTarget = LogTarget_None; \
            if(LogSuccess == check_log_level(g_advl_ctxt_rrc, (AdvL_LogLevel)log_level, &logTarget)) {\
                if (LogTarget_Shared_Mem == logTarget) \
                {\
                    lgr_log_level_et logLevel = LGR_TRACE; \
                    /* SPR 21275 changes start */ \
                    logLevel = rrc_map_log_level((AdvL_LogLevel)log_level);\
                    /* SPR 21820 Start */\
		    print_log_message((U32)logLevel, __FILE__, __LINE__, __func__,\
                        (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), LGR_TRUE, (const char*)format, ##__VA_ARGS__); \
                    /* SPR 21820 end */\
                    /* SPR 21275 changes end */ \
                }\
                else \
                    write_log_message_asn(g_advl_ctxt_rrc, (AdvL_LogLevel)log_level, __FILE__, __LINE__, \
                        __func__ , (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), (const char*)format, ##__VA_ARGS__); \
            }\
        }
/* SPR 21275 changes end */
	/* SPR 13700 Fix stop */
/* SPR_17377_Warning_fix + */
#endif	
/* SPR_17377_Warning_fix - */

#elif (LOGGER_TYPE == RRC_INTERNAL_LOGGER)


    /*
    *   Internal logger
    */

    void rrc_trace_message(S32 log_level,const S8 *facility_name,
        const S8* format,...);
    void rrc_set_loglevel(U8 new_log_level);


    #define RRC_FATAL            QVLOG_CRITICAL
    #define RRC_ERROR            QVLOG_MAJOR
    #define RRC_WARNING          QVLOG_MINOR
    #define RRC_INFO             QVLOG_INFO
    #define RRC_BRIEF            QVLOG_DETAIL
    #define RRC_DETAILED         QVLOG_DUMP
    #define RRC_ASN              RRC_BRIEF
    #define RRC_PROTOCOL_EVENT   RRC_DETAILED
    #define RRC_INT_NONE    QVLOG_NONE
    #define RRC_DETAILEDALL       QVLOG_DUMP
/* SPR_17377_Warning_fix + */
    void rrc_trace_dbg(
            const S8* facility_name,U8 log_level,
            const char* format, ...) __attribute__((format (printf, 3, 4)));

#define RRC_TRACE_DBG(facility_name,log_level,format, ...)   rrc_trace_dbg(facility_name, log_level, format, ## __VA_ARGS__);
#ifdef ATTRIBUTE_COMP
#define RRC_LOG RRC_TRACE_DBG
#else
/* SPR_17377_Warning_fix - */
    #define RRC_LOG(facility_name, log_level, format, ...) \
        rrc_trace_message(log_level, facility_name, (const S8*)format, ##__VA_ARGS__)
/* SPR_17377_Warning_fix + */
#endif
/* SPR_17377_Warning_fix - */
    
    /* SPR 13700 Fix start */
    #define RRC_LOG_ASN(facility_name, log_level, format, ...) \
        rrc_trace_message(log_level, facility_name, (const S8*)format, ##__VA_ARGS__)
    /* SPR 13700 Fix stop */
#elif (LOGGER_TYPE > RRC_ADV_LOGGER)
    #error Wrong logger type specified!
#else
    #define RRC_LOG(facility_name, log_level, format, ...)
#endif

#define RRC_COMP_PARSE     RRC_DETAILEDALL            

U8 rrc_get_loglevel( void );

extern const S8 *rrc_log_facility_name;

#define RRC_LOGGER_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_log_facility_name, log_level, format, ##__VA_ARGS__)

#define RRC_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_log_facility_name, log_level, format, ##__VA_ARGS__)

/* SPR 13700 Fix start */
#define RRC_TRACE_ASN(log_level, format, ...) \
    RRC_LOG_ASN(rrc_log_facility_name, log_level, format, ##__VA_ARGS__)
/* SPR 13700 Fix stop */

/*
*   UT Trace
*/
#ifdef RRC_DEBUG
    #define RRC_UT_TRACE_ENTER() \
        RRC_TRACE(RRC_DETAILED, "Enterning function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)

    #define RRC_UT_TRACE_EXIT() \
        RRC_TRACE(RRC_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#else
    #define RRC_UT_TRACE_ENTER()
    #define RRC_UT_TRACE_EXIT()
#endif

#ifdef RRC_DEBUG
    #define RRC_LOGGER_TRACE_ENTER() \
        RRC_LOGGER_TRACE(RRC_DETAILED, "Enterning function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)

    #define RRC_LOGGER_TRACE_EXIT() \
        RRC_LOGGER_TRACE(RRC_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#else
    #define RRC_LOGGER_TRACE_ENTER()
    #define RRC_LOGGER_TRACE_EXIT()
#endif

/*
*   ASSERT
*/

#ifdef RRC_DEBUG
void
rrc_assert
(
    void* file,
    U32 line,
    void* expression
);
#define RRC_ASSERT(exp) (void)((exp) || (rrc_assert(__FILE__,__LINE__,#exp),0))
#else
#define RRC_ASSERT(exp)
#endif

#define RRC_MSG_RECVD "***************"
#define MAX_ASN_PRINT_BUFF_SIZE                 60000
/*BUG 604 changes start*/
#define RRC_INTRA_CELL_HO_PROC_CODE             0xFD
/*BUG 604 changes stop*/
#define RRC_INTER_CELL_HO_CANCEL_PROC_CODE      0xFE
#define RRC_INTER_CELL_HO_PROC_CODE             0xFF
/* Bug 8198 Fix Start */
#define RRC_INVALID_ESTABLISHMENT_CAUSE 0xFF 
#define RRC_INVALID_MME_ID 0xFF 
#define RRC_INVALID_REL_OF_MME 0xFF 
/* Bug 8198 Fix End */
/*SPR 8708 Fix Start*/
#define RRC_RrcConnectionReq_proc_code 0xFC
#define RRC_INVALID_PLMN_ID 0xFFFFFF
/*SPR 8708 Fix Stop*/
/* SPR 16149 Fix Start */
#define RRC_INVALID_ENB_UE_S1AP_ID 0xFFFFFFFF
/* SPR 16149 Fix Stop */
/* SPR 18168 Fix Start */
#define RRC_INVALID_MME_UE_S1AP_ID 0xFFFFFFFF
/* SPR 18168 Fix Stop */
/******************************************************************************
*   Enum Decleration
******************************************************************************/
typedef enum 
{
    EVENT_VALUE_SUCCESS=0,
    EVENT_VALUE_FAILURE=1,
    EVENT_VALUE_INVALID_FAMILY=2
}rrc_sctp_setup_result_et;

typedef enum
{
    EVENT_VALUE_MME=0,
    EVENT_VALUE_EXTERNAL_ENB=1
}rrc_sctp_endpoint_type_et;     //used to determine sctp endpoint type 

typedef enum
{
    EVENT_VALUE_S_TMSI=0,
    EVENT_VALUE_RANDOM_VALUE=1
}rrc_conn_req_identifier_et;

typedef enum
{
    EVENT_VALUE_LOCAL=0,
    EVENT_VALUE_REMOTE=1
}rrc_conn_sctp_cause_et;        //used to determine sctp initiating node

typedef enum
{
    EVENT_VALUE_NO_X2_CONN_EXISTED=0,
    EVENT_VALUE_X2_CONN_LOST=1
}x2_conn_release_cause;

typedef enum
{
    MESSAGE_DIRECTION_RECEIVED=0,
    MESSAGE_DIRECTION_SEND=1
}rrc_message_direction_et;
/*****************************************************************************/
/* LATENCY_TRACE_ON */
typedef enum

{
    RRC_RECEIVE,
    RRC_SEND
}rrc_send_receive_et;
/* Changes for getting the globals in Latency activity*/ 
typedef struct
{
    rrc_send_receive_et send_rcv_flag;
    rrc_module_id_t     src_mod_id;
    rrc_module_id_t     dest_mod_id;
    U16                 api_id;
    struct  tm          val_time;
    long                time_u_secs;
} g_latency_data_t;

extern g_latency_data_t g_latency_data[200];
extern U8 g_index ;
extern U8 start_logging ;

/******************************************************************************
*   Memory management functions
******************************************************************************/
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
#define rrc_mem_get(size) rrc_mem_Get(size,__func__,__LINE__)

void*
rrc_mem_Get
(
    rrc_size_t size,
    char *func, 
    unsigned int line
);

#else
void*
rrc_mem_get
(
    rrc_size_t size
);
#endif
void*
rrc_asn_mem_get
(
    rrc_size_t size
);
void
rrc_asn_mem_free
(
    void *p_buffer
);
/*SPR_18125_END*/

void
rrc_mem_free
(
    void *p_buffer
);

void*
rrc_msg_mem_get
(
    rrc_size_t size
);

void
rrc_msg_mem_free
(
    void *p_buffer
);

void*
rrc_sys_mem_get
(
    rrc_size_t size
);

void
rrc_sys_mem_free
(
    void *p_buffer
);


/******************************************************************************
*   Timer management functions
******************************************************************************/

rrc_timer_t
rrc_start_timer
(
    U32             timer_duration,
    void            *p_timer_data,
    U16             timer_data_size,
    rrc_bool_t      is_repeated
);

U16 
rrc_timer_now
(
    rrc_timer_t timer
);

void
rrc_stop_timer
(
    rrc_timer_t timer
);

void*
rrc_get_timer_data
(
    rrc_timer_t timer
);

/******************************************************************************
*   Message management functions
******************************************************************************/
/*SPR 20172 FIX START*/
#define rrc_send_message(p_msg,dst_module) do {\
	rrc_send_message_ex(p_msg,dst_module);\
	p_msg = PNULL;\
}while(0)
void
rrc_send_message_ex
/*SPR 20172 FIX End*/
(
    void *p_msg,
    rrc_module_id_t dst_module
);

rrc_return_et
rrc_check_cspl_header
(
    U8 *p_api
);

/* SPR 1643 Start */

rrc_return_et
is_valid_mcc_mnc
(
     PLMN_Identity    *p_plmn_Identity
);


rrc_return_et 
is_valid_plmn_identity
(
    U8 *p_data 
);
/* SPR 1643 Stop */

void rrc_generate_protocol_events
(   
    U8      log_level, 
    void    *p_api
);

/******************************************************************************
*   File management functions
******************************************************************************/

void
rrc_free
(
    void * p_var
);

rrc_return_et validate_ip_addr(U8 *p_ip_addr);

rrc_return_et validate_ipv6_addr(U8 *p_ipv6_addr);

void* cdma_sys_time_caliberate_func(void *);

U32 get_rand_val_from_seed(U32 seed);

void rrc_update_health_monitor_variable
(
    U32 health_monitor_event
);

typedef const struct
{
    S8* thrd_info;
}oamh_health_monitor_desriptors_t;

typedef const struct
{
    S8* thrd_info;
}s1ap_health_monitor_desriptors_t;

#ifdef RRC_PROFILER
extern void sigusr1_handler(int sig);
#endif
/* SPR 16017 Fix Start */ 
rrc_return_et rrc_check_logger_type(U32);
/* SPR 16017 Fix Stop */ 

#ifdef LTE_EMBMS_SUPPORTED
void rrc_asn1PrtToStr_MCCH_Message( U32 log_level, 
        const char* name, 
        MCCH_Message* pvalue) ;
#endif
char* change_data_to_str(U16 len, U8*data);
#endif /* _RRC_COMMON_UTILS_H_ */

#endif
