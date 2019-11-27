/* vim:ts=4:et:ai:ci:sw=4
*/
/*****************************************************************************
 * File Name   : oam_config_mgmt_fsm.c
 *
 * Description : This file contains OAM Configuration Manager FSM Handler and
 *               its associated routines.
 *
 * Revision History:
 *
 * Date         Author           Reference          Comments
 * -----        ------           ---------          --------
 * April,2012   Nitin Agarwal  OAM Design Document  Initial Version
 * Jan, 2014        Ravi Gupta              CSR 00058940 Fix
 * April 2014       Nitin/Vinod             Cell State handling for multicell
 * May,2014     Shalu Sagar      Klockwork Fix     Code Changes for Klockwork Warnings
 * JUne 2014    Shalu Sagar        SPR 12225 FIX
 *
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "oam_config_mgmt.h"
#include <oam_types.h>
#include "oam_rrm_composer.h"
#include "oam_rrc_il_composer.h"
#include <oam_proto.h>
#include <oam_l2_if.h>
#include<oam_s1ap_il_composer.h>
#include<oam_s1ap_il_parser.h>
#include<oam_x2ap_il_composer.h>
#include<oam_x2ap_il_parser.h>
#include<oam_rrc_il_composer.h>
#include<oam_rrc_il_parser.h>
#ifdef LTE_EMBMS_SUPPORTED
#include<oam_m2ap_il_composer.h>
#include<oam_m2ap_il_parser.h>
#include<rrc_m2apOam_intf.h>
#endif
#include "oam_son_composer.h"
#include "oam_son_parser.h"
#include<oam_perf_mgmt.h>
#include<oam_sim.h>
#include<oam_sim_utils.h>
#include<oam_hm.h>
#include<oam_fm_alarm_defs.h>

#ifdef LTE_EMBMS_SUPPORTED
#include<oam_embms_proto.h>
#endif

/* LWA changes start*/
/* LWA changes end*/

/** OAM RRH Support **/
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
    #include <oam_rcm_if.h>
    #include<oam_rcm_proto.h>
    #include<oam_rcm_core.h>
#endif
#endif
#include<oam_rrm_parser.h>
#define OAM_CM_FSM_NAME "OAM Configuration Manager FSM"
#define OAM_CM_FSM_GET_CURRENT_STATE(p_cm_cntxt) \
        (p_cm_cntxt->state)
#define OAM_CM_FSM_SET_STATE(p_cm_cntxt, new_state)\
          p_cm_cntxt->state = new_state;

/* SPR 11175 FIX START */
/* CODE DELETED */
/* SPR 11175 FIX END */
extern int g_no_of_arguments;
extern QTIMER g_admin_state_timer;
extern UInt16 g_admin_state_trans_id; 
extern oam_prov_req_t oam_prov_req;
extern prov_request_t prov_req;
int init_time_startup = OAM_ONE;
/* Start: CSR 00058940 */
extern UInt8 initialisation_flag;
/*End: CSR 00058940 */
/*SPR 21096 Start*/
UInt8 num_mac_init_cell[OAM_MAX_NUM_L2_INSTANCE];
/*SPR 21096 End*/
/* SPR 20908 Start */
extern lower_layer_init_info_t l2_comm_info[OAM_MAX_NUM_L2_INSTANCE];
/* SPR 20908 End */
/* SPR 21889 +- */ 
oam_return_et oam_inform_value_change_event(  
        oam_inform_api_t    *p_input_inform_api, 
        UInt16              sub_module_id,
        UInt16              dest_module_id);

oam_return_et
oam_startup_init
(
    oam_sim_error_info_et *p_error_code
) ;

#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
oam_return_et oam_cm_rrh_msg_handler(void *p_msg, oam_cm_cntxt_t *p_cm_cntxt,
                     oam_error_code_et *p_err);
#endif
#endif

/*OAM CM FSM context*/
oam_cm_cntxt_t *p_cm_cntxt;


/*Global bitmask to implement the response received from
 *the layers
 */
LOCAL UInt16 resp_flag = OAM_ZERO;

/*Strings for various states*/
SInt8 * OAM_CM_STATE[]=
{
    (SInt8 *)"OAM_CM_ST_IDLE",                         
    (SInt8 *)"OAM_CM_ST_LAYER_INIT",                   
    (SInt8 *)"OAM_CM_ST_LAYER_WF_RES",                 
    (SInt8 *)"OAM_CM_ST_ACTIVE"                       
};

/*Strings for various events*/
SInt8 * OAM_CM_EVENT[]=
{
    (SInt8 *)"OAM_CM_EV_LAYER_REQ",
    (SInt8 *)"OAM_CM_EV_LAYER_RES",
    (SInt8 *)"OAM_CM_EV_TIMER_EXPIRY",
    (SInt8 *)"OAM_CM_EV_ABORT"
};

oam_return_et oam_hdle_x2ap_oam_prov_resp( void *p_api, oam_length_t data_len);

/****************************************************************************
 * Two dimensional functional pointer for FSM
 ****************************************************************************/
typedef oam_return_et (*oam_cm_func_ptr)(void *p_msg,
                                            oam_cm_cntxt_t *p_cm_cntxt,
                                            oam_error_code_et *p_err);
/*OAM_REVIEW_CHANGES*/
oam_cm_func_ptr oam_cm_fsm_tbl[OAM_CM_ST_MAX][OAM_CM_EV_MAX];
/*OAM_REVIEW_CHANGES*/

/* SPR 20908 Start */
oam_return_et
oam_check_instance_id_present(
        UInt8 instance_id,
        UInt8 start_idx,
        U8 *cell_index)
{
    UInt8 counter = OAM_ZERO;
    /*SPR 21321 Start*/
    for(counter = start_idx; counter < oam_prov_req.num_cells_configured; counter++)
    /*SPR 21321 End*/
    {
            /* SPR 21206 Start */
            if(instance_id == oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[counter].operator_info.associated_layer2_inst)
            /* SPR 21206 End */
            {
                *cell_index = counter;
                return OAM_SUCCESS;
            }
    }
    return OAM_FAILURE;
}
	oam_return_et
find_l2_instance_id_from_cell_index(
UInt8 cell_index,
UInt8 *instance_id)
{
    UInt8 num_instance = OAM_ZERO;
    UInt8 cid = OAM_ZERO;
    /*SPR 21889 Start */
    for(num_instance = OAM_ZERO; num_instance < OAM_MAX_NUM_L2_INSTANCE; num_instance++)
    {
        for(cid = OAM_ZERO; cid < oam_prov_req.l2_setup_info.instance_info[num_instance].l2_cell_info.num_cells; cid++)
        {
            if(cell_index == oam_prov_req.l2_setup_info.instance_info[num_instance].l2_cell_info.cell_info[cid].cell_idx)
            {
                *instance_id = num_instance;
                return OAM_SUCCESS;
            }
        }
    }
    /*SPR 21889 End */
	/*SRP 21096 Start*/
                return OAM_FAILURE;
	/*SRP 21096 End*/
}
/* SPR 20908 End */
/****************************************************************************
 * Function Name  : oam_cm_fsm_process_event
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : Entry function for OAM CM FSM
 ****************************************************************************/
oam_return_et
oam_cm_fsm_process_event(   	
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err) /*Pointer to error code*/
{
    /* coverity 35624 */
    oam_cm_states_et current_state = OAM_CM_ST_IDLE; /*KLOCWORK FIX*/
    /* coverity 35623 */
    oam_cm_events_et current_event = OAM_CM_EV_LAYER_REQ; /*KLOCWORK FIX*/
    oam_return_et ret_val = OAM_SUCCESS;
    UInt16    api_id  = OAM_ZERO;

    OAM_FUN_ENTRY_TRACE();
/*OAM_REVIEW_CHANGES*/
/* Coverity FIX 41063 */
    OAM_NULL_CHECK(OAM_NULL != p_msg);
    OAM_NULL_CHECK(OAM_NULL != p_cm_cntxt);
/* Coverity FIX 41062 */
    OAM_NULL_CHECK(OAM_NULL != p_err);
/*OAM_REVIEW_CHANGES*/
    
	
    api_id     = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);

	switch(api_id)
		{
		case MAC_INIT_LAYER_IND:
		case MAC_INIT_LAYER_CNF:
		case RLC_INIT_LAYER_IND:
		case RLC_INIT_LAYER_CNF:
		case PDCP_INIT_LAYER_IND:
		case PDCP_INIT_LAYER_CNF:
		case EGTPU_LM_INITDB_CNF:
			
          current_state = p_cm_cntxt->state;
          current_event = p_cm_cntxt->event;
			break;
			
		default:
    /*Get the state and event from context struct*/
    current_state = p_cm_cntxt->state;
    current_event = p_cm_cntxt->event;
			break;
		}

    /*Check range of passed event*/
/*OAM_REVIEW_CHANGES*/
    if ((current_state < OAM_CM_ST_MAX) 
         && (current_event < OAM_CM_EV_MAX))
/*OAM_REVIEW_CHANGES*/
    {    
        OAM_LOG(OAM, OAM_INFO, "### OAM_CM_FSM: Processing State %s Event %s ###",
                OAM_CM_STATE[current_state], OAM_CM_EVENT[current_event]);
        /*Checking the return value*/
        if(OAM_SUCCESS == oam_cm_fsm_tbl[current_state][current_event](p_msg, 
                                                                       p_cm_cntxt,
                                                                       p_err))
        {
            ret_val = OAM_SUCCESS;
        }
        else
        {
            OAM_LOG(OAM, OAM_INFO, "Not processed Event:%d, State:%d. oam_cm_fsm_table failed",
                    current_event,current_state);
/*OAM_REVIEW_CHANGES*/
            /* SPR 19110 Fix +- */
            ret_val = OAM_FAILURE;
        }
/*OAM_REVIEW_CHANGES*/
        OAM_LOG(OAM, OAM_INFO, 
                "### OAM_CM_FSM: State change: %s -> %s Event: %s ###",
                OAM_CM_STATE[current_state], 
                OAM_CM_STATE[p_cm_cntxt->state], 
                OAM_CM_EVENT[current_event]);
    }
    else
    {
/*OAM_REVIEW_CHANGES*/
        OAM_LOG(OAM, OAM_INFO, "Unexpected Event:[%d], State:[%d]",
                    current_event, current_state); /* Coverity Fix: Vivek */
        ret_val = OAM_FAILURE;
    }
/*OAM_REVIEW_CHANGES*/
    
    OAM_FUN_EXIT_TRACE();
    return ret_val;
}
/*SPR 21889 +- */
/****************************************************************************
 * Function Name  : oam_son_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from SON
 ****************************************************************************/
#ifdef OAM_SON_ENABLE 
oam_return_et
oam_son_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
   )
{
	UInt16    api_id  = OAM_ZERO;
	UInt16    msg_size = OAM_ZERO;
	UInt16    data_len = OAM_ZERO;
	UInt16    old_trans_id = OAM_ZERO;
	UInt16    new_trans_id = OAM_ZERO;
    /* SPR 13832 START */ 
    UInt32    timer_data   = OAM_ZERO;
    /* SPR 13832 END */ 
	UInt16    guard_time = OAM_INIT_TIMER_VALUE;
	oam_timer_t  timer;
	oam_return_et  ret_val = OAM_SUCCESS;

#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	UInt32 rrh_layer_init_flag =
		oam_prov_req.rrh_flag && oam_prov_req.rcm_flag;
#endif
#endif

	oam_son_init_config_resp_t oam_son_init_config_resp;
	SInt32 length_read;
/* SPR_13879_FIX Start */
    Char8 *p_al_add_info = OAM_NULL;
    Char8 *p_al_add_txt = OAM_NULL;
/* SPR_13879_FIX End */

	OAM_FUN_ENTRY_TRACE();

	api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);
    msg_size = get_word_from_header((UInt8*)(p_msg) + OAM_INTF_HDR_MSG_SIZE_OFFSET);

	/*Below params are required for further APIs*/
	data_len = msg_size - LOWER_LAYER_INTERFACE_HEADER_SIZE;

	new_trans_id = oam_get_fresh_trans_id(old_trans_id);

	switch (api_id)
	{
		case SON_OAM_INIT_IND:
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
			if ((rrh_layer_init_flag && RCM_OAM_INIT_CNFM & resp_flag) ||
					/*SPR 21212 Start*/					
					(!rrh_layer_init_flag && (EGTPU_OAM_INIT_CNF & resp_flag)))
				/*SPR 21212 End*/					
#else
/*SPR 21889 +- */
/*SPR 21212 Start*/
				if(resp_flag & EGTPU_OAM_INIT_CNF)
/*SPR 21212 End*/
#endif
#else
					/*SPR 21212 Start*/
					if(resp_flag & EGTPU_OAM_INIT_CNF)
						/*SPR 21212 End*/

#endif
				{
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",SON_MODULE_ID); 
                 /* SPR 13832 END */ 
					guard_time = OAM_INIT_TIMER_VALUE;
#ifndef OAM_UT_FRAMEWORK
					timer = oam_start_timer(guard_time, &timer_data,
							sizeof(timer_data), OAM_FALSE);
					if (OAM_NULL == timer)
					{
						ret_val = OAM_FAILURE;
						break;
					}
                 p_cm_cntxt->guard_timer_id = timer;
#endif

					if (OAM_SUCCESS != oam_son_send_son_oam_init_config_req(
								&(oam_prov_req.oam_son_req.oam_son_init_config_req),
								OAM_MODULE_ID, 
                                                                 SON_MODULE_ID,
                                                                 new_trans_id,
                           oam_prov_req.cell_cntxt.curr_cell_cid))
					{
						OAM_LOG(OAM, OAM_ERROR, "Sending SON_OAM_INIT_CONFIG_REQ message failed, cell_index = %d",
                                                                            oam_prov_req.cell_cntxt.curr_cell_cid);
						/*OAM_REVIEW_CHANGES*/
                      /* Klockwork Fix */
                        /* Coverity 64976 Changes Start */
                        if(OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                        {
                            OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                        }
                        /* Coverity 64976 Changes End */
						/*OAM_REVIEW_CHANGES*/
						ret_val = OAM_FAILURE;
						break;
					} 
					else
					{
						OAM_LOG(OAM, OAM_DETAILED, "Sending SON_OAM_INIT_CONFIG_REQ message success");
					}

					/*After sending init config request change the state 
					 *to waiting for response
					 */
					p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
				}
			break;

		case SON_OAM_INIT_CONFIG_RESP:
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
			if ((rrh_layer_init_flag && RCM_OAM_INIT_CNFM & resp_flag) ||
					(!rrh_layer_init_flag && (OAM_ZERO == resp_flag)))
#else
                /* SPR 21983 Fix +- */
/*SPR 21212 Start*/
				if(resp_flag & EGTPU_OAM_INIT_CNF)
/*SPR 21212 End*/
#endif
#endif
				{
					if(OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
					{
						OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
						ret_val = OAM_FAILURE;
						break;
					}

/* SPR_13879_FIX Start */
                    ret_val = oam_parse_son_oam_init_config_resp(&oam_son_init_config_resp,
								p_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE ,
                                                                    data_len, &length_read);
                    if (OAM_SUCCESS != ret_val)
					{
                        OAM_LOG(OAM, OAM_WARNING, "Failed to parse SON_OAM_INIT_CONFIG_RESP message");
						break;
					}
					else
					{
                        OAM_LOG(OAM, OAM_DETAILED, "SON_OAM_INIT_CONFIG_RESP message parsing success");
                    }

                    if(SON_SUCCESS != oam_son_init_config_resp.result)
                    {
                        OAM_LOG(OAM, OAM_WARNING, "SON_OAM_INIT_CONFIG_RESP failed"
                                " with response code: %d",
                                oam_son_init_config_resp.error_code);
                        p_al_add_txt = oam_son_resp_code_to_str(oam_son_init_config_resp.error_code);
                        ret_val = OAM_FAILURE;
                    }
					else
					{
						OAM_LOG(OAM, OAM_DETAILED, "SON_OAM_INIT_CONFIG_RESP message result success");
					}

                    if(OAM_FAILURE == ret_val)
                    {
                        p_al_add_info = "SON Layer initialization failed";

                        oam_handle_init_config_failure(
                                CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                                p_al_add_txt, p_al_add_info);
                        break;
                    }
/* SPR_13879_FIX End */
					resp_flag |= SON_OAM_INIT_CNF;
					p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
				}
			break;

		default:
			OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]",
            api_id);
			break; 
	}

	/*Check if below code is required*/
	/*Abort all the processes in case layer init failed*/
	if (OAM_FAILURE == ret_val)
	{
		p_cm_cntxt->event = OAM_CM_EV_ABORT;

		/*Cov Fix*/

		ret_val = oam_cm_fsm_process_event(p_msg, p_cm_cntxt, p_err);
	}

	OAM_FUN_EXIT_TRACE();
	return ret_val;
}

/****************************************************************************
 * Function Name  : oam_cm_son_req
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the messages received from SON
 ****************************************************************************/
oam_return_et
oam_cm_son_req (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err) /*Pointer to error code*/
{
    oam_return_et  ret_val = OAM_SUCCESS;

    OAM_FUN_ENTRY_TRACE();

    ret_val = oam_son_msg_handler(p_msg, p_cm_cntxt, p_err);

    /*Checking for return value*/
    if(ret_val == OAM_FAILURE) {
        OAM_LOG(OAM, OAM_ERROR, "Error:%d Sending message failed", *p_err);
    }

    OAM_FUN_EXIT_TRACE ();
    return ret_val;
}

