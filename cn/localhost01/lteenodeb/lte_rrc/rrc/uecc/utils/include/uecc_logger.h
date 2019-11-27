/******************************************************************************
*
*   FILE NAME:
*       uecc_logger.h
*
*   DESCRIPTION:
*       This is the file which contains logging and tracing definitions for UECC
*
*   DATE            AUTHOR      REFERENCE       REASON
*   18 Aug 2009     AlexK       ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_LOGGER_H
#define UECC_LOGGER_H

#include "rrc_common_utils.h"
#include "uecc_ue_ctx.h"
#include "uecc_fsm_common.h"

extern const S8 *rrc_uecc_log_facility_name;
/*SPR 15896 Fix Start*/
extern void trace_logging(uecc_ue_context_t *p_uecontext,U8 log_depth ,U8 interface,const char* format,...);
extern void rrc_trace_max_file_write(uecc_ue_context_t *p_ue_context,int message ,U32 buffer_len,U8 *buffer, U8 interface);
/*SPR 15896 Fix Stop*/

#if (LOGGER_TYPE == RRC_ADV_LOGGER)

/* SPR 16017 Fix Start */
#define RRC_UECC_CHECK_LOG_LEVEL(is_log_level_set, log_level) \
{\
    AdvL_LogTarget logTarget = LogTarget_None; \
    if(LogSuccess == check_log_level(g_advl_ctxt_rrc, log_level, &logTarget)) {\
    is_log_level_set = 1; \
    }\
}
/* SPR 16017 Fix Stop */
#ifdef LINUX_PC_TEST
#define RRC_UECC_TRACE(context_index,facility_name,log_level, format, ...) \
	RRC_LOG(facility_name, log_level, format, ##__VA_ARGS__)
#else
#define RRC_UECC_TRACE(context_index,facility_name,log_level, format, ...) \
    RRC_UECC_LOG(context_index,facility_name, log_level, format, ##__VA_ARGS__)
#endif
/* SPR 21275 changes start */
#define RRC_UECC_LOG(context_index,facility_name, log_level, format, ...) \
{\
    AdvL_LogTarget logTarget = LogTarget_None; \
    if(LogSuccess == check_log_level(g_advl_ctxt_rrc, (AdvL_LogLevel)log_level, &logTarget)) {\
        if (LogTarget_Shared_Mem == logTarget) \
        {\
    	    lgr_log_level_et logLevel = LGR_TRACE; \
            logLevel = rrc_map_log_level((AdvL_LogLevel)log_level);\
            print_log_message(logLevel, __FILE__, __LINE__, __func__,\
                    (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), LGR_FALSE, (const char*)format, ##__VA_ARGS__); \
        }\
        else \
        write_log_message(g_advl_ctxt_rrc, log_level, __FILE__, __LINE__, \
                __func__ , (const char*)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), (const char*)format, ##__VA_ARGS__); \
	}\
}
/* SPR 21275 changes end */

#elif (LOGGER_TYPE > RRC_ADV_LOGGER)
    #error Wrong logger type specified!
#else
    #define RRC_UECC_TRACE(context_index,facility_name,log_level, format, ...) \
           RRC_LOG(facility_name, log_level, format, ##__VA_ARGS__)

#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */
/*SPR 15896 Fix Start*/ 
#define P_RRC_TRACE(p_ue_context,log_depth,interface,format,...) \
  trace_logging(p_ue_context, log_depth,interface,(const char*)format,##__VA_ARGS__)
/*SPR 15896 Fix Stop*/ 


#define IF_TRACE_IS_MEDIUM \
    if ( (PNULL != p_ue_context) && \
            ( p_ue_context->p_ue_trace_activation_info != PNULL) && \
            ( p_ue_context->p_ue_trace_activation_info->trace_depth == MEDIUM || \
              p_ue_context->p_ue_trace_activation_info->trace_depth == MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION ))
#ifdef RRC_DEBUG
    #define RRC_UECC_UT_TRACE_ENTER(p_gb_context)    RRC_UECC_TRACE( \
            p_gb_context->context_index, p_gb_context->facility_name, \
            RRC_DETAILED,\
            "Entering function: %s, in file: %s \n",__FUNCTION__,__FILE__)

    #define RRC_UECC_UT_TRACE_EXIT(p_gb_context)     RRC_UECC_TRACE( \
            p_gb_context->context_index, p_gb_context->facility_name, \
            RRC_DETAILED,\
            "Exiting function: %s, in file: %s \n",__FUNCTION__,__FILE__)
#else
    #define RRC_UECC_UT_TRACE_ENTER(p_gb_context)
    #define RRC_UECC_UT_TRACE_EXIT(p_gb_context)
#endif /* RRC_DEBUG */


const S8* uecc_child_fsm_get_event_name(uecc_fsm_event_et     event_id);
/* SPR 22491 Fix Start */
#define UECC_FSM_SET_MAIN_STATE_TRACE(p_ue_context)\
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
         p_ue_context->p_gb_context->facility_name, \
         RRC_INFO, "[UE:%u] Set Main State [%s]",\
        (p_ue_context)->ue_index,\
        (p_ue_context)->uecc_fsm.p_current_state->s_name)

#define UECC_FSM_SET_CHILD_STATE_TRACE(p_ue_context)\
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
        p_ue_context->p_gb_context->facility_name, \
        RRC_INFO, "[UE:%u] [%s] Set Child State [%s]",\
        (p_ue_context)->ue_index,\
        (p_ue_context)->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        (p_ue_context)->uecc_fsm.proc_descriptor.p_current_state->s_name)

#define UECC_FSM_EVENT_TRACE(p_ue_context, event)\
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
        p_ue_context->p_gb_context->facility_name, \
        RRC_DETAILED, "[UE:%u] [EVENT:%s]",\
        (p_ue_context)->ue_index,\
        uecc_child_fsm_get_event_name(event))
/* SPR 22491 Fix End */

#endif /* UECC_LOGGER_H */

