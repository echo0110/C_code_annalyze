/***********************************************************************
************************************************************************
** FILE        : GTPU Entry defines the entry point of the module
**
************************************************************************
**
** FILE NAME   : egtpu_entr.c
**
** DESCRIPTION : This file contains the process_message for GTPU.
**
** DATE        NAME            	REFERENCE    	REASON
** ----        ----            	---------    	------
** Oct,2009    Vipul Aggarwal					Initial
** 
**  Copyright (C) 2009 Aricent Inc. All Rights Reserved
**
************************************************************************
***********************************************************************/

#include <egtpu_glob.h>
#include <egtpu_apihdr.h>
#include <egtpu_error.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_trace.h>

#include <egtpu_util.h>

#include <egtpu_parse.h>
#include <egtpu_cdb.h>
#include <egtpu_udi.h>
#include <egtpu_relay.h>
#include <egtpu_sap.h>
#include <egtpu_init.h>
#include <egtpu_tmr.h>
#include <egtpu_pmm.h>

typedef egtpu_return_t (*evt_parser_t) (egtpu_global_t *,egtpu_info_t *);
typedef egtpu_return_t (*evt_handler_t) (egtpu_global_t *,egtpu_info_t *);

/***********************************************************************
 * DATA FUNCTION
 *     Event service functions per external entity
 **********************************************************************/
struct evt_service {
	evt_parser_t  evt_parser;
    evt_handler_t evt_handler;
};

static struct evt_service udp_service[] = {
    {
        egtpu_parse_msg,
        egtpu_unitdata_ind
    }
};

static struct evt_service relay_service[] = {
    {
        EGTPU_NULL,
        egtpu_data_req
    },
    {
        EGTPU_NULL,
        egtpu_reloc_data_req
    },
    {
        EGTPU_NULL,
        egtpu_send_emi_to_peer_req
    }
#ifdef ENDC_ENABLED
    ,{
        EGTPU_NULL,
        egtpu_x2u_delivery_status
     }
#endif
};

static struct evt_service cp_service[] = {
    {
        egtpu_setup_sap_req_parse,
        egtpu_setup_sap_req
    },
    {
        EGTPU_NULL,
        egtpu_release_sap_req
    }
};

static struct evt_service lm_service[] = {
    {
        EGTPU_NULL,
        egtpu_initdb_req
    },
    {
        EGTPU_NULL,
        egtpu_set_timer_req
    },
    {
        EGTPU_NULL,
        egtpu_get_timer_req
    },
    {
        EGTPU_NULL,
        egtpu_get_max_retries_req
    },
    {
        EGTPU_NULL,
        egtpu_set_max_retries_req
    },
    {
        EGTPU_NULL,
        egtpu_get_stats_req
    },
    {
        EGTPU_NULL,
        egtpu_get_init_stats_req
    },
    {
        EGTPU_NULL,
        egtpu_init_trace_level_req
    },
    {
        EGTPU_NULL,
        egtpu_set_trace_level_req
    },
    {
        EGTPU_NULL,
        egtpu_get_trace_level_req
    },
    {
        egtpu_get_ctxt_info_req_parse,
        egtpu_get_ctxt_info_req
    },
    {
        EGTPU_NULL,
        egtpu_modify_reordering_ie_req 
    }

};

static struct evt_service ts_service[] = {
    {
        EGTPU_NULL,
        /* CSR 00024127 Fix Start */
        EGTPU_NULL
       /* egtpu_echo_timeout */
        /* CSR 00024127 Fix End */
    },
    
    {
        EGTPU_NULL,
        egtpu_reorder_timeout
    }

};

/***********************************************************************
 * FSM Entry Point Table
 **********************************************************************/
static struct evt_service *evt_service_tbl[] = {
    EGTPU_NULL,              /* self */
    udp_service,            /* UDP */
    relay_service,          /* relay */
    cp_service,             /* control plane */
    lm_service,             /* layer management */
    ts_service              /* timer server */
};