/****************************************************************************
 * Function Name  : oam_cm_first_layer_init_req
 * Description    : Initializes the first layer
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_cm_first_layer_init_req(
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err) /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et  ret_val = OAM_SUCCESS;
#ifdef OAM_RADIO_HEAD_SUPPORT
    if(oam_prov_req.rrh_flag && oam_prov_req.rcm_flag) {
        ret_val = oam_cm_rrh_msg_handler(p_msg, p_cm_cntxt, p_err);
    }
    else {
	    ret_val = oam_cm_son_req(p_msg, p_cm_cntxt, p_err);
    }
#else
    ret_val = oam_cm_son_req(p_msg, p_cm_cntxt, p_err);
#endif
    /*Checking for return value*/
    if(ret_val == OAM_FAILURE)
    {
        OAM_LOG(OAM, OAM_ERROR, "Error:%d Sending message failed",
                *p_err);
    }

    OAM_FUN_EXIT_TRACE ();
    return ret_val;
}
#endif

/****************************************************************************
 * Function Name  : oam_rrm_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from RRM
 ****************************************************************************/
oam_return_et
oam_rrm_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    UInt16    api_id  = OAM_ZERO;
    UInt16    msg_size = OAM_ZERO;
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id_1 = OAM_ZERO;
    UInt16    new_trans_id_2 = OAM_ZERO;
    UInt16    timer_data   = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    oam_timer_t  timer;
    oam_return_et  ret_val = OAM_SUCCESS;
    Char8 *p_al_add_info = OAM_NULL;
    Char8 *p_al_add_txt = OAM_NULL;
    /* coverity 35627 */
    oam_rrm_init_config_resp_t rrm_init_config_resp;
       
    /*Coverity 66673 Fix Start */
    oam_error_code_et p_error_code = NO_ERROR;
    oam_rrm_enb_config_resp_t *p_rrm_enb_config_resp=OAM_NULL;
    if(OAM_FAILURE == oam_mem_alloc(sizeof(oam_rrm_enb_config_resp_t),
			    (void*)&p_rrm_enb_config_resp,
			    &p_error_code))
    {     
	    OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_rrm_enb_config_resp failed"
                " with Error Code:%d",
			    p_error_code);
	    OAM_FUN_EXIT_TRACE();
	    return OAM_FAILURE;
    }
    /*Coverity 66673 Fix End */
    SInt32 length_read=OAM_ZERO;
    /*Coverity 66673 Fix Start */
    oam_memset(p_rrm_enb_config_resp, OAM_ZERO, sizeof(oam_rrm_enb_config_resp_t));
    /*Coverity 66673 Fix End */
#ifndef OAM_SON_ENABLE    
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	UInt32 rrh_layer_init_flag =
		oam_prov_req.rrh_flag && oam_prov_req.rcm_flag;
#endif
#endif
#endif
    OAM_FUN_ENTRY_TRACE();

    api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);
    msg_size = get_word_from_header((UInt8*)(p_msg) + OAM_INTF_HDR_MSG_SIZE_OFFSET);

   /*Below params are required for further APIs*/
    new_trans_id_1 = oam_get_fresh_trans_id(old_trans_id);
    new_trans_id_2 = oam_get_fresh_trans_id(new_trans_id_1);

    switch (api_id)
    {
         case RRM_OAM_INIT_IND:
          #ifndef OAM_SON_ENABLE    
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
            if ((rrh_layer_init_flag && RCM_OAM_INIT_CNFM & resp_flag) ||
               (!rrh_layer_init_flag && (OAM_ZERO == resp_flag)))
#else
/*SPR 21212 Start*/
          if (resp_flag & EGTPU_OAM_INIT_CNF)
/*SPR 21212 End*/
#endif 
#endif 
#else
             if (SON_OAM_INIT_CNF & resp_flag)
#endif 
             {
#ifndef OAM_UT_FRAMEWORK
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",RRM_MODULE_ID); 
                 /* SPR 13832 END */ 
                 timer = oam_start_timer(guard_time, &timer_data,
                                           sizeof(timer_data), OAM_FALSE);
                 if (OAM_NULL == timer)
                 {
                     ret_val = OAM_FAILURE;
                     break;
                 }

                 p_cm_cntxt->guard_timer_id = timer;
#endif
                if (OAM_SUCCESS != oam_rrm_send_rrm_oam_init_config_req(
                                      &(oam_prov_req.oam_rrm_req.
                                          oam_rrm_init_config_req),
                                      OAM_MODULE_ID,
                                      RRM_MODULE_ID,
                                      new_trans_id_1,
                                      oam_prov_req.cell_cntxt.curr_cell_cid))
                {
                      OAM_LOG(OAM, OAM_ERROR, "Sending RRM_OAM_INIT_CONFIG_REQ message failed, cell_index:%d",
                                                                      oam_prov_req.cell_cntxt.curr_cell_cid);
#ifndef OAM_UT_FRAMEWORK
/*OAM_REVIEW_CHANGES*/
                      /* Klockwork Fix */
                      /* Coverity 64973 Changes Start */
                      if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                      {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                      }
                      /* Coverity 64973 Changes End */
/*OAM_REVIEW_CHANGES*/
#endif
                      ret_val = OAM_FAILURE;
                      break;
                }
                OAM_LOG(OAM, OAM_DETAILED, "Sending RRM_OAM_INIT_CONFIG_REQ message success, unused parametrr %p", p_err);

                if (OAM_SUCCESS != oam_rrm_send_rrm_oam_enb_config_req(
                                     &(oam_prov_req.oam_rrm_req.
                                         oam_rrm_enb_config_req),
                                     OAM_MODULE_ID,
                                     RRM_MODULE_ID,
                                     new_trans_id_2,
                                     oam_prov_req.cell_cntxt.curr_cell_cid))
                {
                      OAM_LOG(OAM, OAM_ERROR, "Failed to send RRM_OAM_ENB_CONFIG_REQ, cell_index:%d",
                                                                oam_prov_req.cell_cntxt.curr_cell_cid);
#ifndef OAM_UT_FRAMEWORK
                      /* Klockwork Fix */
                      /* Coverity 64973 Changes Start */
                      if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                      {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                      }
                      /* Coverity 64973 Changes End */
#endif
                      ret_val = OAM_FAILURE;
                      break;
                }
                OAM_LOG(OAM, OAM_DETAILED, "Sending RRM_OAM_ENB_CONFIG_REQ message success");
                
#ifndef OAM_UT_FRAMEWORK
                p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
             }
             break;

         case RRM_OAM_INIT_CONFIG_RESP:
             p_msg += LOWER_LAYER_INTERFACE_HEADER_SIZE;
             msg_size -= LOWER_LAYER_INTERFACE_HEADER_SIZE;
             ret_val =
                 oam_parse_rrm_oam_init_config_resp(&rrm_init_config_resp,
                                                    p_msg, msg_size,
                                                    &length_read);
             if(OAM_SUCCESS != ret_val) {
                 OAM_LOG(OAM, OAM_WARNING,
                              "RRM_OAM_INIT_CONFIG_RESP message parsing failed");
                 p_al_add_txt = OAM_INTF_MSG_PARSING_FAILED_STR("RRM");
             }
             else {
                 OAM_LOG(OAM, OAM_DETAILED,
                              "RRM_OAM_INIT_CONFIG_RESP message parsing success");

                 if(RRM_SUCCESS != rrm_init_config_resp.response) {
                     p_al_add_txt = oam_rrm_resp_code_to_str(
                                        rrm_init_config_resp.fail_cause);
                     OAM_LOG(OAM, OAM_WARNING,
                              "Failure returned in RRM_OAM_INIT_CONFIG_RESP response. "
                              "Fail cause: %s", p_al_add_txt);
                     ret_val = OAM_FAILURE;
                 }
                 else {
                     OAM_LOG(OAM, OAM_DETAILED, "Success RRM_OAM_INIT_CONFIG_RESP response received");
                 }
             }

             if(OAM_FAILURE == ret_val) {
                 p_al_add_info = LAYER_INIT_FAILED_STR("RRM");

                 oam_handle_init_config_failure(
                                 CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                                 p_al_add_txt, p_al_add_info);
             }

             break;

         case RRM_OAM_ENB_CONFIG_RESP:
#ifndef OAM_UT_FRAMEWORK
             if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
             {
                 OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                 ret_val = OAM_FAILURE;
                 break;
             }
#endif
             p_msg += LOWER_LAYER_INTERFACE_HEADER_SIZE;
             msg_size -= LOWER_LAYER_INTERFACE_HEADER_SIZE;
             ret_val =
                /* Coverity 66673 Fix Start */
                oam_parse_rrm_oam_enb_config_resp(p_rrm_enb_config_resp,
                /* Coverity 66673 Fix End */
                                                   p_msg, msg_size,
                                                   &length_read);
             if(OAM_SUCCESS != ret_val) {
                 OAM_LOG(OAM, OAM_WARNING,
                              "Failed to parse RRM_OAM_ENB_CONFIG_RESP message");
                 p_al_add_txt = OAM_INTF_MSG_PARSING_FAILED_STR("RRM");
             }
	     else {
		     /* Coverity 66673 Fix Start */
		     if(RRM_SUCCESS == p_rrm_enb_config_resp->response) {
			     /* Coverity 66673 Fix End */
			     OAM_LOG(OAM, OAM_DETAILED,
					     "RRM_OAM_ENB_CONFIG_RESP message response success");
		     }
		     else {
			     p_al_add_txt = oam_rrm_resp_code_to_str(
					     /* Coverity 66673 Fix Start */
					     p_rrm_enb_config_resp->fail_cause);
			     /* Coverity 66673 Fix End */
			     OAM_LOG(OAM, OAM_WARNING,
					     "RRM_OAM_ENB_CONFIG_RESP message response failed."
					     "Fail cause: %s", p_al_add_txt);
			     ret_val = OAM_FAILURE;
		     }
	     }

             if(OAM_FAILURE == ret_val) {
                 p_al_add_info = LAYER_INIT_FAILED_STR("RRM");
                 oam_handle_init_config_failure(
                         CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                         p_al_add_txt, p_al_add_info);
                 break;
             }

             resp_flag |= RRM_OAM_INIT_CNFM;
#ifndef OAM_UT_FRAMEWORK
             p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif
             break;

         default:
             OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
             break;
    }

    OAM_FUN_EXIT_TRACE();
    /* Coverity CID 66673 Fix Start */
    oam_mem_free(p_rrm_enb_config_resp,&p_error_code);
    /* Coverity CID 66673 Fix End */
    return ret_val;
}

/****************************************************************************
 * Function Name  : oam_mac_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context                  
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from MAC
 ****************************************************************************/
oam_return_et
oam_mac_msg_handler
(
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    UInt8 received_mac_instance_idx = OAM_ZERO;    
    /* SPR 20908 Start */
    U8 cell_id = OAM_ZERO;    
    /* SPR 20908 End */
    UInt16    api_id  = OAM_ZERO;
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id_1 = OAM_ZERO;
    UInt16    new_trans_id_2 = OAM_ZERO;
    UInt16    timer_data   = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    oam_timer_t  timer;
    oam_return_et  ret_val = OAM_SUCCESS;
    UInt8 cell_num = OAM_ZERO, instance_num = OAM_ZERO;
    /*SPR 21096 +-*/
    OAM_FUN_ENTRY_TRACE();
    /*Below params are required for further APIs*/
    api_id = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);
    new_trans_id_1 = oam_get_fresh_trans_id(old_trans_id);
    new_trans_id_2 = oam_get_fresh_trans_id(new_trans_id_1);
    Char8 *p_al_add_info = OAM_NULL;
    Char8 *p_al_add_txt = OAM_NULL;
    oam_mac_init_resp_params_t mac_layer_init_resp = {OAM_ZERO};
    oam_mac_init_resp_params_t init_cell_resp =  {OAM_ZERO};
    UInt8 cid = OAM_ZERO;
#ifdef LTE_EMBMS_SUPPORTED
    SInt8 arr_idx = OAM_ZERO;
#endif
    /*SPR21369+-*/
    switch (api_id) {
        case MAC_INIT_LAYER_IND:
/*SPR 21212 Start*/
	/*SPR 21839 Start*/
            received_mac_instance_idx = 
				*((UInt8*)(p_msg) + OAM_INTF_HDR_CELL_IDX_OFFSET);
            if((resp_flag & X2AP_OAM_INIT_CNFM)&&!(PDCP_OAM_INIT_CNF & l2_comm_info[received_mac_instance_idx].init_flag)
              )
        { 
			/*SPR 21839 End*/
/*SPR 21212 End*/
            /* SPR 20908 Start */
            OAM_LOG(OAM, OAM_DETAILED, "--------SENDING MAC_INIT_LAYER_REQ "
                    ", IN MAC_INIT_LAYER_IND-----");  
            received_mac_instance_idx = 
                *((UInt8*)(p_msg) + OAM_INTF_HDR_CELL_IDX_OFFSET);
            /*SPR 21096 Start*/
			if(received_mac_instance_idx >= OAM_MAX_NUM_L2_INSTANCE)
            {
				OAM_LOG(OAM, OAM_WARNING,
                        "received Instance id in MAC_INIT_LAYER_IND: %d should be less then : %d", 
						received_mac_instance_idx, OAM_MAX_NUM_L2_INSTANCE);
                return OAM_FAILURE;
            }

            cid = L2_INSTANCE_CELL_INFO(received_mac_instance_idx, OAM_ZERO).cell_idx;
            OAM_LOG(OAM, OAM_INFO,
                    "Sending MAC_INIT_LAYER_REQ for cell_index: %d",
                    cid);
            /*SPR 21096 End*/
            ret_val = oam_check_instance_id_present(received_mac_instance_idx, OAM_ZERO, &cell_id);
            if(OAM_SUCCESS == ret_val)
            {
                oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.mac_comm_info),
                        &(l2_comm_info[received_mac_instance_idx].mac_comm_info),sizeof(rrm_oam_l2_comm_info_t));
                oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.phy_comm_info),
                        &(l2_comm_info[received_mac_instance_idx].phy_comm_info),sizeof(rrm_oam_l2_comm_info_t));
            }
            else
            {
                OAM_LOG(OAM, OAM_INFO,
                        "context is not present in oam_rrm_config_req for instance_id: %d", received_mac_instance_idx);
                return ret_val;
            }
            ret_val = oam_send_mac_init_layer_req(
                    &L2_MAC_INIT_LAYER_REQ(received_mac_instance_idx),
                    OAM_MODULE_ID,
                    new_trans_id_1, cid,
                    p_err);
            /* SPR 20908 End */
            if(OAM_SUCCESS != ret_val) {
                OAM_LOG(OAM, OAM_ERROR,
                        "Sending MAC_INIT_LAYER_REQ message failed, %d", *p_err);
                break;
            }

            OAM_LOG(OAM, OAM_DETAILED, "Starting timer, for "
                    "MAC_INIT_LAYER_IND");
#ifndef OAM_UT_FRAMEWORK
            /* SPR 13832 START */ 
            oam_sprintf((char *)&timer_data,"%d",MAC_MODULE_ID); 
            /* SPR 13832 END */ 
            timer = oam_start_timer(guard_time, &timer_data,
                    sizeof(timer_data), OAM_FALSE);
            if(OAM_NULL == timer) {
                OAM_LOG(OAM, OAM_DETAILED, "Starting timer, for "
                        "MAC_INIT_LAYER_IND got failed");
                ret_val = OAM_FAILURE;
                break;
            }
            p_cm_cntxt->guard_timer_id = timer;
#endif

			OAM_LOG(OAM, OAM_DETAILED,
                    "Sending MAC_INIT_LAYER_REQ message success");
            L2_INSTANCE_CELL_INFO(received_mac_instance_idx, OAM_ZERO).
                cell_status = OAM_WF_INIT_RESP;
            L2_INSTANCE_INFO(received_mac_instance_idx).bitmask |= L2_CELL1_INIT_REQ_SENT;

            if(L2_INSTANCE_INFO(received_mac_instance_idx).num_cells > OAM_ONE) {
                for(cell_num = OAM_ZERO;
                        cell_num < L2_INSTANCE_INFO(received_mac_instance_idx).num_cells - OAM_ONE;
                        ++cell_num) {
                    /* SPR 20908 Start */
                    ret_val = oam_check_instance_id_present(received_mac_instance_idx, cell_id + OAM_ONE, &cell_id);
                    if(OAM_SUCCESS == ret_val)
                    {
                        oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].
                                    l2_comm_info_params.mac_comm_info),
                                &(l2_comm_info[received_mac_instance_idx].mac_comm_info),sizeof(rrm_oam_l2_comm_info_t));
                        /*SPR 21096 Start*/
                        oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.phy_comm_info),
                                &(l2_comm_info[received_mac_instance_idx].phy_comm_info),sizeof(rrm_oam_l2_comm_info_t));
                        /*SPR 21096 End*/
                    }
                    else
                    {
                        OAM_LOG(OAM, OAM_INFO,
                                "context is not present in oam_rrm_config_req for instance_id: %d", received_mac_instance_idx);
                        return ret_val;
                    }

                    /* SPR 20908 End */
                    ret_val =
                        oam_send_mac_init_cell_req(
                                &L2_MAC_INIT_CELL_REQ(received_mac_instance_idx, cell_num),
                                OAM_MODULE_ID, new_trans_id_2, p_err,
                                L2_INSTANCE_INFO(received_mac_instance_idx).
                                cell_info[cell_num + 1].cell_idx);
                    if(OAM_SUCCESS != ret_val) {
                        OAM_LOG(OAM, OAM_ERROR,
                                "Error returned while sending "
                                "MAC_INIT_CELL_REQ, %d", *p_err);
                        break;
                    }

					OAM_LOG(OAM, OAM_DETAILED, "MAC_INIT_CELL_REQ [%d] "
                            "message successfully sent", cell_num + OAM_ONE);
                    L2_INSTANCE_CELL_INFO(received_mac_instance_idx, cell_num + OAM_ONE).
                        cell_status = OAM_WF_INIT_RESP;
                    L2_INSTANCE_INFO(received_mac_instance_idx).
                        bitmask |= L2_CELL2_INIT_REQ_SENT;
                }
            }
