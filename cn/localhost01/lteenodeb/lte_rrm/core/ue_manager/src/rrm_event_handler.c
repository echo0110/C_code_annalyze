/* vim: ts=4:et:ai:ci:sw=4
*/
/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_event_handler.c $
 *
 *******************************************************************************
 *
 * File Description: This file have the information about processing 
 *                   event messages.
 *
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   7 Nov, 2012     gur19685     Initial
 *   1  May, 2014    gur26515                    Added code changes for SPR 10751
 ******************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrm_ue_context.h"
#include "rrm_events.h"
/* COMP_WARN_1_FEB:compilation warning removal */
#include "rrm_ue_mgr.h"
#include "rrm_rrc_ue_intf.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/
/****************************************************************************
  Private Types
 ****************************************************************************/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
S8 *p_g_rrm_event_facility_name = (S8 *)"RRM_EVENT";

rrm_return_et
rrm_compose_local_event_meas_config_periodical_cdma
(
 U8  *p_buffer,
 rrm_event_meas_config_periodical_cdma_t  *p_rrm_event_meas_config_periodical_cdma
 );

/* To Print Event String Name */ 
const S8 * RRM_PROTOCOL_EVENT[]=
{
    (const S8 *)   "RRM_PE_CELL_SPECIFIC_MIN_ID",
    (const S8 *)   "RRM_PE_CELL_SETUP_REJECT",
    (const S8 *)   "RRM_PE_CELL_SPECIFIC_MAX_ID",
    (const S8 *)   "RRM_PE_UE_SPECIFIC_MIN_ID",
    (const S8 *)   "RRM_PE_RRC_CONN_SETUP_READY",
    (const S8 *)   "RRM_PE_EVENT_UE_CAPABILITY",
    (const S8 *)   "RRM_PE_EVENT_UE_CAPABILITY_FGI_REJECT",
    (const S8 *)   "RRM_PE_UE_SPECIFIC_MAX_ID",
    (const S8 *)   "RRM_PE_ERAB_SPECIFIC_MIN_ID",
    (const S8 *)   "RRM_PE_ERAB_SETUP_ATTEMPT",
    (const S8 *)   "RRM_PE_ERAB_SETUP_READY",
    (const S8 *)   "RRM_PE_ERAB_ADMISSION_REJECT",
    (const S8 *)   "RRM_PE_ERAB_DRB_RELEASE",
    (const S8 *)   "RRM_PE_ERAB_RELEASE_ATTEMPT",
    (const S8 *)   "RRM_PE_ERAB_RELEASE_READY",
    (const S8 *)   "RRM_PE_ERAB_MODIFY_READY",
    (const S8 *)   "RRM_PE_ERAB_MODIFY_ATTEMPT",
    (const S8 *)   "RRM_PE_ERAB_DRB_ALLOCATED",
    (const S8 *)   "RRM_PE_ERAB_SETUP_QCI",
    (const S8 *)   "RRM_PE_EVENT_ERAB_RELEASE_INFO",
    (const S8 *)   "RRM_PE_EVENT_ERAB_SETUP_INFO",
    (const S8 *)   "RRM_PE_ERAB_SPECIFIC_MAX_ID",
    (const S8 *)   "RRM_PE_MEAS_CONFIG_SPECIFIC_MIN_ID",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_A1",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_A2",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_A3",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_A4",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_A5",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_B2_UTRA",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_B2_GERAN",
    (const S8 *)   "RRM_PE_EVENT_UE_MEAS_ABORT",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_EUTRA",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_UTRA",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_GERAN",
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_CDMA",    /* CSR 00058108: code changes */
    (const S8 *)   "RRM_PE_EVENT_MEAS_CONFIG_B2_CDMA2000",
    (const S8 *)   "RRM_PE_MEAS_CONFIG_SPECIFIC_MAX_ID",
    (const S8 *)   "RRM_PE_HO_IN_SPECIFIC_MIN_ID",
    (const S8 *)   "RRM_PE_HO_IN_EXEC_COMPLETE",
    (const S8 *)   "RRM_PE_HO_IN_PREP_COMPLETE",
    (const S8 *)   "RRM_PE_HO_IN_EXEC_ATTEMPT",
    (const S8 *)   "RRM_PE_HO_IN_EXEC_FAILED",
    (const S8 *)   "RRM_PE_EVENT_HO_IN_PREP_ATTEMPT",
    (const S8 *)   "RRM_PE_EVENT_HO_IN_PREP_FAILURE",
    (const S8 *)   "RRM_PE_HO_IN_SPECIFIC_MAX_ID",
    (const S8 *)   "RRM_PE_HO_OUT_SPECIFIC_MIN_ID",
    (const S8 *)   "RRM_PE_HO_OUT_PREP_ATTEMPT",
    (const S8 *)   "RRM_PE_HO_OUT_PREP_FAILED",
    (const S8 *)   "RRM_PE_HO_OUT_PREP_INFO",
    (const S8 *)   "RRM_PE_HO_OUT_EXEC_ATTEMPT",
    (const S8 *)   "RRM_PE_HO_OUT_EXEC_FAILED",
    (const S8 *)   "RRM_PE_HO_OUT_EXEC_COMPLETE",
    (const S8 *)   "RRM_PE_EVENT_HO_UE_DETECTED",
    (const S8 *)   "RRM_PE_HO_OUT_EXEC_INFO",
    (const S8 *)   "RRM_PE_EVENT_TOO_EARLY_HO",
    (const S8 *)   "RRM_PE_EVENT_TOO_LATE_HO",
    (const S8 *)   "RRM_PE_EVENT_HO_WRONG_CELL",
    (const S8 *)   "RRM_PE_EVENT_HO_WRONG_CELL_REEST",
    (const S8 *)   "RRM_PE_EVENT_X2_RLF_INDICATION",
    (const S8 *)   "RRM_PE_HO_OUT_SPECIFIC_MAX_ID",
    (const S8 *)   "RRM_PE_OTHER_MIN_ID",
    (const S8 *)   "RRM_PE_MME_CONFIGURATION_TRANSFER",
    (const S8 *)   "RRM_PE_ENB_CONFIGURATION_TRANSFER",
    (const S8 *)   "RRM_PE_SRB1_SETUP_REJECT",
    (const S8 *)   "RRM_MAX_PROTOCOL_EVENT_ID"
};

#define RRM_PRINT_PROTOCOL_EVENT(index)  \
    (index < RRM_MAX_PROTOCOL_EVENT_ID) ?  RRM_PROTOCOL_EVENT[index] : (const S8 *)"INVALID_EVENT_ID"

/*Cov_fix_54945_start*/
/***********************************************************************
 * Function Name  : rrm_fill_event_subclass
 * Inputs         : event Id
 * Returns        : void
 * Description    : This function return subclass on the basis 
 *                  of event-Id.
 **********************************************************************/
    rrm_oam_event_subclass_et
rrm_fill_event_subclass(rrm_event_message_id_et  event_id)
{
    U8 event_index = RRM_ZERO;
    /*coverity id :29687 , rel 2.0.1 */
    rrm_oam_event_subclass_et sub_class = RRM_OAM_INVALID_SUBCLASS;

    RRM_UT_TRACE_ENTER();

    event_index = rrm_find_protocol_event_index(event_id);

    switch(event_index)
    {
        case RRM_PE_CELL_SPECIFIC_INDEX:
            sub_class = RRM_OAM_PE_CELL_SPECIFIC;
            break;

        case RRM_PE_UE_SPECIFIC_INDEX:
            sub_class = RRM_OAM_PE_UE_SPECIFIC;
            break;

        case RRM_PE_ERAB_SPECIFIC_INDEX:
            sub_class = RRM_OAM_PE_ERAB_SPECIFIC;
            break;

        case RRM_PE_MEAS_INFO_INDEX:
            sub_class = RRM_OAM_PE_MEAS_INFO;
            break;

        case RRM_PE_HO_OUT_INDEX:
            sub_class = RRM_OAM_PE_HO_OUT;
            break;

        case RRM_PE_HO_IN_INDEX:
            sub_class = RRM_OAM_PE_HO_IN;
            break;

        case RRM_PE_OTHERS_INDEX:
            sub_class = RRM_OAM_PE_OTHERS;
            break;

        default:
            sub_class = RRM_OAM_INVALID_SUBCLASS; 
            break;
    }

  /*Coverity_fix_start_54945*/
  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
            "EventId[%d]  Subclass[%d]", event_id, sub_class);
  /*Coverity_fix_end_54945*/
    RRM_UT_TRACE_EXIT();
    return sub_class;
}
/*Cov_fix_54945_end*/
/***********************************************************************
 * Function Name  : fill_rrmuem_rmif_event_notification_header
 * Inputs         : rrmuem_rmif_event_header_t,rrm_event_header_t
 * Returns        : void
 * Description    : This function fill rrmuem_rmif event notification header
 **********************************************************************/