/* a wrapper to enable tracing for handlers */
#define __CALL_HANDLER__(p_global,src, idx, msg_info)                           \
    do {                                                               \
        EGTPU_TRACE_HLR_ENTR (p_global,egtpu_in_api_name_arr[src][idx]);          \
        evt_service_tbl[src][idx].evt_handler (p_global,&(msg_info));           \
        EGTPU_TRACE_HLR_EXIT (p_global,egtpu_in_api_name_arr[src][idx]);          \
    } while (0)

/********************************************************************************
 * Function Name  : egtpu_process_msg
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_msg
 * Outputs        : None
 * Returns        : None
 * Description    : This function is the entry point to the GTP-U module.
 *                  This function takes the API message received and calls the
 *                  required handler function.
 *                  In some cases parser is called prior to handler function.
 ********************************************************************************/
egtpu_void_t egtpu_process_msg(egtpu_global_t *p_global, U8bit *p_egtpu_msg)
{
    egtpu_info_t      egtpu_msg_info;
    egtpu_entity_id_t src = EGTPU_API_GET_SRC_ID (p_egtpu_msg);
    egtpu_api_id_t    api_id = EGTPU_API_GET_API_ID (p_egtpu_msg);
    U32bit           idx;

     EGTPU_TRACE_FN_ENTR (p_global,"egtpu_process_msg");
    egtpu_msg_info.p_buff = p_egtpu_msg;
    egtpu_msg_info.ie_presence = 0;
    egtpu_msg_info.mem_flag = EGTPU_FALSE;
    /** added for sequence disabling **/
    egtpu_msg_info.seq_disable = 0;
    /* + coverity 11453 */
    egtpu_msg_info.cellIndex = 0; 
    egtpu_msg_info.teid_self = 0; 
    egtpu_msg_info.pdu_type = 0; 
    egtpu_msg_info.p_peer_addr = EGTPU_NULL; 
    egtpu_msg_info.p_src_addr = EGTPU_NULL; 
    egtpu_msg_info.p_pvt_extn = EGTPU_NULL;
    /* - coverity 11453 */

    /* validation of sender and api */
    /* src(0) is not a possibility --- GTP-U dont send a msg to self */
    if (!src || (src >= EGTPU_NUM_EI) ||
            (api_id < egtpu_in_api_base_arr[src]) ||
            (api_id > egtpu_in_last_api[src]))
	{
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNKNOWN_API,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],api_id, src));

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_UNKNOWN_API, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, 
                api_id,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/

        /* SPR 2007 Fix - Part2 Start*/
        /*EGTPU_FREE_MSG_BUF (p_egtpu_msg);*/
        /* SPR 2007 Fix - Part2 End*/
        EGTPU_TRACE_FN_EXIT (p_global,"egtpu_process_msg");
        return;
    }
    /* dump the incoming buffer */
    EGTPU_PRINT_IN_TRACE (p_global,p_egtpu_msg, src, api_id);

    /* check if init of the GTP-U entity is done before receiving
     * any api other than EGTPU_INITDB_REQ */
    if (!(p_global->stack_initdb_done) && (api_id != EGTPU_INITDB_REQ))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_INIT_NOT_DONE, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/

        /* no point in putting an error trace as they are off at
         * this time... notify LM */
         /* + SPR_17858_667_CHANGES */
        /*egtpu_send_alarm_notification (p_global,EGTPU_CERR_INIT_NOT_DONE, 0);*/
        /* + SPR_17858_667_CHANGES */
        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_msg);*/
        /* SPR 2007 Fix - Part2 End*/
        EGTPU_TRACE_FN_EXIT (p_global,"egtpu_process_msg");
        return;
    }
	
    idx = api_id - egtpu_in_api_base_arr[src];
    /* Coverity 54579  */
    if (evt_service_tbl[src][idx].evt_parser)
    {
        if (evt_service_tbl[src][idx].evt_parser (p_global,&egtpu_msg_info))
		{
            EGTPU_TRACE_FN_EXIT (p_global,"egtpu_process_msg");
            return; /* error has already been logged, buffer freed */
        }
    }
    /* Coverity 54579  */
    __CALL_HANDLER__ (p_global,src, idx, egtpu_msg_info);

    EGTPU_TRACE_FN_EXIT (p_global,"egtpu_process_msg");
}