#ifndef OAM_UT_FRAMEWORK
            p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
            /*SPR 21212 Start*/
        }
        /*SPR 21212 End*/
        break;

        case MAC_INIT_LAYER_CNF:
        /* SPR 20908 Start */
        /* Code Deleted */
        cid = *((UInt8*)p_msg + OAM_INTF_HDR_CELL_IDX_OFFSET);
			ret_val = find_l2_instance_id_from_cell_index(cid, &instance_num);
        if(OAM_FAILURE == ret_val)
        {
            /*SPR 21096 Start*/
				OAM_LOG(OAM, OAM_WARNING,"instance id not exist for received cell_idex: [%d]", cid);
            /*SPR 21096 End*/
            return ret_val;
        }
        /* SPR 20908 End */
        ret_val = oam_handle_mac_init_layer_cnf(
                &mac_layer_init_resp,
                p_msg);

        if(OAM_MAC_SUCCESS !=
                mac_layer_init_resp.responseCode) {

            p_al_add_info = LAYER_INIT_FAILED_STR("MAC");
            p_al_add_txt = oam_mac_resp_code_to_str(
                    mac_layer_init_resp.responseCode);
				OAM_LOG(OAM, OAM_WARNING,
                    "MAC_INIT_LAYER_CNF message response failed"
                    "Fail cause: %s", p_al_add_txt);

            oam_handle_init_config_failure(
                    CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                    p_al_add_txt, p_al_add_info);
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }
        else {
            L2_INSTANCE_CELL_INFO(instance_num, OAM_ZERO).
                cell_status = OAM_INIT_RESP_RECEIVED;
            L2_INSTANCE_INFO(instance_num).
                bitmask &= ~L2_CELL1_INIT_REQ_SENT;
            OAM_LOG(OAM, OAM_INFO,
                    "MAC_INIT_LAYER_CNF message response success");
#ifdef LTE_EMBMS_SUPPORTED
                    arr_idx = oam_get_m2ap_cell_config_idx(cid);

                    if (OAM_ERROR_RETURN_VAL == arr_idx) {
                        OAM_LOG(OAM, OAM_ERROR, "Invalid cell context");

                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;
                    }

                    M2AP_CELL_SPEC_PARAMS(arr_idx).phy_capability_for_embms =
                        mac_layer_init_resp.phyMbsfnCapability;
#endif

        }
        /* SPR 20908 Start */
        /* Code Deleted */
        /* SPR 20908 End */

        if(L2_INSTANCE_INFO(instance_num).bitmask == OAM_ZERO) {
#ifndef OAM_UT_FRAMEWORK
            if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
            {
					OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                ret_val = OAM_FAILURE;
                break;
            }
#endif
            OAM_LOG(OAM, OAM_INFO,
                    "Received MAC_INIT_LAYER_CNF from all cells "
                    "of instance %d of MAC", instance_num);
            /*SPR 21096 +-*/
        }
        /* SPR 20908 Start */
        /* Code Deleted */
        /* SPR 20908 End */

        /*SPR 21096 Start*/
        num_mac_init_cell[instance_num]++;
	/* SPR 21889 Start */
        if(num_mac_init_cell[instance_num] ==
                L2_INSTANCE_INFO(instance_num).num_cells) {
		/* SPR 21889 End */

#ifndef OAM_UT_FRAMEWORK
            p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif
            l2_comm_info[instance_num].init_flag |= MAC_OAM_INIT_CNF;
        }
        /*SPR 21096 End*/
        break;

		case MAC_INIT_CELL_CNF:
			/*SPR 21096 +-*/
			cid = *((UInt8*)p_msg + OAM_INTF_HDR_CELL_IDX_OFFSET);
			/*SPR 21096 Start*/
			ret_val = find_l2_instance_id_from_cell_index(cid, &instance_num);
			if(OAM_FAILURE == ret_val)
			{
				/*SPR 21096 Start*/
				OAM_LOG(OAM, OAM_WARNING,"instance id not exist for received cell_idex: [%d]", cid);
				/*SPR 21096 End*/
				return ret_val;
			}
			/*SPR 21096 End*/
			if(cid ==
                    /*SPR 21369 Start*/
					L2_INSTANCE_CELL_INFO(instance_num, OAM_ONE).cell_idx) {
                    /*SPR 21369 End*/
				ret_val =
					oam_handle_mac_init_cell_cnf(&init_cell_resp,
							p_msg);
				if(OAM_SUCCESS != ret_val) {
					p_al_add_info =
						LAYER_INIT_FAILED_STR("MAC");

                p_al_add_txt =
                    oam_mac_resp_code_to_str(
                            init_cell_resp.responseCode);
					OAM_LOG(OAM, OAM_WARNING,
                        "MAC_INIT_CELL_CNF message response failed"
                        "Fail cause: %s", p_al_add_txt);

                oam_handle_init_config_failure(
                        CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                        p_al_add_txt, p_al_add_info);

                OAM_FUN_EXIT_TRACE();
                return ret_val;
            }
            else {
                OAM_LOG(OAM, OAM_INFO,
                        "MAC_INIT_CELL_CNF received for cell[%d] of "
                        "instance[%d]", cell_num,
                        instance_num);
                /*SPR 21369 start*/
                L2_INSTANCE_CELL_INFO(instance_num, OAM_ONE).
                    /*SPR 21369 End*/
                    cell_status = OAM_INIT_RESP_RECEIVED;
                L2_INSTANCE_INFO(instance_num).
                    bitmask &= ~L2_CELL2_INIT_REQ_SENT;
#ifdef LTE_EMBMS_SUPPORTED
                        arr_idx = oam_get_m2ap_cell_config_idx(cid);

                        if (OAM_ERROR_RETURN_VAL == arr_idx) {
                            OAM_LOG(OAM, OAM_ERROR, "Invalid cell context");

                            OAM_FUN_EXIT_TRACE();
                            return OAM_FAILURE;
                        }

                        /* SPR 17956 Fix Start */
                        M2AP_CELL_SPEC_PARAMS(arr_idx).phy_capability_for_embms =
                            init_cell_resp.phyMbsfnCapability;
                        /* SPR 17956 Fix End */
#endif
            }
            /*SPR 21369 +-*/
        }
        /*SPR 21096 +-*/
        if(!L2_INSTANCE_INFO(instance_num).bitmask) {
            OAM_LOG(OAM, OAM_INFO,
                    "Received MAC_INIT_CELL_CNF from all "
                    "cells of instance %d of MAC",
                    instance_num);
            /*SPR 21096 +-*/
#ifndef OAM_UT_FRAMEWORK
            ret_val = oam_stop_timer(p_cm_cntxt->guard_timer_id);
            if(OAM_SUCCESS != ret_val) {
					OAM_LOG(OAM, OAM_DETAILED,
                        "Error returned while stopping the timer");
            }
#endif
        }
        else {
            OAM_LOG(OAM, OAM_DETAILED,
                    "Waiting for MAC_INIT_CELL_CNF from other cells");
        }
        /*SPR 21096 +-*/
        /*SPR 21096 Start*/
        num_mac_init_cell[instance_num]++;
	/* SPR 21889 Start */
        if(num_mac_init_cell[instance_num] ==
                L2_INSTANCE_INFO(instance_num).num_cells) {
		/* SPR 21889 End */
#ifndef OAM_UT_FRAMEWORK
            p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif
            l2_comm_info[instance_num].init_flag |= MAC_OAM_INIT_CNF;
            /*SPR 21096 End*/
        }
        break;

        default:
			OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
        break;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/****************************************************************************
 * Function Name  : oam_rlc_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context                  
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from RLC
 ****************************************************************************/
oam_return_et
oam_rlc_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
	UInt16    api_id  = OAM_ZERO;
	UInt16    old_trans_id = OAM_ZERO;
	UInt16    new_trans_id = OAM_ZERO;
	UInt16    timer_data   = OAM_ZERO;
	UInt16    guard_time = OAM_INIT_TIMER_VALUE;
	oam_timer_t  timer;
	/* SPR 20908 Start */
	UInt8 cell_num = OAM_ZERO;
	U8 cell_id = OAM_ZERO;
	UInt8 received_rlc_instance_idx = OAM_ZERO;
	/* SPR 20908 End */
	oam_return_et  ret_val = OAM_SUCCESS;
	UInt8 cid = OAM_ZERO;
	/* SPR 14082 START */
	Char8 *p_al_add_info = OAM_NULL;
	Char8 *p_al_add_txt = OAM_NULL;
	/* SPR 14082 END */


	OAM_FUN_ENTRY_TRACE();

	/*Below params are required for further APIs*/
	api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);
	new_trans_id = oam_get_fresh_trans_id(old_trans_id);

    switch (api_id)
    {
         case RLC_INIT_LAYER_IND:
		    /* SPR 20908 Start */	
            received_rlc_instance_idx = 
                         *((UInt8*)(p_msg) + OAM_INTF_HDR_CELL_IDX_OFFSET);
                 cid = L2_INSTANCE_CELL_INFO(received_rlc_instance_idx, OAM_ZERO).cell_idx;
                 if(received_rlc_instance_idx >= OAM_MAX_NUM_L2_INSTANCE)
                 {
                    OAM_LOG(OAM, OAM_WARNING,
                         "received Instance id in RLC_INIT_LAYER_IND: %d should be less then : %d", 
                         received_rlc_instance_idx, OAM_MAX_NUM_L2_INSTANCE);
                    return OAM_FAILURE;
                 }
                	/*SPR 21839 Start*/
                if((MAC_OAM_INIT_CNF & l2_comm_info[received_rlc_instance_idx].init_flag)&&
                    !(PDCP_OAM_INIT_CNF & l2_comm_info[received_rlc_instance_idx].init_flag))
                {
                	/*SPR 21839 End*/
                /* SPR 20908 End */
#ifndef OAM_UT_FRAMEWORK
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",RLC_MODULE_ID); 
                 /* SPR 13832 END */ 
                 timer = oam_start_timer(guard_time, &timer_data,
                                           sizeof(timer_data), OAM_FALSE);
                 if (OAM_NULL == timer)
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }

				p_cm_cntxt->guard_timer_id = timer;
#endif
				/* SPR 20908 Start */
				ret_val = oam_check_instance_id_present(received_rlc_instance_idx, OAM_ZERO, &cell_id);
				if(OAM_SUCCESS == ret_val)
				{
                        oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.rlc_comm_info),
                                     &(l2_comm_info[received_rlc_instance_idx].rlc_comm_info),sizeof(rrm_oam_l2_comm_info_t));
                 }
                 else
                 {
                     OAM_LOG(OAM, OAM_INFO,
                             "context is not present in oam_rrm_config_req for instance_id: %d", received_rlc_instance_idx);
                     return ret_val;
                 }
                 /* SPR 20908 End */
                 ret_val = oam_send_rlc_init_layer_req(
                                      &(oam_prov_req.l2_setup_info.instance_info[received_rlc_instance_idx].oam_l2_req.oam_rlc_req.rlc_init_req),
                                                                OAM_MODULE_ID,
                                                                new_trans_id,
                                                                cid,
                                                                p_err);
                 if(OAM_SUCCESS != ret_val) {
                      OAM_LOG(OAM, OAM_ERROR,
                          "Sending RLC_INIT_LAYER_REQ message failed: %d", *p_err);
#ifndef OAM_UT_FRAMEWORK
					/*OAM_REVIEW_CHANGES*/
                      /* Coverity 64971 Changes Start */   
                      if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                      {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                      }
                      /* Coverity 64971 Changes End */   
/*OAM_REVIEW_CHANGES*/
#endif
					ret_val = OAM_FAILURE;
					break;
				}

				if(L2_INSTANCE_INFO(received_rlc_instance_idx).num_cells > OAM_ONE) {
					for(cell_num = OAM_ZERO;
							cell_num < L2_INSTANCE_INFO(received_rlc_instance_idx).num_cells - OAM_ONE;
							++cell_num) {
						/* SPR 20908 Start */
						ret_val = oam_check_instance_id_present(received_rlc_instance_idx, cell_id + OAM_ONE, &cell_id);
						if(OAM_SUCCESS == ret_val)
						{
							oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].
										l2_comm_info_params.rlc_comm_info),
									&(l2_comm_info[received_rlc_instance_idx].rlc_comm_info),sizeof(rrm_oam_l2_comm_info_t));
						}
						else
						{
							OAM_LOG(OAM, OAM_INFO,
									"context is not present in oam_rrm_config_req for instance_id: %d", received_rlc_instance_idx);
							return ret_val;
						}

					}
				}
				/* SPR 20908 End */
				OAM_LOG(OAM, OAM_DETAILED, "Sending RLC_INIT_LAYER_REQ message success");
#ifndef OAM_UT_FRAMEWORK
				p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
			}
			break;

		case RLC_INIT_LAYER_CNF:
#ifndef OAM_UT_FRAMEWORK
			/* SPR 20908 Start */
			cid = *((UInt8*)p_msg + OAM_INTF_HDR_CELL_IDX_OFFSET);
			ret_val = find_l2_instance_id_from_cell_index(cid, &received_rlc_instance_idx);
                if(OAM_FAILURE == ret_val)
                {
                    OAM_LOG(OAM, OAM_INFO,"instance id not exist for received cell_idex: [%d]", cid);
                    return ret_val;
                }
                if(l2_comm_info[received_rlc_instance_idx].init_flag & MAC_OAM_INIT_CNF)
                {
                /* SPR 20908 End */
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }
#endif               
                 if (OAM_SUCCESS != oam_handle_rlc_init_layer_cnf(
                                                             (Oam_Rlc_Cnf_t*)p_msg))
                 {
                      OAM_LOG(OAM, OAM_WARNING, "RLC_INIT_LAYER_CNF message response is failed");
                      ret_val = OAM_FAILURE;
                      /* SPR 14082 CODE REMOVED */
                 }
                 /* SPR 14082 START */
                 if(OAM_FAILURE == ret_val) 
                 {
                     p_al_add_info = "RLC Layer initialization failed";

					oam_handle_init_config_failure(
							CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
							p_al_add_txt, p_al_add_info);
					break;
				}
				/* SPR 14082 END */
				l2_comm_info[received_rlc_instance_idx].init_flag |= RLC_OAM_INIT_CNF;
#ifndef OAM_UT_FRAMEWORK
				p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif  

			}
			break;

         default:
             OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
             break;
    }

	OAM_FUN_EXIT_TRACE();
	return ret_val;
}

/****************************************************************************
 * Function Name  : oam_pdcp_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from PDCP
 ****************************************************************************/
oam_return_et
oam_pdcp_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    UInt16    api_id  = OAM_ZERO;
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    UInt16    timer_data   = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    oam_timer_t  timer; 
    oam_return_et  ret_val = OAM_SUCCESS;
    /* SPR 20908 Start */
    UInt8 received_pdcp_instance_idx = OAM_ZERO;
    UInt8 num_cell_on_pdcp_instance = OAM_ZERO;
    UInt8 cell_num = OAM_ZERO;
    UInt8 cid = OAM_ZERO;
    U8 cell_id = OAM_ZERO;
    /* SPR 20908 End */
    /* SPR 14082 START */
    /* SPR22359 MBMS changes start */
    /* Code deleted */
    /* SPR22359 MBMS changes stop */
    /* SPR 14082 END */
    /*SPR 21212 Start*/
#ifndef LTE_EMBMS_SUPPORTED
    oam_rrm_cell_config_req_t *p_rrm_cell_config = OAM_NULL;
    UInt8    retVal = OAM_ZERO;
    UInt8 cell_cnt = OAM_ZERO;