void fill_rrmuem_rmif_event_notification_header(rrmuem_rmif_event_header_t *p_rmif_event_header,
        rrm_event_header_t         *p_event_header)
{
    RRM_UT_TRACE_ENTER();

    /*Filling Time Stamp*/
    p_rmif_event_header->time_stamp.year  = p_event_header->time_stamp.year;
    p_rmif_event_header->time_stamp.month = p_event_header->time_stamp.month;
    p_rmif_event_header->time_stamp.day   = p_event_header->time_stamp.day;
    p_rmif_event_header->time_stamp.hour  = p_event_header->time_stamp.hour;
    p_rmif_event_header->time_stamp.min   = p_event_header->time_stamp.min;
    p_rmif_event_header->time_stamp.sec   = p_event_header->time_stamp.sec;

    p_rmif_event_header->event_id = p_event_header->event_id;
    p_rmif_event_header->event_type = RRM_OAM_PROTOCOL_EVENT_TYPE;
    p_rmif_event_header->event_sub_type = 
        rrm_fill_event_subclass(p_event_header->event_id);

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_log_and_send_protocol_event
 * Inputs         : event_msg, event_id, api_len, action
 * Returns        : void
 * Description    : This function log & notify the raised protocol event 
 *                  based on the action.
 **********************************************************************/
void rrm_log_and_send_protocol_event(
        rrm_action_prfmd_et  action,
        U16                  event_id,
        U16                  api_len,
        void 		*event_msg)
{
    rrm_bool_et  log_flag = RRM_FALSE;
    rrm_bool_et  notify_flag = RRM_FALSE;

    RRM_UT_TRACE_ENTER();

    switch (action)
    {
        case EVENT_ACTION_LOG:
            {
                log_flag = RRM_TRUE;
            }
            break;

        case EVENT_ACTION_NOTIFY:
            {
                notify_flag = RRM_TRUE;
            }
            break;

        case EVENT_ACTION_NOTIFY_LOG:
            {
                log_flag = RRM_TRUE;
                notify_flag = RRM_TRUE;
            }
            break;

        case EVENT_ACTION_NONE:
        default:
      RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                    "EVENT_ACTION_NONE");
            break;
    }/*switch case ends*/ 

    /* print in the rrm logs*/ 
    if (log_flag)
    {
        /* SPR 21251 +- */
     /*RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_BRIEF,
                "EventId[%d]  [%s]" , event_id, (RRM_PRINT_PROTOCOL_EVENT(event_id)));*/
    }

    /*notify to the oam module*/
    if (notify_flag)
    {
     /*coverity 29027, rel2.0.1*/
        if(RRM_SUCCESS == rrm_uem_send_internal_msg(RRM_MIF_MODULE_ID,
                    RRMUEM_RMIF_EVENT_NOTIFICATION_IND,
                    api_len,
                    event_msg))
        {
            /* SPR 21251 +- */
     /* RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_BRIEF,
                    "Message sent to UEM"); */
        }
        else
        {
     RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_BRIEF,
                    "Message not sent to UEM"); 
        }
    }

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_find_protocol_event_index
 * Inputs         : event_id
 * Returns        : U8 (index value)
 * Description    : This function return the subclass index based 
 *                  on the event-id`s subclass. 
 **********************************************************************/
    U8 
rrm_find_protocol_event_index(U16 event_id)
{
    U8 index = RRM_EVENT_MAX_INDEX;

    if((event_id > RRM_PE_CELL_SPECIFIC_MIN_ID)&&
            (event_id < RRM_PE_CELL_SPECIFIC_MAX_ID))
    {
        index = RRM_PE_CELL_SPECIFIC_INDEX;
    }
    else if((event_id > RRM_PE_UE_SPECIFIC_MIN_ID)&&
            (event_id < RRM_PE_UE_SPECIFIC_MAX_ID))
    {
        index = RRM_PE_UE_SPECIFIC_INDEX;
    }
    else if((event_id > RRM_PE_ERAB_SPECIFIC_MIN_ID)&&
            (event_id <RRM_PE_ERAB_SPECIFIC_MAX_ID))
    {
        index = RRM_PE_ERAB_SPECIFIC_INDEX;
    }
    else if((event_id > RRM_PE_MEAS_CONFIG_SPECIFIC_MIN_ID)&&
            (event_id < RRM_PE_MEAS_CONFIG_SPECIFIC_MAX_ID))
    {
        index = RRM_PE_MEAS_INFO_INDEX;
    }
    else if((event_id > RRM_PE_HO_IN_SPECIFIC_MIN_ID)&&
            (event_id < RRM_PE_HO_IN_SPECIFIC_MAX_ID))
    {
        index = RRM_PE_HO_IN_INDEX;
    }
    else if((event_id > RRM_PE_HO_OUT_SPECIFIC_MIN_ID)&&
            (event_id < RRM_PE_HO_OUT_SPECIFIC_MAX_ID))
    {
        index = RRM_PE_HO_OUT_INDEX;
    }
    else if((event_id > RRM_PE_OTHER_MIN_ID)&&
            (event_id < RRM_MAX_PROTOCOL_EVENT_ID))
    {
        index = RRM_PE_OTHERS_INDEX;
    }

    return index;
} 

/***********************************************************************
 * Function Name  : rrm_handle_event_subclass
 * Inputs         : event_msg, event_id, api_len, ue_global_context
 * Returns        : void
 * Description    : This function enable_disable logging/notification 
 *                  based on the subclass.
 **********************************************************************/
void rrm_handle_event_subclass(
        rrm_ue_global_context_t  *p_ue_gb_ct, 
        void                     *event_msg,
        U16                      api_len,
        U16                      event_id)
{
    U8   pe_subclass_index = RRM_EVENT_MAX_INDEX;

    RRM_UT_TRACE_ENTER();

    pe_subclass_index = rrm_find_protocol_event_index(event_id);
    /* Klocwork changes start 22 may */
    if( pe_subclass_index < RRM_EVENT_MAX_INDEX)
    {

        rrm_log_and_send_protocol_event(p_ue_gb_ct->event_config.action_prfmd[pe_subclass_index], 
                event_id, 
                api_len, 
                event_msg);
    }
    /* Klocwork changes end 22 may */
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_enable_disable_protocol_event
 * Inputs         : event_msg, event_id, api_len
 * Returns        : void
 * Description    : This function enable_disable action perform 
 *                  on the protocol event. 
 **********************************************************************/
void rrm_enable_disable_protocol_event(void    *event_msg, 
        U16     api_len, 
        U16     event_id)
{
    rrm_ue_global_context_t   *p_ue_glb_ctxt = RRM_PNULL; 
    RRM_UT_TRACE_ENTER();

    /* fetching global context */
    p_ue_glb_ctxt = (rrm_ue_global_context_t *)qvThisContext();

    /* ,coverity ,id :: 30219 , added if condition to check return value of qvThisContext */
    if(RRM_PNULL != p_ue_glb_ctxt)
    {
        /* SPR 14024 Fix Start */
        if((event_id > RRM_PE_CELL_SPECIFIC_MIN_ID)&&
                (event_id < RRM_PE_CELL_SPECIFIC_MAX_ID))
        {
            p_ue_glb_ctxt->event_config.pe_subcls = RRM_TRUE;
            p_ue_glb_ctxt->event_config.action_prfmd[RRM_PE_CELL_SPECIFIC_INDEX] = EVENT_ACTION_NOTIFY_LOG;
        }
        /* SPR 14024 Fix Stop */

        if (RRM_FALSE == p_ue_glb_ctxt->event_config.pe_subcls)
        { 
            rrm_log_and_send_protocol_event(p_ue_glb_ctxt->event_config.action_prfmd[RRM_PE_INDEX],
                    event_id,
                    api_len,
                    event_msg);
        }
        else
        {
            rrm_handle_event_subclass(p_ue_glb_ctxt, event_msg, api_len, event_id);    
        }
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                "qThisContext returned null to p_ue_glb_ctxt");
    }
    RRM_UT_TRACE_EXIT();
}  

/***********************************************************************
 * Function Name  : rrm_process_event_handler
 * Inputs         : event_buff, event_id, src_id
 * Returns        : void
 * Description    : This function process event received from the RRM 
 *                  modules.
 **********************************************************************/
void rrm_process_event_handler(void   *event_buff, 
        U16     event_id, 
        U16     src_id)
{
    rrm_return_et ret_val = RRM_SUCCESS; 
    U16 api_len = RRM_ZERO;

    rrmuem_rmif_event_notification_ind_t 
        *p_rrmuem_rmif_event_notification_ind = RRM_PNULL;

    api_len = sizeof(rrmuem_rmif_event_notification_ind_t);

    p_rrmuem_rmif_event_notification_ind = rrm_msg_mem_get(api_len);

    /*+ Coverity CID : 21192 +*/
    if(RRM_PNULL == p_rrmuem_rmif_event_notification_ind)
    {
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Failed due to no memory");
    /*coverity id: 29145, rel2.0.1, return added*/
        RRM_ASSERT(RRM_PNULL != p_rrmuem_rmif_event_notification_ind);
	/* Coverity 29145 Fix Start */
	return;
	/* Coverity 29145 Fix End */
    }
    /*- Coverity CID : 21192 -*/

    RRM_MEMSET(p_rrmuem_rmif_event_notification_ind, RRM_ZERO, api_len);
    switch (event_id)
    {
        case RRM_PE_RRC_CONN_SETUP_READY:
            { 
                rrm_event_local_rrc_setup_ready_t  *p_event_local_rrc_setup_ready = RRM_PNULL;
                p_event_local_rrc_setup_ready = (rrm_event_local_rrc_setup_ready_t*)event_buff;

                /* Filling rrmuem_rmif_event_notification_ind_t API Header */
                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_event_local_rrc_setup_ready->msg_header));
            } 
            break;

        case RRM_PE_ERAB_SETUP_ATTEMPT:
            {
                rrm_event_local_erab_setup_attempt_t  *p_rrm_event_local_erab_setup_attempt  = RRM_PNULL;
                p_rrm_event_local_erab_setup_attempt = (rrm_event_local_erab_setup_attempt_t*) event_buff;

                /* Filling rrmuem_rmif_event_notification_ind_t API Header */
                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_erab_setup_attempt->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_setup_attempt(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_erab_setup_attempt);
            }
            break;

        case RRM_PE_ERAB_ADMISSION_REJECT:
            { 
                rrm_event_local_erab_admission_reject_t *p_rrm_event_local_erab_admission_reject = RRM_PNULL;
                p_rrm_event_local_erab_admission_reject = (rrm_event_local_erab_admission_reject_t*)event_buff;

                /* Filling rrmuem_rmif_event_notification_ind_t API Header */
                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_erab_admission_reject->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_adm_rej(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_erab_admission_reject);
            }
            break;

        case RRM_PE_ERAB_SETUP_READY:
            {
                rrm_event_local_erab_setup_ready_t *p_rrm_event_local_erab_setup_ready = RRM_PNULL;
                p_rrm_event_local_erab_setup_ready = (rrm_event_local_erab_setup_ready_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_erab_setup_ready->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_setup_ready(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_erab_setup_ready);
            }
            break;

        case RRM_PE_ERAB_DRB_RELEASE:
            { 
                rrm_event_local_erab_drb_release_t *p_rrm_event_local_erab_drb_release = RRM_PNULL;
                p_rrm_event_local_erab_drb_release = (rrm_event_local_erab_drb_release_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_erab_drb_release->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_drb_release(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_erab_drb_release);
            }
            break;

        case RRM_PE_ERAB_RELEASE_ATTEMPT:
            {
                rrm_event_local_erab_release_attempt_t *p_rrm_event_local_erab_release_attempt = RRM_PNULL;
                p_rrm_event_local_erab_release_attempt = (rrm_event_local_erab_release_attempt_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_erab_release_attempt->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_drb_release_attempt(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_erab_release_attempt);
            }
            break;

        case RRM_PE_ERAB_RELEASE_READY:
            {
                rrm_event_local_erab_release_ready_t *p_rrm_event_local_erab_release_ready = RRM_PNULL;
                p_rrm_event_local_erab_release_ready = (rrm_event_local_erab_release_ready_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_erab_release_ready->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_release_ready(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_erab_release_ready);
            }
            break;

        case RRM_PE_CELL_SETUP_REJECT:
            { 
                rrm_event_local_cell_setup_reject_t *p_rrm_event_local_cell_setup_reject = RRM_PNULL;
                p_rrm_event_local_cell_setup_reject = (rrm_event_local_cell_setup_reject_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_cell_setup_reject->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_cell_setup_reject(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_cell_setup_reject);
            }
            break;

        case RRM_PE_EVENT_UE_CAPABILITY:
            { 
                rrm_event_local_event_ue_capability_t  *p_rrm_event_local_event_ue_capability = RRM_PNULL;
                p_rrm_event_local_event_ue_capability = (rrm_event_local_event_ue_capability_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_event_ue_capability->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ue_capability(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_event_ue_capability);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_A1:
            {
                rrm_event_meas_config_a1_t  *p_rrm_event_meas_config_a1 = RRM_PNULL;
                p_rrm_event_meas_config_a1 = (rrm_event_meas_config_a1_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_a1->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;

                /*  
                //Using memcopy,compose function will be provided later
                p_rrm_event_meas_config_a1 = p_rrm_event_meas_config_a1 + sizeof(rrm_event_header_t);
                RRM_MEMCPY(p_rrmuem_rmif_event_notification_ind->buffer,p_rrm_event_meas_config_a1,
                (sizeof(rrm_event_meas_config_a1_t) - sizeof(rrm_event_header_t)));
                 */
                rrm_compose_local_event_meas_config_a1(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_a1);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_A2:
            {
                rrm_event_meas_config_a2_t  *p_rrm_event_meas_config_a2 = RRM_PNULL;
                p_rrm_event_meas_config_a2 = (rrm_event_meas_config_a2_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_a2->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_a2(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_a2);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_A3:
            {
                rrm_event_meas_config_a3_t  *p_rrm_event_meas_config_a3 = RRM_PNULL;
                p_rrm_event_meas_config_a3 = (rrm_event_meas_config_a3_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_a3->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_a3(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_a3);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_A4:
            {
                rrm_event_meas_config_a4_t  *p_rrm_event_meas_config_a4 = RRM_PNULL;
                p_rrm_event_meas_config_a4 = (rrm_event_meas_config_a4_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_a4->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_a4(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_a4);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_A5:
            {
                rrm_event_meas_config_a5_t  *p_rrm_event_meas_config_a5 = RRM_PNULL;
                p_rrm_event_meas_config_a5 = (rrm_event_meas_config_a5_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_a5->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_a5(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_a5);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_B2_GERAN:
            {
                rrm_event_meas_config_b2_geran_t  *p_rrm_event_meas_config_b2_geran = RRM_PNULL;
                p_rrm_event_meas_config_b2_geran = (rrm_event_meas_config_b2_geran_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_b2_geran->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_b2_geran(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_b2_geran);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_B2_UTRA:
            {
                rrm_event_meas_config_b2_utra_t  *p_rrm_event_meas_config_b2_utra = RRM_PNULL;
                p_rrm_event_meas_config_b2_utra = (rrm_event_meas_config_b2_utra_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_b2_utra->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_b2_utra(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_b2_utra);
            }
            break;

        case RRM_PE_EVENT_UE_MEAS_ABORT:
            {
                rrm_event_ue_meas_abort_t  *p_rrm_event_ue_meas_abort = RRM_PNULL;
                p_rrm_event_ue_meas_abort = (rrm_event_ue_meas_abort_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ue_meas_abort->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ue_meas_abort(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ue_meas_abort);
            }
            break;

        case RRM_PE_EVENT_UE_CAPABILITY_FGI_REJECT:
            { 
                rrm_event_local_ue_capability_fgi_reject_t *p_rrm_event_local_ue_capability_fgi_reject = RRM_PNULL;
                p_rrm_event_local_ue_capability_fgi_reject = (rrm_event_local_ue_capability_fgi_reject_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_ue_capability_fgi_reject->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ue_cap_fgi_reject(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_ue_capability_fgi_reject);
            }
            break;

        case RRM_PE_ERAB_MODIFY_READY:
            { 
                rrm_event_erab_modify_ready_t *p_rrm_event_erab_modify_ready = RRM_PNULL;
                p_rrm_event_erab_modify_ready = (rrm_event_erab_modify_ready_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_erab_modify_ready->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_modify_ready(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_erab_modify_ready);
            }
            break;

        case RRM_PE_ERAB_MODIFY_ATTEMPT:
            { 
                rrm_event_local_erab_modify_attempt_t *p_rrm_event_local_erab_modify_attempt = RRM_PNULL;
                p_rrm_event_local_erab_modify_attempt = (rrm_event_local_erab_modify_attempt_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_local_erab_modify_attempt->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_modify_attempt(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_local_erab_modify_attempt);
            }
            break;

        case RRM_PE_ERAB_DRB_ALLOCATED:
            {
                rrm_event_erab_drb_allocated_t *p_rrm_event_erab_drb_allocated = RRM_PNULL;
                p_rrm_event_erab_drb_allocated = (rrm_event_erab_drb_allocated_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_erab_drb_allocated->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_drb_allocated(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_erab_drb_allocated);
            }
            break;

        case RRM_PE_MME_CONFIGURATION_TRANSFER:
            {
                rrm_event_mme_config_transfer_t *p_rrm_event_mme_config_transfer = RRM_PNULL;
                p_rrm_event_mme_config_transfer = (rrm_event_mme_config_transfer_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_mme_config_transfer->msg_header));
            }
            break;

        case RRM_PE_ENB_CONFIGURATION_TRANSFER:
            {
                rrm_event_enb_config_transfer_t *p_rrm_event_enb_config_transfer = RRM_PNULL;
                p_rrm_event_enb_config_transfer = (rrm_event_enb_config_transfer_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_enb_config_transfer->msg_header));
                /*RRM_MEMCPY(p_rrmuem_rmif_event_notification_ind->buffer,
                  (U8*)p_rrm_event_enb_config_transfer,
                  sizeof(rrm_event_enb_config_transfer_t)); */
            }
            break;

        case RRM_PE_SRB1_SETUP_REJECT:
            {
                rrm_event_srb1_setup_reject_t *p_rrm_event_srb1_setup_reject = RRM_PNULL;
                p_rrm_event_srb1_setup_reject = (rrm_event_srb1_setup_reject_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_srb1_setup_reject->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_srb1_setup_reject(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_srb1_setup_reject);
            }
            break;

        case RRM_PE_ERAB_SETUP_QCI:
            {
                rrm_event_erab_setup_qci_t *p_rrm_event_erab_setup_qci = RRM_PNULL;
                p_rrm_event_erab_setup_qci = (rrm_event_erab_setup_qci_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_erab_setup_qci->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_setup_qci(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_erab_setup_qci);
            }
            break;

        case RRM_PE_EVENT_ERAB_RELEASE_INFO:
            {
                rrm_event_local_erab_release_info_t *p_erab_release_info = RRM_PNULL;
                p_erab_release_info =  (rrm_event_local_erab_release_info_t *)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_erab_release_info->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_release_info(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_erab_release_info);
            }
            break;

        case RRM_PE_EVENT_ERAB_SETUP_INFO:
            {
                rrm_event_local_erab_setup_info_t *p_erab_setup_info = RRM_PNULL;
                p_erab_setup_info = (rrm_event_local_erab_setup_info_t *)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_erab_setup_info->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_erab_setup_info(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_erab_setup_info);
            }
            break;

        case RRM_PE_EVENT_HO_IN_PREP_ATTEMPT:
            {
                rrm_event_local_ho_in_prep_attempt_t *p_ho_in_prep_attempt = RRM_PNULL;
                p_ho_in_prep_attempt = (rrm_event_local_ho_in_prep_attempt_t *)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_ho_in_prep_attempt->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_in_prep_attempt(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_ho_in_prep_attempt);
            }
            break;

        case RRM_PE_EVENT_HO_IN_PREP_FAILURE:
            {
                rrm_event_local_ho_in_prep_failure_t *p_ho_in_prep_failure = RRM_PNULL;
                p_ho_in_prep_failure = (rrm_event_local_ho_in_prep_failure_t *)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_ho_in_prep_failure->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_in_prep_failure(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_ho_in_prep_failure);
            }
            break;
        case RRM_PE_HO_OUT_PREP_ATTEMPT:
            {
                rrm_event_ho_out_prep_attempt_t *p_rrm_event_ho_out_prep_attempt = RRM_PNULL;
                p_rrm_event_ho_out_prep_attempt = (rrm_event_ho_out_prep_attempt_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_out_prep_attempt->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_out_prep_attempt(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ho_out_prep_attempt);
            }
            break;

        case RRM_PE_HO_OUT_PREP_FAILED:
            {
                rrm_event_ho_out_prep_failed_t *p_rrm_event_ho_out_prep_failed = RRM_PNULL;
                p_rrm_event_ho_out_prep_failed = (rrm_event_ho_out_prep_failed_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_out_prep_failed->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_out_prep_failed(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ho_out_prep_failed);
            }
            break;

        case RRM_PE_HO_OUT_PREP_INFO:
            {
                rrm_event_ho_out_prep_info_t *p_rrm_event_ho_out_prep_info = RRM_PNULL;
                p_rrm_event_ho_out_prep_info = (rrm_event_ho_out_prep_info_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_out_prep_info->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_out_prep_info(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ho_out_prep_info);
            }
            break;

        case RRM_PE_HO_OUT_EXEC_ATTEMPT:
            {
                rrm_event_ho_out_exec_attempt_t *p_rrm_event_ho_out_exec_attempt = RRM_PNULL;
                p_rrm_event_ho_out_exec_attempt = (rrm_event_ho_out_exec_attempt_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_out_exec_attempt->msg_header));

                /* 
                   p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                //Using memcopy,compose function will be provided later
                p_rrm_event_ho_out_exec_attempt = p_rrm_event_ho_out_exec_attempt + sizeof(rrm_event_header_t);
                RRM_MEMCPY(p_rrmuem_rmif_event_notification_ind->buffer,p_rrm_event_ho_out_exec_attempt,
                (sizeof(rrm_event_ho_out_exec_attempt_t) - sizeof(rrm_event_header_t)));*/
            }
            break;

        case RRM_PE_HO_OUT_EXEC_FAILED:
            {
                rrm_event_ho_out_exec_failed_t *p_rrm_event_ho_out_exec_failed = RRM_PNULL;
                p_rrm_event_ho_out_exec_failed = (rrm_event_ho_out_exec_failed_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_out_exec_failed->msg_header));

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_out_exec_failed(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ho_out_exec_failed);
            }
            break;

        case RRM_PE_HO_OUT_EXEC_COMPLETE:
            {
                rrm_event_ho_out_exec_complete_t *p_rrm_event_ho_out_exec_complete = RRM_PNULL;
                p_rrm_event_ho_out_exec_complete = (rrm_event_ho_out_exec_complete_t*)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_out_exec_complete->msg_header));
                /*
                   p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                //Using memcopy,compose function will be provided later
                p_rrm_event_ho_out_exec_complete = p_rrm_event_ho_out_exec_complete + sizeof(rrm_event_header_t);
                RRM_MEMCPY(p_rrmuem_rmif_event_notification_ind->buffer, p_rrm_event_ho_out_exec_complete,
                (sizeof(rrm_event_ho_out_exec_complete_t) - sizeof(rrm_event_header_t)));*/
            }
            break;

        case RRM_PE_HO_IN_EXEC_COMPLETE:
            {
                rrm_event_local_ho_in_exec_complete_t  *p_ho_in_exec_comp = RRM_PNULL;
                p_ho_in_exec_comp = (rrm_event_local_ho_in_exec_complete_t *)event_buff;       

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_ho_in_exec_comp->msg_header)); 
            }
            break;

        case RRM_PE_HO_IN_PREP_COMPLETE:
            {
                rrm_event_local_ho_in_prep_complete_t  *p_ho_in_prep_comp = RRM_PNULL;
                p_ho_in_prep_comp = (rrm_event_local_ho_in_prep_complete_t *)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_ho_in_prep_comp->msg_header));
                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
            }
            break;

        case RRM_PE_HO_IN_EXEC_ATTEMPT:
            {
                rrm_event_local_ho_in_exec_attempt_t  *p_ho_in_exec_attempt = RRM_PNULL;
                p_ho_in_exec_attempt = (rrm_event_local_ho_in_exec_attempt_t *)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_ho_in_exec_attempt->msg_header));
                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
            }
            break;

        case RRM_PE_HO_IN_EXEC_FAILED:
            {
                rrm_event_local_ho_in_exec_failed_t  *p_ho_in_exec_fail = RRM_NULL;
                p_ho_in_exec_fail = (rrm_event_local_ho_in_exec_failed_t *)event_buff;

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_ho_in_exec_fail->msg_header));
                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
            } 
            break;


        case RRM_PE_EVENT_HO_UE_DETECTED:
            {
                rrm_event_ho_ue_detected_t  *p_rrm_event_ho_ue_detected = 
                    (rrm_event_ho_ue_detected_t *)(event_buff);

                fill_rrmuem_rmif_event_notification_header(&(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_ue_detected->msg_header) );
            }
            break;

        case RRM_PE_HO_OUT_EXEC_INFO:
            {
                rrm_event_ho_out_exec_info_t    *p_rrm_event_ho_out_exec_info = 
                    (rrm_event_ho_out_exec_info_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_out_exec_info->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_out_exec_info(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ho_out_exec_info);
            }
            break;
            /* List of supported events for Release 1.3 End */

        case RRM_PE_EVENT_TOO_EARLY_HO:
            {
                rrm_event_too_early_ho_t    *p_rrm_event_too_early_ho =
                    (rrm_event_too_early_ho_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_too_early_ho->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_too_early_ho(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_too_early_ho);
            }
            break;

        case RRM_PE_EVENT_TOO_LATE_HO:
            {
                rrm_event_too_late_ho_t    *p_rrm_event_too_late_ho =
                    (rrm_event_too_late_ho_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_too_late_ho->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_too_late_ho(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_too_late_ho);
            }
            break;

        case RRM_PE_EVENT_HO_WRONG_CELL:
            {
                rrm_event_ho_wrong_cell_t    *p_rrm_event_ho_wrong_cell =
                    (rrm_event_ho_wrong_cell_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_wrong_cell->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_wrong_cell(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ho_wrong_cell);
            }
            break;

        case RRM_PE_EVENT_HO_WRONG_CELL_REEST:
            {
                rrm_event_ho_wrong_cell_reest_t    *p_rrm_event_ho_wrong_cell_reest =
                    (rrm_event_ho_wrong_cell_reest_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_ho_wrong_cell_reest->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_ho_wrong_cell_reest(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_ho_wrong_cell_reest);
            }
            break;

        case RRM_PE_EVENT_X2_RLF_INDICATION:
            {
                rrm_event_x2_rlf_indication_t    *p_rrm_event_x2_rlf_indication =
                    (rrm_event_x2_rlf_indication_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_x2_rlf_indication->msg_header) );
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_EUTRA:
            {
                rrm_event_meas_config_periodical_eutran_t    *p_rrm_event_meas_config_periodical_eutran =
                    (rrm_event_meas_config_periodical_eutran_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_periodical_eutran->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_periodical_eutran(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_periodical_eutran);
            }
            /*+ Coverity CID : 28808 +*/
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_B2_CDMA2000:
            {
                /*+ Coverity CID : 28810 +*/
                ret_val = RRM_FAILURE;
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_GERAN:
            {
                rrm_event_meas_config_periodical_geran_t    *p_rrm_event_meas_config_periodical_geran =
                    (rrm_event_meas_config_periodical_geran_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_periodical_geran->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_periodical_geran(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_periodical_geran);
            }
            /*+ Coverity CID : 28809 +*/
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_UTRA:
            {
                rrm_event_meas_config_periodical_utra_t    *p_rrm_event_meas_config_periodical_utra =
                    (rrm_event_meas_config_periodical_utra_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_periodical_utra->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_periodical_utra(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_periodical_utra);
            }
            break;

        case RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_CDMA:
            {
                rrm_event_meas_config_periodical_cdma_t    *p_rrm_event_meas_config_periodical_cdma =
                    (rrm_event_meas_config_periodical_cdma_t *)(event_buff);
                fill_rrmuem_rmif_event_notification_header( &(p_rrmuem_rmif_event_notification_ind->header),
                        &(p_rrm_event_meas_config_periodical_cdma->msg_header) );

                p_rrmuem_rmif_event_notification_ind->bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
                rrm_compose_local_event_meas_config_periodical_cdma(p_rrmuem_rmif_event_notification_ind->buffer,
                        p_rrm_event_meas_config_periodical_cdma);
            }
            break;
            default:
            {
                ret_val = RRM_FAILURE;
      RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                        "Invalid Event Id [%d] Received", event_id);  
            }
            break; 
    }/*switch ends here*/


    /* Sending to the MIF module*/ 
    if (RRM_SUCCESS == ret_val) 
    {
        rrm_enable_disable_protocol_event((void*)p_rrmuem_rmif_event_notification_ind, 
                api_len, 
                event_id);
    }
    else
    {
    /*SPR 17777 +-*/    
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "p_rrmuem_rmif_event_notification_ind memory allocation failed;Unused variable src_id=%u",src_id);
    /*SPR 17777 +-*/
    }




    rrm_msg_mem_free(p_rrmuem_rmif_event_notification_ind);
    RRM_UT_TRACE_EXIT();
}

/**************************  Event-Handler  ***************************/

/***********************************************************************
 * Function Name  : rrm_raise_event_ue_cap_fgi_reject 
 * Inputs         : p_rrm_ue_context
 * Returns        : void
 * Description    : This function raise UE_CAPABILITY_FGI_REJECT event.
 **********************************************************************/
void rrm_raise_event_ue_cap_fgi_reject(void)
    /*SPR 17777 +-*/    
{
    rrm_event_local_ue_capability_fgi_reject_t   *p_event_ue_capability_fgi_reject = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_event_ue_capability_fgi_reject = (rrm_event_local_ue_capability_fgi_reject_t *) 
        rrm_mem_get(sizeof(rrm_event_local_ue_capability_fgi_reject_t));
  /*coverity 21435, rel2.0.1, */
    if(RRM_PNULL != p_event_ue_capability_fgi_reject)
    {
        RRM_MEMSET(p_event_ue_capability_fgi_reject,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_ue_capability_fgi_reject_t));
        rrm_fill_event_notification_header(&(p_event_ue_capability_fgi_reject->msg_header),
                RRM_PE_EVENT_UE_CAPABILITY_FGI_REJECT);

        /* Populating fgi_bits value from ue-ctxt */

        rrm_process_event_handler((void *)p_event_ue_capability_fgi_reject,
                RRM_PE_EVENT_UE_CAPABILITY_FGI_REJECT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_ue_capability_fgi_reject);
    }
    else
    {
         RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_ue_capability_fgi_reject failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_adm_rej 
 * Inputs         : erab_id,reject_result,cell_index
 * Returns        : void
 * Description    : This function raise ERAB_ADMISSION_REJECT event.
 **********************************************************************/
void rrm_raise_event_erab_adm_rej(U8                          erab_id,
        rrm_erab_adm_rej_result_et  reject_result)
    /*SPR 17777 +-*/    
{
    rrm_event_local_erab_admission_reject_t *p_erab_admission_rej = RRM_PNULL;

    RRM_UT_TRACE_ENTER(); 
  /*coverity 21417, rel2.0.1, */
    p_erab_admission_rej = (rrm_event_local_erab_admission_reject_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_admission_reject_t)); 

    if(RRM_PNULL != p_erab_admission_rej)
    {
        RRM_MEMSET(p_erab_admission_rej,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_erab_admission_reject_t));
        p_erab_admission_rej->erab_id = erab_id;
        p_erab_admission_rej->erab_setup_result = reject_result;

        rrm_fill_event_notification_header(&(p_erab_admission_rej->msg_header),
                RRM_PE_ERAB_ADMISSION_REJECT);

        rrm_process_event_handler((void *)p_erab_admission_rej,
                RRM_PE_ERAB_ADMISSION_REJECT,
                RRM_UEM_MODULE_ID);

        RRM_MEM_FREE(p_erab_admission_rej);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_erab_admission_rej failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_rrc_setup_ready
 * Inputs         : cell_index
 * Returns        : void
 * Description    : This function raise RRC_CONN_SETUP_READY event.
 **********************************************************************/
void rrm_raise_event_rrc_setup_ready(void)
    /*SPR 17777 +-*/    
{
    rrm_event_local_rrc_setup_ready_t  *p_event_local_rrc_setup_ready = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_event_local_rrc_setup_ready = (rrm_event_local_rrc_setup_ready_t *)
        rrm_mem_get(sizeof(rrm_event_local_rrc_setup_ready_t));

  /*coverity 21433, rel2.0.1, */
    if(RRM_PNULL != p_event_local_rrc_setup_ready)
    {
        RRM_MEMSET(p_event_local_rrc_setup_ready,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_rrc_setup_ready_t));
        rrm_fill_event_notification_header(&(p_event_local_rrc_setup_ready->msg_header),
                RRM_PE_RRC_CONN_SETUP_READY);
        rrm_process_event_handler((void *)p_event_local_rrc_setup_ready,
                RRM_PE_RRC_CONN_SETUP_READY,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_local_rrc_setup_ready);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_local_rrc_setup_ready failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_setup_attempt
 * Inputs         : cell_index,p_erb_setup_req
 * Returns        : void
 * Description    : This function raise ERAB_SETUP_ATTEMP event.
 **********************************************************************/
void rrm_raise_event_erab_setup_attempt(
    /*SPR 17777 +-*/    
        rrc_rrm_erb_setup_req_t* p_erb_setup_req)
{
    rrm_event_local_erab_setup_attempt_t  *p_event_local_erab_setup_attempt = RRM_NULL;
    U8    index = RRM_NULL;
    U64   p_accumulated_ul_req_gbr = RRM_NULL;
    U64   p_accumulated_dl_req_gbr = RRM_NULL;
    RRM_UT_TRACE_ENTER();

    p_event_local_erab_setup_attempt = (rrm_event_local_erab_setup_attempt_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_setup_attempt_t));
  /*coverity 21424, rel2.0.1, */
    if(RRM_PNULL != p_event_local_erab_setup_attempt)
    {
        RRM_MEMSET(p_event_local_erab_setup_attempt,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_erab_setup_attempt_t));
        for(index=RRM_NULL;index<p_erb_setup_req->erab_to_be_setup_item_list.num_of_list;index++ )
        {
            p_event_local_erab_setup_attempt->requested_qci[p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_id] = p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_level_qos_params.qci;

            p_event_local_erab_setup_attempt->priority_level[p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_id] = p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_level_qos_params.alloc_and_reten_prior.priority_level;

            p_event_local_erab_setup_attempt->preemption_capability[p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_id] = p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_level_qos_params.alloc_and_reten_prior.preemption_capability;

            p_event_local_erab_setup_attempt->preemption_vulnerability[p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_id] = p_erb_setup_req->erab_to_be_setup_item_list.
                erab_to_be_setup_item[index].erab_level_qos_params.alloc_and_reten_prior.preemption_vulnerability;

            if(p_erb_setup_req->\
                    erab_to_be_setup_item_list.erab_to_be_setup_item[index].
                    erab_level_qos_params.bitmask & ERAB_LEVEL_QOS_PARAM_GBR_QOS_INFO_PRESENT)
            {
                p_accumulated_ul_req_gbr = p_accumulated_ul_req_gbr + p_erb_setup_req->\
                                           erab_to_be_setup_item_list.erab_to_be_setup_item[index].
                                           erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul;

                p_accumulated_dl_req_gbr = p_accumulated_dl_req_gbr +  p_erb_setup_req->\
                                           erab_to_be_setup_item_list.erab_to_be_setup_item[index].
                                           erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl;

                p_event_local_erab_setup_attempt->ul_admitted_gbr = p_accumulated_ul_req_gbr;
                p_event_local_erab_setup_attempt->dl_admitted_gbr = p_accumulated_dl_req_gbr;
            }
        } 

        rrm_fill_event_notification_header(&(p_event_local_erab_setup_attempt->msg_header),
                RRM_PE_ERAB_SETUP_ATTEMPT);

        rrm_process_event_handler((void *)p_event_local_erab_setup_attempt,
                RRM_PE_ERAB_SETUP_ATTEMPT,
                RRM_UEM_MODULE_ID);

        RRM_MEM_FREE(p_event_local_erab_setup_attempt);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_local_erab_setup_attempt failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_modify_attempt
 * Inputs         : cell_index,rrc_ue_erb_modify_req
 * Returns        : void
 * Description    : This function raise ERAB_MODIFY_ATTEMPT event.
 **********************************************************************/
void rrm_raise_event_erab_modify_attempt(
    /*SPR 17777 +-*/    
        rrc_rrm_erab_modify_req_t *rrc_ue_erb_modify_req)
{
    rrm_event_local_erab_modify_attempt_t  *p_event_local_erab_mod = RRM_PNULL;
    U8      index = RRM_NULL;
    RRM_UT_TRACE_ENTER();

    p_event_local_erab_mod = (rrm_event_local_erab_modify_attempt_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_modify_attempt_t));

  /*coverity 21421, rel2.0.1, */
    if(RRM_PNULL != p_event_local_erab_mod)
    {
        RRM_MEMSET( p_event_local_erab_mod,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_erab_modify_attempt_t));
        for(index = RRM_ZERO; index < rrc_ue_erb_modify_req->erab_to_be_modified_item_list.num_of_list; index++)
        {
            p_event_local_erab_mod->rqstd_qci[rrc_ue_erb_modify_req->erab_to_be_modified_item_list.
                erab_to_be_setup_item[index].erab_id] =  rrc_ue_erb_modify_req->erab_to_be_modified_item_list.
                erab_to_be_setup_item[index].erab_level_qos_params.qci;
        } 


        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_event_local_erab_mod->msg_header),
                RRM_PE_ERAB_MODIFY_ATTEMPT);

        rrm_process_event_handler((void *)p_event_local_erab_mod,
                RRM_PE_ERAB_MODIFY_ATTEMPT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_local_erab_mod);
    }
    else
    {

 RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_local_erab_mod failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_setup_ready
 * Inputs         : p_ue_context
 * Returns        : void
 * Description    : This function raise ERAB_SETUP_READY event.
 **********************************************************************/
void rrm_raise_event_erab_setup_ready(rrm_ue_context_t  *p_ue_context)
{
    rrm_event_local_erab_setup_ready_t  *p_event_local_erab_setup_ready = RRM_PNULL;
    U64     ul_total_admitted_gbr = RRM_NULL;
    U64     dl_total_admitted_gbr = RRM_NULL;
    U16     drb_configured_index = RRM_NULL;
    rrm_ue_erab_failed_item_list_t  *p_failed_erbs = RRM_PNULL;
    U16                              erb_failed_count = RRM_NULL;
    rrm_ue_erab_setup_item_list_t   *p_successful_erbs = RRM_PNULL;
    U16                              erb_count = RRM_NULL;


    RRM_UT_TRACE_ENTER();

    p_event_local_erab_setup_ready = (rrm_event_local_erab_setup_ready_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_setup_ready_t));

  /*coverity 21426, rel2.0.1, */
    if(RRM_PNULL != p_event_local_erab_setup_ready)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_event_local_erab_setup_ready,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_erab_setup_ready_t));

        //calculating total admitted ul/dl gbr
        for(drb_configured_index = RRM_NULL; drb_configured_index<p_ue_context->drb_configured.num_of_list;
                drb_configured_index ++)
        {
            dl_total_admitted_gbr += p_ue_context->drb_configured.erab_item[drb_configured_index].
                erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl;

            ul_total_admitted_gbr += p_ue_context->drb_configured.erab_item[drb_configured_index].
                erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul;
        }

        //Filling total admitted ul gbr
        p_event_local_erab_setup_ready->ul_admitted_gbr = ul_total_admitted_gbr;

        //Filling total admitted dl gbr
        p_event_local_erab_setup_ready->dl_admitted_gbr = dl_total_admitted_gbr;

        //Filling result for successfully modified erabs
        /* SPR 10751 Fix Start */
        if (RRM_PNULL != p_ue_context->current_procedure.p_erb_success_list)
        {
            p_successful_erbs = (rrm_ue_erab_setup_item_list_t*)
                (p_ue_context->current_procedure.p_erb_success_list);
            /* COVERITY : : CID : 55330 fix start */
            for(erb_count = RRM_ZERO ;((erb_count < p_successful_erbs->num_of_list) && (erb_count < RRM_MAX_NUM_DRB_PER_UE) && (p_successful_erbs->erab_item[erb_count].erab_id  < MAX_ERAB_ID));erb_count++)
            {
                p_event_local_erab_setup_ready->erab_setup_result[p_successful_erbs->erab_item[erb_count].erab_id] = RRM_ONE;
            }
            /* COVERITY : RESOURCE LEAK : CID : 55330 fix end */
        }

        //Filling result for erb modify failed
        if (RRM_PNULL != p_ue_context->current_procedure.p_erb_failed_list)
        {
            p_failed_erbs  = (rrm_ue_erab_failed_item_list_t*)
                (p_ue_context->current_procedure.p_erb_failed_list);
            for (erb_failed_count = RRM_ZERO; erb_failed_count < p_failed_erbs->num_of_list;erb_failed_count++)
            {
                p_event_local_erab_setup_ready->erab_setup_result[p_failed_erbs->erab_to_be_release_item[erb_failed_count].erab_id] = RRM_ZERO;
            }
        }
        /* SPR 10751 Fix End */

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_event_local_erab_setup_ready->msg_header),
                RRM_PE_ERAB_SETUP_READY);

        rrm_process_event_handler((void *)p_event_local_erab_setup_ready,
                RRM_PE_ERAB_SETUP_READY,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_local_erab_setup_ready);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_local_erab_setup_ready failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_modify_ready
 * Inputs         : p_rrm_ue_context
 * Returns        : void
 * Description    : This function raise ERAB_MODIFY_READY event.
 **********************************************************************/
void rrm_raise_event_erab_modify_ready(rrm_ue_context_t *p_rrm_ue_context)
{
    rrm_event_erab_modify_ready_t   *p_event_erab_modify_ready = RRM_PNULL;
    rrm_ue_erab_failed_item_list_t  *p_failed_erbs = RRM_PNULL;
    U16                              erb_failed_count = RRM_NULL;
    rrm_ue_erab_setup_item_list_t   *p_successful_erbs = RRM_PNULL;
    U16                              erb_count = RRM_NULL;

    RRM_UT_TRACE_ENTER();
    p_event_erab_modify_ready = (rrm_event_erab_modify_ready_t *)
        rrm_mem_get(sizeof(rrm_event_erab_modify_ready_t));
  /*coverity 21422, rel2.0.1, */  
    if(RRM_PNULL != p_event_erab_modify_ready)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET( p_event_erab_modify_ready,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_erab_modify_ready_t));

        //Filling result for successfully modified erabs
        p_successful_erbs = (rrm_ue_erab_setup_item_list_t*)
            (p_rrm_ue_context->current_procedure.p_erb_success_list); 
  /* Coverity_ID_Fix_55328 */
  /* Coverity_ID_Fix_55328 */
  for(erb_count = RRM_ZERO ; ((erb_count < p_successful_erbs->num_of_list) && (erb_count < RRM_MAX_NUM_DRB_PER_UE)  && (p_successful_erbs->erab_item[erb_count].erab_id < MAX_ERAB_ID) )
                        ;erb_count++)
        {
            p_event_erab_modify_ready->modify_result[p_successful_erbs->erab_item[erb_count].erab_id] = RRM_ONE;
        }

        //Filling result for erb modify failed
        p_failed_erbs  = (rrm_ue_erab_failed_item_list_t*)
            (p_rrm_ue_context->current_procedure.p_erb_failed_list);
        for (erb_failed_count = RRM_ZERO; erb_failed_count < p_failed_erbs->num_of_list;erb_failed_count++)
        {
            p_event_erab_modify_ready->modify_result[p_failed_erbs->erab_to_be_release_item[erb_failed_count].erab_id] = RRM_ZERO;
        }


        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_event_erab_modify_ready->msg_header),
                RRM_PE_ERAB_MODIFY_READY);

        rrm_process_event_handler((void *)p_event_erab_modify_ready,
                RRM_PE_ERAB_MODIFY_READY,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_erab_modify_ready);
    }
    else
    {
  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_erab_modify_ready  failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_drb_release_attempt
 * Inputs         : cell_index,rrc_ue_erb_release_req
 * Returns        : void
 * Description    : This function raise ERAB_RELEASE_ATTEMPT event.
 **********************************************************************/
void rrm_raise_event_erab_drb_release_attempt(
    /*SPR 17777 +-*/    
        rrc_rrm_erb_release_req_t  *rrc_ue_erb_release_req)
{
    rrm_event_local_erab_release_attempt_t  *p_event_erab_rel_attempt = RRM_PNULL;
    U8  index = RRM_NULL;

    RRM_UT_TRACE_ENTER();

    p_event_erab_rel_attempt = (rrm_event_local_erab_release_attempt_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_release_attempt_t));
  /*coverity 21420, rel2.0.1, */
    if(RRM_PNULL != p_event_erab_rel_attempt)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_event_erab_rel_attempt,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_erab_release_attempt_t));

        for (index = RRM_NULL; index<rrc_ue_erb_release_req->erab_to_be_released_item_list.count; index++)
        {
            //erab id's to be released present in request
            p_event_erab_rel_attempt->erab_id_to_release[rrc_ue_erb_release_req->erab_to_be_released_item_list.
                erab_to_be_release_item[index].erab_id] = RRM_ONE;

            //Cause group for erab to be release
            p_event_erab_rel_attempt->cause_group[rrc_ue_erb_release_req->erab_to_be_released_item_list.
                erab_to_be_release_item[index].erab_id] = rrc_ue_erb_release_req->erab_to_be_released_item_list.
                erab_to_be_release_item[index].cause.type;

            //Cause value for erab to be release
            p_event_erab_rel_attempt->cause_value[rrc_ue_erb_release_req->erab_to_be_released_item_list.
                erab_to_be_release_item[index].erab_id] = rrc_ue_erb_release_req->erab_to_be_released_item_list.
                erab_to_be_release_item[index].cause.value;
        }

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_event_erab_rel_attempt->msg_header),
                RRM_PE_ERAB_RELEASE_ATTEMPT);

        rrm_process_event_handler((void *)p_event_erab_rel_attempt,
                RRM_PE_ERAB_RELEASE_ATTEMPT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_erab_rel_attempt);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_erab_rel_attempt failed");
    }

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_release_ready
 * Inputs         : p_rrm_ue_context
 * Returns        : void
 * Description    : This function raise ERAB_RELEASE_READY event.
 **********************************************************************/
void rrm_raise_event_erab_release_ready(rrm_ue_context_t *p_rrm_ue_context)
{

    rrm_event_local_erab_release_ready_t  *p_event_erab_rel_ready = RRM_PNULL;
    rrm_ue_erab_failed_item_list_t        *p_failed_erbs = RRM_PNULL;
    U16                                    drb_failed_count = RRM_NULL;
    rrm_ue_erab_setup_item_list_t         *p_successful_erbs = RRM_PNULL;
    U16                                    drb_count = RRM_NULL;

    RRM_UT_TRACE_ENTER();

    p_event_erab_rel_ready = (rrm_event_local_erab_release_ready_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_release_ready_t));
  /*coverity 21423, rel2.0.1, */
    if(RRM_PNULL != p_event_erab_rel_ready)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_event_erab_rel_ready,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_erab_release_ready_t)); 

        //Filling result for successfully released erbs
        p_successful_erbs = (rrm_ue_erab_setup_item_list_t*)
            (p_rrm_ue_context->current_procedure.p_erb_success_list); 
  /* Coverity_ID_Fix_55329 */
  /* Coverity_ID_Fix_55329 */
  for(drb_count = RRM_ZERO ; ((drb_count < p_successful_erbs->num_of_list) && (drb_count < RRM_MAX_NUM_DRB_PER_UE) &&  (p_successful_erbs->erab_item[drb_count].erab_id < MAX_ERAB_ID))
        ;drb_count++)
        {
            p_event_erab_rel_ready->succ_erab_id[p_successful_erbs->erab_item[drb_count].erab_id] = RRM_ONE;
        }

        //Filling result for erb release failed
        p_failed_erbs  = (rrm_ue_erab_failed_item_list_t*)
            (p_rrm_ue_context->current_procedure.p_erb_failed_list);  
        for (drb_failed_count = RRM_ZERO; drb_failed_count < p_failed_erbs->num_of_list;drb_failed_count++)
        {
            p_event_erab_rel_ready->succ_erab_id[p_failed_erbs->erab_to_be_release_item[drb_failed_count].erab_id] = RRM_ZERO;
        } 

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_event_erab_rel_ready->msg_header),
                RRM_PE_ERAB_RELEASE_READY);

        rrm_process_event_handler((void *)p_event_erab_rel_ready,
                RRM_PE_ERAB_RELEASE_READY,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_erab_rel_ready);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_erab_rel_ready failed");
    }

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_drb_release
 * Inputs         : cell_index,rrc_ue_erb_release_cnf
 * Returns        : void
 * Description    : This function raise ERAB_DRB_RELEASE event.
 **********************************************************************/
void rrm_raise_event_erab_drb_release(   
    /*SPR 17777 +-*/    
        rrc_rrm_erb_release_cnf_t *rrc_ue_erb_release_cnf)
{
    rrm_event_local_erab_drb_release_t  *p_event_local_erab_drb_rel = RRM_PNULL;
    U8                                   index = RRM_NULL;
    RRM_UT_TRACE_ENTER();

    p_event_local_erab_drb_rel = (rrm_event_local_erab_drb_release_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_drb_release_t));
  /*coverity 21419, rel2.0.1*/
    if(RRM_PNULL != p_event_local_erab_drb_rel)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_event_local_erab_drb_rel,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_erab_drb_release_t));

        for(index=RRM_NULL; index<rrc_ue_erb_release_cnf->erab_cnf_list.erab_count; index++)
        {
            //Filling list of drb that has been released
            p_event_local_erab_drb_rel->erab_id_released[rrc_ue_erb_release_cnf->erab_cnf_list.
                erab_cnf_info[index].erab_id] = RRM_ONE;
        }

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_event_local_erab_drb_rel->msg_header),
                RRM_PE_ERAB_DRB_RELEASE);

        rrm_process_event_handler((void *)p_event_local_erab_drb_rel,
                RRM_PE_ERAB_DRB_RELEASE,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_local_erab_drb_rel);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_local_erab_drb_rel failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ue_capability
 * Inputs         : cell_index,access_stratum_release,
 *                  ue_category,fgi_bitmap
 * Returns        : void
 * Description    : This function raise UE_CAPABILITY event.
 **********************************************************************/
void rrm_raise_event_ue_capability(   
    /*SPR 17777 +-*/    
        U8               access_stratum_release,
        U8               ue_category,
        U8               fgi_bitmap[4])
{
    rrm_event_local_event_ue_capability_t  *p_event_local_event_ue_cap = RRM_PNULL;
    U8  index = RRM_NULL;
    RRM_UT_TRACE_ENTER();

    p_event_local_event_ue_cap = (rrm_event_local_event_ue_capability_t *)
        rrm_mem_get(sizeof(rrm_event_local_event_ue_capability_t));
  /*coverity 21436, rel2.0.1, */
    if(RRM_PNULL != p_event_local_event_ue_cap)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_event_local_event_ue_cap,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_event_ue_capability_t));

        p_event_local_event_ue_cap->access_stratum_release = access_stratum_release;
        p_event_local_event_ue_cap->ue_category = ue_category;
        for(index=RRM_ZERO; index<RRM_FOUR; index++)
        {
            p_event_local_event_ue_cap->fgi_bitmap[index] = fgi_bitmap[index];
        }

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_event_local_event_ue_cap->msg_header),
                RRM_PE_EVENT_UE_CAPABILITY);

        rrm_process_event_handler((void *)p_event_local_event_ue_cap,
                RRM_PE_EVENT_UE_CAPABILITY,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_local_event_ue_cap);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_local_event_ue_cap failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_cell_setup_reject
 * Inputs         : fail_cause,cell_index
 * Returns        : void
 * Description    : This function raise 
 *                  RRM_PE_CELL_SETUP_REJECT event.
 **********************************************************************/
void rrm_raise_event_cell_setup_reject(rrm_cell_setup_fail_cause_et fail_cause)
{
    rrm_event_local_cell_setup_reject_t *p_cell_setup_rej = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_cell_setup_rej = (rrm_event_local_cell_setup_reject_t *) 
        rrm_mem_get(sizeof(rrm_event_local_cell_setup_reject_t));
  /*coverity id: 21415, rel2.0.1, */
    if(RRM_PNULL != p_cell_setup_rej)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_cell_setup_rej,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_local_cell_setup_reject_t));

        //Filling cell setup fail cause
  /* coverity id 21791 , ,rel 2.0.1 */
        p_cell_setup_rej->fail_cause = fail_cause;

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_cell_setup_rej->msg_header),
                RRM_PE_CELL_SETUP_REJECT);

        rrm_process_event_handler((void *)p_cell_setup_rej,
                RRM_PE_CELL_SETUP_REJECT,
                RRM_CM_MODULE_ID);
        RRM_MEM_FREE(p_cell_setup_rej);
    }
    else
    {
  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_cell_setup_rej failed");
    }

}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_drb_allocated
 * Inputs         : cell_index,rrc_ue_erb_setup_cnf
 * Returns        : void
 * Description    : This function raise 
 *                  RRM_PE_ERAB_DRB_ALLOCATED event.
 **********************************************************************/
void rrm_raise_event_erab_drb_allocated(   
    /*SPR 17777 +-*/    
        rrc_rrm_erb_setup_cnf_t *rrc_ue_erb_setup_cnf)
{
    rrm_event_erab_drb_allocated_t *p_erab_drb_allocated = RRM_PNULL;
    U8                              index = RRM_NULL;

    RRM_UT_TRACE_ENTER();
  /*coverity 21418: rel2.0.1, */
    p_erab_drb_allocated = (rrm_event_erab_drb_allocated_t *)
        rrm_mem_get(sizeof(rrm_event_erab_drb_allocated_t)); 
    if(RRM_PNULL != p_erab_drb_allocated)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_erab_drb_allocated,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_erab_drb_allocated_t));

        for(index=RRM_NULL;index< rrc_ue_erb_setup_cnf->erab_cnf_list.erab_count;index++)
        {
            p_erab_drb_allocated->drb_allocated[rrc_ue_erb_setup_cnf->erab_cnf_list.erab_cnf_info[index].
                erab_id] = RRM_ONE;
        }

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_erab_drb_allocated->msg_header),
                RRM_PE_ERAB_DRB_ALLOCATED);

        rrm_process_event_handler((void *) p_erab_drb_allocated, 
                RRM_PE_ERAB_DRB_ALLOCATED,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_erab_drb_allocated);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_erab_drb_allocated failed");
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_enb_config_transfer
 * Inputs         : parameter can be added in future
 * Returns        : void
 * Description    : This function process event
 LOCAL_ENB_CONFIGURATION_TRANSFER
 **********************************************************************/
/* + SPR 17439 */
void rrm_raise_event_enb_config_transfer(void)
/* - SPR 17439 */
{
    rrm_event_enb_config_transfer_t *p_rrm_event_enb_config_transfer=RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    /*coverity 21416, rel2.0.1, */
    p_rrm_event_enb_config_transfer = (rrm_event_enb_config_transfer_t *)
        rrm_mem_get(sizeof(rrm_event_enb_config_transfer_t));

    if(RRM_PNULL != p_rrm_event_enb_config_transfer)
    {
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_enb_config_transfer->msg_header),
                RRM_PE_ENB_CONFIGURATION_TRANSFER);

        rrm_process_event_handler(p_rrm_event_enb_config_transfer,
                RRM_PE_ENB_CONFIGURATION_TRANSFER,
                RRM_CM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_enb_config_transfer);
    }
    else
    {
  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_enb_config_transfer failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_mme_config_transfer
 * Inputs         : parameter can be added in future
 * Returns        : void
 * Description    : This function process event
 LOCAL_MME_CONFIGURATION_TRANSFER
 **********************************************************************/
/* + SPR 17439 */
void rrm_raise_event_mme_config_transfer(void)
/* - SPR 17439 */
{
    rrm_event_mme_config_transfer_t *p_rrm_event_mme_config_transfer=RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_mme_config_transfer = (rrm_event_mme_config_transfer_t *)
        rrm_mem_get(sizeof(rrm_event_mme_config_transfer_t));

    /*coverity 21432, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_mme_config_transfer)
    {
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_mme_config_transfer->msg_header),
                RRM_PE_MME_CONFIGURATION_TRANSFER);

        rrm_process_event_handler(p_rrm_event_mme_config_transfer,
                RRM_PE_MME_CONFIGURATION_TRANSFER,
                RRM_CM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_mme_config_transfer);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_mme_config_transfer failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_srb1_setup_reject
 * Inputs         : ho_type
 * Returns        : void
 * Description    : This function process event
 LOCAL_SRB1_SETUP_REJECT
 **********************************************************************/
void rrm_raise_event_srb1_setup_reject(U8               ho_type)
    /*SPR 17777 +-*/    
{
    rrm_event_srb1_setup_reject_t  *p_rrm_event_srb1_setup_reject=RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_srb1_setup_reject = (rrm_event_srb1_setup_reject_t *)
        rrm_mem_get(sizeof(rrm_event_srb1_setup_reject_t));
    /*coverity 21434, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_srb1_setup_reject)
    {
    /* coverity id 21790 , ,rel 2.0.1 */
        p_rrm_event_srb1_setup_reject->ho_type =(rrc_handover_type_et)ho_type;

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_srb1_setup_reject->msg_header),
                RRM_PE_SRB1_SETUP_REJECT);

        rrm_process_event_handler(p_rrm_event_srb1_setup_reject,
                RRM_PE_SRB1_SETUP_REJECT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_srb1_setup_reject);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_srb1_setup_reject failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_setup_qci
 * Inputs         : erab_id,qci
 * Returns        : void
 * Description    : This function process event
 LOCAL_ERAB_SETUP_QCI
 **********************************************************************/
void rrm_raise_event_erab_setup_qci(U8               erab_id,
        U8               qci)
    /*SPR 17777 +-*/    
{
    rrm_event_erab_setup_qci_t  *p_rrm_event_erab_setup_qci=RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    /*coverity 21425, rel2.0.1, */
    p_rrm_event_erab_setup_qci = (rrm_event_erab_setup_qci_t *)
        rrm_mem_get(sizeof(rrm_event_erab_setup_qci_t));
    if(RRM_PNULL != p_rrm_event_erab_setup_qci)
    {    
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_erab_setup_qci->msg_header),
                RRM_PE_ERAB_SETUP_QCI);
        /* Filling ERAB Id*/
        p_rrm_event_erab_setup_qci->erab_id = erab_id;
        /* Filling qci for the erab*/
        p_rrm_event_erab_setup_qci->qci = qci;

        rrm_process_event_handler((void *)p_rrm_event_erab_setup_qci,
                RRM_PE_ERAB_SETUP_QCI,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_erab_setup_qci);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_erab_setup_qci failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_fill_event_notification_header
 * Inputs         : p_event_header,msg_len,event_id,core_id,
 *                  ue_id,cell_id,msg_direction,mom_id
 * Returns        : void
 * Description    : This function will fill the event notification header
 **********************************************************************/
/* Coverity_ID : 54431 */
void rrm_fill_event_notification_header( rrm_event_header_t *p_event_header,
        rrm_event_message_id_et   event_id)
{
    RRM_UT_TRACE_ENTER();

    /*Filling Time Stamp*/
    rrm_fill_time_stamp(&(p_event_header->time_stamp));

    /*Filling event Id*/
    p_event_header->event_id = event_id;

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_fill_time_stamp
 * Inputs         : time_stamp
 * Returns        : void
 * Description    : This function will fill the timestamp
 **********************************************************************/
void rrm_fill_time_stamp(rrm_event_time_stamp_t  *time_stamp)
{
    RRM_UT_TRACE_ENTER();

    time_t  currentTimestamp ;
    struct tm  *currentTime;

    RRM_TIME(&currentTimestamp);

    /*localtime function will fill the structure currentTime*/
    currentTime = RRM_LOCALTIME(&currentTimestamp);
  /*coverity 21414, , rel2.0.1*/
    if(RRM_PNULL != currentTime)
    {
        time_stamp->year  = (RRM_TIME_STAMP_BASE_YEAR+currentTime->tm_year);
        time_stamp->month = ++currentTime->tm_mon;
        time_stamp->day   = currentTime->tm_mday ;
        time_stamp->hour  = currentTime->tm_hour ;
        time_stamp->min   = currentTime->tm_min;
        time_stamp->sec   = currentTime->tm_sec;

        /* sprintf(time_stamp,"%d.%d.%d %d:%d:%d",RRM_TIME_STAMP_BASE_YEAR+currentTime->tm_year,currentTime->tm_mon,currentTime->tm_mday,
           currentTime->tm_hour,currentTime->tm_min,currentTime->tm_sec); */
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_a1
 * Inputs         : p_meas_config
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_A1
 **********************************************************************/
void rrm_raise_event_meas_config_a1( rrm_ue_context_t *p_rrm_ue_context)
{
    rrm_event_meas_config_a1_t      *p_rrm_event_meas_config_a1=RRM_PNULL;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_a1 = (rrm_event_meas_config_a1_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_a1_t));
    /*coverity 21427, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_a1)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_a1,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_a1_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_a1->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_A1);

        p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                    (p_rrm_ue_context->cell_index);

        p_rrm_event_meas_config_a1->meas_object_id = 13; //Need to correct the hard code value in future

    /* coverity 23106, typecast to rrm_time_to_trigger_et in rel 1.3: */
        p_rrm_event_meas_config_a1->time_to_trigger = (rrm_time_to_trigger_et)p_common_params_for_eutra->time_to_trigger;
        p_rrm_event_meas_config_a1->hysteresis =  p_common_params_for_eutra->hysteresis;

    /* coverity 23105, typecast to rrm_report_quantity_et in rel 1.3: */
        p_rrm_event_meas_config_a1->report_quantity =  (rrm_report_quantity_et)p_common_params_for_eutra->report_quantity;
        p_rrm_event_meas_config_a1->max_report_cells = p_common_params_for_eutra->max_report_cells;
    /* coverity 23104, typecast to rrm_meas_report_interval_et in rel 1.3: */
        p_rrm_event_meas_config_a1->report_interval =  (rrm_meas_report_interval_et)p_common_params_for_eutra->report_interval;
    /* coverity 23103, typecast to rrm_meas_report_amount_et in rel 1.3: */
        p_rrm_event_meas_config_a1->report_amount =  (rrm_meas_report_amount_et)p_common_params_for_eutra->report_amount;


        rrm_process_event_handler(p_rrm_event_meas_config_a1,
                RRM_PE_EVENT_MEAS_CONFIG_A1,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_a1);
    }
    else
    {
      RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_a1 failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_a2
 * Inputs         : p_meas_config
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_A2
 **********************************************************************/
void rrm_raise_event_meas_config_a2( rrm_ue_context_t *p_rrm_ue_context )

{
    rrm_event_meas_config_a2_t      *p_rrm_event_meas_config_a2=RRM_PNULL;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_a2 = (rrm_event_meas_config_a2_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_a2_t));
    /*coverity 21428, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_a2)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_a2,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_a2_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_a2->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_A2);

        p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                    (p_rrm_ue_context->cell_index);

        p_rrm_event_meas_config_a2->meas_object_id = 13; //Need to correct the hard code value in future

    /* coverity 23110, typecast to rrm_time_to_trigger_et in rel 1.3: */
        p_rrm_event_meas_config_a2->time_to_trigger = (rrm_time_to_trigger_et)p_common_params_for_eutra->time_to_trigger;
        p_rrm_event_meas_config_a2->hysteresis =  p_common_params_for_eutra->hysteresis;

    /* coverity 23109, typecast to rrm_report_quantity_et in rel 1.3: */
        p_rrm_event_meas_config_a2->report_quantity =  (rrm_report_quantity_et)p_common_params_for_eutra->report_quantity;
        p_rrm_event_meas_config_a2->max_report_cells =  p_common_params_for_eutra->max_report_cells;
    /* coverity 23108, typecast to rrm_meas_report_interval_et in rel 1.3: */
        p_rrm_event_meas_config_a2->report_interval =  (rrm_meas_report_interval_et)p_common_params_for_eutra->report_interval;
    /* coverity 23107, typecast to rrm_meas_report_amount_et in rel 1.3: */
        p_rrm_event_meas_config_a2->report_amount =  (rrm_meas_report_amount_et)p_common_params_for_eutra->report_amount;

        rrm_process_event_handler(p_rrm_event_meas_config_a2,
                RRM_PE_EVENT_MEAS_CONFIG_A2,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_a2);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_a2 failed");
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_a3
 * Inputs         : p_meas_config
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_A3
 **********************************************************************/
void rrm_raise_event_meas_config_a3( rrm_ue_context_t *p_rrm_ue_context )

{
    rrm_event_meas_config_a3_t      *p_rrm_event_meas_config_a3=RRM_PNULL;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_a3 = (rrm_event_meas_config_a3_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_a3_t));
    /*coverity 21429, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_a3)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_a3,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_a3_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_a3->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_A3);

        p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                    (p_rrm_ue_context->cell_index);


        p_rrm_event_meas_config_a3->measurement_id = 17; //Need to correct the hard code value in future
        p_rrm_event_meas_config_a3->meas_object_id = 13; //Need to correct the hard code value in future

    /* coverity 23114, typecast to rrm_time_to_trigger_et in rel 1.3: */
        p_rrm_event_meas_config_a3->time_to_trigger = (rrm_time_to_trigger_et)p_common_params_for_eutra->time_to_trigger;
        p_rrm_event_meas_config_a3->hysteresis =  p_common_params_for_eutra->hysteresis;

        /* The offset value for eventA3*/
        p_rrm_event_meas_config_a3->event_a3_offset =  p_common_params_for_eutra->a3_offset;
    /* coverity 23113, typecast to rrm_report_quantity_et in rel 1.3: */
        p_rrm_event_meas_config_a3->report_quantity =  (rrm_report_quantity_et)p_common_params_for_eutra->report_quantity;
        p_rrm_event_meas_config_a3->max_report_cells =  p_common_params_for_eutra->max_report_cells;
    /* coverity 23112, typecast to rrm_meas_report_interval_et in rel 1.3: */
        p_rrm_event_meas_config_a3->report_interval =  (rrm_meas_report_interval_et)p_common_params_for_eutra->report_interval;
    /* coverity 23111, typecast to rrm_meas_report_amount_et in rel 1.3: */
        p_rrm_event_meas_config_a3->report_amount =  (rrm_meas_report_amount_et)p_common_params_for_eutra->report_amount;

        rrm_process_event_handler(p_rrm_event_meas_config_a3,
                RRM_PE_EVENT_MEAS_CONFIG_A3,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_a3);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_a3 failed");

    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_a4
 * Inputs         : p_meas_config
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_A4
 **********************************************************************/
void rrm_raise_event_meas_config_a4( rrm_ue_context_t *p_rrm_ue_context)

{
    rrm_event_meas_config_a4_t      *p_rrm_event_meas_config_a4=RRM_PNULL;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_a4 = (rrm_event_meas_config_a4_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_a4_t));
    /*coverity 21430, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_a4)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_a4,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_a4_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_a4->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_A4);

        p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                    (p_rrm_ue_context->cell_index);


        p_rrm_event_meas_config_a4->measurement_id = 18; //Need to correct the hard code value in future
        p_rrm_event_meas_config_a4->meas_object_id = 13; //Need to correct the hard code value in future

    /* coverity 23118, typecast to rrm_time_to_trigger_et in rel 1.3: */
        p_rrm_event_meas_config_a4->time_to_trigger = (rrm_time_to_trigger_et)p_common_params_for_eutra->time_to_trigger;
        p_rrm_event_meas_config_a4->hysteresis =  p_common_params_for_eutra->hysteresis;

    /* coverity 23117, typecast to rrm_report_quantity_et in rel 1.3: */
        p_rrm_event_meas_config_a4->report_quantity =  (rrm_report_quantity_et)p_common_params_for_eutra->report_quantity;
        p_rrm_event_meas_config_a4->max_report_cells =  p_common_params_for_eutra->max_report_cells;
    /* coverity 23116, typecast to rrm_meas_report_interval_et in rel 1.3: */
        p_rrm_event_meas_config_a4->report_interval =  (rrm_meas_report_interval_et)p_common_params_for_eutra->report_interval;
    /* coverity 23115, typecast to rrm_meas_report_amount_et in rel 1.3: */
        p_rrm_event_meas_config_a4->report_amount =  (rrm_meas_report_amount_et)p_common_params_for_eutra->report_amount;

        rrm_process_event_handler(p_rrm_event_meas_config_a4,
                RRM_PE_EVENT_MEAS_CONFIG_A4,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_a4);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_a4 failed");
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_a5
 * Inputs         : p_meas_config
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_A5
 **********************************************************************/
void rrm_raise_event_meas_config_a5( rrm_ue_context_t *p_rrm_ue_context )

{
    rrm_event_meas_config_a5_t      *p_rrm_event_meas_config_a5=RRM_PNULL;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_a5 = (rrm_event_meas_config_a5_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_a5_t));
    /*coverity 21431, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_a5)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_a5,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_a5_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_a5->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_A5);

        p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                    (p_rrm_ue_context->cell_index);


    /* coverity 23122, typecast to rrm_time_to_trigger_et in rel 1.3: */
        p_rrm_event_meas_config_a5->time_to_trigger = (rrm_time_to_trigger_et)p_common_params_for_eutra->time_to_trigger;
        p_rrm_event_meas_config_a5->hysteresis =  p_common_params_for_eutra->hysteresis;

    /* coverity 23121, typecast to rrm_report_quantity_et in rel 1.3: */
        p_rrm_event_meas_config_a5->report_quantity =  (rrm_report_quantity_et)p_common_params_for_eutra->report_quantity;
        p_rrm_event_meas_config_a5->max_report_cells =  p_common_params_for_eutra->max_report_cells;
    /* coverity 23120, typecast to rrm_meas_report_interval_et in rel 1.3: */
        p_rrm_event_meas_config_a5->report_interval =  (rrm_meas_report_interval_et)p_common_params_for_eutra->report_interval;
    /* coverity 23119, typecast to rrm_meas_report_amount_et in rel 1.3: */
        p_rrm_event_meas_config_a5->report_amount =  (rrm_meas_report_amount_et)p_common_params_for_eutra->report_amount;

        rrm_process_event_handler(p_rrm_event_meas_config_a5,
                RRM_PE_EVENT_MEAS_CONFIG_A5,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_a5);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_a5 failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_event_fill_erab_release_info
 * Inputs         : p_erab_rel_info, p_rrm_ue_context, p_rrm_erab_cnf_info
 * Returns        : void
 * Description    : This function fill erab release info event buffer.
 **********************************************************************/
void rrm_event_fill_erab_release_info(rrm_event_local_erab_release_info_t *p_erab_rel_info, 
        rrm_ue_context_t                    *p_rrm_ue_context, 
        rrm_ue_proc_erab_cnf_data_t         *p_rrm_erab_cnf_info)
{
    U8   erab_index = RRM_ZERO;
    U8   drb_conf_count = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    p_erab_rel_info->succful_released_erab_count = p_rrm_erab_cnf_info->erab_cnf_list.erab_count;
    for (erab_index = RRM_ZERO; erab_index < MAX_ERAB_ID; erab_index++)
    {
        for (drb_conf_count = RRM_ZERO; drb_conf_count < p_rrm_ue_context->drb_configured.num_of_list; drb_conf_count++)        
        {
            if (p_rrm_ue_context->drb_configured.erab_item[drb_conf_count].erab_id == erab_index)
            {
         RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                        "erab id [%d] matches", erab_index);

                p_erab_rel_info->requested_qci[erab_index] = p_rrm_ue_context->drb_configured.
                    erab_item[drb_conf_count].
                    erab_level_qos_params.qci;
                p_erab_rel_info->priority_level[erab_index]= p_rrm_ue_context->drb_configured.
                    erab_item[drb_conf_count].
                    erab_level_qos_params.
                    alloc_and_reten_prior.priority_level;
                p_erab_rel_info->preemption_capability[erab_index]= p_rrm_ue_context->drb_configured.
                    erab_item[drb_conf_count].erab_level_qos_params.
                    alloc_and_reten_prior.preemption_capability;
                p_erab_rel_info->preemption_vulnerability[erab_index]= p_rrm_ue_context->drb_configured.
                    erab_item[drb_conf_count].
                    erab_level_qos_params.
                    alloc_and_reten_prior.
                    preemption_vulnerability;
            }    
            else
            {
         RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                        "erab id[%d] not matches", erab_index);
                p_erab_rel_info->requested_qci[erab_index] = RRM_EVENT_U8_INVALID_VALUE;
                p_erab_rel_info->priority_level[erab_index]= RRM_EVENT_U8_INVALID_VALUE;
                p_erab_rel_info->preemption_capability[erab_index]= RRM_EVENT_U8_INVALID_VALUE;
                p_erab_rel_info->preemption_vulnerability[erab_index]= RRM_EVENT_U8_INVALID_VALUE;
            }  
        }/*inner for loop ends here*/
    }/*outer for loop ends here*/

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_release_info
 * Inputs         : p_rrm_ue_context, p_rrm_ue_proc_erab_cnf_data
 * Returns        : void
 * Description    : This function process event 
 *                  RRM_PE_EVENT_ERAB_RELEASE_INFO
 **********************************************************************/
void rrm_raise_event_erab_release_info(rrm_ue_context_t             *p_rrm_ue_context,
        rrm_ue_proc_erab_cnf_data_t  *p_rrm_ue_proc_erab_cnf_data)
{
    rrm_event_local_erab_release_info_t   *p_event_erab_release_info = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_event_erab_release_info =  (rrm_event_local_erab_release_info_t *)
        rrm_mem_get(sizeof(rrm_event_local_erab_release_info_t));
  /*coverity 23033, rel2.0.1, */
    if(RRM_PNULL != p_event_erab_release_info)
    {
        rrm_fill_event_notification_header(&p_event_erab_release_info->msg_header,
                RRM_PE_EVENT_ERAB_RELEASE_INFO);
        /* filling erab release info */
        rrm_event_fill_erab_release_info(p_event_erab_release_info, 
                p_rrm_ue_context, 
                p_rrm_ue_proc_erab_cnf_data); 

        rrm_process_event_handler((void *)p_event_erab_release_info,
                RRM_PE_EVENT_ERAB_RELEASE_INFO,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_erab_release_info);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_erab_release_info failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_fill_event_erab_setup_info
 * Inputs         : p_erab_setup_info, p_erab_setup_item_list
 * Returns        : void
 * Description    : This function fill erab setup info.
 **********************************************************************/
void rrm_fill_event_erab_setup_info(rrm_event_local_erab_setup_info_t  *p_erab_setup_info, 
        rrm_ue_erab_setup_item_list_t      *p_erab_setup_item_list)
{
    U8   erab_index = RRM_ZERO;
    U8   erab_add_count = RRM_ZERO;

    RRM_UT_TRACE_ENTER();
    for (erab_index=RRM_ZERO; erab_index<MAX_ERAB_ID; erab_index++)
    {
        for (erab_add_count=RRM_ZERO; ((erab_add_count<p_erab_setup_item_list->num_of_list)&&
                    (erab_add_count<RRM_MAX_NUM_DRB_PER_UE)); erab_add_count++)
        {
            if (p_erab_setup_item_list->erab_item[erab_add_count].erab_id == erab_index)
            {   
                p_erab_setup_info->erab_setup_result[erab_index] = RRM_EVENT_VALID_VALUE;         
            }
            else
            {
                p_erab_setup_info->erab_setup_result[erab_index] = RRM_EVENT_U8_INVALID_VALUE;
            }
        }/*inner loop ends here*/
    }/*outer loop ends here*/
    RRM_UT_TRACE_EXIT();
}
/*klock_works_changes_start*/

void rrm_fill_erb_setup_info_from_erb_success_and_failed_list
(
 rrm_ue_context_t                   *p_rrm_ue_context,
 rrm_event_local_erab_setup_info_t  *p_event_erab_setup_info
)
{
    rrm_bool_et   raise_erb_setup_info_event                    = RRM_FALSE;
     
    RRM_UT_TRACE_ENTER(); 

        /* SPR 10751 Fix Start */
        /* filling erab setup info */
        if(RRM_PNULL != p_rrm_ue_context->current_procedure.p_erb_success_list)
        {
            rrm_fill_event_erab_setup_info(p_event_erab_setup_info, (rrm_ue_erab_setup_item_list_t *)(p_rrm_ue_context->current_procedure.p_erb_success_list));

            raise_erb_setup_info_event = RRM_TRUE;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,"Event not filled with p_erb_success_list as it is null");
        }

        if(RRM_PNULL != p_rrm_ue_context->current_procedure.p_erb_failed_list)
        {
            rrm_fill_event_erab_setup_info(p_event_erab_setup_info, (rrm_ue_erab_setup_item_list_t*)
                    (p_rrm_ue_context->current_procedure.p_erb_failed_list));

            raise_erb_setup_info_event = RRM_TRUE;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,"Event not filled with p_erb_failed_list as it is null");
        }

        if(RRM_TRUE == raise_erb_setup_info_event)
        {
            /* SPR 10751 Fix Start */
            raise_erb_setup_info_event = RRM_TRUE;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"Event not filled with p_erb_success_list as it is null");
        }
        /*Bug:321 */

        if(RRM_PNULL != p_rrm_ue_context->current_procedure.p_erb_failed_list)
        {
            rrm_fill_event_erab_setup_info(p_event_erab_setup_info, (rrm_ue_erab_setup_item_list_t*)
                    (p_rrm_ue_context->current_procedure.p_erb_failed_list));

            raise_erb_setup_info_event = RRM_TRUE;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,"Event not filled with p_erb_failed_list as it is null");
        }

        if(RRM_TRUE == raise_erb_setup_info_event)
        {
            /* SPR 21251 +- */
            //RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED ,"Raise ERAB setup info Event ");
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "ERAB setup info Event NOT raised as both p_erb_success_list and p_erb_failed_list are null");
        }
        /* SPR 10751 Fix End */
    RRM_UT_TRACE_EXIT();
}
/*klock_works_changes_end*/

/***********************************************************************
 * Function Name  : rrm_raise_event_erab_setup_info
 * Inputs         : void
 * Returns        : void
 * Description    : This function process event
 *                  RRM_PE_EVENT_ERAB_SETUP_INFO
 **********************************************************************/
void rrm_raise_event_erab_setup_info(rrm_ue_context_t    *p_rrm_ue_context) 
{
    rrm_event_local_erab_setup_info_t  *p_event_erab_setup_info = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_event_erab_setup_info =  (rrm_event_local_erab_setup_info_t *)
                                rrm_mem_get(sizeof(rrm_event_local_erab_setup_info_t));
  
  /*coverity 23034, rel2.0.1, aditya*/
  if(RRM_PNULL != p_event_erab_setup_info)
  {
      rrm_fill_event_notification_header(&p_event_erab_setup_info->msg_header,
              RRM_PE_EVENT_ERAB_SETUP_INFO);
      /*klock_works_changes_start*/ 
      rrm_fill_erb_setup_info_from_erb_success_and_failed_list( p_rrm_ue_context, p_event_erab_setup_info); 
      /*klock_works_changes_end*/
        RRM_MEM_FREE(p_event_erab_setup_info);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_erab_setup_info failed");
    }
    RRM_UT_TRACE_EXIT();
}
/*Cov_fix_65160_end*/
/***********************************************************************
 * Function Name  : rrm_fill_event_ho_in_prep_attempt
 * Inputs         : p_ho_in_prep_attempt, p_ho_adm_msg
 * Returns        : void
 * Description    : This function fill ho in prep attempt event buffer
 **********************************************************************/
void rrm_fill_event_ho_in_prep_attempt(rrm_event_local_ho_in_prep_attempt_t  *p_ho_in_prep_attempt,
        rrc_rrm_ue_ho_adm_req_t               *p_ho_adm_msg)
{
    U8 erab_index = RRM_ZERO;
    U8 erab_setup_count = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
  /*coverity 23125, typecast to rrc_handover_type_et in rel 1.3:*/
    p_ho_in_prep_attempt->ho_type =  (rrc_handover_type_et)p_ho_adm_msg->ho_type;
    for(erab_index=RRM_ZERO; erab_index<MAX_ERAB_ID; erab_index++)
    {
        for (erab_setup_count=RRM_ZERO; 
                ((erab_setup_count<p_ho_adm_msg->erab_to_be_setup_item_list.num_of_list)&&
                 (erab_setup_count<MAX_ERAB_COUNT)); erab_setup_count++)
        {
            if (erab_index == p_ho_adm_msg->erab_to_be_setup_item_list.erab_to_be_setup_item[erab_setup_count].erab_id) 
            {
                p_ho_in_prep_attempt->ho_in_prep_erab[erab_index] = RRM_EVENT_VALID_VALUE;
                p_ho_in_prep_attempt->priority_level[erab_index] = p_ho_adm_msg->erab_to_be_setup_item_list.
                    erab_to_be_setup_item[erab_setup_count].erab_level_qos_params.
                    alloc_and_reten_prior.priority_level;
                p_ho_in_prep_attempt->preemption_capability[erab_index] = p_ho_adm_msg->erab_to_be_setup_item_list.
                    erab_to_be_setup_item[erab_setup_count].
                    erab_level_qos_params.alloc_and_reten_prior.preemption_capability;
                p_ho_in_prep_attempt->preemption_vulnerability[erab_index] = p_ho_adm_msg->erab_to_be_setup_item_list.
                    erab_to_be_setup_item[erab_setup_count].
                    erab_level_qos_params.alloc_and_reten_prior.preemption_vulnerability;
            }
        }/*inner loop ends here*/
    }/*outer loop ends here*/

    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_ho_in_prep_attempt
 * Inputs         : void
 * Returns        : void
 * Description    : This function process event
 *                  RRM_PE_EVENT_HO_IN_PREP_ATTEMPT
 **********************************************************************/
void rrm_raise_event_ho_in_prep_attempt(rrc_rrm_ue_ho_adm_req_t   *p_ho_adm_req_msg)
{
    rrm_event_local_ho_in_prep_attempt_t   *p_event_ho_in_prep_attempt = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_event_ho_in_prep_attempt = (rrm_event_local_ho_in_prep_attempt_t *)
        rrm_mem_get(sizeof(rrm_event_local_ho_in_prep_attempt_t));
  /*coverity 23035,rel2.0.1,*/
    if(RRM_PNULL != p_event_ho_in_prep_attempt)
    {
        rrm_fill_event_notification_header(&p_event_ho_in_prep_attempt->msg_header,
                RRM_PE_EVENT_HO_IN_PREP_ATTEMPT);

        rrm_fill_event_ho_in_prep_attempt(p_event_ho_in_prep_attempt, p_ho_adm_req_msg);

        rrm_process_event_handler((void *)p_event_ho_in_prep_attempt,
                RRM_PE_EVENT_HO_IN_PREP_ATTEMPT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_ho_in_prep_attempt);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_ho_in_prep_attempt failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_event_map_ho_src
 * Inputs         : rrm_ue_context_t
 * Returns        : U8
 * Description    : This function maps handover source.
 **********************************************************************/
U8 rrm_event_map_ho_src(rrm_ue_context_t  *p_rrm_ue_context)
{
    U8 ho_src = RRM_EVENT_U8_INVALID_VALUE;
    U8 ho_type = RRM_EVENT_U8_INVALID_VALUE;

    RRM_UT_TRACE_ENTER();

    /*SPR-650 Fix Starts*/
    ho_src = p_rrm_ue_context->ho_type;    
    /*SPR-650 Fix Ends*/
    switch(ho_src)
    {
        case HANDOVER_TYPE_INTRA_LTE_S1:
        case HANDOVER_TYPE_INTRA_LTE_X2:
            {
                ho_type = RRM_EVENT_LTE_INTRA_FREQ;
            }      
            break;

        case HANDOVER_TYPE_UTRAN_TO_LTE:
            {
                ho_type = RRM_EVENT_LTE_WCDMA;
            }
            break; 

        case HANDOVER_TYPE_GERAN_TO_LTE:
            {
                ho_type = RRM_EVENT_LTE_GERAN;
            }
            break; 

        case HANDOVER_TYPE_INTRA_CELL:
        case HANDOVER_TYPE_LTE_TO_UTRAN:
        case HANDOVER_TYPE_LTE_TO_GERAN:
        default:
            break;
    }

    RRM_UT_TRACE_EXIT();
    return ho_type;
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_in_prep_failure
 * Inputs         : void
 * Returns        : void
 * Description    : This function process event
 *                  RRM_PE_EVENT_HO_IN_PREP_FAILURE
 **********************************************************************/
void rrm_raise_event_ho_in_prep_failure(rrm_ue_context_t    *p_rrm_ue_context,
        U8                   error_cause)
{
    rrm_event_local_ho_in_prep_failure_t  *p_event_ho_in_prep_failure = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_event_ho_in_prep_failure = (rrm_event_local_ho_in_prep_failure_t *)
        rrm_mem_get(sizeof(rrm_event_local_ho_in_prep_failure_t));
  /*coverity 23036, rel2.0.1, */
    if(RRM_PNULL != p_event_ho_in_prep_failure)
    {
        rrm_fill_event_notification_header(&p_event_ho_in_prep_failure->msg_header,
                RRM_PE_EVENT_HO_IN_PREP_FAILURE);

        /* fill ho in prep failure*/
  /*coverity 23124, typecast to rrm_event_ho_src_rat_et in rel 1.3:*/
        p_event_ho_in_prep_failure->ho_src = (rrm_event_ho_src_rat_et)rrm_event_map_ho_src(p_rrm_ue_context);
  /*coverity 23123, typecast to rrm_event_ho_prep_fail_cause_et in rel 1.3:*/
        p_event_ho_in_prep_failure->ho_prep_fail_cause = (rrm_event_ho_prep_fail_cause_et)error_cause;

        rrm_process_event_handler((void *)p_event_ho_in_prep_failure,
                RRM_PE_EVENT_HO_IN_PREP_FAILURE,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_event_ho_in_prep_failure);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_event_ho_in_prep_failure failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_out_prep_attempt
 * Inputs         : 
 * Returns        : void
 * Description    : This function process event
 RRM_PE_HO_OUT_PREP_ATTEMPT
 **********************************************************************/
void rrm_raise_event_ho_out_prep_attempt(rrm_ue_context_t      *p_rrm_ue_context,
        rrc_rrm_ho_required_t *p_rrc_rrm_ho_required)
{
    rrm_event_ho_out_prep_attempt_t  *p_rrm_event_ho_out_prep_attempt=RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_out_prep_attempt = (rrm_event_ho_out_prep_attempt_t *)
        rrm_mem_get(sizeof(rrm_event_ho_out_prep_attempt_t));
    /*coverity 23040, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_out_prep_attempt)
    {
        //Filling HO target RAT type
        if( p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_LTE_TO_UTRAN )
        {    
            p_rrm_event_ho_out_prep_attempt->ho_source_target_rat = RRM_HO_UTRAN;
        }
        else if( p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_LTE_TO_GERAN )
        {
            p_rrm_event_ho_out_prep_attempt->ho_source_target_rat = RRM_HO_GERAN;
        }
        else if(( p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_LTE_X2 ||
                    p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_LTE_S1) &&
                (p_rrm_ue_context->ho_params.last_reported_meas_event == MEAS_REP_EVENT_A3 ||
                 p_rrm_ue_context->ho_params.last_reported_meas_event == MEAS_REP_EVENT_A4))
        {
            p_rrm_event_ho_out_prep_attempt->ho_source_target_rat = RRM_HO_LTE_INTRA_FREQ;
        }

        else if(( p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_LTE_X2 ||
                    p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_LTE_S1) &&
                (p_rrm_ue_context->ho_params.last_reported_meas_event == MEAS_REP_EVENT_A5 ))
        {
            p_rrm_event_ho_out_prep_attempt->ho_source_target_rat = RRM_HO_LTE_INTER_FREQ;
        }

        else if(  p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_UTRAN_TO_LTE )
        {
            p_rrm_event_ho_out_prep_attempt->ho_source_target_rat = RRM_HO_UTRAN_SOURCE;
        }
        else if( p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_CELL )
        {
        RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                    "Intra Cell handover not supported as of now for ho type [%d]",
                    HANDOVER_TYPE_INTRA_CELL);

        }

        //Filling HO type
        if( p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_LTE_TO_UTRAN ||
                p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_LTE_S1 ||
                p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_LTE_TO_GERAN )
        {
            p_rrm_event_ho_out_prep_attempt->ho_type = RRM_EVENT_VALUE_S1;
        }
        else if( p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_LTE_X2 )
        {
            p_rrm_event_ho_out_prep_attempt->ho_type = RRM_EVENT_VALUE_X2;
        }
        else if(  p_rrc_rrm_ho_required->ho_type == HANDOVER_TYPE_INTRA_CELL )
        {
            p_rrm_event_ho_out_prep_attempt->ho_type = RRM_EVENT_VALUE_INTRA_ENB;
        }

        //Filling cause of the procedure for HO Preparation
        p_rrm_event_ho_out_prep_attempt->ho_prep_attempt_cause = p_rrm_ue_context->ho_params.
            last_reported_meas_event;

        /*Filling type of selection for the handover target cell: 
          blind or measurement based*/
    /* SPR 16406 Start */
     if( p_rrm_ue_context->ho_params.eutra_known_cell_selection_list.bitmask &
                RRM_UE_BLIND_CELL_HO_TRIGGERED )
        {
            p_rrm_event_ho_out_prep_attempt->ho_target_selection_type = RRM_EVENT_VALUE_BLIND;
        }
    /* SPR 16406 End */
        else
        {
            p_rrm_event_ho_out_prep_attempt->ho_target_selection_type = RRM_EVENT_VALUE_MEAS_BASED;
        }


        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_out_prep_attempt->msg_header),
                RRM_PE_HO_OUT_PREP_ATTEMPT);                                    

        rrm_process_event_handler(p_rrm_event_ho_out_prep_attempt,
                RRM_PE_HO_OUT_PREP_ATTEMPT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_out_prep_attempt);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_out_prep_attempt failed");
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_ho_out_prep_failed
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_HO_OUT_PREP_FAILED
 **********************************************************************/
void rrm_raise_event_ho_out_prep_failed(rrm_ue_context_t *p_rrm_ue_context)
{
    rrm_event_ho_out_prep_failed_t  *p_rrm_event_ho_out_prep_failed = RRM_PNULL;
    rrc_rrm_ho_failure_t            *p_rrm_ho_failed = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_out_prep_failed = (rrm_event_ho_out_prep_failed_t *)
        rrm_mem_get(sizeof(rrm_event_ho_out_prep_failed_t));
    /*coverity 23041, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_out_prep_failed)
    {
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_out_prep_failed->msg_header),
                RRM_PE_HO_OUT_PREP_FAILED);

        p_rrm_ho_failed = (rrc_rrm_ho_failure_t*)(p_rrm_ue_context->current_procedure.p_proc_data);

        //HO preperation fail cause type 
        p_rrm_event_ho_out_prep_failed->ho_prep_fail_cause_type = p_rrm_ho_failed->rrm_cause.type;


        //HO preperation fail cause value
        p_rrm_event_ho_out_prep_failed->ho_prep_fail_cause_value = p_rrm_ho_failed->rrm_cause.value;

        /*Filling type of selection for the handover target cell:
          blind or measurement based*/
    /* SPR 16406 Start */
     if( p_rrm_ue_context->ho_params.eutra_known_cell_selection_list.bitmask &
                RRM_UE_BLIND_CELL_HO_TRIGGERED )
        {
            p_rrm_event_ho_out_prep_failed->ho_target_selection_type = RRM_EVENT_VALUE_BLIND;
        }
    /* SPR 16406 End */
        else
        {
            p_rrm_event_ho_out_prep_failed->ho_target_selection_type = RRM_EVENT_VALUE_MEAS_BASED;
        }

        rrm_process_event_handler(p_rrm_event_ho_out_prep_failed,
                RRM_PE_HO_OUT_PREP_FAILED,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_out_prep_failed);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_out_prep_failed failed");
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_ho_out_prep_info
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_HO_OUT_PREP_INFO
 **********************************************************************/
void rrm_raise_event_ho_out_prep_info(rrm_ue_context_t *p_rrm_ue_context,
        rrm_ue_proc_ho_command_request_t *p_ho_command_request)
{
    rrm_event_ho_out_prep_info_t  *p_rrm_event_ho_out_prep_info = RRM_PNULL;
    U8    count = RRM_NULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_out_prep_info = (rrm_event_ho_out_prep_info_t *)
        rrm_mem_get(sizeof(rrm_event_ho_out_prep_info_t));
    /*coverity 23042, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_out_prep_info)
    {
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_out_prep_info->msg_header),
                RRM_PE_HO_OUT_PREP_INFO);

        //Filling bitmap for requested erabs
        p_rrm_event_ho_out_prep_info->erab_req_bitmap = RRM_ZERO;
        if( p_ho_command_request->bitmask & 
                RRC_RRM_UE_HO_CMD_REQ_ERAB_SUB_TO_FORWARD_LIST_PRESENT)
        {

            //Filling number of erab requested
            p_rrm_event_ho_out_prep_info->num_drab_requested = p_ho_command_request->erab_sub_to_fwd_list.
                erab_count;

            //Filling bitmap for requested erabs
            p_rrm_event_ho_out_prep_info->erab_req_bitmap = RRM_ZERO;
            /* COVERITY : RESOURCE LEAK : CID : 55326 fix start */
            for( count=RRM_ZERO; ((count < p_rrm_event_ho_out_prep_info->num_drab_requested ) 
                        && ( count < MAX_ERAB ) ); count++ )
                /* COVERITY : RESOURCE LEAK : CID : 55326 fix end */
            {
                p_rrm_event_ho_out_prep_info->erab_req_bitmap |= ( RRM_ONE<<p_ho_command_request->erab_sub_to_fwd_list.
                        drb_sub_to_fwd[count].erab_id);
            } 
        } 

        //Filling bitmap for erab in ue context
        p_rrm_event_ho_out_prep_info->erab_bitmap = RRM_ZERO;

        for( count=RRM_ZERO; count< p_rrm_ue_context->drb_configured.num_of_list;count++ )
        {
            p_rrm_event_ho_out_prep_info->erab_bitmap |= ( RRM_ONE<<p_rrm_ue_context->drb_configured.
                    erab_item[count].erab_id );
        }

        rrm_process_event_handler(p_rrm_event_ho_out_prep_info,
                RRM_PE_HO_OUT_PREP_INFO,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_out_prep_info);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_out_prep_info failed");
    }
    RRM_UT_TRACE_EXIT();
}



/***********************************************************************
 * Function Name  : rrm_raise_event_ho_out_exec_attempt
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_HO_OUT_EXEC_ATTEMPT
 **********************************************************************/
void rrm_raise_event_ho_out_exec_attempt(void) 
    /*SPR 17777 +-*/    
{
    rrm_event_ho_out_exec_attempt_t  *p_rrm_event_ho_out_exec_attempt = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_out_exec_attempt = (rrm_event_ho_out_exec_attempt_t *)
        rrm_mem_get(sizeof(rrm_event_ho_out_exec_attempt_t));
    /*coverity 23037, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_out_exec_attempt)
    {
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_out_exec_attempt->msg_header),
                RRM_PE_HO_OUT_EXEC_ATTEMPT);

        rrm_process_event_handler(p_rrm_event_ho_out_exec_attempt,
                RRM_PE_HO_OUT_EXEC_ATTEMPT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_out_exec_attempt);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_out_exec_attempt failed");
    }
    RRM_UT_TRACE_EXIT();
}



/***********************************************************************
 * Function Name  : rrm_raise_event_ho_out_exec_failed
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_HO_OUT_EXEC_FAILED
 **********************************************************************/
void rrm_raise_event_ho_out_exec_failed(rrm_ue_context_t *p_rrm_ue_context)
{
    rrm_event_ho_out_exec_failed_t  *p_rrm_event_ho_out_exec_failed = RRM_PNULL;
    rrc_rrm_ho_failure_t            *p_rrm_ho_failed = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_out_exec_failed = (rrm_event_ho_out_exec_failed_t *)
        rrm_mem_get(sizeof(rrm_event_ho_out_exec_failed_t));
    /*coverity 23039, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_out_exec_failed)
    {
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_out_exec_failed->msg_header),
                RRM_PE_HO_OUT_EXEC_FAILED);                       

        p_rrm_ho_failed = (rrc_rrm_ho_failure_t*)(p_rrm_ue_context->current_procedure.p_proc_data);

        //HO preperation fail cause type
        p_rrm_event_ho_out_exec_failed->ho_exec_fail_cause_type = p_rrm_ho_failed->rrm_cause.type;


        //HO preperation fail cause value
        p_rrm_event_ho_out_exec_failed->ho_exec_fail_cause_value = p_rrm_ho_failed->rrm_cause.value;

        /*Filling type of selection for the handover target cell:
          blind or measurement based*/
    /* SPR 16406 Start */
     if( p_rrm_ue_context->ho_params.eutra_known_cell_selection_list.bitmask &
                RRM_UE_BLIND_CELL_HO_TRIGGERED )
        {
            p_rrm_event_ho_out_exec_failed->ho_target_selection_type = RRM_EVENT_VALUE_BLIND;
        }
    /* SPR 16406 End */
        else
        {
            p_rrm_event_ho_out_exec_failed->ho_target_selection_type = RRM_EVENT_VALUE_MEAS_BASED;
        }

        rrm_process_event_handler(p_rrm_event_ho_out_exec_failed,
                RRM_PE_HO_OUT_EXEC_FAILED,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_out_exec_failed);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_out_exec_failed failed");
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_ho_out_exec_complete
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_HO_OUT_EXEC_COMPLETE
 **********************************************************************/
void rrm_raise_event_ho_out_exec_complete(void)
    /*SPR 17777 +-*/    
{
    rrm_event_ho_out_exec_complete_t  *p_rrm_event_ho_out_exec_complete = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_out_exec_complete = (rrm_event_ho_out_exec_complete_t *)
        rrm_mem_get(sizeof(rrm_event_ho_out_exec_complete_t));
    /*coverity 23038, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_out_exec_complete)
    {
        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_out_exec_complete->msg_header),
                RRM_PE_HO_OUT_EXEC_COMPLETE);

        rrm_process_event_handler(p_rrm_event_ho_out_exec_complete,
                RRM_PE_HO_OUT_EXEC_COMPLETE,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_out_exec_complete);
    }
    else
    {
	  RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_out_exec_complete failed");
    }
    RRM_UT_TRACE_EXIT();
}

/* List of supported events for Release 1.3 Start */
/***********************************************************************
 * Function Name  : rrm_raise_event_ho_in_exec_complete
 * Inputs         : void
 * Returns        : void
 * Description    : This function process event
 *                  RRM_PE_HO_IN_EXEC_COMPLETE
 **********************************************************************/
void rrm_raise_event_ho_in_exec_complete(void)
    /*SPR 17777 +-*/    
{
    rrm_event_local_ho_in_exec_complete_t  *p_ho_in_exec_comp = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_ho_in_exec_comp = (rrm_event_local_ho_in_exec_complete_t *)rrm_mem_get(
            sizeof(rrm_event_local_ho_in_exec_complete_t));

 /*coverity 29551, rel2.0.1, */
    if(RRM_PNULL != p_ho_in_exec_comp)
    {
        rrm_fill_event_notification_header(&p_ho_in_exec_comp->msg_header,
                RRM_PE_HO_IN_EXEC_COMPLETE);
        rrm_process_event_handler((void *)p_ho_in_exec_comp,
                RRM_PE_HO_IN_EXEC_COMPLETE,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_ho_in_exec_comp);
    }
    else
    {
     RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_ho_in_exec_comp failed");


    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_fill_ho_in_prep_comp
 * Inputs         : void
 * Returns        : void
 * Description    : This function fill data for the event
 *                  RRM_PE_HO_IN_PREP_COMPLETE
 **********************************************************************/
void rrm_fill_ho_in_prep_comp(rrm_event_local_ho_in_prep_complete_t *p_ho_in_prep_comp,
        rrm_ue_context_t *p_ue_ctxt)

{
    U8 drb_count = RRM_ZERO;
    U8 erab_succ_index = RRM_ZERO;
    U16 bitmap = RRM_BIT15_MASK;

    rrm_ue_erab_setup_item_list_t  *p_erb_succ_list = RRM_NULL;
    RRM_UT_TRACE_ENTER();

    p_erb_succ_list = (rrm_ue_erab_setup_item_list_t *)(p_ue_ctxt->
            current_procedure.p_erb_success_list);

    p_ho_in_prep_comp->erab_admtd_count = p_erb_succ_list->num_of_list;

    for (drb_count=RRM_ZERO; drb_count<MAX_ERAB_ID; drb_count++)
    {
        for (erab_succ_index=RRM_ZERO; (erab_succ_index<p_erb_succ_list->num_of_list)
                /* Coverity 21oct Start : 55004 */
                && (erab_succ_index < RRM_MAX_NUM_DRB_PER_UE); erab_succ_index++)
            /* Coverity 21oct End : 55004 */
        {
            if (p_erb_succ_list->erab_item[erab_succ_index].erab_id == drb_count)
            {
                p_ho_in_prep_comp->erab_bitmap |= bitmap;
            }
        }
        bitmap = bitmap > RRM_ONE; 
    }/* for loop ends here */

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_in_prep_complete 
 * Inputs         : void
 * Returns        : void
 * Description    : This function process event 
 *                  RRM_PE_HO_IN_PREP_COMPLETE
 **********************************************************************/
void rrm_raise_event_ho_in_prep_complete(rrm_ue_context_t *p_ue_cntx)
{
    rrm_event_local_ho_in_prep_complete_t  *p_ho_in_prep_comp = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_ho_in_prep_comp = (rrm_event_local_ho_in_prep_complete_t *)rrm_mem_get(
            sizeof(rrm_event_local_ho_in_prep_complete_t));

    if(RRM_PNULL != p_ho_in_prep_comp)
    {
        /*coverity_54418 ++*/
        RRM_MEMSET(p_ho_in_prep_comp,RRM_NULL,sizeof(rrm_event_local_ho_in_prep_complete_t));
        /*coverity_54418 --*/
        p_ho_in_prep_comp->erab_bitmap = RRM_ZERO; 
        /*coverity 29553, rel2.0.1, */
        rrm_fill_event_notification_header(&p_ho_in_prep_comp->msg_header,
                RRM_PE_HO_IN_PREP_COMPLETE);

        rrm_fill_ho_in_prep_comp(p_ho_in_prep_comp, p_ue_cntx);

        rrm_process_event_handler((void *)p_ho_in_prep_comp,
                RRM_PE_HO_IN_PREP_COMPLETE,
                RRM_UEM_MODULE_ID);

        RRM_MEM_FREE(p_ho_in_prep_comp);
    }
    else
    {
     RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_ho_in_prep_comp failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_in_exec_attempt 
 * Inputs         : void
 * Returns        : void
 * Description    : This function process event
 *                  RRM_PE_HO_IN_EXEC_ATTEMPT 
 **********************************************************************/
void rrm_raise_event_ho_in_exec_attempt(rrm_ue_context_t *p_ue_cntxt)
{
    rrm_event_local_ho_in_exec_attempt_t  *p_ho_in_exec_attempt = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_ho_in_exec_attempt = (rrm_event_local_ho_in_exec_attempt_t *)rrm_mem_get(
            sizeof(rrm_event_local_ho_in_exec_attempt_t));

  /*coverity 29550, rel2.0.1, */
    if(RRM_PNULL != p_ho_in_exec_attempt)
    {
        rrm_fill_event_notification_header(&p_ho_in_exec_attempt->msg_header,
                RRM_PE_HO_IN_EXEC_ATTEMPT);
/* coverity id 29686 ,  ,rel 2.0.1 */
        /*SPR-650 Fix Starts*/
        p_ho_in_exec_attempt->ho_type = (rrc_handover_type_et)p_ue_cntxt->ho_type;    
        /*SPR-650 Fix Ends*/
        rrm_process_event_handler((void *)p_ho_in_exec_attempt,
                RRM_PE_HO_IN_EXEC_ATTEMPT,
                RRM_UEM_MODULE_ID);

        RRM_MEM_FREE(p_ho_in_exec_attempt);
    }
    else
    {
     RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_ho_in_exec_attempt failed");


    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_in_exec_failed
 * Inputs         : void
 * Returns        : void
 * Description    : This function process event
 *                  RRM_PE_HO_IN_EXEC_FAILED
 **********************************************************************/
void rrm_raise_event_ho_in_exec_failed(rrm_ue_context_t *p_ue_ctxt)
{

    rrm_event_local_ho_in_exec_failed_t  *p_ho_in_exec_fail = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_ho_in_exec_fail = (rrm_event_local_ho_in_exec_failed_t *)rrm_mem_get(
            sizeof(rrm_event_local_ho_in_exec_failed_t));

    /*coverity 29552, rel2.0.1, */
    if(RRM_PNULL != p_ho_in_exec_fail)
    {
        rrm_fill_event_notification_header(&p_ho_in_exec_fail->msg_header,
                RRM_PE_HO_IN_EXEC_FAILED);

        p_ho_in_exec_fail->fail_cause = RRM_EVENT_HO_IN_FAILED;
        p_ho_in_exec_fail->rat_type = rrm_event_map_ho_src(p_ue_ctxt);

        rrm_process_event_handler((void *)p_ho_in_exec_fail,
                RRM_PE_HO_IN_EXEC_FAILED,
                RRM_UEM_MODULE_ID);

        RRM_MEM_FREE(p_ho_in_exec_fail);
    }
    else
    {
     RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_ho_in_exec_fail failed");
    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_ho_ue_detected
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_HO_UE_DETECTED
 **********************************************************************/
    void
rrm_raise_event_ho_ue_detected(void)
    /*SPR 17777 +-*/    
{

    rrm_event_ho_ue_detected_t    rrm_event_ho_ue_detected;

    RRM_UT_TRACE_ENTER();

    rrm_fill_event_notification_header( &(rrm_event_ho_ue_detected.msg_header),
            RRM_PE_EVENT_HO_UE_DETECTED);

    /* fill the data in rrm_event_ho_ue_detected : gur30485 : */

    rrm_process_event_handler( &rrm_event_ho_ue_detected,
            RRM_PE_EVENT_HO_UE_DETECTED,
            RRM_UEM_MODULE_ID );

    RRM_UT_TRACE_EXIT();

}


    void
rrm_set_bitmask_ho_out_exec_info( U8   erab_id , U16    *bitmap)
{
    RRM_UT_TRACE_ENTER();

    switch( erab_id )
    {
        case RRM_ZERO:
            *bitmap |= RRM_BIT0_MASK; 
            break;
        case RRM_ONE:
            *bitmap |= RRM_BIT1_MASK;
            break;
        case RRM_TWO:
            *bitmap |= RRM_BIT2_MASK;
            break;
        case RRM_THREE:
            *bitmap |= RRM_BIT3_MASK;
            break;
        case RRM_FOUR:
            *bitmap |= RRM_BIT4_MASK;
            break;
        case RRM_FIVE:
            *bitmap |= RRM_BIT5_MASK;
            break;
        case RRM_SIX:
            *bitmap |= RRM_BIT6_MASK;
            break;
        case RRM_SEVEN:
            *bitmap |= RRM_BIT7_MASK;
            break;
        case RRM_EIGHT:
            *bitmap |= RRM_BIT8_MASK;
            break;
        case RRM_TEN:
            *bitmap |= RRM_BIT9_MASK;
            break;
        case RRM_ELEVEN:
            *bitmap |= RRM_BIT10_MASK;
            break;
        case RRM_TWELVE:
            *bitmap |= RRM_BIT11_MASK;
            break;
        case RRM_THIRTEEN:
            *bitmap |= RRM_BIT12_MASK;
            break;
        case RRM_FOURTEEN:
            *bitmap |= RRM_BIT13_MASK;
            break;
        case RRM_FIFTEEN:
            *bitmap |= RRM_BIT14_MASK;
            break;
    }
    RRM_UT_TRACE_EXIT();
}



void
rrm_fill_data_ho_out_exec_info( const rrm_ue_context_t *p_ue_context,
        U16                    *p_ho_prep_erab_req_bitmap,
        U16                    *p_ho_exec_erab_req_bitmap
        )
{
    U8						erab_index = RRM_ZERO;
    U8						erab_id = RRM_ZERO;
    U8						ue_erb_index = RRM_ZERO;
    const rrm_ue_ho_erab_info_list_t	        *p_erb_sub_fwd_list = RRM_PNULL;
    const rrm_ue_erab_setup_item_list_t         *p_erb_configured_list = RRM_PNULL;
    const rrm_ue_proc_ho_command_request_t      *p_rrm_proc_ho_cmd_req = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_rrm_proc_ho_cmd_req = (rrm_ue_proc_ho_command_request_t *)p_ue_context->\
                            current_procedure.p_proc_data;

    if(p_rrm_proc_ho_cmd_req->bitmask & \
            RRC_RRM_UE_HO_CMD_REQ_ERAB_SUB_TO_FORWARD_LIST_PRESENT)
    {

        /* Loss-less data transfer changes start */
        p_erb_sub_fwd_list = &(p_rrm_proc_ho_cmd_req->erab_sub_to_fwd_list);


        for(erab_index = RRM_ZERO ;erab_index < p_erb_sub_fwd_list->erab_count; erab_index++)
        {
            erab_id = p_erb_sub_fwd_list->drb_sub_to_fwd[erab_index].erab_id;
            p_erb_configured_list = &(p_ue_context->drb_configured);

            rrm_set_bitmask_ho_out_exec_info(erab_id,
                    p_ho_prep_erab_req_bitmap);

            for(ue_erb_index = RRM_ZERO ; ue_erb_index < p_erb_configured_list->num_of_list; \
                    ue_erb_index++)
            {
                if((erab_id == p_erb_configured_list->\
                            erab_item[ue_erb_index].erab_id) && (RRM_TRUE == \
                                p_erb_configured_list->erab_item[ue_erb_index].dl_fwding))
                {
                    /* set the exec erab req bitmap */
                    rrm_set_bitmask_ho_out_exec_info(erab_id,
                            p_ho_exec_erab_req_bitmap);

                } /*If erab_id matches && dl_fwding is set */
            }
        } 
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_out_exec_info
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_HO_OUT_EXEC_INFO
 **********************************************************************/
    void
rrm_raise_event_ho_out_exec_info(const rrm_ue_context_t    *p_rrm_ue_context)
{

    rrm_event_ho_out_exec_info_t    rrm_event_ho_out_exec_info;

    RRM_UT_TRACE_ENTER();

    rrm_fill_event_notification_header(&(rrm_event_ho_out_exec_info.msg_header),
            RRM_PE_HO_OUT_EXEC_INFO);

    rrm_event_ho_out_exec_info.ho_out_prep_erab_req_bitmap = RRM_ZERO;
    rrm_event_ho_out_exec_info.ho_out_exec_erab_req_bitmap = RRM_ZERO;
    /*rrm_event_ho_out_exec_info.drx_config_index = RRM_ZERO;*/

    /* fill the data in rrm_event_ho_out_exec_info */
    rrm_fill_data_ho_out_exec_info(p_rrm_ue_context,
            &(rrm_event_ho_out_exec_info.ho_out_prep_erab_req_bitmap),
            &(rrm_event_ho_out_exec_info.ho_out_exec_erab_req_bitmap));

    rrm_process_event_handler(&rrm_event_ho_out_exec_info,
            RRM_PE_HO_OUT_EXEC_INFO,
            RRM_UEM_MODULE_ID);

    RRM_UT_TRACE_EXIT();

}

/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_b2_geran
 * Inputs         : p_meas_config,  cell_index
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_B2_GERAN.
 **********************************************************************/
void rrm_raise_event_meas_config_b2_geran(
    /*SPR 17777 +-*/    
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_trigger_quantity_et        eutran_trigger_quantity,
        U8                             eutra_thresh_rsrx)
{
    rrm_event_meas_config_b2_geran_t      *p_rrm_event_meas_config_b2_geran=RRM_PNULL;

    report_config_interrat_trigger_type_event_t *p_event            = RRM_PNULL;
    p_event = &( p_report_config_to_add_mod->report_config.report_config_interrat.
            trigger_type.event);

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_b2_geran = (rrm_event_meas_config_b2_geran_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_b2_geran_t));

    /*coverity 29556, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_b2_geran)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_b2_geran,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_b2_geran_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_b2_geran->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_B2_GERAN);


        /* The measurement ID for this UE measurement */
        p_rrm_event_meas_config_b2_geran->measurement_id = 24; 

        /* The measurement object ID for this UE measurement */
        p_rrm_event_meas_config_b2_geran->meas_object_id = 19;

        /* The quantity (RSRP or RSRQ) that triggers the measurements */
    /* coverity id 29680 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_geran->trigger_quantity = eutran_trigger_quantity; 

        /* Time to trigger */
    /* coverity id 29679 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_geran->time_to_trigger =(rrm_time_to_trigger_et)( p_event->time_to_trigger);

        /* Hysterisis value */
        p_rrm_event_meas_config_b2_geran->hysteresis = p_event->hysteresis;

        /* E-UTRA Threshold for serving cell */
        /* Threshhold will be same for both RSRP and RSRQ*/
        p_rrm_event_meas_config_b2_geran->eutra_threshold1 = eutra_thresh_rsrx;

        /* GERAN Threshold  */
        if(  p_event->event_id.event_b2.bitmask &
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_GERAN_PRESENT)
        {
            p_rrm_event_meas_config_b2_geran->geran_threshold2 = p_event->event_id.
                event_b2.b2_threshold_geran;
        }
        /* Maximum number of cells to include in the measurement report */
        p_rrm_event_meas_config_b2_geran->max_report_cells = p_report_config_to_add_mod->report_config.
            report_config_interrat.max_report_cells;

        /* Indicates the interval between periodical reports*/
    /* coverity id 29678 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_geran->report_interval = (rrm_meas_report_interval_et)(p_report_config_to_add_mod->report_config.
                report_config_interrat.report_interval);

        /* Number of measurement reports applicable for trigger type 'event' and 'periodical'*/
    /* coverity id 29677 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_geran->report_amount = (rrm_meas_report_amount_et)(p_report_config_to_add_mod->report_config.
                report_config_interrat.report_amount);

        rrm_process_event_handler(p_rrm_event_meas_config_b2_geran,
                RRM_PE_EVENT_MEAS_CONFIG_B2_GERAN,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_b2_geran);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_b2_geran failed");


    }

    RRM_UT_TRACE_EXIT();

}
/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_b2_utra
 * Inputs         : p_meas_config,  cell_index
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_B2_UTRA.
 **********************************************************************/
void rrm_raise_event_meas_config_b2_utra(
    /*SPR 17777 +-*/    
        report_config_to_add_mod_t *p_report_config_to_add_mod,
        rrm_trigger_quantity_et     eutran_trigger_quantity,
        U8                          eutra_thresh_rsrx)
{
    rrm_event_meas_config_b2_utra_t      *p_rrm_event_meas_config_b2_utra=RRM_PNULL;

    report_config_interrat_trigger_type_event_t *p_event            = RRM_PNULL;
    p_event = &( p_report_config_to_add_mod->report_config.report_config_interrat.
            trigger_type.event);

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_b2_utra = (rrm_event_meas_config_b2_utra_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_b2_utra_t));

    /*coverity 29557, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_b2_utra)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_b2_utra,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_b2_utra_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_b2_utra->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_B2_UTRA);

        /* The measurement ID for this UE measurement */
        p_rrm_event_meas_config_b2_utra->measurement_id = 22;

        /* The measurement object ID for this UE measurement */
        p_rrm_event_meas_config_b2_utra->meas_object_id = 17;

        /* The quantity (RSRP or RSRQ) that triggers the measurements */
    /* coverity id 29684 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_utra->trigger_quantity = eutran_trigger_quantity;

        /* Time to trigger */
    /* coverity id 29683 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_utra->time_to_trigger = (rrm_time_to_trigger_et)(p_event->time_to_trigger);

        /* Hysterisis value */
        p_rrm_event_meas_config_b2_utra->hysteresis = p_event->hysteresis;

        /* E-UTRA Threshold for serving cell,threshold will be same for RSRQ/RSRP */
        p_rrm_event_meas_config_b2_utra->eutra_threshold1 = eutra_thresh_rsrx;

        if( p_event->event_id.event_b2.bitmask &
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT )
        {
            if( p_event->event_id.event_b2.b2_threshold_utra.bitmask & THRESHOLD_UTRA_RSCP_PRESENT)
            {
                /*The quantity (RSCP/ECNO) that triggers the measurements on UTRA frequencies*/
                p_rrm_event_meas_config_b2_utra->trigger_quantity_utra = RRM_EVENT_VALUE_RSCP;

                /* UTRA Threshold RSCP */
                p_rrm_event_meas_config_b2_utra->threshold2_rscp =  
                    p_event->event_id.event_b2.b2_threshold_utra.threshold_rscp;
            }

            else if(  p_event->event_id.event_b2.b2_threshold_utra.bitmask & THRESHOLD_UTRA_ECN0_PRESENT )
            {
                /*The quantity (RSCP/ECNO) that triggers the measurements on UTRA frequencies*/
                p_rrm_event_meas_config_b2_utra->trigger_quantity_utra = RRM_EVENT_VALUE_ECN0;

                /* UTRA Threshold ECN0 */
                p_rrm_event_meas_config_b2_utra->threshold2_ecn0 =  
                    p_event->event_id.event_b2.b2_threshold_utra.threshold_ecn0;
            }
        }

        /* Maximum number of cells to include in the measurement report */
        p_rrm_event_meas_config_b2_utra->max_report_cells = p_report_config_to_add_mod->report_config.
            report_config_interrat.max_report_cells;

        /* Indicates the interval between periodical reports*/
    /* coverity id 29682 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_utra->report_interval =(rrm_meas_report_interval_et)( p_report_config_to_add_mod->report_config.
                report_config_interrat.report_interval);

        /* Number of measurement reports applicable for trigger type 'event' and 'periodical'*/
    /* coverity id 29681 , ,rel 2.0.1 */
        p_rrm_event_meas_config_b2_utra->report_amount = (rrm_meas_report_amount_et)(p_report_config_to_add_mod->report_config.
                report_config_interrat.report_amount);

        rrm_process_event_handler(p_rrm_event_meas_config_b2_utra,
                RRM_PE_EVENT_MEAS_CONFIG_B2_UTRA,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_b2_utra);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_b2_utra failed");


    }	

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ue_meas_abort
 * Inputs         : p_meas_config,  cell_index
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_UE_MEAS_ABORT
 **********************************************************************/
void rrm_raise_event_ue_meas_abort(
    /*SPR 17777 +-*/    
        U8                 meas_id)
{
    rrm_event_ue_meas_abort_t   *p_rrm_event_ue_meas_abort=RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_ue_meas_abort = (rrm_event_ue_meas_abort_t *)
        rrm_mem_get(sizeof(rrm_event_ue_meas_abort_t));

    /*coverity 29563, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ue_meas_abort)
    {
        //Setting array with invalid value 0xff
    /*coverity 24986: parameter in sizeof changed to rrm_event_ue_meas_abort_t: rel 1.3*/
        RRM_MEMSET(p_rrm_event_ue_meas_abort,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_ue_meas_abort_t));

        p_rrm_event_ue_meas_abort->measurement_id = meas_id;

        rrm_fill_event_notification_header(&(p_rrm_event_ue_meas_abort->msg_header),
                RRM_PE_EVENT_UE_MEAS_ABORT);

        rrm_process_event_handler(p_rrm_event_ue_meas_abort,
                RRM_PE_EVENT_UE_MEAS_ABORT,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ue_meas_abort);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ue_meas_abort failed");


    }
    RRM_UT_TRACE_EXIT();
}



/***********************************************************************
 * Function Name  : rrm_raise_event_too_early_ho
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_TOO_EARLY_HO
 **********************************************************************/
void rrm_raise_event_too_early_ho(rrm_ue_context_t  *p_ue_context)
{
    rrm_event_too_early_ho_t   *p_rrm_event_too_early_ho=RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_too_early_ho = (rrm_event_too_early_ho_t *)
        rrm_mem_get(sizeof(rrm_event_too_early_ho_t));

    /*coverity 29561, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_too_early_ho)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_too_early_ho,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_too_early_ho_t));

        p_rrm_event_too_early_ho->cell_id = p_ue_context->cell_index;
        p_rrm_event_too_early_ho->target_rat_type =
            rrm_fill_ho_target_rat_info(p_ue_context);

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_too_early_ho->msg_header),
                RRM_PE_EVENT_TOO_EARLY_HO);

        rrm_process_event_handler(p_rrm_event_too_early_ho,
                RRM_PE_EVENT_TOO_EARLY_HO,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_too_early_ho);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_too_early_ho failed");

    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_too_late_ho
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_TOO_LATE_HO
 **********************************************************************/
void rrm_raise_event_too_late_ho(rrm_ue_context_t  *p_ue_context)
{
    rrm_event_too_late_ho_t   *p_rrm_event_too_late_ho=RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_too_late_ho = (rrm_event_too_late_ho_t *)
        rrm_mem_get(sizeof(rrm_event_too_late_ho_t));

    /*coverity 29562, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_too_late_ho)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_too_late_ho,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_too_late_ho_t));

        p_rrm_event_too_late_ho->cell_id = p_ue_context->cell_index;
        p_rrm_event_too_late_ho->target_rat_type =
            rrm_fill_ho_target_rat_info(p_ue_context);

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_too_late_ho->msg_header),
                RRM_PE_EVENT_TOO_LATE_HO);

        rrm_process_event_handler(p_rrm_event_too_late_ho,
                RRM_PE_EVENT_TOO_LATE_HO,
                RRM_UEM_MODULE_ID);

        RRM_MEM_FREE(p_rrm_event_too_late_ho);
    }
    else
    {
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_too_late_ho failed");
    }

    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_wrong_cell
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_HO_WRONG_CELL
 **********************************************************************/
void rrm_raise_event_ho_wrong_cell(rrm_ue_context_t  *p_ue_context)
{
    rrm_event_ho_wrong_cell_t   *p_rrm_event_ho_wrong_cell = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_wrong_cell = (rrm_event_ho_wrong_cell_t *)
        rrm_mem_get(sizeof(rrm_event_ho_wrong_cell_t));

    /*coverity 29554, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_wrong_cell)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_ho_wrong_cell,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_ho_wrong_cell_t));

        p_rrm_event_ho_wrong_cell->cell_id = p_ue_context->cell_index;
        p_rrm_event_ho_wrong_cell->target_rat_type =
            rrm_fill_ho_target_rat_info(p_ue_context);

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_wrong_cell->msg_header),
                RRM_PE_EVENT_HO_WRONG_CELL);

        rrm_process_event_handler(p_rrm_event_ho_wrong_cell,
                RRM_PE_EVENT_HO_WRONG_CELL,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_wrong_cell);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_wrong_cell failed");


    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_ho_wrong_cell_reest
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_HO_WRONG_CELL_REEST
 **********************************************************************/
void rrm_raise_event_ho_wrong_cell_reest(rrm_ue_context_t  *p_ue_context)
{
    rrm_event_ho_wrong_cell_reest_t  *p_rrm_event_ho_wrong_cell_reest = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_ho_wrong_cell_reest = (rrm_event_ho_wrong_cell_reest_t *)
        rrm_mem_get(sizeof(rrm_event_ho_wrong_cell_reest_t));

    /*coverity 29555, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_ho_wrong_cell_reest)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_ho_wrong_cell_reest,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_ho_wrong_cell_reest_t));

        p_rrm_event_ho_wrong_cell_reest->cell_id = p_ue_context->cell_index;
        p_rrm_event_ho_wrong_cell_reest->target_rat_type =
            rrm_fill_ho_target_rat_info(p_ue_context);

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_ho_wrong_cell_reest->msg_header),
                RRM_PE_EVENT_HO_WRONG_CELL_REEST);

        rrm_process_event_handler(p_rrm_event_ho_wrong_cell_reest,
                RRM_PE_EVENT_HO_WRONG_CELL_REEST,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_ho_wrong_cell_reest);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_ho_wrong_cell_reest failed");


    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_x2_rlf_indication
 * Inputs         :
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_X2_RLF_INDICATION
 **********************************************************************/
void rrm_raise_event_x2_rlf_indication()
{
    rrm_event_x2_rlf_indication_t  *p_rrm_event_x2_rlf_indication = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_x2_rlf_indication = (rrm_event_x2_rlf_indication_t *)
        rrm_mem_get(sizeof(rrm_event_x2_rlf_indication_t));

    /*coverity 29564, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_x2_rlf_indication)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_x2_rlf_indication,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_x2_rlf_indication_t));

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_x2_rlf_indication->msg_header),
                RRM_PE_EVENT_X2_RLF_INDICATION);

        rrm_process_event_handler(p_rrm_event_x2_rlf_indication,
                RRM_PE_EVENT_X2_RLF_INDICATION,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_x2_rlf_indication);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_x2_rlf_indication failed");


    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_fill_ho_target_rat_info
 * Inputs         :
 * Returns        : rrm_rat_type_et
 * Description    : This function fill target rat info.
 **********************************************************************/
rrm_rat_type_et rrm_fill_ho_target_rat_info(rrm_ue_context_t  *p_ue_context)
{
    RRM_UT_TRACE_ENTER();
    rrm_rat_type_et  rat_type = RRM_CELL_UNDEFINED;

    if (p_ue_context->ue_handover_report.trgt_cgi.bitmask& 
            RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT)
    {
        rat_type = RRM_EUTRAN_CELL;
    }
    else if (p_ue_context->ue_handover_report.trgt_cgi.bitmask& 
            RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT )
    {
        rat_type = p_ue_context->ue_handover_report.trgt_cgi.
            inter_rat_global_cell_id.
            utran_geran_global_cell_id.rat_type;
    }

    RRM_UT_TRACE_EXIT();
    return rat_type;
}

/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_periodical_eutra
 * Inputs         : p_rrc_rrm_meas_config_req,cell_index
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_EUTRA
 **********************************************************************/
void rrm_raise_event_meas_config_periodical_eutra(rrc_rrm_meas_config_req_t  *p_rrc_rrm_meas_config_req,
        rrm_cell_index_t               cell_index)
{
    rrm_event_meas_config_periodical_eutran_t   *p_rrm_event_meas_config_periodical_eutran=RRM_PNULL;
    U8  index = RRM_NULL;
    U8  meas_report_index = RRM_NULL;
    U8  report_config_index = RRM_NULL;
    RRM_UT_TRACE_ENTER();

    p_rrm_event_meas_config_periodical_eutran = (rrm_event_meas_config_periodical_eutran_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_periodical_eutran_t));

    /*coverity 29558, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_periodical_eutran)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_periodical_eutran,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_periodical_eutran_t));

        meas_report_index = p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.count;

        /* Filling cell_id */
        p_rrm_event_meas_config_periodical_eutran->cell_id = cell_index;

        for( index=RRM_ZERO; index < meas_report_index ; index++)
        {   
            report_config_index = p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.
                meas_id_to_add_mod[index].report_config_id;

            if(  p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].report_config.bitmask & REPORT_CONFIG_INTERRAT_PRESENT)

            {
                /* Filling measurement Id*/
                p_rrm_event_meas_config_periodical_eutran->measurement_id = p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_id;

                /*Filling measurement confid Id*/
                p_rrm_event_meas_config_periodical_eutran->meas_object_id =  p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_object_id;

                /*Filling periodical measurement purpose*/
            /* coverity id 29674 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_eutran->periodical_purpose =(rrm_trigger_type_periodical_purpose_et)( p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_eutra.
                        trigger_type.periodical.purpose);

                /*Filling max number of cells to report measurement*/
                p_rrm_event_meas_config_periodical_eutran->event_param_max_report_cells = p_rrc_rrm_meas_config_req->meas_config.
                    report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].
                    report_config.report_config_eutra.max_report_cells;
                /*Filling report interval*/
            /* coverity id 29676 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_eutran->report_interval =(rrm_meas_report_interval_et)( p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_eutra.report_interval);

                /*Filling report amount*/
            /* coverity id 29675 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_eutran->report_amount = (rrm_meas_report_amount_et)( p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_eutra.report_amount);
                break;
            }
        }

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_periodical_eutran->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_EUTRA);

        rrm_process_event_handler(p_rrm_event_meas_config_periodical_eutran,
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_EUTRA,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_periodical_eutran);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_periodical_eutran failed");


    }
    RRM_UT_TRACE_EXIT();
}


/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_periodical_utra
 * Inputs         : p_rrc_rrm_meas_config_req,cell_index
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_UTRA
 **********************************************************************/
void rrm_raise_event_meas_config_periodical_utra(rrc_rrm_meas_config_req_t  *p_rrc_rrm_meas_config_req,
        rrm_cell_index_t            cell_index)
{
    rrm_event_meas_config_periodical_utra_t   *p_rrm_event_meas_config_periodical_utra=RRM_PNULL;
    U8  index = RRM_NULL;
    U8  meas_report_index = RRM_NULL;
    U8  report_config_index = RRM_NULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_periodical_utra = (rrm_event_meas_config_periodical_utra_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_periodical_utra_t));


    /*coverity 29560, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_periodical_utra)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_periodical_utra,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_periodical_utra_t));

        meas_report_index = p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.count;

        /* Filling cell_id */
        p_rrm_event_meas_config_periodical_utra->cell_id = cell_index;

        for( index=RRM_ZERO; index < meas_report_index ; index++)
        {
            report_config_index = p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.
                meas_id_to_add_mod[index].report_config_id;   

            if(  p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].report_config.bitmask & REPORT_CONFIG_INTERRAT_PRESENT)
            {
                /* Filling measurement Id*/
                p_rrm_event_meas_config_periodical_utra->measurement_id = p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_id;

                /*Filling measurement confid Id*/
                p_rrm_event_meas_config_periodical_utra->meas_object_id =  p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_object_id;

                /*Filling periodical measurement purpose*/
            /* coverity id 29671 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_utra->periodical_purpose =(rrm_trigger_type_periodical_purpose_et)( p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_interrat.
                        trigger_type.periodical.purpose);

                /*Filling max number of cells to report measurement*/
                p_rrm_event_meas_config_periodical_utra->event_param_max_report_cells =  p_rrc_rrm_meas_config_req->meas_config.
                    report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].
                    report_config.report_config_interrat.max_report_cells;

                /*Filling report interval*/
            /* coverity id 29673 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_utra->report_interval =(rrm_meas_report_interval_et)( p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_interrat.report_interval);


                /*Filling report amount*/
            /* coverity id 29672 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_utra->report_amount =  (rrm_meas_report_amount_et)(p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_interrat.report_amount);
                break;
            }
        }

        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_periodical_utra->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_UTRA);

        rrm_process_event_handler(p_rrm_event_meas_config_periodical_utra,
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_UTRA,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_periodical_utra);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_periodical_utra failed");


    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_periodical_geran
 * Inputs         : p_rrc_rrm_meas_config_req,cell_index
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_GERAN
 **********************************************************************/
void rrm_raise_event_meas_config_periodical_geran(rrc_rrm_meas_config_req_t *p_rrc_rrm_meas_config_req,
        rrm_cell_index_t           cell_index)
{
    rrm_event_meas_config_periodical_geran_t   *p_rrm_event_meas_config_periodical_geran=RRM_PNULL;
    U8  index = RRM_NULL;
    U8  meas_report_index = RRM_NULL;
    U8  report_config_index = RRM_NULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_periodical_geran = (rrm_event_meas_config_periodical_geran_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_periodical_geran_t));

    /*coverity 29559, rel2.0.1, */
    if(RRM_PNULL != p_rrm_event_meas_config_periodical_geran)
    {
        //Setting array with invalid value 0xff
        RRM_MEMSET(p_rrm_event_meas_config_periodical_geran,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_periodical_geran_t));

        meas_report_index = p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.count;

        /* Filling cell_id */
        p_rrm_event_meas_config_periodical_geran->cell_id = cell_index;

        for( index=RRM_ZERO; index < meas_report_index ; index++)
        {
            report_config_index = p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.
                meas_id_to_add_mod[index].report_config_id;

            if(  p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].report_config.bitmask & REPORT_CONFIG_INTERRAT_PRESENT)

            {
                /* Filling measurement Id*/
                p_rrm_event_meas_config_periodical_geran->measurement_id = p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_id;

                /*Filling measurement confid Id*/
                p_rrm_event_meas_config_periodical_geran->meas_object_id =  p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_object_id;

                /*Filling periodical measurement purpose*/
            /* coverity id 29668 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_geran->periodical_purpose = (rrm_trigger_type_periodical_purpose_et)(p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_interrat.
                        trigger_type.periodical.purpose);

                /*Filling max number of cells to report measurement*/
                p_rrm_event_meas_config_periodical_geran->event_param_max_report_cells =  p_rrc_rrm_meas_config_req->meas_config.
                    report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].
                    report_config.report_config_interrat.max_report_cells;

                /*Filling report interval*/
             /* coverity id 29670 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_geran->report_interval =(rrm_meas_report_interval_et)( p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_interrat.report_interval);


                /*Filling report amount*/
             /*coverity id 29669 , ,rel 2.0.1 */
                p_rrm_event_meas_config_periodical_geran->report_amount = (rrm_meas_report_amount_et)( p_rrc_rrm_meas_config_req->meas_config.
                        report_config_to_add_mod_list.
                        report_config_to_add_mod[report_config_index].
                        report_config.report_config_interrat.report_amount);
                break;
            }
        }


        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_periodical_geran->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_GERAN);

        rrm_process_event_handler(p_rrm_event_meas_config_periodical_geran,
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_GERAN,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_periodical_geran);
    }
    else
    {
	RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_event_meas_config_periodical_geran failed");


    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_raise_event_meas_config_periodical_cdma
 * Inputs         : p_rrc_rrm_meas_config_req,cell_index
 * Returns        : void
 * Description    : This function process event
 RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_CDMA
 **********************************************************************/
void rrm_raise_event_meas_config_periodical_cdma(rrc_rrm_meas_config_req_t *p_rrc_rrm_meas_config_req,
        rrm_cell_index_t           cell_index)
{
    rrm_event_meas_config_periodical_cdma_t   *p_rrm_event_meas_config_periodical_cdma = RRM_PNULL;
    U8  index = RRM_NULL;
    U8  meas_report_index = RRM_NULL;
    U8  report_config_index = RRM_NULL;

    RRM_UT_TRACE_ENTER();
    p_rrm_event_meas_config_periodical_cdma = (rrm_event_meas_config_periodical_cdma_t *)
        rrm_mem_get(sizeof(rrm_event_meas_config_periodical_cdma_t));

    if(RRM_PNULL != p_rrm_event_meas_config_periodical_cdma) /* Coverity: CID 30918 */
    {
    /*Setting array with invalid value 0xff*/
        RRM_MEMSET(p_rrm_event_meas_config_periodical_cdma,RRM_EVENT_U16_INVALID_VALUE,sizeof(rrm_event_meas_config_periodical_cdma_t));

        meas_report_index = p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.count;

        /* Filling cell_id */
        p_rrm_event_meas_config_periodical_cdma->cell_id = cell_index;

        for( index=RRM_ZERO; index < meas_report_index ; index++)
        {
            report_config_index = p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.
                meas_id_to_add_mod[index].report_config_id;

            if(  p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].report_config.bitmask & REPORT_CONFIG_INTERRAT_PRESENT)

            {
                /* Filling measurement Id*/
                p_rrm_event_meas_config_periodical_cdma->measurement_id = p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_id;

                /*Filling measurement confid Id*/
                p_rrm_event_meas_config_periodical_cdma->meas_object_id =  p_rrc_rrm_meas_config_req->meas_config.
                    meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                    meas_object_id;

                /*Filling periodical measurement purpose*/
                /* coverity : CID 30999 */
                p_rrm_event_meas_config_periodical_cdma->periodical_purpose = 
                    (rrm_trigger_type_periodical_purpose_et)p_rrc_rrm_meas_config_req->meas_config.
                    report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].
                    report_config.report_config_interrat.
                    trigger_type.periodical.purpose;

                /*Filling max number of cells to report measurement*/
                p_rrm_event_meas_config_periodical_cdma->event_param_max_report_cells =  p_rrc_rrm_meas_config_req->meas_config.
                    report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].
                    report_config.report_config_interrat.max_report_cells;

                /*Filling report interval*/
                /* coverity : CID 31001*/
                p_rrm_event_meas_config_periodical_cdma->report_interval =
                    (rrm_meas_report_interval_et) p_rrc_rrm_meas_config_req->meas_config.
                    report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].
                    report_config.report_config_interrat.report_interval;


                /*Filling report amount*/
                /* coverity  : CID 31000 */
                p_rrm_event_meas_config_periodical_cdma->report_amount = 
                    (rrm_meas_report_amount_et) p_rrc_rrm_meas_config_req->meas_config.
                    report_config_to_add_mod_list.
                    report_config_to_add_mod[report_config_index].
                    report_config.report_config_interrat.report_amount;
                break;
            }
        }


        //Filling Event Notification Header
        rrm_fill_event_notification_header(&(p_rrm_event_meas_config_periodical_cdma->msg_header),
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_CDMA);

        rrm_process_event_handler(p_rrm_event_meas_config_periodical_cdma,
                RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_CDMA,
                RRM_UEM_MODULE_ID);
        RRM_MEM_FREE(p_rrm_event_meas_config_periodical_cdma);
    }
    else
    {
	    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation for p_rrm_event_meas_config_periodical_cdma failed");
    }
    RRM_UT_TRACE_EXIT();
}

/***********************************************************************
 * Function Name  : rrm_get_err_event_type
 * Inputs         : event_id
 * Returns        : rrm_error_event_type_et
 * Description    : This function process event id & returns 
 *                  event type on the basis of event id
 **********************************************************************/
    rrm_error_event_type_et
rrm_get_err_event_type(rrm_error_event_id_et  event_id)
{

    rrm_error_event_type_et   event_type = RRM_EE_INVALID_TYPE;  
    RRM_UT_TRACE_ENTER();

    switch(event_id)
    {
        case RRM_EE_MEM_ALLOC_FAIL:
        case RRM_EE_MEM_FREE_FAIL:
            event_type = RRM_EE_TYPE_MEM_UNAVAILABLE;
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                    "RRM_EE_TYPE_MEM_UNAVAILABLE"); 
            break;

        case RRM_EE_IPC_SEND_MSG_FAIL:
        case RRM_EE_IPC_RECV_MSG_FAIL:
        case RRM_EE_TIMER_START_FAIL:
        case RRM_EE_TIMER_STOP_FAIL:
        /*SPR 14811 start*/
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_OVER_LOAD:
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_HIGH_LOAD:
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_MID_LOAD:
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_LOW_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_OVER_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_HIGH_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_MID_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_LOW_LOAD:
        case RRM_EE_ABS_NOT_FOUND_LOW_LOAD:
        case RRM_EE_ABS_FOUND_LOW_LOAD:
        case RRM_EE_ABS_NOT_FOUND_MID_LOAD:
        case RRM_EE_ABS_FOUND_MID_LOAD:
        case RRM_EE_ABS_NOT_FOUND_HIGH_LOAD:
        case RRM_EE_ABS_FOUND_HIGH_LOAD:
        case RRM_EE_ABS_NOT_FOUND_OVER_LOAD:
        case RRM_EE_ABS_FOUND_OVER_LOAD:
        case RRM_EE_VICTIM_ABS_NOT_FOUND:
        case RRM_EE_VICTIM_ABS_FOUND:
        case RRM_EE_VICTIM_MEAS_SUBSET_NOT_FOUND:        
        case RRM_EE_VICTIM_MEAS_SUBSET_FOUND:        
        case RRM_EE_VICTM_ABS_NOT_APPLIED:
        case RRM_EE_VICTM_ABS_APPLIED:
        /*SPR 14811 end*/
            event_type = RRM_EE_TYPE_SW_PROCESSING_ERR;
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                    "RRM_EE_TYPE_SW_PROCESSING_ERR"); 
            break;

        default:
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                    "Invalid eventId[%d] received", event_id); 
            break;
    }  

    RRM_UT_TRACE_EXIT();
    return event_type;
}

/***********************************************************************
 * Function Name  : rrm_get_err_event_subclass
 * Inputs         : event_id
 * Returns        : void
 * Description    : This function returns the raised event subclass.
 **********************************************************************/
    rrm_oam_event_subclass_et
rrm_get_err_event_subclass(U32   event_id)
{
    rrm_oam_event_subclass_et subtype = RRM_OAM_MAX_SUBCLASS;

    RRM_UT_TRACE_ENTER();

    switch(event_id)
    {
        case RRM_EE_MEM_ALLOC_FAIL:
        case RRM_EE_MEM_FREE_FAIL:
            subtype = RRM_OAM_EE_MEMORY_SPECIFIC;
      RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                    "RRM_OAM_EE_MEMORY_SPECIFIC"); 
            break;

        case RRM_EE_IPC_SEND_MSG_FAIL:
        case RRM_EE_IPC_RECV_MSG_FAIL:
        /*SPR 14811 start*/
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_OVER_LOAD:
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_HIGH_LOAD:
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_MID_LOAD:
        case RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_LOW_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_OVER_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_HIGH_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_MID_LOAD:
        case RRM_EE_AGGR_ABS_APPLIED_FOR_LOW_LOAD:
        case RRM_EE_ABS_NOT_FOUND_LOW_LOAD:
        case RRM_EE_ABS_FOUND_LOW_LOAD:
        case RRM_EE_ABS_NOT_FOUND_MID_LOAD:
        case RRM_EE_ABS_FOUND_MID_LOAD:
        case RRM_EE_ABS_NOT_FOUND_HIGH_LOAD:
        case RRM_EE_ABS_FOUND_HIGH_LOAD:
        case RRM_EE_ABS_NOT_FOUND_OVER_LOAD:
        case RRM_EE_ABS_FOUND_OVER_LOAD:
        case RRM_EE_VICTIM_ABS_NOT_FOUND:
        case RRM_EE_VICTIM_ABS_FOUND:
        case RRM_EE_VICTIM_MEAS_SUBSET_NOT_FOUND:        
        case RRM_EE_VICTIM_MEAS_SUBSET_FOUND:        
        case RRM_EE_VICTM_ABS_NOT_APPLIED:
        case RRM_EE_VICTM_ABS_APPLIED:
        /*SPR 14811 end*/
            subtype = RRM_OAM_EE_IPC_SEND_RECV_SPECIFIC;
      RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                    "RRM_OAM_EE_IPC_SEND_RECV_SPECIFIC"); 
            break;

        case RRM_EE_TIMER_START_FAIL:
        case RRM_EE_TIMER_STOP_FAIL:
            subtype = RRM_OAM_EE_TIMER_SPECIFIC;
       RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED,
                    "RRM_OAM_EE_TIMER_SPECIFIC"); 
            break;

        default:
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_ERROR,
                    "Invalid eventId[%d] received", event_id);
            break;
    }

    RRM_UT_TRACE_EXIT();
    return subtype;
}

/***********************************************************************
 * Function Name  : rrm_raise_error_event_notification
 * Inputs         : err_event_id 
 * Returns        : void
 * Description    : This function process error event
 **********************************************************************/
void rrm_raise_error_event_notification(U32    err_event_id)

{
    U16   msg_size = RRM_ZERO;
    rrm_event_error_info_t  error_event_info = {RRM_ZERO};
    rrmuem_rmif_event_notification_ind_t  rrm_uem_rmif_event_notification_ind = {RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    /*filling error info*/
    error_event_info.bitmask = RRM_ZERO;
    /* coverity  CID : 31002 */
    error_event_info.error_type = rrm_get_err_event_type((rrm_error_event_id_et)err_event_id);

    /*filling event notification ind*/
    rrm_uem_rmif_event_notification_ind.bitmask = RRM_ZERO;
    rrm_fill_time_stamp((rrm_event_time_stamp_t *)
            &(rrm_uem_rmif_event_notification_ind.header.time_stamp));
    rrm_uem_rmif_event_notification_ind.header.event_type = RRM_OAM_ERROR_EVENT_TYPE;
    rrm_uem_rmif_event_notification_ind.header.event_sub_type = 
        rrm_get_err_event_subclass(err_event_id);
    rrm_uem_rmif_event_notification_ind.header.event_id = err_event_id;

    /*composer_function here */
    rrm_uem_rmif_event_notification_ind.bitmask |= RRM_MIF_EVENT_DATA_PRESENT;
    rrm_compose_error_event_info(rrm_uem_rmif_event_notification_ind.buffer, 
            &error_event_info);

    /*sending error event to oam module*/
    msg_size = sizeof(rrmuem_rmif_event_notification_ind_t);
    if(RRM_FAILURE == rrm_uem_send_internal_msg(RRM_MIF_MODULE_ID,
                RRMUEM_RMIF_EVENT_NOTIFICATION_IND,
                msg_size,
                (void *)&rrm_uem_rmif_event_notification_ind))
    {
        /* Coverity: CID 29028 Rel 2.1.3 */
    RRM_TRACE(RRM_TRUE, p_g_rrm_event_facility_name, RRM_DETAILED, "UEM send to MIF failed");
    }

    RRM_UT_TRACE_EXIT();
}