#endif
    /*SPR 21212 End*/

    OAM_FUN_ENTRY_TRACE();

    /*Below params are required for further APIs*/
    api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);
    new_trans_id = oam_get_fresh_trans_id(old_trans_id);

    switch (api_id)
    {
        case PDCP_INIT_LAYER_IND:
        /* SPR 20908 Start */
        received_pdcp_instance_idx = 
            *((UInt8*)(p_msg) + OAM_INTF_HDR_CELL_IDX_OFFSET);
        if(received_pdcp_instance_idx >= OAM_MAX_NUM_L2_INSTANCE)
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "received Instance id in PDCP_INIT_LAYER_IND: %d should be less then : %d", 
                    received_pdcp_instance_idx, OAM_MAX_NUM_L2_INSTANCE);
            return OAM_FAILURE;
        }
	/*SPR 21839 Start*/ 
	if((RLC_OAM_INIT_CNF & l2_comm_info[received_pdcp_instance_idx].init_flag)&& 
			!(PDCP_OAM_INIT_CNF & l2_comm_info[received_pdcp_instance_idx].init_flag))
		/*SPR 21839 End*/ 
        {
            /* SPR 20908 End */
#ifndef OAM_UT_FRAMEWORK
            /* SPR 13832 START */ 
            oam_sprintf((char *)&timer_data,"%d",PDCP_MODULE_ID); 
            /* SPR 13832 END */ 
            timer = oam_start_timer(guard_time, &timer_data,
                    sizeof(timer_data), OAM_FALSE);
            if (OAM_NULL == timer)
            {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                ret_val = OAM_FAILURE;
                break;
            }

            p_cm_cntxt->guard_timer_id = timer;
#endif
            /* SPR 20908 Start */
            oam_prov_req.cell_cntxt.curr_cell_cid =
                L2_INSTANCE_CELL_INFO(received_pdcp_instance_idx, OAM_ZERO).cell_idx;
            cid = L2_INSTANCE_CELL_INFO(received_pdcp_instance_idx, OAM_ZERO).cell_idx;
            ret_val = oam_check_instance_id_present(received_pdcp_instance_idx, OAM_ZERO, &cell_id);
            if(OAM_SUCCESS == ret_val)
            {
                oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.pdcp_comm_info),
                        &(l2_comm_info[received_pdcp_instance_idx].pdcp_comm_info),sizeof(rrm_oam_l2_comm_info_t));
                oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.bitmask |= RRM_OAM_NON_CRAN_PDCP_COMM_INFO_PRESENT;
            }
            else
            {
                OAM_LOG(OAM, OAM_INFO,
                        "context is not present in oam_rrm_config_req for instance_id: %d", received_pdcp_instance_idx);
                return ret_val;
            }

            if (OAM_SUCCESS != oam_send_pdcp_init_layer_req(
                                    &(oam_prov_req.l2_setup_info.instance_info[received_pdcp_instance_idx].oam_l2_req.oam_pdcp_req.pdcp_init_req),
                        OAM_MODULE_ID,
                        new_trans_id,
                        p_err))
            {
                OAM_LOG(OAM, OAM_ERROR, "Sending PDCP_INIT_LAYER_REQ message failed, %d", *p_err);
#ifndef OAM_UT_FRAMEWORK
                /*OAM_REVIEW_CHANGES*/
                /* Coverity 64970 Changes Start */
                if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                }
                /* Coverity 64970 Changes End */
                /*OAM_REVIEW_CHANGES*/
#endif
                ret_val = OAM_FAILURE;
                break;
            }
            num_cell_on_pdcp_instance = L2_INSTANCE_INFO(received_pdcp_instance_idx).num_cells;
            if(num_cell_on_pdcp_instance > OAM_ONE) {
                for(cell_num = OAM_ZERO;
                        cell_num < num_cell_on_pdcp_instance - OAM_ONE;
                        ++cell_num) {
                    ret_val = oam_check_instance_id_present(received_pdcp_instance_idx, cell_id + OAM_ONE, &cell_id);
                    if(OAM_SUCCESS == ret_val)
                    {
                        /*SPR 21212 Start*/
                        oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].
                                    l2_comm_info_params.pdcp_comm_info),
                                &(l2_comm_info[received_pdcp_instance_idx].pdcp_comm_info),sizeof(rrm_oam_l2_comm_info_t));

                        /*SPR 21212 End*/
                        oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.bitmask |= RRM_OAM_NON_CRAN_PDCP_COMM_INFO_PRESENT;
                    }
                    else
                    {
                        OAM_LOG(OAM, OAM_INFO,
                                "context is not present in oam_rrm_config_req for instance_id: %d", received_pdcp_instance_idx);
                        return ret_val;
                    }
                }
            }
            /* SPR 20908 End */

            OAM_LOG(OAM, OAM_DETAILED, "Sending PDCP_INIT_LAYER_REQ message success");
#ifndef OAM_UT_FRAMEWORK
            p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
        }
        break;

        case PDCP_INIT_LAYER_CNF:
        /* SPR 20908 Start */
        cid = *((UInt8*)p_msg + OAM_INTF_HDR_CELL_IDX_OFFSET);
            ret_val = find_l2_instance_id_from_cell_index(cid, &received_pdcp_instance_idx);
        if(OAM_FAILURE == ret_val)
        {
            OAM_LOG(OAM, OAM_INFO,"instance id not exist for received cell_idex: [%d]", cid);
            return ret_val;
        }

        if(RLC_OAM_INIT_CNF & l2_comm_info[received_pdcp_instance_idx].init_flag)
            /* SPR 20908 End */
        {
#ifndef OAM_UT_FRAMEWORK
            if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
            {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                ret_val = OAM_FAILURE;
                break;
            }
#endif
            if (OAM_SUCCESS != oam_handle_pdcp_init_layer_cnf(
                        (oam_pdcp_cnf_t*)p_msg))
            {
                     OAM_LOG(OAM, OAM_WARNING, "PDCP_INIT_LAYER_CNF returned failure response message");
                ret_val = OAM_FAILURE;
                /* SPR 14082 CODE REMOVED */
            }
            /*SPR 21212 Start*/
            else{
/* SPR 22248 MBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
                l2_comm_info[received_pdcp_instance_idx].init_flag |= PDCP_OAM_INIT_CNF;
                resp_flag |= PDCP_EMBMS_INIT_CNF;
#ifndef OAM_UT_FRAMEWORK
                p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif
#else
/* SPR 22248 MBMS Changes Stop */
                /*SPR 21485 Start*/
                     oam_prov_req.l2_setup_info.num_instances++;
                oam_sdm_send_active_ind(); /* Send Active Indication To SDM */
                oam_cm_handle_active_state();
                oam_maintain_system_state(OAM_INITIALIZATION_COMPLETE);
                /* Set operational state as enabled and inform management interface */   
                oam_prov_req.system_status.op_state = ENODEB_OP_STATE_ENABLED;
                OAM_LOG(OAM, OAM_DETAILED, "Value of TR069 mode is: %d", 
                        oam_prov_req.oam_tr069_req.tr069_mode);
                if((OAM_ZERO == oam_prov_req.oam_tr069_req.tr069_mode) ||
                        (ENODEB_ADMIN_STATE_UNLOCKED == oam_prov_req.system_status.admin_state))
                { 
                    oam_prov_req.system_status.admin_state = ENODEB_ADMIN_STATE_UNLOCKED;
                    oam_maintain_system_state(OAM_INITIALIZATION_COMPLETE);
                    UInt16   gen_trans_id = OAM_ZERO;
                    if ((LOWER_LAYERS_PROVISIONING_MASK & initialisation_flag)
                            /* SPR Fix 18741 Start */
#ifdef OAM_SON_ENABLE 
                            && (INIT_TIME_PCI_SELECTION_MASK & initialisation_flag) 
                            && (INIT_TIME_CARRIER_FREQ_AND_POWER_SELECTION_MASK & initialisation_flag) 
#endif
                            /* SPR Fix 18741 End */       
                       )
                    {    
                        init_time_startup = OAM_ZERO; 
                        for (cell_cnt = OAM_ZERO;cell_cnt < oam_prov_req.num_cells_configured; ++cell_cnt)
                        {
                            p_rrm_cell_config = &RRM_PROV_REQ.oam_rrm_cell_config_req[cell_cnt];
                                    if ((l2_comm_info[p_rrm_cell_config->operator_info.associated_layer2_inst].init_flag & MAC_OAM_INIT_CNF)&&
                                            (l2_comm_info[p_rrm_cell_config->operator_info.associated_layer2_inst].init_flag & RLC_OAM_INIT_CNF ))
                                    {
                                        if(p_rrm_cell_config->operator_info.associated_layer2_inst == received_pdcp_instance_idx)
                                        {
                                            OAM_LOG(OAM,OAM_DETAILED, "Initiating oam_handle_initial_admin_state_unlocked for L2 Instance[%d]",
                                                    received_pdcp_instance_idx);
					    /* SPR 22248 MBMS Changes Start */
                                            retVal =oam_handle_initial_admin_state_unlocked((UInt8 *)INVALID_ARGUMENT, p_rrm_cell_config,
					    /* SPR 22248 MBMS Changes Stop */
                                                    &gen_trans_id);

                                            if(OAM_FAILURE == retVal)
                                            {
                                             OAM_LOG(OAM,OAM_WARNING, "Failed in oam_handle_initial_admin_state_unlocked");
                                                return OAM_FAILURE; 
                                            }
                                        }
                                    }
                        /* SPR 22248 MBMS Changes Start */  
                           /* Code deleted */ 
                        /* SPR 22248 MBMS Changes Stop */
                        }
                        /*SPR 21485 Start*/
                    }
                }
                /*SPR 21212 End*/
                /* SPR 14082 END */
                 l2_comm_info[received_pdcp_instance_idx].init_flag |= PDCP_OAM_INIT_CNF;
                 /*SPR 21369 Start*/
                 oam_start_hm();
                 /*SPR 21369 End*/
                 /* SPR 21983 Fix + */
                /*SPR 22327 Start*/
                 /* SPR 22302 Start */
                /*SPR 21212 Start*/
                 /*SPR 22355 Start*/
#ifndef OAM_UT_FRAMEWORK
                p_cm_cntxt->state = OAM_CM_ST_ACTIVE;
/* SPR 22248 MBMS Changes Start */
#endif
#endif /*LTE_EMBMS_SUPPORTED*/
                /*SPR 22355 End*/
                /*SPR 21212 End*/
                /*SPR 22327 End*/
                /* SPR 22302 End */
                /* SPR 21983 Fix - */
                /* SPR 17585 +-*/
             }
           }
/* SPR 22248 MBMS Changes Stop */
             break;

            default:
            OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
            break;
        }

        OAM_FUN_EXIT_TRACE();
        return ret_val;
}

/****************************************************************************
 * Function Name  : oam_egtpu_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from eGPTU
 ****************************************************************************/
oam_return_et
oam_egtpu_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    UInt16    api_id  = OAM_ZERO;
    oam_return_et  ret_val = OAM_SUCCESS;
/* SPR 14082 START */
    Char8 *p_al_add_info = OAM_NULL;
    Char8 *p_al_add_txt = OAM_NULL;
/* SPR 14082 END */
    /*SPR 17585 +*/
    oam_timer_t  timer;
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    UInt16    timer_data   = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    new_trans_id = oam_get_fresh_trans_id(old_trans_id);
    /*SPR 17585 -*/
    UInt8 received_s1u_instance_idx = OAM_ZERO;
    UInt8 cell_num = OAM_ZERO;
    UInt8 cid = OAM_ZERO;
    U8 cell_id = OAM_ZERO;
    /* SPR 20908 End */

    OAM_FUN_ENTRY_TRACE();

    api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);

    switch (api_id)
    {
         case EGTPU_LM_INITDB_CNF:
                /* SPR 20908 Start */
		    /*SPR 21212 Start*/
	             OAM_LOG(OAM, OAM_DETAILED, "S1U_INIT_LAYER_CNF Recvd");
		    cid = *((UInt8*)p_msg + OAM_INTF_HDR_CELL_IDX_OFFSET);
		    ret_val = find_l2_instance_id_from_cell_index(cid, &received_s1u_instance_idx);
		    if(OAM_FAILURE == ret_val)
		    {
			    OAM_LOG(OAM, OAM_INFO,"instance id not exist for received cell_idex: [%d]", cid);
			    return ret_val;
		    }
		    if(resp_flag == OAM_ZERO)
             {
	             OAM_LOG(OAM, OAM_INFO, "S1U_INIT_LAYER_CNF_1 Recvd");

			    /*SPR 21212 End*/

#ifndef OAM_UT_FRAMEWORK
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_INFO, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }
#endif
	         if(OAM_SUCCESS != oam_handle_egtpu_init_layer_cnf((oam_egtpu_initdb_cnf_t *)p_msg))
                 {
                      OAM_LOG(OAM, OAM_ERROR, "EGTPU_LM_INITDB_CNF message response failed");
                      ret_val = OAM_FAILURE;
                      /* SPR 14082 CODE REMOVED */
                 }
			    else
			    {
				    ret_val= OAM_SUCCESS;
			    }
                 /* SPR 14082 START */
                 if(OAM_FAILURE == ret_val) 
                 {
                     p_al_add_info = "EGTPU Layer initialization failed";

                     oam_handle_init_config_failure(
                             CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                             p_al_add_txt, p_al_add_info);
                     break;
                 }
			        /*SPR 21064 Start*/

				/*SPR 21064 End*/
                 /* SPR 14082 END */

			    /*SPR 21212 Start*/
			    resp_flag |= EGTPU_OAM_INIT_CNF ;
			    /*SPR 21212 End*/
#ifndef OAM_UT_FRAMEWORK
                 p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif
			    /*SPR 21212 +-*/
			    break;
             }
             /*SPR 17585 +*/
         case S1U_INIT_LAYER_IND:
            {
                /* SPR 20908 Start */
                /*SPR 21212 Start*/
                received_s1u_instance_idx =
                    *((UInt8*)(p_msg) + OAM_INTF_HDR_CELL_IDX_OFFSET);
			    if(received_s1u_instance_idx >= OAM_MAX_NUM_L2_INSTANCE)
                {
				    OAM_LOG(OAM, OAM_WARNING,
                            "Received Instance id in S1U_INIT_LAYER_IND: %d should be less then : %d", 
						    received_s1u_instance_idx, OAM_MAX_NUM_L2_INSTANCE);
                    return OAM_FAILURE;
                }

                if (resp_flag == OAM_ZERO)
                {
                    /*SPR 21212 End*/
#ifndef OAM_UT_FRAMEWORK
                    timer = oam_start_timer(guard_time, &timer_data,
                            sizeof(timer_data), OAM_FALSE);
                    if (OAM_NULL == timer)
                    {
                         OAM_LOG( OAM, OAM_DETAILED, 
                                "Error returned while starting the timer");
                        ret_val = OAM_FAILURE;
                        break;
                    }

                    p_cm_cntxt->guard_timer_id = timer;
#endif
                    /* SPR 20908 Start */
                    /*SPR 21212 Start*/
                    oam_prov_req.cell_cntxt.curr_cell_cid =
                        L2_INSTANCE_CELL_INFO(received_s1u_instance_idx, OAM_ZERO).cell_idx;

                    ret_val = oam_check_instance_id_present(received_s1u_instance_idx, OAM_ZERO, &cell_id);
                    if(OAM_SUCCESS == ret_val)
                    {
                        oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_id].l2_comm_info_params.egtpu_comm_info),
                                &(l2_comm_info[received_s1u_instance_idx].egtpu_comm_info),sizeof(rrm_oam_l2_comm_info_t));
                    }
                    else
                    {
                        OAM_LOG(OAM, OAM_INFO,
                                "context is not present in oam_rrm_config_req for instance_id: %d", received_s1u_instance_idx);
                        return ret_val;
                    }
                    /*SPR 21212 Start*/
                    /* SPR 20908 End */
                    if (OAM_SUCCESS != oam_send_egtpu_initdb_req(
							    &(oam_prov_req.l2_setup_info.instance_info[OAM_ZERO].oam_l2_req.oam_egtpu_req.egtpu_initdb_req),
                                OAM_MODULE_ID, new_trans_id, p_err ) )
                    {
                        OAM_LOG(OAM, OAM_ERROR, "Sending EGTPU_INITDB_REQ message failed, %d", *p_err);
#ifndef OAM_UT_FRAMEWORK
                        if(OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                        {
                             OAM_LOG(OAM, OAM_DETAILED, 
                                     "Error returned while stopping the timer");
                        }
#endif
                        ret_val = OAM_FAILURE;
                        break;
                    }
                    else
                    {
                        /*SPR 21212 Start*/
                        ret_val = OAM_SUCCESS;
                        /*SPR 21212 End*/
                         OAM_LOG(OAM, OAM_DETAILED, "Sending EGTPU INIT config request message success");
                    }
                    /* SPR 20908 Start */
                    /*SPR 21759 changes start*/
                    /*SPR 21321 Start*/
                    for(cell_num = OAM_ONE;
                            cell_num < oam_prov_req.num_cells_configured;
                            /*SPR 21321 End*/
                            ++cell_num) {
                        if(OAM_SUCCESS == ret_val)
                        {
                            /*SPR 21321 End*/

                            oam_memcpy(&(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_num].
                                        /*SPR 21321 End*/
                                        l2_comm_info_params.egtpu_comm_info),
                                    &(l2_comm_info[OAM_ZERO].egtpu_comm_info),sizeof(rrm_oam_l2_comm_info_t));
                        }
                        else
                        {
							    OAM_LOG(OAM, OAM_INFO,
                                    "context is not present in oam_rrm_config_req for instance_id: %d", received_s1u_instance_idx);
                            return ret_val;
                        }
                        /*SPR 21321 Start*/
                        cell_id = cell_id + OAM_ONE;
                        /*SPR 21321 End*/
                    }
                    /*SPR 21759 changes end*/
                    /* SPR 20908 End */
#ifndef OAM_UT_FRAMEWORK
                    p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif

				    /*SPR 21212 Start*/
				    /*SPR 21212 +-*/
                }
                break;
            }
             /*SPR 17585 -*/

         default:
             /* SPR 17777 fix */
             OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
             break;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

#ifdef OAM_RADIO_HEAD_SUPPORT
/****************************************************************************
* Function Name : oam_send_rrh_init_config_req
* Description   : This function sends the config request to RRH layer
* Inputs        : p_req: Pointer to void
*                 src_module_id: source module identifier
*                 trans_id: transaction identifier
* Output        : p_err_code: Pointer to Error code
* Returns       : OAM_SUCCESS/OAM_FAILURE
******************************************************************************/
oam_return_et oam_send_rrh_init_config_req(
    void *p_req, 
    UInt16 src_module_id,
    UInt16 trans_id,
    oam_error_code_et *p_err_code
    )
{
	oam_return_et retVal = OAM_SUCCESS;
#ifdef OAM_RRH_TYPE_1_SUPPORT
    oam_rcm_init_config_req_t* p_init_config =
                        (oam_rcm_init_config_req_t*)p_req;

	retVal = oam_send_rcm_init_config_req(p_init_config,
										trans_id, p_err_code);
#endif
    return retVal;
}

/****************************************************************************
* Function Name : oam_handle_rrh_status_init_ind 
* Description   : This function handles RRH status indication message
* Inputs        : p_api: Pointer to location for reading for parsing
*                 api_len: Length of message left to be read
* Outputs       : p_err_code:  Pointer to Error Code
* Returns       : OAM_SUCCESS/OAM_FAILURE
****************************************************************************/
oam_return_et
oam_handle_rrh_status_init_ind(
    UInt8 *p_api,
    UInt16 api_len,
    oam_error_code_et *p_err_code
    )
{
    oam_return_et retVal = OAM_SUCCESS;
#ifdef OAM_RRH_TYPE_1_SUPPORT
	retVal = oam_rcm_handle_rrh_status_init_ind(p_api, api_len,
							  &oam_prov_req.oam_rcm_req.rrh_status_ind,
							  p_err_code);
        if(OAM_SUCCESS == retVal)
        {
            OAM_LOG(OAM, OAM_INFO, "RFTxStatus is set to True");
            oam_prov_req.system_status.rftx_status = OAM_TRUE;   
        }
#endif
    return retVal;
}

/****************************************************************************
* Function Name : oam_handle_rrh_init_config_resp
* Description   : This function handles RRH init config response message 
* Inputs        : p_msg: Pointer to location for reading for parsing
*                 msg_len: Length of message left to be read
* Outputs       : p_err_code: Pointer to Error Code
* Returns       : OAM_SUCCESS/OAM_FAILURE
*********************************************************************************/
oam_return_et
oam_handle_rrh_init_config_resp(
    UInt8 *p_msg,
    UInt16 msg_len,
    oam_error_code_et *p_err_code
    )
{
	oam_return_et retVal = OAM_SUCCESS;
#ifdef OAM_RRH_TYPE_1_SUPPORT
	retVal = oam_handle_rcm_init_config_resp(p_msg, msg_len, p_err_code);
#endif
    return retVal;
}


/****************************************************************************
 * Function Name  : oam_cm_rrh_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from RCM
 ****************************************************************************/
oam_return_et
oam_cm_rrh_msg_handler(
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err)/*Pointer to error code*/
{
    UInt16        api_id       = OAM_ZERO;
    UInt16        msg_size     = OAM_ZERO;
    void          *p_data      = OAM_NULL;
    UInt16        data_len     = OAM_ZERO;
    UInt16        new_trans_id = OAM_ZERO;
    UInt16        timer_data   = OAM_ZERO;
    UInt16        guard_time   = OAM_INIT_TIMER_VALUE;
    oam_timer_t   timer;
    oam_return_et retVal = OAM_SUCCESS;
    oam_return_et retVal2 = OAM_SUCCESS;
    oam_error_code_et error_code = NO_ERROR;

    OAM_FUN_ENTRY_TRACE(); 

	if(!oam_prov_req.rcm_flag) {
		OAM_LOG(OAM, OAM_WARNING, "RCM layer is disabled!!!");
		return retVal;
	}

    api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);
    msg_size = get_word_from_header((UInt8*)(p_msg) + OAM_INTF_HDR_MSG_SIZE_OFFSET);

   /*Below params are required for further APIs*/
    p_data  = p_msg + OAM_MGMT_INTF_HEADER_SIZE;
    data_len = msg_size - OAM_MGMT_INTF_HEADER_SIZE;

    new_trans_id = oam_get_new_trans_id();

    switch(api_id) {
        case RCM_OAM_INIT_IND:
#ifndef OAM_EVM_TARGET
            if (OAM_ZERO == resp_flag) {
#else
            OAM_LOG(OAM, OAM_INFO, "RCM_OAM_INIT_IND received from RCM. "
                    "Value of rrh_sup_flow is: %d",
                                    oam_prov_req.oam_rcm_req.rrh_sup_flow);
            oam_prov_req.oam_rcm_req.rrh_sup_flow = OAM_ONE;
            if((OAM_ZERO == oam_prov_req.oam_rcm_req.rrh_sup_flow &&
                            OAM_ZERO == resp_flag) ||
			    (OAM_ONE == oam_prov_req.oam_rcm_req.rrh_sup_flow)) 
	    {
#endif
		    guard_time = OAM_RCM_INIT_TIMER_VALUE;
		    timer = oam_start_timer(guard_time, &timer_data,
				    sizeof(timer_data), OAM_FALSE);

		    if (OAM_NULL == timer) {

                OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
			    retVal = OAM_FAILURE;
			    break;
		    }
		    p_cm_cntxt->guard_timer_id = timer;

		    if ((retVal = oam_send_rrh_init_config_req(
						    &(oam_prov_req.oam_rcm_req.rcm_init_config_req),
						    OAM_MODULE_ID, new_trans_id, p_err))
				    != OAM_SUCCESS) {
			    OAM_LOG(OAM, OAM_ERROR,
					    "Sending RCM INIT config request failed: %d",
					    *p_err);
			    oam_stop_timer(p_cm_cntxt->guard_timer_id)
				    break;
		    }
		    else {
			    OAM_LOG(OAM, OAM_DETAILED,
					    "Sending RCM INIT config request success");
		    }

		    /*After sending init config request change the state 
		     *to waiting for response
		     */
		    p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
	    }
            break;

        case OAM_RCM_INIT_CONFIG_RESP:
			OAM_LOG(OAM, OAM_INFO, "Received OAM_RCM_INIT_CONFIG_RESP from RCM.");
#ifndef OAM_EVM_TARGET
            if(OAM_ZERO == resp_flag) {
#else
            if((OAM_ZERO == oam_prov_req.oam_rcm_req.rrh_sup_flow &&
                            OAM_ZERO == resp_flag) ||
               (OAM_ONE == oam_prov_req.oam_rcm_req.rrh_sup_flow)) {
#endif
                 if((retVal = oam_stop_timer(p_cm_cntxt->guard_timer_id))
                     != OAM_SUCCESS) {
                     OAM_LOG(OAM, OAM_DETAILED,
                             "Error returned while stopping the timer");
                     break;
                 }

                 if ((retVal = oam_handle_rrh_init_config_resp(p_data,
                                    data_len, p_err)) != OAM_SUCCESS) {
                      OAM_LOG(OAM, OAM_WARNING,
                        "OAM_RCM_INIT_CONFIG_RESP message parsing failed, %d", *p_err);
                      retVal = OAM_FAILURE;
                      break;
                 }
                 else {
                     OAM_LOG(OAM, OAM_DETAILED,
                        "OAM_RCM_INIT_CONFIG_RESP handled successfully");
                 }
             }
             break;

        case RCM_OAM_RRH_STATUS_IND:
            retVal = oam_handle_rrh_status_init_ind(p_data, data_len, p_err);
            if(OAM_SUCCESS == retVal) {
                resp_flag |= RCM_OAM_INIT_CNFM;
#ifndef OAM_EVM_TARGET
                p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#else
                if (OAM_ZERO == oam_prov_req.oam_rcm_req.rrh_sup_flow) {
                p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
                    OAM_LOG(OAM, OAM_DETAILED,
                        "OAM Layer initialization starts");
                }
                else  {
                    p_cm_cntxt->state = OAM_CM_ST_ACTIVE;
                    OAM_LOG(OAM, OAM_DETAILED,
                        "OAM Layer initialization completed successfully");
                }
#endif
				if(RRH_DETECTED ==
                        oam_prov_req.oam_rcm_req.rrh_status_ind.state) {
					retVal2 = oam_rcm_send_inventory_req(OAM_MODULE_ID,
                                                        new_trans_id,
														&error_code);
					if(OAM_SUCCESS != retVal2) {
						OAM_LOG(OAM, OAM_WARNING,
                               "Inventory fetch request failed for the first"
                               " time. Inventory will not be available for"
                               " further requests!!! Error: %d", error_code);
					}
				}
            }

            break;
            
        default:
             OAM_LOG(OAM, OAM_INFO, "Invalid message:[api_id=%d]", api_id);
             break; 
    }
    
    /*Abort all the processes in case layer init failed*/
    if (OAM_FAILURE == retVal) 
    {
        p_cm_cntxt->event = OAM_CM_EV_ABORT;
        retVal = oam_cm_fsm_process_event(p_msg, p_cm_cntxt, p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
#endif

/****************************************************************************
 * Function Name  : oam_rrc_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from RRC
 ****************************************************************************/
oam_return_et
oam_rrc_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt /*Pointer to CM FSM context*/
    /*SPR 17777 fix*/
    )
{
    UInt16    api_id  = OAM_ZERO;
    UInt16    msg_size = OAM_ZERO;
    void   *p_data  = OAM_NULL;
    /* CID-113207 Start */
    SInt32    data_len = OAM_ZERO;
    /* CID-113207 End */
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    UInt16    timer_data   = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    oam_timer_t  timer;
    oam_return_et  ret_val = OAM_SUCCESS;
   #ifdef OAM_EVM_TARGET
     XML_struct_cell_config   *p_cell_spec_params ;
     UInt8 cell_cnt = OAM_ZERO;
    #endif
    oam_rrc_communication_info_resp_t oam_rrc_communication_info_resp;
    oam_rrc_provision_resp_t oam_rrc_provision_resp;
    oam_s1ap_provision_resp_t oam_s1ap_provision_resp;
    Char8 *p_al_add_txt = OAM_NULL;
    Char8 *p_al_add_info = OAM_NULL;

    SInt32 length_read = OAM_ZERO;
    
    OAM_FUN_ENTRY_TRACE();

    api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);
    msg_size = get_word_from_header((UInt8*)(p_msg) + OAM_INTF_HDR_MSG_SIZE_OFFSET);

   /*Below params are required for further APIs*/
    p_data  = p_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    data_len = msg_size - LOWER_LAYER_INTERFACE_HEADER_SIZE;

    new_trans_id = oam_get_fresh_trans_id(old_trans_id);

    switch (api_id)
    {
         case RRC_OAM_INIT_IND:
            /* SPR 20908 Start */
             if (RRM_OAM_INIT_CNFM & resp_flag)
            /* SPR 20908 End */
 
             {
#ifndef OAM_UT_FRAMEWORK
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",RRC_MODULE_ID); 
                 /* SPR 13832 END */ 
                 timer = oam_start_timer(guard_time, &timer_data,
                                           sizeof(timer_data), OAM_FALSE);
                 if (OAM_NULL == timer)
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }

                 p_cm_cntxt->guard_timer_id = timer;
#endif
                 if (OAM_SUCCESS != oam_rrc_il_send_rrc_oam_communication_info_req(
                                      &prov_req.commn_info,
                                                           OAM_MODULE_ID, 
                                                           RRC_MODULE_ID,
                                                           new_trans_id,
                                      oam_prov_req.cell_cntxt.curr_cell_cid))
                 {
                      OAM_LOG(OAM, OAM_ERROR, "Sending RRC_OAM_COMMUNICATION_INFO_REQ message failed, cell_index=%d",
                              oam_prov_req.cell_cntxt.curr_cell_cid);
#ifndef OAM_UT_FRAMEWORK
/*OAM_REVIEW_CHANGES*/
                      /* Coverity 64972 Changes Start */
                      if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                      {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                      }
                      /* Coverity 64972 Changes End */
/*OAM_REVIEW_CHANGES*/
#endif
                      ret_val = OAM_FAILURE;
                      break;
                 }
                 OAM_LOG(OAM, OAM_DETAILED, "Sending RRC_OAM_COMMUNICATION_INFO_REQ message success");

#ifndef OAM_UT_FRAMEWORK
                 p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
             }
             break;

         case RRC_OAM_COMMUNICATION_INFO_RESP:
             length_read = OAM_ZERO;
#ifndef OAM_UT_FRAMEWORK
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }
#endif

             ret_val = oam_rrc_il_parse_rrc_oam_communication_info_resp(
					                   	      &oam_rrc_communication_info_resp,
                     p_data, data_len, &length_read);
             if(OAM_SUCCESS != ret_val) {
                 OAM_LOG(OAM, OAM_WARNING,
                         "Failed to parse RRC_OAM_COMMUNICATION_INFO_RESP message");
                 p_al_add_txt = OAM_INTF_MSG_PARSING_FAILED_STR("RRC");
             }
             else {
                 OAM_LOG(OAM, OAM_INFO,
                         "RRC_OAM_COMMUNICATION_INFO_RESP message parsing success");

                 if(RRC_SUCCESS != oam_rrc_communication_info_resp.response) {
                     OAM_LOG(OAM, OAM_WARNING,
                             "RRC_OAM_COMMUNICATION_INFO_RESP failed "
                             "with response code: %d, and fail cause: %d",
                             oam_rrc_communication_info_resp.response,
                             oam_rrc_communication_info_resp.cause); 

                     p_al_add_txt = oam_rrc_resp_code_to_str(
                             oam_rrc_communication_info_resp.cause);
                     ret_val = OAM_FAILURE;
                 }
             }

             if(OAM_FAILURE == ret_val) {
                 p_al_add_info = LAYER_INIT_FAILED_STR("RRC");

                 oam_handle_init_config_failure(
                         CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                         p_al_add_txt, p_al_add_info);
                 break;
             }

             resp_flag |= RRC_OAM_COMM_INIT_CNF;

#ifndef OAM_UT_FRAMEWORK
             p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
             /*Send the Provisioning request to RRC as this is sent by OAM after
              *receiving the communication info response and there is no other
              *trigger for the same.
              */
             timer = oam_start_timer(guard_time, &timer_data,
                     sizeof(timer_data), OAM_FALSE);
             if (OAM_NULL == timer)
             {
                 OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                 ret_val = OAM_FAILURE;
                 break;
             }

             p_cm_cntxt->guard_timer_id = timer;
#endif

             if (OAM_SUCCESS != oam_rrc_il_send_rrc_oam_provision_req(
                         &(oam_prov_req.oam_l3_req.oam_rrc_req.rrc_oam_prov),
                         OAM_MODULE_ID,
                         RRC_MODULE_ID,
                         new_trans_id,
                         oam_prov_req.cell_cntxt.curr_cell_cid))
             {
                 OAM_LOG(OAM, OAM_ERROR, "Sending RRC_OAM_PROVISION_REQ message failed");
                 ret_val = OAM_FAILURE;
                 break;
             }

             OAM_LOG(OAM, OAM_DETAILED, "Sending RRC_OAM_PROVISION_REQ message success");

#ifndef OAM_UT_FRAMEWORK
             p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
             break;

         case RRC_OAM_PROVISION_RESP:
                 length_read = OAM_ZERO;
#ifndef OAM_UT_FRAMEWORK
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }

#endif    
             ret_val = oam_rrc_il_parse_rrc_oam_provision_resp(
						      &oam_rrc_provision_resp,
                                                  p_data, data_len,
                                                  &length_read);
             if(OAM_SUCCESS != ret_val) {
                 OAM_LOG(OAM, OAM_WARNING,
                              "Failed to parse RRC_OAM_PROVISION_RESP message");
                 p_al_add_txt = OAM_INTF_MSG_PARSING_FAILED_STR("RRC");
             }
             else {
                 OAM_LOG(OAM, OAM_DETAILED,
                        "RRC_OAM_PROVISION_RESP message parsing success");

                 if(RRC_SUCCESS != oam_rrc_provision_resp.response) {
                     OAM_LOG(OAM, OAM_WARNING, "RRC_OAM_PROVISION_RESP failed "
                                "with response code: %d",
                                oam_rrc_provision_resp.response);
                     p_al_add_txt = "RRC layer provisioning failed";

                     ret_val = OAM_FAILURE;
                 }
             }

             if(OAM_FAILURE == ret_val) {
                 p_al_add_info = LAYER_INIT_FAILED_STR("RRC");
                 oam_handle_init_config_failure(
                         CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                         p_al_add_txt, p_al_add_info);
                 break;
             }


             resp_flag |= RRC_OAM_PROV_INIT_CNF;
#ifndef OAM_UT_FRAMEWORK
             p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif
             break;

         case S1AP_OAM_INIT_IND:
             if (RRC_OAM_PROV_INIT_CNF & resp_flag) {
#ifndef OAM_UT_FRAMEWORK
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",RRC_S1AP_MODULE_ID); 
                 /* SPR 13832 END */ 
                 timer = oam_start_timer(guard_time, &timer_data,
                                           sizeof(timer_data), OAM_FALSE);
                 if (OAM_NULL == timer)
                 {    
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }    

                 p_cm_cntxt->guard_timer_id = timer;
#endif
                 if (OAM_SUCCESS != oam_s1ap_il_send_s1ap_oam_provision_req(
                                    &(oam_prov_req.oam_l3_req.oam_s1ap_req.s1ap_oam_prov),
                                                           OAM_MODULE_ID,
                                                           RRC_MODULE_ID,
                                                           new_trans_id,
                                      oam_prov_req.cell_cntxt.curr_cell_cid))
                 {
                      OAM_LOG(OAM, OAM_ERROR, "Sending S1AP_OAM_PROVISION_REQ message failed, cell_index=%d",
                                            oam_prov_req.cell_cntxt.curr_cell_cid);
#ifndef OAM_UT_FRAMEWORK
/*OAM_REVIEW_CHANGES*/
                      /* Coverity 64972 Changes Start */
                      if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                      {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                      }
                      /* Coverity 64972 Changes End */
/*OAM_REVIEW_CHANGES*/
#endif
                      ret_val = OAM_FAILURE;
                      break;
                 }

                 OAM_LOG(OAM, OAM_DETAILED, "Sending S1AP_OAM_PROVISION_REQ message success");

#ifndef OAM_UT_FRAMEWORK
                 p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
             }
             break;

         case S1AP_OAM_PROVISION_RESP:
             length_read = OAM_ZERO;
             OAM_LOG(OAM,OAM_INFO,
                     "S1AP_OAM_PROVISION_RESP received. "
                     "AdminState : %d, OpState : %d, RFTxStatus : %d, AvailabilityStatus : %d",
                     oam_prov_req.system_status.admin_state,oam_prov_req.system_status.op_state,
                     oam_prov_req.system_status.rftx_status,oam_prov_req.system_status.availability_status);

             if ((RRC_OAM_PROV_INIT_CNF & resp_flag))
             {
#ifndef OAM_UT_FRAMEWORK
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }
#endif
                 if (OAM_SUCCESS != oam_rrc_il_parse_s1ap_oam_provision_resp(
						    &oam_s1ap_provision_resp,
                            p_data,
						    data_len,
						    &length_read)) 
                 {
                      OAM_LOG(OAM, OAM_WARNING, "S1AP_OAM_PROVISION_RESP message parsing failed");
                      ret_val = OAM_FAILURE;
                 }

                 OAM_LOG(OAM, OAM_DETAILED, "S1AP_OAM_PROVISION_RESP message parsing success");

                 if(RRC_SUCCESS != oam_s1ap_provision_resp.response) {
                     OAM_LOG(OAM, OAM_WARNING, "S1AP_OAM_PROVISION_RESP failed"
                                           " with response code: %d",
                                           oam_s1ap_provision_resp.response);
                     ret_val = OAM_FAILURE;
                     break;
		 }

		 resp_flag |= S1AP_OAM_INIT_CNFM;
		 /* SPR 12316 FIXED Start */

                   /* code remove */
                 
		 /* SPR 12316 FIXED End */
#ifndef OAM_UT_FRAMEWORK
         p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#endif
	     }
	     break;

	 case X2AP_OAM_INIT_IND:
             if (S1AP_OAM_INIT_CNFM & resp_flag)
             {
#ifndef OAM_UT_FRAMEWORK
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",RRC_X2AP_MODULE_ID); 
                 /* SPR 13832 END */ 
                 timer = oam_start_timer(guard_time, &timer_data,
                                           sizeof(timer_data), OAM_FALSE);
                 if (OAM_NULL == timer)
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }

                 p_cm_cntxt->guard_timer_id = timer;
#endif
                     if (OAM_SUCCESS != oam_x2ap_il_send_x2ap_oam_provision_req(
                                         &(oam_prov_req.oam_l3_req.oam_x2ap_req.x2ap_oam_prov),
                                                           OAM_MODULE_ID,
                                                           RRC_MODULE_ID,
                                                           new_trans_id,
                                       oam_prov_req.cell_cntxt.curr_cell_cid))
                 {
                         OAM_LOG(OAM, OAM_ERROR,
                            "Sending X2AP_OAM_PROVISION_REQ message failed, cell_index=%d",
                             oam_prov_req.cell_cntxt.curr_cell_cid);
#ifndef OAM_UT_FRAMEWORK
/*OAM_REVIEW_CHANGES*/
                         /* Coverity 64972 Changes Start */
                         if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                         {
                             OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                         }
                         /* Coverity 64972 Changes End */
/*OAM_REVIEW_CHANGES*/
#endif
                      return  OAM_FAILURE;
                      break;
                 }

                 OAM_LOG(OAM, OAM_DETAILED, "Sending X2AP_OAM_PROVISION_REQ message success");

#ifndef OAM_UT_FRAMEWORK
                 p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
#endif
             }
             break;

         case X2AP_OAM_PROVISION_RESP:
             if (S1AP_OAM_INIT_CNFM & resp_flag)
             {
                 length_read = OAM_ZERO;
#ifndef OAM_UT_FRAMEWORK
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }
#endif
                 oam_x2ap_provision_resp_t x2ap_oam_provision_resp;
                 if (OAM_SUCCESS !=oam_rrc_il_parse_x2ap_oam_provision_resp(&x2ap_oam_provision_resp,
                                                      p_data,
						                              data_len,
                                                      &length_read))
                 {
                      OAM_LOG(OAM, OAM_WARNING, "X2AP_OAM_PROVISION_RESP message response failed");
                      ret_val = OAM_FAILURE;
                      break;
                 }
/* Start: CSR 00058940 */                     
                 initialisation_flag |= LOWER_LAYERS_PROVISIONING_MASK;
/*End: CSR 00058940 */ 

                 if(RRC_SUCCESS != x2ap_oam_provision_resp.response) {
                     OAM_LOG(OAM, OAM_WARNING, "X2AP_OAM_PROVISION_RESP failed"
                                           " with response code: %d",
                                           x2ap_oam_provision_resp.response);
                     ret_val = OAM_FAILURE;
                     break;
                 }
                 OAM_LOG(OAM, OAM_DETAILED, "X2AP_OAM_PROVISION_RESP message response success");

                 resp_flag |= X2AP_OAM_INIT_CNFM;
#ifndef OAM_EVM_TARGET                
#ifndef OAM_UT_FRAMEWORK
/*SPR 21212 Start*/
                 p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
/*SPR 21212 End*/
#endif
#else
         /*Below statement is hardcoded bcoz of RCM need to work value from XML is not correct bcoz relevant tag is not present*/
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
         oam_prov_req.oam_rcm_req.rrh_sup_flow = OAM_ONE;
         if(oam_prov_req.rrh_flag && oam_prov_req.rcm_flag
                     && (OAM_ONE == oam_prov_req.oam_rcm_req.rrh_sup_flow)) {
                 p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
         }
        else {
            p_cm_cntxt->state = OAM_CM_ST_ACTIVE;
            OAM_LOG(OAM,OAM_DETAILED,"CM State is set to Active");
        }
#endif
#elif defined (LTE_EMBMS_SUPPORTED)
            p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
#else
        p_cm_cntxt->state = OAM_CM_ST_ACTIVE;
#endif  /* Startup flow change */
#endif

                   
                OAM_LOG(OAM,OAM_INFO,"CM State is set to Active");
		
                /*SPR 21369 +-*/
                oam_sdm_send_active_ind(); /* Send Active Indication To SDM */
		        oam_cm_handle_active_state();
                 oam_fetch_sctp_data_from_l3_x2ap();

                 oam_fetch_sctp_data_from_l3_s1ap();
                oam_maintain_system_state(OAM_INITIALIZATION_COMPLETE);
                /* Set operational state as enabled and inform management interface */   
                oam_prov_req.system_status.op_state = ENODEB_OP_STATE_ENABLED;

                /* SPR 20938 Changes Start */
                OAM_LOG(OAM, OAM_DETAILED, "Value of TR069 mode is: %u and admin state is: %u", 
                        oam_prov_req.oam_tr069_req.tr069_mode,
                        oam_prov_req.system_status.admin_state);
                /* SPR 20938 Changes End */

	        /* Patch for triggering cell setup if admin state is already 1 */
                /* SPR 20788 FIXED START */           
                   /* code removed */
                /* SPR 20788 FIXED END */            
    /*SPR 21561+-*/

             }
/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
        case M2AP_OAM_INIT_IND:
        case M2AP_OAM_PROVISION_RESP:
        ret_val = oam_m2ap_msg_handler(p_msg, p_cm_cntxt);
        break;
#endif
        /* EMBMS Changes End */


             
             break;

/* LWA changes start*/
/* LWA changes end*/

         default:
             OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
             break;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/****************************************************************************
 * Function Name  : oam_fetch_sctp_data_from_l3_x2ap()
 * Inputs         : none
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This is a function fetches sctp data from X2AP
 ****************************************************************************/
 /* + SPR 17439 */
oam_return_et
oam_fetch_sctp_data_from_l3_x2ap(void)
{
 /* - SPR 17439 */

    OAM_FUN_ENTRY_TRACE();
    oam_return_et  ret_val = OAM_SUCCESS;
    oam_counter_t counter = OAM_ZERO; 
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    
    new_trans_id = oam_get_fresh_trans_id(old_trans_id);
 

    oam_x2ap_get_sctp_status_req_t oam_x2ap_get_sctp_status_req;


    /*preparing X2AP GET SCTP STATUS REQ to be sent */
    oam_memset((void *)&oam_x2ap_get_sctp_status_req, OAM_ZERO,
    sizeof(oam_x2ap_get_sctp_status_req_t));

    oam_x2ap_get_sctp_status_req.bitmask |= X2AP_OAM_GLOBAL_ENB_STATUS_LIST_PRESENT;
    oam_x2ap_get_sctp_status_req.x2ap_oam_gb_enb_status_list.num_peer_enb = 
    oam_prov_req.oam_l3_req.peer_enb_data.num_peer_enb;
    for ( counter = OAM_ZERO; 
          counter < oam_x2ap_get_sctp_status_req.x2ap_oam_gb_enb_status_list.num_peer_enb;
          counter++
         )
    {

        oam_memcpy(&(oam_x2ap_get_sctp_status_req.x2ap_oam_gb_enb_status_list.gb_enb_id[counter]), 
                   &(oam_prov_req.oam_l3_req.peer_enb_data.eNB_id[counter]),sizeof(x2_gb_enb_id_t));
    }
 
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    oam_x2ap_get_sctp_status_req.bitmask |= X2AP_OAM_GLOBAL_GNB_STATUS_LIST_PRESENT;
    oam_x2ap_get_sctp_status_req.x2ap_oam_gb_gnb_status_list.num_peer_gnb = 
    oam_prov_req.oam_l3_req.peer_gnb_data.num_peer_enb;
    for ( counter = OAM_ZERO; 
          counter < oam_x2ap_get_sctp_status_req.x2ap_oam_gb_gnb_status_list.num_peer_gnb;
          counter++
         )
    {
        oam_memcpy(&(oam_x2ap_get_sctp_status_req.x2ap_oam_gb_gnb_status_list.gb_gnb_id[counter]), 
                   &(oam_prov_req.oam_l3_req.peer_gnb_data.gNB_id[counter]),sizeof(x2_gb_gnb_id_t));
    }
    /* MENB CHANGES - END */
    #endif


     if (OAM_SUCCESS != oam_x2ap_il_send_x2ap_oam_get_sctp_status_req(
                        &(oam_x2ap_get_sctp_status_req),
                        OAM_MODULE_ID,
                        RRC_MODULE_ID,
                        new_trans_id,
                        oam_prov_req.cell_cntxt.curr_cell_cid))
     {
         OAM_LOG(OAM, OAM_ERROR, "Sending X2AP_OAM_GET_SCTP_STATUS_REQ message failed, cell_index=%d",
                    oam_prov_req.cell_cntxt.curr_cell_cid);
         ret_val = OAM_FAILURE;
         return ret_val;
     }

     OAM_LOG(OAM, OAM_DETAILED, "Sending X2AP_OAM_GET_SCTP_STATUS_REQ message success");
     OAM_FUN_EXIT_TRACE();
     return ret_val;
}
/****************************************************************************
 * Function Name  : oam_fetch_sctp_data_from_l3_s1ap()
 * Inputs         : none
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This is a function fetches sctp data from S1AP 
 ****************************************************************************/
 /* + SPR 17439 */
oam_return_et
oam_fetch_sctp_data_from_l3_s1ap(void)
{
 /* - SPR 17439 */

    OAM_FUN_ENTRY_TRACE();
    oam_return_et  ret_val = OAM_SUCCESS;
    oam_counter_t counter = OAM_ZERO; 
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    
    new_trans_id = oam_get_fresh_trans_id(old_trans_id);
 

    oam_s1ap_get_sctp_status_req_t oam_s1ap_get_sctp_status_req;



    /*preparing S1AP GET SCTP STATUS REQ to be sent */
    oam_memset((void *)&oam_s1ap_get_sctp_status_req, OAM_ZERO,
    sizeof(oam_s1ap_get_sctp_status_req_t));
    oam_s1ap_get_sctp_status_req.bitmask |= S1AP_OAM_MME_STATUS_LIST_PRESENT;
    oam_s1ap_get_sctp_status_req.s1ap_oam_mme_status_list.num_mme_id = oam_prov_req.oam_l3_req.mme_data.num_mme_id;
     
    /* SPR 15882 START */
    /* Send s1ap_get_sctp_status_req only when atleast 1 mme_id is present. */
    if(oam_s1ap_get_sctp_status_req.s1ap_oam_mme_status_list.num_mme_id > OAM_ZERO)
    {

   for ( counter = OAM_ZERO;
          counter < oam_s1ap_get_sctp_status_req.s1ap_oam_mme_status_list.num_mme_id;
          counter++
         )
    {
         oam_s1ap_get_sctp_status_req.s1ap_oam_mme_status_list.mme_id[counter] = 
         oam_prov_req.oam_l3_req.mme_data.mme_id[counter];
    }
    
     OAM_LOG(OAM, OAM_DETAILED, "--------SENDING OAM S1AP GET SCTP STATUS REQ-------"); 
     if (OAM_SUCCESS != oam_s1ap_il_send_s1ap_oam_get_sctp_status_req(
                        &(oam_s1ap_get_sctp_status_req),
                        OAM_MODULE_ID,
                        RRC_MODULE_ID,
                        new_trans_id,
                        oam_prov_req.cell_cntxt.curr_cell_cid))
     {
           OAM_LOG(OAM, OAM_ERROR, "Sending S1AP_OAM_GET_SCTP_STATUS_REQ message failed, cell_index=%d",
                   oam_prov_req.cell_cntxt.curr_cell_cid);
           ret_val = OAM_FAILURE;
           return ret_val; 
      }
      OAM_LOG(OAM, OAM_DETAILED, "Sending S1AP SCTP STATUS  request message success");
    }
    /* SPR 15882 END */


    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/* LWA changes start*/
/* LWA changes end*/

/****************************************************************************
 * Function Name  : oam_cm_handle_active_state
 * Inputs         : none
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This is a common function handles the active state of
 *                  file download and control command
 ****************************************************************************/
 /* + SPR 17439 */
oam_return_et
oam_cm_handle_active_state(void)
{
 /* - SPR 17439 */
    oam_return_et  ret_val = OAM_SUCCESS;
    UInt16  trans_id = OAM_ZERO;

              /* Send File Download response after all layers are configured */
                if(OAM_FILE_DOWNLOAD_RESP_PENDING & oam_prov_req.system_status.event_ongoing)
                {
                   /* Reset Event */
                   oam_prov_req.system_status.event_ongoing &= ~OAM_FILE_DOWNLOAD_RESP_PENDING;
			   OAM_LOG(OAM,OAM_DETAILED,"Event OAM_FILE_DOWNLOAD_RESP_PENDING reset");

                   /* Stop timer when all layers get configured */ 
                   if (OAM_SUCCESS != oam_stop_timer(oam_prov_req.system_status.timer_id))
                   {
                      	OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                       	ret_val = OAM_FAILURE;
			return ret_val;
                   }
                   else
                   {
                       OAM_LOG(OAM, OAM_DETAILED, "Layers Config Timer Stopped");
                   }

                   trans_id = oam_prov_req.system_status.trans_id;  
                   OAM_LOG(OAM,OAM_INFO,"Layers Configured, Trans_ID: %d",trans_id); 
                   /* coverity 35622*/ 
		        if(OAM_FAILURE == oam_send_file_download_ind_resp(OAM_RESULT_SUCCESS,
		                  (oam_error_code_et)OAM_ZERO,trans_id,(UInt8 *)""))
                   {
                       OAM_LOG(OAM,OAM_ERROR,"Error in sending OAM_FILE_DOWNLOAD_IND_RESP, Trans_ID: %d",
                                        trans_id);
                       OAM_FUN_EXIT_TRACE(); 
                       return OAM_FAILURE;     
                   }
                } 
                
                /* SPR 12225 FIX START */
                /* CODE REMOVED */
                /* SPR 12225 FIX END */
	OAM_FUN_EXIT_TRACE();
        return ret_val;
}

/****************************************************************************
 * Function Name  : oam_s1ap_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from S1AP
 ****************************************************************************/
oam_return_et
oam_s1ap_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt /*Pointer to CM FSM context*/
    /*SPR 17777 fix*/
    )
{
    UInt16    api_id  = OAM_ZERO;
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    UInt16    timer_data   = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    oam_timer_t  timer;
    oam_return_et  ret_val = OAM_SUCCESS;

    OAM_FUN_ENTRY_TRACE();

    api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);

   /*Below params are required for further APIs*/
    new_trans_id = oam_get_fresh_trans_id(old_trans_id);

    switch (api_id)
    {
         case S1AP_OAM_INIT_IND:
             if (RRC_OAM_PROV_INIT_CNF & resp_flag)
             {
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",RRC_S1AP_MODULE_ID); 
                 /* SPR 13832 END */ 
                 timer = oam_start_timer(guard_time, &timer_data,
                                           sizeof(timer_data), OAM_FALSE);
                 if (OAM_NULL == timer)
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }

                 p_cm_cntxt->guard_timer_id = timer;

                 if (OAM_SUCCESS != oam_s1ap_il_send_s1ap_oam_provision_req(
                                               (oam_s1ap_provision_req_t*)p_msg,
                                                           OAM_MODULE_ID,
                                                           RRC_MODULE_ID,
                                                           new_trans_id,
                                      oam_prov_req.cell_cntxt.curr_cell_cid))
                 {
                      OAM_LOG(OAM, OAM_ERROR, "Sending S1AP_OAM_PROVISION_REQ message failed, cell_index=%d",
                              oam_prov_req.cell_cntxt.curr_cell_cid);
/*OAM_REVIEW_CHANGES*/
                      /* Coverity 64974 Changes Start */
                      if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                      {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                      }
                      /* Coverity 64974 Changes End */
/*OAM_REVIEW_CHANGES*/
                      ret_val = OAM_FAILURE;
                      break;
                 }
                 OAM_LOG(OAM, OAM_DETAILED, "Sending S1AP_OAM_PROVISION_REQ message success");

                 p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
             }
             break;

         case S1AP_OAM_PROVISION_RESP:
             if (RRC_OAM_INIT_CNF & resp_flag)
             {
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }
                 OAM_LOG(OAM, OAM_DETAILED, "S1AP_OAM_PROVISION_RESP message response success");

                 resp_flag |= S1AP_OAM_INIT_CNFM;
                 p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
             }
             break;

         default:
             OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
             break;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
/****************************************************************************
 * Function Name  : oam_m2ap_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from M2AP
 ****************************************************************************/
oam_return_et
oam_m2ap_msg_handler (
        void *p_msg, /*Pointer to input api buffer*/
        oam_cm_cntxt_t *p_cm_cntxt /*Pointer to CM FSM context*/
        )
{
    OAM_FUN_ENTRY_TRACE();

    UInt16    api_id  = OAM_ZERO;
    UInt16    msg_size = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    Char8     timer_data[OAM_LAYER_INIT_TIMEOUT_DATA_SIZE] = {OAM_ZERO};
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    oam_timer_t  timer;
    oam_return_et  ret_val = OAM_SUCCESS;
    oam_error_code_et err = NO_ERROR;
    m2ap_oam_provision_resp_t *p_m2ap_oam_provision_resp = OAM_NULL;
    oam_rrm_cell_config_req_t  *p_rrm_cell_config = OAM_NULL;
    UInt8   *p_data  = OAM_NULL;
    UInt16    data_len = OAM_ZERO;

    SInt32 length_read = OAM_ZERO;

    api_id   = get_word_from_header((UInt8*)p_msg + OAM_EXT_API_ID_FROM_HEADER);
    msg_size = get_word_from_header((UInt8*)p_msg + OAM_INTF_HDR_MSG_SIZE_OFFSET);

    p_data  = (UInt8*)p_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    data_len = msg_size - LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /*Below params are required for further APIs*/
    new_trans_id = oam_get_new_trans_id();

    switch (api_id)
    {
        case M2AP_OAM_INIT_IND:
        /* SPR 22248 MBMS Changes Start */
        //if ((X2AP_OAM_INIT_CNFM & resp_flag))
         if((PDCP_EMBMS_INIT_CNF & resp_flag))
        /* SPR 22248 MBMS Changes Stop */
        {
            ret_val = oam_m2ap_il_send_oam_m2ap_provision_req(
                    &M2AP_PROV_REQ,
                    OAM_MODULE_ID,
                    RRC_MODULE_ID,
                    new_trans_id,
                    oam_prov_req.cell_cntxt.curr_cell_cid);
            if (OAM_SUCCESS != ret_val)
            {
                OAM_LOG(OAM, OAM_WARNING,
                        "Sending M2AP provisioning request message failed"
                        "eMBMS FEATURE MAY NOT WORK");

                ret_val = OAM_FAILURE;
                break;
            }

#ifndef OAM_UT_FRAMEWORK
            oam_sprintf(timer_data, "%d", RRC_M2AP_MODULE_ID);
            timer = oam_start_timer(guard_time, timer_data,
                    sizeof(timer_data), OAM_FALSE);
            if (OAM_NULL == timer)
            {
                OAM_LOG(OAM, OAM_WARNING, "Error in starting timesr "
                        "eMBMS FEATURE MAY NOT WORK");

                ret_val = OAM_SUCCESS;
                break;
            }

            p_cm_cntxt->guard_timer_id = timer;
#endif

            OAM_LOG(OAM, OAM_DETAILED,
                    "Sending M2AP provisioning request message success");

            p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
        }
        break;

        case M2AP_OAM_PROVISION_RESP:
        if (X2AP_OAM_INIT_CNFM & resp_flag)
        {
#ifndef OAM_UT_FRAMEWORK
            if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
            {
                OAM_LOG(OAM, OAM_WARNING,
                        "Error returned while stopping the timer"
                        "eMBMS FEATURE MAY NOT WORK");
                ret_val = OAM_FAILURE;
                break;
            }
#endif
            ret_val = oam_mem_alloc(sizeof(m2ap_oam_provision_resp_t),
                    (void**)&p_m2ap_oam_provision_resp,
                    &err);
            if(OAM_FAILURE == ret_val)
            {
                OAM_LOG(OAM, OAM_WARNING,
                        "Error in memory allocation with error code %d. "
                        "eMBMS FEATURE MAY NOT WORK", err);
                ret_val = OAM_FAILURE;

                break;
            }

            ret_val = oam_rrc_il_parse_m2ap_oam_provision_resp(
                    p_m2ap_oam_provision_resp,
                    p_data,
                    data_len,
                    &length_read);
            if (OAM_SUCCESS == ret_val)
            {
                OAM_LOG(OAM, OAM_DETAILED,
                        "M2AP provisioning response message parsing success");

                if (M2AP_SUCCESS == p_m2ap_oam_provision_resp->response)
                {
                    OAM_LOG(OAM, OAM_INFO, "Success response received in "
                            "M2AP_OAM_PROVISION_RESP");

                    resp_flag |= M2AP_OAM_INIT_CNFM;
                    oam_prov_req.oam_l3_req.oam_m2ap_req.
                        mbms_system_support = 1;
                }
                else
                {
                    if (p_m2ap_oam_provision_resp->cause == OAM_ZERO)
                    {
                        OAM_LOG(OAM, OAM_ERROR, "Failure response received in "
                                "M2AP_OAM_PROVISION_RESP with "
                                "error code: %d. "
                                "eMBMS FEATURE MAY NOT WORK",
                                p_m2ap_oam_provision_resp->cause);
                        ret_val = OAM_SUCCESS;
                        /* Coverity Fix 97675 Start */
                        oam_mem_free(p_m2ap_oam_provision_resp, &err);
                        /* Coverity Fix 97675 End */
                        break;
                    }
                    else
                    {
                    OAM_LOG(OAM, OAM_ERROR, "Failure response received in "
                            "M2AP_OAM_PROVISION_RESP with "
                            "error code: %d. "
                            "eMBMS FEATURE MAY NOT WORK",
                            p_m2ap_oam_provision_resp->cause);
                    ret_val = OAM_FAILURE;
                        /* Coverity Fix 97675 Start */
                        oam_mem_free(p_m2ap_oam_provision_resp, &err);
                        /* Coverity Fix 97675 End */
                    break;
                }
            }
            }
            else
            {
                OAM_LOG(OAM, OAM_ERROR, "M2AP_OAM_PROVISION_RESP message "
                        "parsiing failed with error code: %d. "
                        "eMBMS FEATURE MAY NOT WORK", err);
                ret_val = OAM_FAILURE;
                /* Coverity Fix 97675 Start */
                oam_mem_free(p_m2ap_oam_provision_resp, &err);
                /* Coverity Fix 97675 End */
                break;
            }

            p_cm_cntxt->state = OAM_CM_ST_ACTIVE;
            initialisation_flag |= LOWER_LAYERS_PROVISIONING_MASK;

            OAM_LOG(OAM, OAM_DETAILED, "CM State is set to Active");

            oam_start_hm();
            oam_sdm_send_active_ind(); /* Send Active Indication To SDM */
            oam_cm_handle_active_state();
            oam_prov_req.system_status.op_state = ENODEB_OP_STATE_ENABLED;

            /* Patch for triggering cell setup if admin state is already 1 */
            if ((OAM_ZERO == oam_prov_req.oam_tr069_req.tr069_mode) ||
                    (ENODEB_ADMIN_STATE_UNLOCKED == oam_prov_req.system_status.admin_state))
            {
                /* For Non-TR mode, Unlock admin state internally for now */
                oam_prov_req.system_status.admin_state = ENODEB_ADMIN_STATE_UNLOCKED;
                oam_maintain_system_state(OAM_INITIALIZATION_COMPLETE);

                UInt16   gen_trans_id = OAM_ZERO;
                if ((LOWER_LAYERS_PROVISIONING_MASK & initialisation_flag)
#ifdef NETWORK_SNIFFING_ENABLE
                        && (INIT_TIME_PCI_SELECTION_MASK & initialisation_flag)
                        && (INIT_TIME_CARRIER_FREQ_AND_POWER_SELECTION_MASK & initialisation_flag)
#endif
                   )
                {    
                    init_time_startup = OAM_ZERO; 

                    UInt8 cell_cnt = OAM_ZERO;
                    for (cell_cnt = OAM_ZERO;cell_cnt < oam_prov_req.num_cells_configured; ++cell_cnt) {
                        p_rrm_cell_config = &RRM_PROV_REQ.oam_rrm_cell_config_req[cell_cnt];

                        /* Handle admin state unlocked for startup flow */
                        ret_val = oam_handle_initial_admin_state_unlocked((UInt8 *)INVALID_ARGUMENT,
                                p_rrm_cell_config,
                                &gen_trans_id);
                    }
                }    
            }

            oam_mem_free(p_m2ap_oam_provision_resp, &err);
        }
        break;

        default:
        OAM_LOG(OAM, OAM_DETAILED, "Invalid message\n");
        break;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}
#endif
/* EMBMS Changes End */
/****************************************************************************
 * Function Name  : oam_x2ap_msg_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the request/response received
 *                  from X2AP
 ****************************************************************************/
oam_return_et
oam_x2ap_msg_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt /*Pointer to CM FSM context*/
    /*SPR 17777 fix*/
    )
{
    UInt16    api_id  = OAM_ZERO;
    UInt16    old_trans_id = OAM_ZERO;
    UInt16    new_trans_id = OAM_ZERO;
    UInt16    timer_data   = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    oam_timer_t  timer;
    oam_return_et  ret_val = OAM_SUCCESS;

    OAM_FUN_ENTRY_TRACE();

    api_id   = get_word_from_header((UInt8*)(p_msg) + OAM_EXT_API_ID_FROM_HEADER);

   /*Below params are required for further APIs*/
    new_trans_id = oam_get_fresh_trans_id(old_trans_id);

    switch (api_id)
    {
         case X2AP_OAM_INIT_IND:
             if ((S1AP_OAM_INIT_CNFM & resp_flag)
  /* Start: CSR 00058940 */
                 && (INIT_TIME_PCI_SELECTION_MASK & initialisation_flag)
                 )
  /*End: CSR 00058940 */                 
             {
                 /* SPR 13832 START */ 
                 oam_sprintf((char *)&timer_data,"%d",RRC_X2AP_MODULE_ID); 
                 /* SPR 13832 END */ 
                 timer = oam_start_timer(guard_time, &timer_data,
                                           sizeof(timer_data), OAM_FALSE);
                 if (OAM_NULL == timer)
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while starting the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }

                 p_cm_cntxt->guard_timer_id = timer;

	             if (OAM_SUCCESS != oam_x2ap_il_send_x2ap_oam_provision_req(
                                         (oam_x2ap_provision_req_t*)p_msg,
                                                           OAM_MODULE_ID,
                                                           RRC_MODULE_ID,
                                                           new_trans_id,
                                      oam_prov_req.cell_cntxt.curr_cell_cid))
                 {
                      OAM_LOG(OAM, OAM_ERROR, "Sending X2AP_OAM_PROVISION_REQ message failed, cell_index=%d",
                              oam_prov_req.cell_cntxt.curr_cell_cid);
/*OAM_REVIEW_CHANGES*/
                      /* Coverity 64975 Changes Start */
                      if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                      {
                          OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                      }
                      /* Coverity 64975 Changes End */
/*OAM_REVIEW_CHANGES*/
                      ret_val = OAM_FAILURE;
                      break;
                 }
                 OAM_LOG(OAM, OAM_DETAILED, "Sending X2AP_OAM_PROVISION_REQ message success");

                 p_cm_cntxt->state = OAM_CM_ST_LAYER_WF_RES;
             }
             else
             {
                 OAM_LOG(OAM, OAM_INFO, "Not sending X2AP_OAM_PROVISION_REQ as value resp %u initialisation_flag %u",
                         resp_flag, initialisation_flag);
             }
             break;

         case X2AP_OAM_PROVISION_RESP:
             if (S1AP_OAM_INIT_CNFM & resp_flag)
             {
                 if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
                 {
                     OAM_LOG(OAM, OAM_DETAILED, "Error returned while stopping the timer");
                     ret_val = OAM_FAILURE;
                     break;
                 }
                 OAM_LOG(OAM, OAM_DETAILED, "X2AP_OAM_PROVISION_RESP message response success");

                 resp_flag |= X2AP_OAM_INIT_CNFM;
                 //p_cm_cntxt->state = OAM_CM_ST_ACTIVE;
             }
             break;

         default:
             OAM_LOG(OAM, OAM_INFO, "Invalid message[api_id=%d]", api_id);
             break;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}


/****************************************************************************
 * Function Name  : oam_cm_layer_handler
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the INIT indications received 
 *                  from various layers  
 ****************************************************************************/
oam_return_et
oam_cm_layer_handler (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    UInt16    src_id  = OAM_ZERO;
    oam_return_et  ret_val = OAM_SUCCESS;

    OAM_FUN_ENTRY_TRACE();

    src_id   = get_word_from_header((UInt8*)(p_msg) + OAM_INTF_HDR_SRC_ID_OFFSET);

    switch (src_id)
    {
	 /*SPR 21889 +- */
#ifdef OAM_SON_ENABLE 
         case SON_MODULE_ID:
             ret_val = oam_son_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;
#endif
        
         case RRM_MODULE_ID:
             ret_val = oam_rrm_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;
 
         case MAC_MODULE_ID:
             ret_val = oam_mac_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;
         
         case RLC_MODULE_ID:
             ret_val = oam_rlc_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case PDCP_MODULE_ID:
             ret_val = oam_pdcp_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case EGTPU_MODULE_ID:
            /*SPR 17777 fix*/
             ret_val = oam_egtpu_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case RRC_MODULE_ID:
             ret_val = oam_rrc_msg_handler(p_msg, p_cm_cntxt);
             break;

         case RRC_S1AP_MODULE_ID:
             ret_val = oam_s1ap_msg_handler(p_msg, p_cm_cntxt);
             break;

         case RRC_X2AP_MODULE_ID:
             ret_val = oam_x2ap_msg_handler(p_msg, p_cm_cntxt);
              /*SPR 17777 fix*/
             break;

#ifdef OAM_RADIO_HEAD_SUPPORT
        case RCM_MODULE_ID:
#ifndef OAM_EVM_TARGET
            if(oam_prov_req.rrh_flag) {
#else
            if(OAM_ONE == oam_prov_req.oam_rcm_req.start_rcm_config &&
                          oam_prov_req.rrh_flag) {
#endif
                ret_val = oam_cm_rrh_msg_handler(p_msg, p_cm_cntxt, p_err);
            }
            else {
                OAM_LOG(OAM, OAM_WARNING, "RRH module disabled!!!");
                ret_val = OAM_SUCCESS;
            }
            break;
#endif

         default:
             /*Invalid Option*/
             OAM_LOG(OAM, OAM_INFO, "Request received from invalid module[src_id=%d]", src_id);
             break;
    }

    if (OAM_FAILURE == ret_val)
    {
        OAM_LOG(OAM, OAM_WARNING, "oam_cm_layer_handler failed with OAM Error Code=[%d]", *p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}    

/****************************************************************************
 * Function Name  : oam_cm_layer_resp
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the responses received from the 
 *                  various layers 
 ****************************************************************************/
oam_return_et
oam_cm_layer_resp (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    UInt16    src_id  = OAM_ZERO;
    oam_return_et  ret_val = OAM_SUCCESS;

    OAM_FUN_ENTRY_TRACE();
   
    src_id   = get_word_from_header((UInt8*)(p_msg) + OAM_INTF_HDR_SRC_ID_OFFSET);
 
    switch (src_id)
    {
         case RRM_MODULE_ID:
             ret_val = oam_rrm_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case MAC_MODULE_ID:
             ret_val = oam_mac_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case RLC_MODULE_ID:
             ret_val = oam_mac_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case PDCP_MODULE_ID:
             ret_val = oam_mac_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case RRC_MODULE_ID:
             ret_val = oam_mac_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case RRC_S1AP_MODULE_ID:
             ret_val = oam_mac_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;

         case RRC_X2AP_MODULE_ID:
             ret_val = oam_mac_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;
        #ifdef OAM_SON_ENABLE 
         case SON_MODULE_ID:
             ret_val = oam_son_msg_handler(p_msg, p_cm_cntxt, p_err);
             break;
	#endif
 
#ifdef OAM_RADIO_HEAD_SUPPORT
         case RCM_MODULE_ID:
            if(oam_prov_req.rrh_flag) {
                ret_val = oam_cm_rrh_msg_handler(p_msg, p_cm_cntxt, p_err);
            }
            else {
                OAM_LOG(OAM, OAM_WARNING, "RRH module disabled!!!");
                ret_val = OAM_SUCCESS;
            }
            break;
#endif

         default:
             /*Invalid Option*/
             OAM_LOG(OAM, OAM_INFO, "Request received from invalid module[src_id=%d]", src_id);
             break;
    }
  
    if (OAM_FAILURE == ret_val)
    {
        OAM_LOG(OAM, OAM_WARNING, "oam_cm_layer_resp failed with OAM Error Code=[%d]", *p_err);
    }

    OAM_FUN_EXIT_TRACE();    
    return ret_val;
}    


/****************************************************************************
 * Function Name  : oam_cm_abort
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 *                  p_err :Pointer to error code
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function performs all the cleanup in case of abort 
 *                  event
 ****************************************************************************/
oam_return_et
oam_cm_abort(
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    oam_return_et  ret_val = OAM_SUCCESS;

    OAM_FUN_ENTRY_TRACE();

    #ifdef OAM_SON_ENABLE 
    system("killall -9 lte_son");
    #endif
    system("killall -9 lte_rrm");
    system("killall -9 lteLayer2");
    system("killall -9 rrc_entity");
     /* SPR 13832 START */
    system("killall -9 ltebinread");
     /* SPR 13832 END */
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
    if(oam_prov_req.rrh_flag && oam_prov_req.rcm_flag)
		system("killall -9 rcm");
#endif
#endif

    /* SPR 17777 fix */
    OAM_LOG(OAM, OAM_DETAILEDALL, "unused parameter %p and %p", p_msg, p_err);

    OAM_LOG(OAM, OAM_ERROR, "System ABORTED in Event:%s and State:%s",
                       OAM_CM_EVENT[p_cm_cntxt->event],
                       OAM_CM_STATE[p_cm_cntxt->state]);

    p_cm_cntxt->state = OAM_CM_ST_IDLE;

    OAM_FUN_EXIT_TRACE();

    return ret_val; 
}

/****************************************************************************
 * Function Name  : oam_cm_ignore_event
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context
 *                  p_err :Pointer to error code
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles all the state and event cases
 *                  where no action is required
 ****************************************************************************/
oam_return_et
oam_cm_ignore_event(
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    oam_return_et  ret_val = OAM_FAILURE;

    OAM_FUN_ENTRY_TRACE();

    /*No handling required for this state and event combination*/
    /* SPR 17777 fix */
    OAM_LOG(OAM, OAM_DETAILED, "Event %d in state %d ignored, unused parameter %p and %p",
                                              p_cm_cntxt->event,
                                             p_cm_cntxt->state, p_msg, p_err);

    return ret_val;
}

/****************************************************************************
 * Function Name  : oam_start_timer
 * Inputs         : timer_duration :Time Duration 
 *                  p_timer_data :Pointer to Timer data 
 *                  timer_data_size :Size of timer data
 *                  is_repeated :Repeated or not
 * Outputs        : none
 * Returns        : oam_timer_t
 * Description    : This function starts the timer.
 ****************************************************************************/
oam_timer_t
oam_start_timer
(
 /* Duration (in milliseconds) of the timer to be started */
 UInt32             timer_duration,
 /* Timer data that will be attached to timer */
 void            *p_timer_data,
 UInt16          timer_data_size,    /* Size of timer data */
 oam_bool_et     is_repeated
 )
{
    void *p_timer_buf   = OAM_NULL;
    oam_timer_t timer = OAM_ZERO;
 
    p_timer_buf = oam_msg_mem_get(timer_data_size);
    /* Coverity 71112 Changes Start */
    if(OAM_NULL == p_timer_buf)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_timer_bug failed");
        return OAM_NULL;
    }
    /* Coverity 71112 Changes End */
    /* SPR 13832 START */ 
    oam_memset(p_timer_buf,OAM_ZERO,timer_data_size);
    oam_memcpy(p_timer_buf,p_timer_data,timer_data_size); 
    /* SPR 13832 END */ 

    /* SPR 13832 START */ 
    timer = oam_start_new_timer(OAM_SUB_MODULE_CM, OAM_MGMT_INTF_MODULE_ID,
                    OAM_GUARD_TIMER_TYPE, timer_duration,
                    p_timer_buf, timer_data_size, is_repeated);
    return timer;
}
/****************************************************************************
 * Function Name  : oam_stop_timer
 * Inputs         : timer :Timer
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function stops the timer.
 ****************************************************************************/
oam_return_et
oam_stop_timer
(
 oam_timer_t timer
 )
{
    void *timer_ptr;
    /* coverity 35626 */
    oam_error_code_et p_error_code = NO_ERROR;
    if (OAM_NULL == timer)
    {
        OAM_LOG(OAM,OAM_INFO, "Timer Stop failed because Timer Id is NULL.");
        return OAM_FAILURE ;
    }
    else
    {
        if(qvTimerRunning((QTIMER)(timer)) == OAM_ONE)
        {
	    /* SPR 21619 Fix Start */
            timer_ptr = qvTimerData(timer);
            if(OAM_NULL != timer_ptr)
            oam_mem_free(timer_ptr, &p_error_code);
            if(qvModuleTimerStop(timer, OAM_NULL,OAM_NULL))
            {
                OAM_LOG(OAM, OAM_INFO, "Timer Stopped for Timer Id [%p]", timer);
            }
            else
            {
                OAM_LOG(OAM, OAM_WARNING, "Timer Stop for Timer Id [%p] failed", timer);
            }
	    /* SPR 21619 Fix End */
        }
        else
        {
            OAM_LOG(OAM, OAM_WARNING, "Timer for Timer Id [%p] not running", timer);
        }
    }
    return OAM_SUCCESS ;
}

/****************************************************************************
 * Function Name  : oam_cm_timer_expired
 * Inputs         : p_msg :Pointer to input api buffer
 *                  p_cm_cntxt :Pointer to CM FSM context 
 * Outputs        : p_err :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the timer expiry events
 ****************************************************************************/
oam_return_et
oam_cm_timer_expired (
    void *p_msg, /*Pointer to input api buffer*/
    oam_cm_cntxt_t *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
    )
{
    oam_return_et  ret_val = OAM_SUCCESS;

    OAM_FUN_ENTRY_TRACE();

    p_cm_cntxt->event = OAM_CM_EV_ABORT;
    ret_val = oam_cm_fsm_process_event(p_msg, p_cm_cntxt, p_err);
 
    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/****************************************************************************
 * Function Name  : oam_cm_fsm_init
 * Inputs         : None
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function initializes the CM FSM.
 ****************************************************************************/
oam_return_et
oam_cm_fsm_init(
    void
    )
{
    UInt16 length;
	UInt8  cell_index;
    oam_error_code_et p_err;

    OAM_FUN_ENTRY_TRACE();

    length = sizeof(oam_cm_cntxt_t);


    /* Perform the memory allocation */
    if(OAM_FAILURE == oam_mem_alloc(length, (void*)&p_cm_cntxt, &p_err))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_cm_cntxt failed"
                " with Error Code:%d",
                                                                          p_err);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    else
    {
        OAM_LOG(OAM, OAM_DETAILED, "Memory allocation success");
    }

    p_cm_cntxt->state = OAM_CM_ST_IDLE;
/*OAM_REVIEW_CHANGES*/
    p_cm_cntxt->event = OAM_CM_MAX_EVENT;
    p_cm_cntxt->guard_timer_id = OAM_NULL;
    


    for (cell_index = OAM_ZERO; cell_index < OAM_MAX_CELL_SUPPORTED; cell_index++)
    	{
        p_cm_cntxt->state = OAM_CM_ST_LAYER_INIT;
        p_cm_cntxt->event = OAM_CM_MAX_EVENT;
        p_cm_cntxt->guard_timer_id = OAM_NULL;
    	}


    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Array Name     : oam_cm_fsm_tbl
 * Inputs         : OAM_CM_ST_MAX : Current OAM CM State 
 *                  OAM_CM_EV_MAX : Current OAM CM Event
 * Outputs        : None
 * Returns        : None
 * Description    : Definition of two dimensional array of functional pointer 
 *		    for OAM CM FSM
 ****************************************************************************/

/*OAM_REVIEW_CHANGES*/
oam_cm_func_ptr oam_cm_fsm_tbl[OAM_CM_ST_MAX][OAM_CM_EV_MAX] =
/*OAM_REVIEW_CHANGES*/
{
    /* OAM_CM_ST_IDLE */
   { 
        /*OAM_CM_EV_LAYER_REQ*/
	#ifndef OAM_SON_ENABLE 
        oam_cm_layer_handler,
	#else
        oam_cm_first_layer_init_req,
	#endif
        /*OAM_CM_EV_LAYER_RES*/
        oam_cm_ignore_event,
        /*OAM_CM_EV_TIMER_EXPIRY*/
        oam_cm_ignore_event,
        /*OAM_CM_EV_ABORT*/
        oam_cm_ignore_event
    },

    /* OAM_CM_ST_LAYER_INIT */
    {
        /*OAM_CM_EV_LAYER_REQ*/
        oam_cm_layer_handler,
        /*OAM_CM_EV_LAYER_RES*/
        oam_cm_ignore_event,
        /*OAM_CM_EV_TIMER_EXPIRY*/
        oam_cm_ignore_event,
        /*OAM_CM_EV_ABORT*/
        oam_cm_abort
    },

    /* OAM_CM_ST_LAYER_WF_RES */
    {
        /*OAM_CM_EV_LAYER_REQ*/
        oam_cm_ignore_event,
        /*OAM_CM_EV_LAYER_RES*/
        oam_cm_layer_handler,
        /*OAM_CM_EV_TIMER_EXPIRY*/
        oam_cm_timer_expired,
        /*OAM_CM_EV_ABORT*/
        oam_cm_abort
    },

    /* OAM_CM_ST_ACTIVE */
    {
        /*OAM_CM_EV_LAYER_REQ*/
        oam_cm_layer_handler,
        /*OAM_CM_EV_LAYER_RES*/
        oam_cm_ignore_event,
        /*OAM_CM_EV_TIMER_EXPIRY*/
        oam_cm_ignore_event,
        /*OAM_CM_EV_ABORT*/
        oam_cm_abort
    }
};


/****************************************************************************
 * Function Name  : oam_handle_initial_admin_state_unlocked
 * Inputs         : p_trans_id :Pointer to transaction identifier
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles the initial admin state unlocked
****************************************************************************/

oam_return_et
/* spr_21013_fix_start */
oam_handle_initial_admin_state_unlocked
(
/* SPR 22248 MBMS Changes Start */
	UInt8 *p_is_oam_startup_required,
/* SPR 22248 MBMS Changes Stop */
    oam_rrm_cell_config_req_t  *p_rrm_cell_config,
    UInt16  *p_trans_id
)
/* spr_21013_fix_end */
{
    /* Coverity 63709 Fix */ 
    oam_return_et          ret_val = OAM_SUCCESS;
    /* coverity 35625 */
    oam_error_code_et      err = NO_ERROR;
    UInt8 cid = OAM_ZERO;
    UInt16 transaction_id = OAM_ZERO; 
    static oam_bool_et s_admin_spv_init = OAM_FALSE;
    UInt8 operation_type = 0;
    OAM_FUN_ENTRY_TRACE();

    OAM_LOG(OAM, OAM_INFO, "Admin State is Unlocked");
    
    transaction_id = oam_get_new_trans_id();
    *p_trans_id = transaction_id; 
    /* SPR 20788 changes start */
    if( (OAM_CM_ST_ACTIVE == p_cm_cntxt->state) &&
        (OAM_ONE == init_time_startup) )
    {
        init_time_startup = OAM_ZERO;
    }      
    /* SPR 20788 changes end */
    
    if(init_time_startup == OAM_ONE)
    {
        /* CID 70312 FIX START */
    	ret_val = oam_validate_tr196_schema(&oam_prov_req.system_status.p_xml_doc); 
        if (OAM_SUCCESS == ret_val) {
        OAM_LOG(OAM, OAM_DETAILED,
                "value is validated successfully");
        }
        else
        {
            OAM_LOG(OAM, OAM_WARNING,
                "TR196 schema validation failed");
            return ret_val;
        }
        /* CID 70312 FIX END */
    
        oam_sim_error_info_et start_error = SIM_NO_ERROR;
        s_admin_spv_init = OAM_TRUE;

    /* spr_21013_fix_start */
	/* check added to avoid multiple spawning of all layers through oam */
/* SPR 22248 MBMS Changes Start */
//Code commented as p_is_oam_startup_required is not used in the function anymore
	if(OAM_FALSE == *p_is_oam_startup_required)
	{
		oam_startup_init(&start_error);
		*p_is_oam_startup_required = OAM_TRUE;
	} 

/* SPR 22248 MBMS Changes Stop */
    /* spr_21013_fix_end */
        /* SPR 20788 changes reverted start */
        return OAM_SUCCESS;  
        /* SPR 20788 changes reverted end */
    }

/* SPR 11175 FIX START */
    cid = oam_extract_cid_from_eutran_cellidentity(
                                p_rrm_cell_config->global_cell_info.
                                eutran_global_cell_id.cell_identity);
/* SPR 11175 FIX END */

    ret_val = oam_rrm_send_rrm_oam_cell_config_req(p_rrm_cell_config,
                                                   OAM_MODULE_ID,
                                                   RRM_MODULE_ID,
                                                   transaction_id,
/* SPR 11175 FIX START */
                                                   cid);
        /* SPR 11175 FIX END */
    if(OAM_SUCCESS !=  ret_val) {
    /*SPR 21660 Fix Start_shivani*/
        OAM_LOG(OAM, OAM_ERROR, "Sending RRM_OAM_CELL_CONFIG_REQ message failed, cell_index=%d, cell_type %d", 
         cid,operation_type);
    }
    else {
        OAM_LOG(OAM, OAM_INFO, "Sending RRM_OAM_CELL_CONFIG_REQ message success for cell_type %d",
           operation_type);
    /*SPR 21660 Fix End_shivani*/
       /* Send cell config request */
       /* Switch on radio transmission */
        OAM_LOG(OAM, OAM_INFO, "RFTxStatus is set to True");
       oam_prov_req.system_status.rftx_status = OAM_TRUE;   

        if (OAM_TRUE == s_admin_spv_init) {
            if (OAM_SUCCESS != oam_stop_timer(g_admin_state_timer)) {
                OAM_LOG(OAM, OAM_DETAILED,
                        "Error returned while stopping the timer");
            }

           oam_construct_n_send_set_resp_to_usr(OAM_RESULT_SUCCESS,
                    OAM_ZERO, OAM_NULL, OAM_ZERO,
                                    OAM_VERSION_ID, OAM_MODULE_ID,
                    TR069_MODULE_ID, g_admin_state_trans_id,
                    OAM_ZERO, OAM_ZERO, &err);

       }

        if (oam_prov_req.oam_tr069_req.tr069_mode) {
            ret_val = oam_inform_value_change_event(OAM_NULL, OAM_ZERO,
                    TR069_MODULE_ID);

            if (OAM_FAILURE == ret_val) {
                OAM_LOG(OAM, OAM_ERROR,
                        "Sending inform message to TR069 agent failed");
                ret_val = OAM_SUCCESS;
            }
            else {
                OAM_LOG(OAM, OAM_DETAILED,
                        "Sending inform message to TR069 agent successful");
            }
        }
        /*EMBMS UT_Changes Start */
#ifndef OAM_UT_FRAMEWORK

        /* Start timer to wait response from lower layer */
        oam_prov_req.system_status.timer_id =
                    oam_start_new_timer(OAM_SUB_MODULE_MIH, OAM_MODULE_ID,
                        OAM_CELL_SETUP_TIMER_TYPE,
                        OAM_CELL_OPERATION_TIMEOUT_DURATION*OAM_MSEC_IN_ONE_SEC,
                    OAM_NULL, OAM_ZERO, OAM_FALSE);

        if(oam_prov_req.system_status.timer_id) {
            OAM_LOG(OAM,OAM_DETAILED,"Cell Setup Timer started successfully"); 
        }
        else {
            OAM_LOG(OAM, OAM_DETAILED, "Failed to start Cell Setup Timer"); 
            ret_val = OAM_FAILURE;
        }
#endif
        /*EMBMS UT_Changes End */
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}
