
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_oam.c,v 1.8 2010/10/13 07:04:24 gur18569 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the definitions of functions handling
 *                     OAM requests.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_oam.c,v $
 * Revision 1.8  2010/10/13 07:04:24  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.7.4.5  2010/10/04 13:31:54  gur04518
 * removed backslash
 *
 * Revision 1.7.4.4  2010/09/10 06:40:59  gur22047
 * after mem_free doing ho_ctx NULL
 *
 * Revision 1.7.4.3  2010/09/09 10:11:05  gur22047
 * modified_during_s1ap_UT added_stream_id
 *
 * Revision 1.7.4.2  2010/08/16 12:10:31  gur22047
 * lokesh_review_incorporate
 *
 * Revision 1.7.4.1  2010/06/24 12:58:52  gur22047
 * internal source and s1ap code
 *
 * Revision 1.7  2010/05/14 09:36:58  gur19827
 * checklist,warning_removal,dst mod id,tdd,phy cell id added
 *
 * Revision 1.6  2010/04/06 04:03:50  gur18569
 * reset fwk chg
 *
 * Revision 1.5  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.4  2010/03/04 07:15:19  gur21006
 * code comments incorporated
 *
 * Revision 1.3  2010/02/18 04:25:20  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.12  2009/12/28 10:14:35  gur21006
 * system calls removed
 *
 * Revision 1.1.2.11  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.10  2009/12/27 08:39:04  gur18569
 * removed tabs
 *
 * Revision 1.1.2.9  2009/12/26 12:05:01  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.8  2009/12/23 07:01:35  gur21006
 * Warnings Removed
 *
 * Revision 1.1.2.7  2009/12/23 06:34:14  gur21006
 * Warnings Removed
 *
 * Revision 1.1.2.6  2009/12/22 14:37:00  gur18569
 * fixed 154
 *
 * Revision 1.1.2.5  2009/12/17 15:09:23  gur18569
 * setting num_of_prov_mme when parsing prov req from OAM
 *
 * Revision 1.1.2.4  2009/12/03 10:53:43  gur21006
 * handling cleanup even before receiving prov req from oam
 *
 * Revision 1.1.2.3  2009/12/03 10:11:36  gur18569
 * integration changes
 *
 * Revision 1.1.2.2  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.1  2009/11/25 13:11:05  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.27  2009/11/24 12:21:13  gur20470
 * Added Error tracein clean up function
 *
 * Revision 1.1.2.26  2009/11/23 15:08:53  gur18569
 * removed commented code
 *
 * Revision 1.1.2.25  2009/11/23 15:07:59  gur18569
 * ut fixes
 *
 * Revision 1.1.2.24  2009/11/23 13:52:52  gur20470
 * Added code to stop timers in s1ap global context
 *
 * Revision 1.1.2.23  2009/11/23 10:54:31  gur21006
 * rrc_stop_timer added in oam cleanup
 *
 * Revision 1.1.2.21  2009/11/23 09:25:30  gur21006
 * Oam cleanup function : Reset active mme count added
 *
 * Revision 1.1.2.20  2009/11/23 08:11:11  gur20470
 * Included is_initialised_once flag to check the initialisation of s1ap once
 *
 * Revision 1.1.2.19  2009/11/23 07:23:58  gur20470
 * Included NULL check for pointer to mme context
 *
 * Revision 1.1.2.18  2009/11/22 12:05:43  gur18569
 * freeing deleted nodes from search tree
 *
 * Revision 1.1.2.17  2009/11/22 11:48:17  gur18569
 * ut fixes
 *
 * Revision 1.1.2.16  2009/11/21 10:49:26  gur20470
 * Fixed logical error in s1ap_process_oam_cleanup_req
 *
 * Revision 1.1.2.15  2009/11/21 06:38:27  gur21006
 * Error check added
 *
 * Revision 1.1.2.14  2009/11/20 09:53:48  gur18569
 * setting s1ap state to wait for prov req after cleanup
 *
 * Revision 1.1.2.13  2009/11/20 08:43:09  gur18569
 * bug fixes during UT
 *
 * Revision 1.1.2.12  2009/11/18 11:55:27  gur21006
 * Review Comments incorporated
 *
 * Revision 1.1.2.11  2009/11/17 13:26:08  gur20470
 * Incorporated review comments
 *
 * Revision 1.1.2.10  2009/11/17 04:28:31  gur20470
 * Function s1ap_process_oam_provision_req modified
 *
 * Revision 1.1.2.9  2009/11/11 04:39:16  gur18569
 * using max_retry_for_reset in place of DEFAULT_MAX_RESET_RETRY_COUNT and also setting values in global context during handling provision request
 *
 * Revision 1.1.2.8  2009/11/10 11:41:55  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.7  2009/11/09 09:18:23  gur18569
 * removed compilation warnings
 *
 * Revision 1.1.2.6  2009/11/08 11:24:00  gur20470
 * removed compilation warnings
 *
 * Revision 1.1.2.5  2009/10/29 05:36:47  gur18569
 * added stats case in process oam messages and removed commented code
 *
 * Revision 1.1.2.4  2009/10/28 07:19:16  gur18569
 * added cause in reset req asn compose
 *
 * Revision 1.1.2.3  2009/10/26 09:15:34  gur21006
 * Formatting Done
 *
 * Revision 1.1.2.2  2009/10/23 18:27:44  gur20052
 * Remove Compilation errors
 *
 * Revision 1.1.2.1  2009/10/23 16:10:17  gur18569
 * Initial version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "s1ap_oam.h"
#include "list_wrap.h"
#include "s1ap_mme_fsm.h"
#include "s1ap_enb_config_fsm.h"
#include "rrc_s1apCommon_intf.h"
#include "rrc_common_md.h"
#include "s1ap_utils.h"
#include "rrc_logging.h"
#include "s1ap_uecc.h"
/*sctp parameters start */
#if 0
#ifdef ARICENT_SCTP_IPR
#include "s_mibtyp.h"
#include "s_stats.h"
#include "s_mibdef.h"
#endif
#endif
/*sctp parameters stop */

/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
QLOCK s1ap_lock;
#endif
/*Lockless Implementation stop*/

 /******************************************************************************
   Private Definitions
  *****************************************************************************/



 /******************************************************************************
   Private Types
  *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
static void s1ap_set_mme_reset_info(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        reset_req_t    ** p_mme_reset_info_arr,
        s1ap_enb_reset_info_t    *p_reset_input_info,
        U8* p_num_reset_msgs_sent);

S32
compare_mmeUeS1apId_search_tree(
        const void * p_elem1,
        const void * p_elem2);

const void *
key_of_mmeUeS1apId_search_tree(
        const YTNODE * p_node);

static void  establish_sctp_associations(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        s1apInfo_t * p_s1_setup_info_req);

static rrc_return_et update_oam_mme_connection_status_resp(
        s1ap_oam_mme_conn_status_response_t *p_oam_mme_connection_status_resp,
        rrc_response_et response,
        U8 mme_id,
        U8 index_to_update);

static rrc_bool_et validate_s1ap_oam_ip_addresses(
        s1apInfo_t *p_s1ap_oam_info
        );

/* SPR 1643 Start */
static rrc_bool_et validate_s1ap_oam_plmn_identity(
         s1_setup_req_t  *p_s1_setup_req_parameters);
/* SPR 1643 Stop */

rrc_bool_et validate_dynamic_s1ap_oam_ip_addresses(
        s1ap_add_mme_req_t *p_s1ap_add_mme_info
        );

void  establish_dynamic_sctp_associations(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        s1ap_add_mme_req_t* p_s1ap_add_mme_info);

void s1ap_oam_process_add_mme_req(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx);

void s1ap_process_oam_set_log_level_req(
                 void                   *p_api,
                 s1ap_gb_context_t *p_s1ap_gb_ctx);

void s1ap_process_oam_get_log_level_req(
                 void                   *p_api,
                 s1ap_gb_context_t *p_s1ap_gb_ctx);

void s1ap_process_oam_log_enable_req(
                 void                   *p_api,
                 s1ap_gb_context_t *p_s1ap_gb_ctx);

U8 get_oam_log_level_from_s1ap_log_level
(
    U8 s1ap_log_level
    );

/*sctp parameters start*/
#if 0	
#ifdef ARICENT_SCTP_IPR
void s1ap_oam_convert_sctp_mib_status(
       sctpmib_assoc_state_et *p_assoc_state,
      U32 *p_s1ap_oam_sctp_status);
#endif
#endif
/*sctp parameters stop*/
void s1ap_oam_process_proc_req(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx);

U32 s1ap_sctp_gl_health_monitor_timer_val = 0;

 /******************************************************************************
  * Private Constants
  *****************************************************************************/

 /******************************************************************************
  * Exported Variables
  *****************************************************************************/

/*Bug 9305 start*/
extern U32 s1ap_health_monitor_val[S1AP_SCTP_MAX_THREADS];
/*Bug 9305 end*/
extern s1ap_health_monitor_desriptors_t s1ap_health_monitor_desriptors[S1AP_SCTP_MAX_THREADS];

extern void s1ap_gb_ctx_reset(s1ap_gb_context_t *p_s1ap_gl_ctx);//bug 115771
#ifdef LINUX_PC_TEST
extern void test_s1ap_init(void);
#endif
 /*****************************************************************************
  * Private Variables (Must be declared static)
  *****************************************************************************/

 /*****************************************************************************
  * Function Name  : s1ap_process_oam_messages
  * Inputs         : p_api - Pointer to input API buffer
  *                  p_s1ap_gb_ctx - Pointer to S1AP global context.
  * Outputs        : None
  * Returns        : None
  * Description    : This function is invoked to handle OAM messages to S1AP.
  ******************************************************************************/
 void s1ap_process_oam_messages(
         /* Pointer to input API buffer */
         void            *p_api,         

         /* Pointer to the S1AP global context data */
         s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    switch (rrc_get_api_id((U8*)p_api))
    {
        case S1AP_OAM_PROVISION_REQ:
            s1ap_process_oam_provision_req(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_OAM_RESET_REQ:
            s1ap_process_oam_reset_req(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_OAM_CLEANUP_REQ:
            s1ap_process_oam_cleanup_req(p_api,p_s1ap_gb_ctx);
            break;

/* CSR 00058587 Fix Start */
        case S1AP_OAM_STATUS_REQ:
/* CSR 00058587 Fix Start */
            s1ap_oam_process_stats_req(p_api,p_s1ap_gb_ctx);
            break;

        case S1AP_OAM_ENB_CONFIG_UPDATE:
            s1ap_oam_process_enb_config_update(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_OAM_CLOSE_MME_CONN:
            s1ap_oam_process_close_mme_conn(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_OAM_REESTAB_MME_CONN: 
            s1ap_oam_process_reestab_mme_conn(p_api, p_s1ap_gb_ctx);
            break;
        case S1AP_ADD_MME_REQ:
            s1ap_oam_process_add_mme_req(p_api, p_s1ap_gb_ctx);
            break;
        case S1AP_OAM_GET_SCTP_STATUS_REQ:
            s1ap_oam_process_get_sctp_status_req(p_api, p_s1ap_gb_ctx);
            break;
        case S1AP_OAM_PROC_SUPP_REQ:
            s1ap_oam_process_proc_req(p_api,p_s1ap_gb_ctx);
            break;
        case S1AP_OAM_SET_LOG_LEVEL_REQ:
            s1ap_process_oam_set_log_level_req(p_api, p_s1ap_gb_ctx);
            break;
        case S1AP_OAM_GET_LOG_LEVEL_REQ:
            s1ap_process_oam_get_log_level_req(p_api, p_s1ap_gb_ctx);
            break;
        case S1AP_OAM_LOG_ENABLE_REQ:
            s1ap_process_oam_log_enable_req(p_api, p_s1ap_gb_ctx);
            break;
#ifdef LINUX_PC_TEST
		case S1AP_GB_STATUS_REQ:
			s1ap_process_gb_status_req(p_api, p_s1ap_gb_ctx);
			break;
#endif
        default:
            RRC_S1AP_TRACE(RRC_WARNING,
             "s1ap_process_oam_messages: "
             "unexpected API from OAM module received");
            break;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * *   FUNCTION NAME: s1ap_process_oam_set_log_level_req
 * *   INPUTS       : void                  *p_api
 * *                  s1ap_gb_context_t     *p_s1ap_gb_context.
 * *   OUTPUTS      :
 * *   DESCRIPTION:
 * *       This function handles S1AP_OAM_SET_LOG_LEVEL_REQ msg
 * *
 * *   RETURNS:
 * *       None
 * ******************************************************************************/
void s1ap_process_oam_set_log_level_req(
        void                   *p_api,
        s1ap_gb_context_t      *p_s1ap_gb_context)
{
    s1ap_oam_set_log_level_req_t    in_message;
    s1ap_oam_set_log_level_resp_t   out_message;
    S32 length_read    = RRC_NULL;
    U16 transaction_id = RRC_NULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));
   if(RRC_SUCCESS !=  rrc_il_parse_s1ap_oam_set_log_level_req(
                       &in_message,(U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                       rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                        &length_read))
   {
       RRC_S1AP_TRACE(S1AP_WARNING, "Bad message - ignore.");
       RRC_S1AP_UT_TRACE_EXIT();
       return;
   }

   transaction_id = rrc_get_transaction_id(p_api);

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    /* Set new log level */
        s1ap_change_logging_parameters(p_s1ap_gb_context,
                get_s1ap_log_level_from_oam_log_level(in_message.log_level),
                        p_s1ap_gb_context->log_enabled);
#endif

   out_message.response = RRC_SUCCESS;

  rrc_s1apOam_il_send_s1ap_oam_set_log_level_resp(&out_message,
                                              RRC_S1AP_MODULE_ID,
                                              RRC_OAM_MODULE_ID,
                                              transaction_id,
                                              RRC_NULL);
   RRC_S1AP_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: s1ap_process_oam_get_log_level_req
*   INPUTS       : void                  *p_api
*                  s1ap_gb_context_t     *p_s1ap_gb_context
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles S1AP_OAM_GET_LOG_LEVEL_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
void s1ap_process_oam_get_log_level_req
(
    void                  *p_api,
    s1ap_gb_context_t     *p_s1ap_gb_context
)
{
    s1ap_oam_get_log_level_resp_t    out_message;
    U16 transaction_id = RRC_NULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));
    transaction_id = rrc_get_transaction_id(p_api);
    if (p_s1ap_gb_context->log_enabled == RRC_TRUE)
    {
        out_message.log_on_off = OAM_LOG_ON;
        out_message.bitmask |= S1AP_OAM_LOG_LEVEL_PRESENT;
        out_message.log_level = get_oam_log_level_from_s1ap_log_level(p_s1ap_gb_context->log_level);
    }
    else
    {
        out_message.log_on_off = OAM_LOG_OFF;
    }
    rrc_s1apOam_il_send_s1ap_oam_get_log_level_resp(
                        &out_message,
                        RRC_S1AP_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

    RRC_S1AP_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: s1ap_oam_process_log_enable_req
*
*   INPUTS       : void                  *p_api
*                  s1ap_gb_context_t *p_s1ap_gb_context
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles S1AP_OAM_LOG_ENABLE_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
void s1ap_process_oam_log_enable_req
(
    void                  *p_api,
    s1ap_gb_context_t *p_s1ap_gb_context
)
{
    s1ap_oam_log_enable_req_t    in_message;
    s1ap_oam_log_enable_resp_t   out_message;
    S32 length_read         = RRC_NULL;
    U16 transaction_id      = RRC_NULL;
    rrc_bool_et log_enabled = RRC_TRUE;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));

    if(RRC_SUCCESS !=
       rrc_il_parse_s1ap_oam_log_enable_req(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }
    transaction_id = rrc_get_transaction_id(p_api);

    if (OAM_LOG_ON == in_message.log_on_off)
    {
        log_enabled = RRC_TRUE;
    }
    else
    {
        log_enabled = RRC_FALSE;
    }
#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    /* Enable/Disable logging */
    s1ap_change_logging_parameters(p_s1ap_gb_context, p_s1ap_gb_context->
        log_level, log_enabled);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

    out_message.response = RRC_SUCCESS;

    rrc_s1apOam_il_send_s1ap_oam_log_enable_resp(
                        &out_message,
                        RRC_S1AP_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

    RRC_S1AP_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: get_oam_log_level_from_s1ap_log_level
*   INPUTS       : U8 s1ap_log_level
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function returns OAM log level identifier from S1AP log level
*       identifier
*
*   RETURNS:
*       OAM log level identifier
******************************************************************************/
U8 get_oam_log_level_from_s1ap_log_level
(
    U8 s1ap_log_level /* S1AP log level identifier */
    )
{
    U8 oam_log_level = RRC_NULL;

    switch (s1ap_log_level)
    {
        case S1AP_FATAL:
            oam_log_level = (U8)OAM_LOG_LEVEL_FATAL;
            break;

        case S1AP_ERROR:
            oam_log_level = (U8)OAM_LOG_LEVEL_ERROR;
            break;

        case S1AP_WARNING:
            oam_log_level = (U8)OAM_LOG_LEVEL_WARNING;
            break;

        case S1AP_INFO:
            oam_log_level = (U8)OAM_LOG_LEVEL_INFO;
            break;

        case S1AP_BRIEF:
            oam_log_level = (U8)OAM_LOG_LEVEL_BRIEF;
            break;

        case S1AP_DETAILED:
            oam_log_level = (U8)OAM_LOG_LEVEL_DETAILED;
            break;

        case S1AP_DETAILEDALL:
            oam_log_level = (U8)OAM_LOG_LEVEL_DETAILEDALL;
            break;

        default:
            RRC_ASSERT(RRC_FALSE);
    }

    return oam_log_level;
}

/*****************************************************************************
 * Function Name  : s1ap_process_init_timer
 * Inputs         : p_s1ap_timer_buf - Pointer to timer buffer.
 *                  p_s1ap_gb_ctx - Pointer to S1AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes OAM init timer message. It creates
 *                  and send S1AP_OAM_INIT_IND to OAM module.
 ******************************************************************************/
void s1ap_process_init_timer( 
        /* Pointer to buffer associated with timer msg */

        /* Pointer to the S1AP global context data */
        s1ap_gb_context_t        *p_s1ap_gb_ctx)
{
    U8    *p_api = PNULL;
    U8    *p_msg = PNULL;
    U16   msg_len = CV_HDRSIZE + RRC_INTERFACE_API_HEADER_SIZE;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);


    p_api =(U8*) rrc_msg_mem_get(msg_len);

    if (PNULL == p_api)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                "rrc_msg_mem_get failed.",__FUNCTION__);
        return;
    }

    p_msg = p_api;

    /* Fill CSPL Header */
    rrc_construct_api_header(p_msg, RRC_VERSION_ID, RRC_S1AP_MODULE_ID,
            RRC_OAM_MODULE_ID, S1AP_OAM_INIT_IND, msg_len);

    /* Fill Interface Header */
    p_msg = p_msg + CV_HDRSIZE;
    rrc_construct_interface_api_header(p_msg, RRC_TRANSACTION_ID_ABSENT,
            RRC_S1AP_MODULE_ID, RRC_OAM_MODULE_ID, S1AP_OAM_INIT_IND, 0, RRC_NULL);

    RRC_S1AP_TRACE(RRC_INFO, "Src(%d)->Dst(%d):S1AP_OAM_INIT_IND", RRC_S1AP_MODULE_ID, RRC_OAM_MODULE_ID);
    /* Send message to OAM */
    /*SPR 20172 Fix Start*/
    rrc_send_message(p_api,RRC_OAM_MODULE_ID);
    /*SPR 20172 Fix End*/

    /* set the state to Wait for init cnf from OAM */
    S1AP_SET_FSM_STATE(p_s1ap_gb_ctx,S1AP_W_FOR_PROV_REQ);

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : s1ap_process_oam_reset_req
 * Inputs         : p_api -  Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data 
 * Outputs        : None
 * Returns        : None
 * Description    : The function processes reset request from OAM.
 *****************************************************************************/
void s1ap_process_oam_reset_req(
        void            *p_api,      
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{

    U16 trans_id = 0;
    s1ap_enb_reset_info_t  reset_input_info ;
    s1ap_reset_msg_ind_t     s1ap_reset_ind ;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    /* Get the transaction id */
    trans_id = rrc_get_transaction_id(p_api);

    memset_wrapper((void*)&reset_input_info, 0 ,sizeof(s1ap_enb_reset_info_t));

    /*Build RESET MSG to MME with 
     * choice type :s1 interface (RESET ALL) */

    reset_input_info.num_of_items = 0;
    /**** Cause ****/
    /* BUG_9257 START */
    reset_input_info.cause.t = T_s1ap_Cause_misc;
    reset_input_info.cause.u.radioNetwork = s1ap_om_intervention;
    /* BUG_9257 END */
    s1ap_add_and_process_reset_context(
            p_s1ap_gb_ctx,
            &reset_input_info,
            RRC_OAM_MODULE_ID,0,0,0);

    /* Build and send S1AP_RESET_MSG_IND to UECC with bitmask field
     * set to zero */
    memset_wrapper((void*)&s1ap_reset_ind,RRC_NULL,sizeof(s1ap_reset_msg_ind_t));

    s1ap_reset_ind.bitmask = 0;

    s1ap_build_and_send_reset_ind_msg
        (&s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

    RRC_S1AP_TRACE(RRC_INFO,"[%s]: Transaction id saved %d"
            " \n ",__FUNCTION__,trans_id);

    p_s1ap_gb_ctx->s1ap_oam_tx_info.s1ap_oam_transaction_id = trans_id;

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : s1ap_build_and_send_reset_ind_msg
 * Inputs         : p_s1ap_reset_ind -  Pointer to input API buffer 
 *                  dest_mod_id - destination id
 * Outputs        : None
 * Returns        : None
 * Description    : The function creates S1AP_REEST_IND message and copies 
 *                  the contents of p_s1ap_reset_ind into the message buffer 
 *                  created and sends the message to RRC(UECC).
 *****************************************************************************/
void s1ap_build_and_send_reset_ind_msg( 
        s1ap_reset_msg_ind_t *p_s1ap_reset_ind,
        U32 dest_mod_id)
{
    U8    *p_api = PNULL;
    U8    *p_msg = PNULL;
    U16   msg_len = RRC_NULL;

    msg_len = (U16)(sizeof(s1ap_reset_msg_ind_t));

    RRC_S1AP_UT_TRACE_ENTER();

    p_api = (U8*)s1ap_alloc_intrl_msg(RRC_UECC_MODULE_ID,
            S1AP_RESET_MSG_IND,
            msg_len);
    if(PNULL == p_api)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }
    if (PNULL != p_api)
    {
        p_msg = p_api + CV_HDRSIZE;

        l3_memcpy_wrapper((void*)p_msg,(const void*)p_s1ap_reset_ind, 
                sizeof(s1ap_reset_msg_ind_t));

        /* Send message to UECC */
    /*SPR 20172 Fix Start*/
        rrc_send_message(p_api,(rrc_module_id_t) dest_mod_id);
    /*SPR 20172 Fix End*/
    }

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : s1ap_build_and_send_oam_reset_resp
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP global context 
 * Outputs        : None
 * Returns        : RRC_SUCCESS - Message was successfully composed and sent 
 *                                to OAM.
 *                  RRC_FAILURE - Problem in message composition.
 * Description    : The function creates and send S1AP_OAM_RESET_RESP.
 *****************************************************************************/
rrc_return_et s1ap_build_and_send_oam_reset_resp(
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
    s1ap_oam_reset_resp_t    s1ap_oam_reset_resp;

    RRC_S1AP_UT_TRACE_ENTER();


    s1ap_oam_reset_resp.response = 
        p_s1ap_gb_ctx->s1ap_oam_tx_info.reset_response;

    p_s1ap_gb_ctx->s1ap_oam_tx_info.reset_response = RRC_SUCCESS;

    /* Encode and send the reset response to OAM */
    if (RRC_FAILURE == rrc_s1apOam_il_send_s1ap_oam_reset_resp(
                &s1ap_oam_reset_resp,
                RRC_S1AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                p_s1ap_gb_ctx->s1ap_oam_tx_info.s1ap_oam_transaction_id,
                RRC_NULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "build_and_send_s1ap_oam_reset_resp: "
                "rrc_s1apOam_il_send_s1ap_oam_reset_resp returned failure");
        return RRC_FAILURE;
    }
    RRC_S1AP_TRACE(RRC_INFO, "%s Sent Message to OAM for TX ID: %d "
            "\n",__FUNCTION__,p_s1ap_gb_ctx->s1ap_oam_tx_info.
            s1ap_oam_transaction_id);

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : s1ap_process_mme_reset_timer_expiry
 * Inputs         : p_s1ap_timer_buf -  Pointer to buffer associated with timer
 *                                       msg
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : None
 * Description    : The function process mme reset timer expiry. A check is 
 *                  performed to see whether max retries have been reached or
 *                  not. If max retries reached mme_reset_ack_count is 
 *                  decremented and reset response to be sent is set to 
 *                  failure else RESET is sent to MME.
 *****************************************************************************/
void s1ap_process_mme_reset_timer_expiry( 
        s1ap_timer_buffer_t     *p_s1ap_timer_buf,
        s1ap_gb_context_t        *p_s1ap_gb_ctx)
{
    reset_req_t    ** p_mme_reset_info_arr = PNULL;
    U8 mme_count= 0;
    mme_context_t *p_mme_context = PNULL;
    U16 asn_msg_len  = 0;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN]  = {0};
    reset_req_t  reset_req  ;
    reset_context_node_t * p_reset_context_node = PNULL;
    LOCAL_EVENT_NO_RESET_ACK_FROM_MME_STRUCT *p_no_reset_ack_from_mme = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1ap_timer_buf);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    memset_wrapper((void*)&reset_req, 0 ,sizeof(reset_req_t));

    p_reset_context_node = p_s1ap_timer_buf->p_timer_specific_data;
    if (PNULL != p_reset_context_node)
    {
        if (p_reset_context_node->reset_context.reset_retry_count 
                == p_s1ap_gb_ctx->max_retry_for_reset)
        {
            RRC_S1AP_TRACE(RRC_INFO,"[%s]: Reset retry count reach Max val"
                    " \n ",__FUNCTION__);
            
            /* Stop the timer set for RESET message */
            rrc_stop_timer(p_reset_context_node->reset_context.timer_id);
            p_reset_context_node->reset_context.timer_id = PNULL;

            /* Delete S1AP contexts per MMEs RESET Info */
            for (mme_count = 0; mme_count < MAX_NUM_MME ; mme_count++)
            {
                if (p_reset_context_node->reset_context.mme_reset_info[mme_count])
                {  
                    if (PNULL != p_s1ap_gb_ctx->mme_contexts[mme_count])
                    {
                        p_no_reset_ack_from_mme = 
                            rrc_mem_get(sizeof(LOCAL_EVENT_NO_RESET_ACK_FROM_MME_STRUCT));

                        if (PNULL != p_no_reset_ack_from_mme)
                        {
                            p_no_reset_ack_from_mme->header.length = 
                                sizeof(LOCAL_EVENT_NO_RESET_ACK_FROM_MME_STRUCT);

                            p_no_reset_ack_from_mme->header.event_id = 
                                LOCAL_EVENT_NO_RESET_ACK_FROM_MME;

                            s1ap_fill_gummei_info(p_s1ap_gb_ctx->mme_contexts[mme_count],
                                    &p_no_reset_ack_from_mme->EVENT_PARAM_GUMMEI);

                            l3_memcpy_wrapper(p_no_reset_ack_from_mme->EVENT_PARAM_MME_IP_ADDRESS,
                                    p_s1ap_gb_ctx->mme_contexts[mme_count]->
                                    connection_info.ip_addr[0].ip_addr,
                                    (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

                            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_no_reset_ack_from_mme);
                        }
                    }

                    if (!p_reset_context_node->reset_context.is_s1ap_ctx_deleted)
                    {
                        delete_reset_ue_s1ap_ctx(p_s1ap_gb_ctx,
                                p_reset_context_node->reset_context.
                                mme_reset_info[mme_count],
                                mme_count,
                                p_reset_context_node->reset_context.is_ue_ctx_deleted);
                    }

                    if (PNULL != 
                            p_reset_context_node->reset_context.mme_reset_info[mme_count])
                    {
                        rrc_mem_free(p_reset_context_node->
                                reset_context.mme_reset_info[mme_count]);
                        p_reset_context_node->reset_context.
                            mme_reset_info[mme_count] = PNULL;
                    }
                }
            }

            /* Set the response to be sent to OAM to failure
             */
            p_s1ap_gb_ctx->s1ap_oam_tx_info.reset_response
                =  RRC_FAILURE;

            if ( RRC_MAX_EXT_MODULE_ID 
                    >=  p_reset_context_node->reset_context.
                    send_resp_to_module)
            {
                switch (p_reset_context_node->reset_context.
                        send_resp_to_module)
                {
                    case RRC_OAM_MODULE_ID:
                        s1ap_build_and_send_oam_reset_resp(p_s1ap_gb_ctx);
                        RRC_TRACE(RRC_INFO,"%s RESPONSE Sent to "
                                "OAM\n",__FUNCTION__);
                        break;

                    default:
                        RRC_TRACE(RRC_WARNING,"%s Resp to be sent to "
                                "UNKNOWN MODULE\n",__FUNCTION__);
                }
            }
            /* Delete reset_context from the reset_context_list */

            /* Remove the reset_context from global reset_context_list */
            list_delete_node(&p_s1ap_gb_ctx->reset_context_list,
                    &p_reset_context_node->node);

            rrc_mem_free(p_reset_context_node);

        }
        else
        {
            /* Increment the reset retry count */
            p_reset_context_node->reset_context.reset_retry_count++;

            RRC_S1AP_TRACE(RRC_INFO,"[%s]: Re sending RESET REQ "
                    "retry count %d Max retry count %d \n ",__FUNCTION__,
                    p_reset_context_node->reset_context.reset_retry_count,
                    p_s1ap_gb_ctx->max_retry_for_reset);

            /* For all the MMEs to which RESET message is to be sent,
               compose RESET ASN Message and send */
            p_mme_reset_info_arr = &(p_reset_context_node->reset_context.
                    mme_reset_info[0]);

            for (mme_count = 0; mme_count < MAX_NUM_MME ; mme_count++)
            {
                if (p_mme_reset_info_arr[mme_count])
                {
                    if (RRC_FAILURE == 
                            compose_asn_reset(p_mme_reset_info_arr[mme_count],
                                asn_msg_buff,
                                &asn_msg_len))
                    {
                        RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                                "compose_asn_reset returned failure.",
                                __FUNCTION__);
                        continue;
                    }

                    /*CSR 51442 Start*/
                    ALARM_MSG_L3(RRC_MODULE_ID, S1AP_RESET_ALARM_ID, MAJOR_ALARM, INVALID_ERR_CAUSE_ID);
                    /*CSR 51442 Stop*/
                    
                    p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_count];
 
                    /* Check whether MME's sctp assoc is active or not */
                    if (p_mme_context->current_mme_state != MME_ACTIVE)
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%s:"
                                " MME context with id %d is not active.",
                                __FUNCTION__,p_mme_context->mme_id);
                        continue;
                    }
#ifndef LINUX_PC_TEST
                    if (SCTP_SOCKET_ERROR == l3_sctp_send(
                                p_mme_context->sctp_sd,
                                asn_msg_buff,
                                asn_msg_len,
                                p_mme_context
                                ->stream_info.stream_ids[
                                SCTP_COMMON_PROC_STREAMS]))
#else
			if(SCTP_SOCKET_ERROR == pctest_forward_mme_message_to_pc(
						RRC_S1AP_MODULE_ID,
						RRC_MME_MODULE_ID,
						MME_MESSAGE_PS_TO_TEST,
						p_mme_context->sctp_sd,
						p_mme_context->stream_info.stream_ids[SCTP_COMMON_PROC_STREAMS], //stream id
						asn_msg_buff,
						asn_msg_len))

#endif                                
                    {
                        RRC_S1AP_TRACE(RRC_WARNING,"[%s]:l3_sctp_send"
                                " failed\n",__FUNCTION__);
                    }
                    else
                    {
                        RRC_S1AP_TRACE(RRC_INFO,"[%s]:RESET resent to MME\n"
                                ,__FUNCTION__);
                    }
                }
            }
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]:"
                "p_reset_context_node is NULL n",__FUNCTION__);

    }
    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : s1ap_oam_process_stats_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepares the statistics of all active MMEs
 *                  sends them to OAM.
 ****************************************************************************/
void 
s1ap_oam_process_stats_req(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{

    /* CSR 00058587 Fix Start */
    s1ap_oam_status_resp_t     s1ap_oam_status_resp ;

    U16 counter  = RRC_NULL;
    U8 mme_id   = RRC_NULL;
    U16 trans_id = RRC_NULL;
    U32 ue_count = RRC_NULL;
    U32 active_mme_count = RRC_NULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Get the Transaction Id from API header */
    trans_id = rrc_get_transaction_id(p_api);

    memset_wrapper((void*)&s1ap_oam_status_resp,RRC_NULL,
                          sizeof(s1ap_oam_status_resp_t));

    /* Populating the data structure for s1ap statistics request from OAM */    
    s1ap_oam_status_resp.active_mme_count = 
        (p_s1ap_gb_ctx->active_mme_info).active_mme_count;

    active_mme_count = s1ap_oam_status_resp.active_mme_count;

    for (counter = 0; counter < active_mme_count; counter++)
    {
        mme_id = (p_s1ap_gb_ctx->active_mme_info).mme_id_arr[counter].mme_id;

        ue_count = st_get_node_count( 
                &(p_s1ap_gb_ctx->mme_contexts[mme_id]->
                    mmeUeS1apId_search_tree));

        s1ap_oam_status_resp.active_mme_list[counter]. 
                                      count_of_ue = (U16)ue_count;
        s1ap_oam_status_resp.active_mme_list[counter].mme_id = mme_id;

    }

    if (RRC_FAILURE == rrc_s1apOam_il_send_s1ap_oam_status_resp(
                &s1ap_oam_status_resp,
                RRC_S1AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                trans_id,
                RRC_NULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING, 
                "rrc_s1apOam_il_send_s1ap_oam_stats"
                "_resp returned failure\n");
    }
    /* CSR 00058587 Fix Stop */

    RRC_S1AP_UT_TRACE_EXIT();

}

#ifdef LINUX_PC_TEST
/*****************************************************************************
 * Function Name  : s1ap_process_gb_status_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepares the statistics of global context
 *                  sends them to PC-TEST site.
 ****************************************************************************/
void 
s1ap_process_gb_status_req(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
	s1ap_gb_status_resp_t s1ap_gb_status_resp;
	U8 counter = 0;
	U8 mme_id = 0; 

	memset_wrapper((void*)&s1ap_gb_status_resp,RRC_NULL,
							  sizeof(s1ap_gb_status_resp_t));

	s1ap_gb_status_resp.s1ap_current_state = p_s1ap_gb_ctx->s1ap_current_state;
	s1ap_gb_status_resp.current_enb_mgr_state = p_s1ap_gb_ctx->current_enb_mgr_state;

	s1ap_gb_status_resp.mme_count = 
        (p_s1ap_gb_ctx->active_mme_info).active_mme_count;

	for (counter = 0; counter < s1ap_gb_status_resp.mme_count; counter++)
	{
		s1ap_gb_status_resp.mme_info[counter].mme_id =
			(p_s1ap_gb_ctx->active_mme_info).mme_id_arr[counter].mme_id;

		mme_id = s1ap_gb_status_resp.mme_info[counter].mme_id;			
		s1ap_gb_status_resp.mme_info[counter].ue_count = 
			st_get_node_count( &(p_s1ap_gb_ctx->mme_contexts[mme_id]->mmeUeS1apId_search_tree));
	
		s1ap_gb_status_resp.mme_info[counter].current_mme_state = 
			p_s1ap_gb_ctx->mme_contexts[counter]->current_mme_state;			
	}

	for(counter = 0; counter < MAX_CELL_NUM;counter++)
	{
		if (PNULL == p_s1ap_gb_ctx->cell_s1ap_contexts[counter])
			continue;

		s1ap_gb_status_resp.cell_count ++;
	}

	rrc_s1apOam_il_send_s1ap_gb_status_resp(
					&s1ap_gb_status_resp,
					RRC_S1AP_MODULE_ID,
					RRC_OAM_MODULE_ID,
					rrc_get_transaction_id(p_api),
					RRC_NULL);

	return;	
}
#endif

/*****************************************************************************
 *  Function Name  : compare_mmeUeS1apId_search_tree
 *  Inputs         : p_elem1        - Pointer to first element being compared
 *                   p_elem2        - Pointer to second element being compared
 *  Outputs        : None
 *  Returns        : 0 : if both elements are equal
 *                   1 :if first is greater than second.
 *                   2 if first is less than second.
 *  Description    : This function is used by CSPL Tree types to compare
 *                   elements in a tree.This function compares data of type U32.
 *****************************************************************************/
S32
compare_mmeUeS1apId_search_tree(
        const void * p_elem1,
        const void * p_elem2)
{
    if (p_elem1 == PNULL || p_elem2 == PNULL)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s: passed null arg",__FUNCTION__);
        return ERROR_IN_COMPARE;
    }

    if ( * ((const U32 *) p_elem1) == * ((const U32 *) p_elem2))
    {
        return FIRST_IS_EQUAL_TO_SECOND;
    }
    else if ( * ((const U32 *) p_elem1) < * ((const U32 *) p_elem2))
    {
        return FIRST_IS_LESS_THAN_SECOND;
    }
    else /*( * ((U32 *) p_elem1) > * ((U32 *) p_elem2))*/
    {
        return FIRST_IS_GREATER_THAN_SECOND;
    }
}

/*****************************************************************************
 * Function Name  : key_of_mmeUeS1apId_search_tree
 * Inputs         : p_node             - Pointer to Tree node
 * Outputs        : None
 * Returns        : PNULL: Error
 *                  Pointer to key element in this case sctp_sd.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/

const void *
key_of_mmeUeS1apId_search_tree(
        const YTNODE * p_node)
{
    return (p_node) ? &(((const mmeUeS1apId_search_node_t *)
                p_node)->mmeUeS1apId) : PNULL;
}

/*****************************************************************************
 * Function Name  : establish_sctp_associations
 * Inputs         : p_s1ap_gb_ctx        - pointer to S1AP global context
 *                  p_s1_setup_info_req  - Pointer to s1_setup_info in prov req
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when S1AP_OAM_PROV_REQ message is
 *                  received from OAM. The event in this case is
 *                  EVENT_S1_MME_INIT_REQ.
 *****************************************************************************/
void  establish_sctp_associations(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        s1apInfo_t * p_s1_setup_info_req)
{
    mme_context_t * p_mme_ctx = PNULL;
    U8 num_mme = RRC_NULL;
    U8 mme_count= 0;
    U8 mme_index =0;
    /* SPR 13432 Fix Start */
    U8 num_mme_connected = S1AP_FALSE;
    /* SPR 13432 Fix Stop */

    num_mme = p_s1_setup_info_req->num_mme;

    p_s1ap_gb_ctx->num_of_mme_prov = num_mme;

    /* Initialize the enb_config_updated_count to zero */
    p_s1ap_gb_ctx->enb_config_updated_gb_count = 0;

    for ( mme_count = 0 ; mme_count < num_mme ; mme_count++)
    {
        /*Create a mme_context at p_gb_context->mme_contexts[mme_count]*/
        /* Allocate Memory for UE global context data structure */
        p_mme_ctx = (mme_context_t *)rrc_mem_get(sizeof(
                    mme_context_t));

        if (PNULL == p_mme_ctx)
        {
            RRC_S1AP_TRACE(RRC_FATAL, "p_mme_ctx: unable to"
                    " Allocate Memory for MME Global context!!");
            return /*PNULL*/;
        }

        memset_wrapper((void*)p_mme_ctx, 0, sizeof(mme_context_t));

        /* fill mme comm info */
        p_mme_ctx->mme_id = mme_index;

        p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;
        /*Storing the UE context in the S1AP global contexts*/
        p_s1ap_gb_ctx->mme_contexts[mme_index++] = p_mme_ctx;

        /*Set the elements of the mme_context*/
        /*current_mme_state  = MME_IDLE*/
        p_mme_ctx->current_mme_state = MME_IDLE ;

        p_mme_ctx->current_enb_config_update_state = 
            ENB_STATE_RECV_ENB_CONFIG_FROM_OAM;

        p_mme_ctx->p_enb_configuration_update = PNULL;


        /*Filling mme connection_info */
        l3_memcpy_wrapper ((void*)&p_mme_ctx->connection_info,
                (const void*)&(p_s1_setup_info_req->mme_comm_info[mme_count]), 
                sizeof ( mme_comm_info_t));

        p_mme_ctx->s1_config_retry_count = 0;
        p_mme_ctx->oam_transaction_id = 0;
        p_mme_ctx->enb_config_updated_count = 0;
        /*mmeUeS1apId Tree Initialisation*/
        st_init(&(p_mme_ctx->mmeUeS1apId_search_tree),
                &compare_mmeUeS1apId_search_tree,
                key_of_mmeUeS1apId_search_tree);

        /* SPR 13432 Fix Start */
        if (p_s1ap_gb_ctx->s1_conn_mode_one_flag)
        {
            if ((S1AP_FALSE == num_mme_connected) &&
                    (MAX_NUM_MME == p_s1ap_gb_ctx->last_selected_mme_id))
            {
                mme_fsm_process_event(
                    MME_EVENT_S1_MME_PROV_REQ,
                    &(p_mme_ctx->connection_info),
                    &(p_s1_setup_info_req->enb_comm_info),
                    p_s1ap_gb_ctx,
                    p_mme_ctx);
                
                num_mme_connected = S1AP_TRUE;
            }
        }
        else
        {
            mme_fsm_process_event(
                MME_EVENT_S1_MME_PROV_REQ,
                &(p_mme_ctx->connection_info),
                &(p_s1_setup_info_req->enb_comm_info),
                p_s1ap_gb_ctx,
                p_mme_ctx);

        }
        /* SPR 13432 Fix Stop */
    }
}


/*****************************************************************************
 * Function Name  : process_s1ap_oam_prov_req
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the provisioning request 
 *                  received from OAM module to establish SCTP 
 *                  associations and sends the response back to OAM.
 ****************************************************************************/
void s1ap_process_oam_provision_req(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1apInfo_t *p_s1ap_info = PNULL;

    s1ap_oam_provision_req_t s1ap_prov_req ;
    S32 length_read = RRC_NULL;
    s1ap_oam_provision_resp_t s1ap_oam_prov_resp = {0} ;
    rrc_return_et response = RRC_FAILURE;
    U16 trans_id = RRC_NULL;
    static rrc_bool_et is_initialised_once = RRC_FALSE;
    /* SPR Fix 5479 Start */
    U8 cell_index = RRC_NULL;
    /* SPR Fix 5479 End */
/*BUG 604 changes start*/
    /*valgrind_fix_start*/
    /*rrc_return_t result = RRC_FAILURE;*/
    /*valgrind_fix_start*/
/*BUG 604 changes stop*/

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    /* Get the transaction id */
    trans_id = rrc_get_transaction_id(p_api);

    /* Check the state of s1ap */
    if (S1AP_W_FOR_PROV_REQ != p_s1ap_gb_ctx->s1ap_current_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "S1ap is in active state but should "
                "ideally wait for provisioning request");
        return;
    }

    /* Decode the received message from OAM */
    response = rrc_il_parse_s1ap_oam_provision_req(
            &s1ap_prov_req ,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - 
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    if (RRC_SUCCESS == response)
    {
        RRC_S1AP_TRACE(RRC_INFO, "S1AP OAM PROV REQ decode success");
        RRC_S1AP_TRACE(RRC_INFO, "MME Count is ---- [%d]\n",s1ap_prov_req.s1apInfo.num_mme);
        s1ap_oam_prov_resp.response = RRC_SUCCESS;

        /* SPR 1643 Start */
        /* PLMN Identity Validation */
        if (RRC_FALSE == 
                validate_s1ap_oam_plmn_identity(&s1ap_prov_req.
                    s1apInfo.s1_setup_req_parameters))
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "[S1AP_OAM_PROV_REQ]:PLMN ID Range Invalid");
            s1ap_oam_prov_resp.response = RRC_FAILURE;
        }
        /* SPR 1643 Stop */
        if (RRC_FALSE == 
                validate_s1ap_oam_ip_addresses(&s1ap_prov_req.s1apInfo))
        {
            s1ap_oam_prov_resp.response = RRC_FAILURE;
        }

		/*bugid 117155 begin*/
		if((s1ap_prov_req.s1apInfo.num_mme > 1) &&
				(s1ap_prov_req.s1apInfo.bitmask & S1AP_OAM_MME_S1_CONN_MODE_ONE_PRESENT))
		{
			RRC_S1AP_TRACE(RRC_WARNING,
                    "[S1AP_OAM_PROV_REQ]:mme number must equal to 1 under s1 connect mode one.");
            s1ap_oam_prov_resp.response = RRC_FAILURE;
		}
		/*bugid 117155 end*/			
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING, "S1AP OAM PROV REQ decode failed");
        s1ap_oam_prov_resp.response = RRC_FAILURE;        
    }

//#ifndef RRC_UNIT_TEST_FRAMEWORK
    if(s1ap_prov_req.s1apInfo.bitmask & S1AP_OAM_S1_HEALTH_MONITORING_TIMER_PRESENT)
    {
/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
        s1ap_lock = qvNewLock(); 
#endif
/*Lockless Implementation stop*/
        start_s1ap_health_monitor_timer(/*SPR 17777 +-*/
                ((s1ap_prov_req.s1apInfo.s1ap_health_mon_time_int * 1000)/2));
/*Lockless Implementation start*/
#ifdef LOCKLESS_IMPL
 /*Bug 9305 start*/
        sync_fetch_and_and_wrapper(&s1ap_sctp_gl_health_monitor_timer_val,0,PNULL);
        sync_fetch_and_add_wrapper(&s1ap_sctp_gl_health_monitor_timer_val,
                ((s1ap_prov_req.s1apInfo.s1ap_health_mon_time_int * 1000)/2),PNULL);
 /*Bug 9305 end*/
#else
 /*Bug 9305 start*/
        sync_fetch_and_and_wrapper(&s1ap_sctp_gl_health_monitor_timer_val,
            0,s1ap_lock);
        sync_fetch_and_add_wrapper(&s1ap_sctp_gl_health_monitor_timer_val,
            ((s1ap_prov_req.s1apInfo.s1ap_health_mon_time_int * 1000)/2),s1ap_lock);
 /*Bug 9305 end*/
#endif
/*Lockless Implementation stop*/
   
    }
//#endif

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    s1ap_change_logging_parameters(p_s1ap_gb_ctx,
        get_s1ap_log_level_from_oam_log_level(s1ap_prov_req.s1apInfo.log_level),
        p_s1ap_gb_ctx->log_enabled);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

    /* Encode and send the provisioning response to OAM */
    response = rrc_s1apOam_il_send_s1ap_oam_provision_resp(
            &s1ap_oam_prov_resp,
            RRC_S1AP_MODULE_ID,
            RRC_OAM_MODULE_ID,
            trans_id,
            RRC_NULL);

    if (RRC_SUCCESS == response)
    {
        if (s1ap_oam_prov_resp.response == RRC_SUCCESS)
        {
            RRC_S1AP_TRACE(RRC_INFO, "S1AP OAM PROV RESP encode success");

            if (RRC_FALSE == is_initialised_once)
            {
                if (p_s1ap_gb_ctx->oam_init_ind_timer)
                {
                    rrc_stop_timer(p_s1ap_gb_ctx->oam_init_ind_timer);
                    p_s1ap_gb_ctx->oam_init_ind_timer =PNULL;
                }
            }

            p_s1ap_info = 
                &(s1ap_prov_req.s1apInfo);
            //Trace Change Start
            //memset_wrapper() s1_setup_info in s1ap global context.
            //Incase optional parameter enbName is not in the message then a NULL value will get set
           
            memset_wrapper(&(p_s1ap_gb_ctx->s1_setup_info), RRC_NULL, sizeof(s1_setup_req_t));
            
            //Trace Change End

            l3_memcpy_wrapper((void*)(&(p_s1ap_gb_ctx->s1_setup_info)),
                    (const void* )(&p_s1ap_info->s1_setup_req_parameters),
                    sizeof(s1_setup_req_t));

            l3_memcpy_wrapper((void*)&(p_s1ap_gb_ctx->enb_comm_info),
                    (const void*)(&p_s1ap_info->enb_comm_info),
                    sizeof(enb_comm_info_t));

            /* SPR Fix 5479 Start */
            for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
            {
                if (PNULL != p_s1ap_gb_ctx->cell_s1ap_contexts[cell_index])
                {
                    l3_memcpy_wrapper(p_s1ap_gb_ctx->cell_s1ap_contexts[cell_index]->
                           s1ap_cell_area_info.plmn_id.plmn_id,
                            p_s1ap_gb_ctx->s1_setup_info.gb_enb_id.plmn_identity.plmn_id,
                            sizeof(p_s1ap_gb_ctx->s1_setup_info.gb_enb_id.plmn_identity.plmn_id));
                }
            }
            /* SPR Fix 5479 End */

            p_s1ap_gb_ctx->s1ap_current_state = S1AP_ACTIVE;
            p_s1ap_gb_ctx->max_retry_for_s1_config = 
                p_s1ap_info->max_s1_config_retries;
            p_s1ap_gb_ctx->max_retry_for_reset = 
                p_s1ap_info->max_reset_retries;
            p_s1ap_gb_ctx->s1_config_resp_duration = 
                (U16) p_s1ap_info->s1_config_timer;

            p_s1ap_gb_ctx->reset_duration = 
                (U16) p_s1ap_info->reset_ep_timer;

            p_s1ap_gb_ctx->s1_pws_duration = 
                (U16) p_s1ap_info->s1ap_pws_timer;

            p_s1ap_gb_ctx->s1_kill_duration = 
                (U16) p_s1ap_info->s1ap_kill_timer;
            
            if(S1AP_OAM_MME_SELECTION_ALGO_PRESENT & p_s1ap_info->bitmask)
            {
                p_s1ap_gb_ctx->mme_selection_algo = 
                    p_s1ap_info->mme_selection_algo;
            }
            else
            {
                p_s1ap_gb_ctx->mme_selection_algo = MME_ROUND_ROBIN_ALGO;
            }
            /* SPR_5240 Start */
            if(S1AP_OAM_S1_CONN_RECOVERY_DURATION_PRESENT & p_s1ap_info->bitmask)
            {
                p_s1ap_gb_ctx->s1ap_conn_recovery_duration = 
                    (U16) p_s1ap_info->s1_conn_recovery_duration;
            }
            /* SPR_5240 Stop */
            /*sctp parameters start*/
            if(S1AP_OAM_SCTP_CONF_INFO_PRESENT & p_s1ap_info->bitmask)
            {
                p_s1ap_gb_ctx->bitmask |= 
                    RRC_S1AP_SCTP_CONFIG_PARAMS_PRESENT;
    
                /* dscp_marking_start */
                if (S1AP_OAM_DSCP_VALUE_PRESENT & p_s1ap_info->s1ap_sctp_conf_param.bitmask)
                {
                    p_s1ap_gb_ctx->s1ap_sctp_conf_param.bitmask |= 
                        S1AP_OAM_DSCP_VALUE_PRESENT;
                }
                /* dscp_marking_stop */

                l3_memcpy_wrapper(&p_s1ap_gb_ctx->s1ap_sctp_conf_param,
                        &p_s1ap_info->s1ap_sctp_conf_param,
                        sizeof(s1ap_sctp_conf_info_t));
            }
            /*sctp parameters stop */
            /* SPR 16117 Fix start */
            /* code removed */
            /* SPR 16117 Fix end */


            is_initialised_once = RRC_TRUE;

            /* SPR 13432 Fix Start */
            if ( p_s1ap_info->bitmask & S1AP_OAM_MME_S1_CONN_MODE_ONE_PRESENT )
            {
                p_s1ap_gb_ctx->s1_conn_mode_one_flag = S1AP_TRUE;
            }
            /* SPR 13432 Fix Stop */

            if (0 != p_s1ap_info->num_mme)
            {
                /* Function call to form sctp association */
                establish_sctp_associations(p_s1ap_gb_ctx, p_s1ap_info);
                RRC_S1AP_TRACE(RRC_INFO, "S1AP has entered Active State");
            }
        }
        else
        {
            ALARM_MSG_L3(RRC_MODULE_ID, S1AP_PROVISIONING_FAILURE_ALARM_ID, MAJOR_ALARM,INVALID_ERR_CAUSE_ID);
        }

    }
    else
    {
        ALARM_MSG_L3(RRC_MODULE_ID, S1AP_PROVISIONING_FAILURE_ALARM_ID, MAJOR_ALARM,INVALID_ERR_CAUSE_ID);
        RRC_S1AP_TRACE(RRC_WARNING, "S1AP OAM PROV RESP encode failed");
        return;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : s1ap_process_oam_cleanup_req
 * Inputs         : p_api - pointer to input api data 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function cleans all the memory allocated for the 
 *                  contexts created from the CSPL pool during the process of 
 *                  s1ap module.
 ****************************************************************************/
void s1ap_process_oam_cleanup_req(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    reset_context_node_t * p_reset_context_node = PNULL;
    reset_context_node_t * p_reset_context_next_node = PNULL;
    search_tree_node_t *p_temp = PNULL;    
    search_tree_node_t *p_next = PNULL;    

    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node_next = PNULL;

    U32 mme_count = 0;
    S32 socket = INVALID_SCTP_SOCKET;
    U16 trans_id = RRC_NULL;
    /* Get the transaction id */
    trans_id = rrc_get_transaction_id(p_api);

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    if (p_s1ap_gb_ctx->oam_init_ind_timer)
    {
        rrc_stop_timer(p_s1ap_gb_ctx->oam_init_ind_timer);
        p_s1ap_gb_ctx->oam_init_ind_timer =PNULL;
    }
    /* To delete the UE contexts cell wise */
    s1ap_delete_all_ue_contexts(p_s1ap_gb_ctx,RRC_TRUE);
    /* To delete the search tree whose key value is sctp sd */
    /* get the first node of the search tree */
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::NUM of SCTP SDS to be deleted %d\n",
            __FUNCTION__,
        st_get_node_count(&p_s1ap_gb_ctx->sctp_socket_desc_search_tree));

    p_temp = st_get_first_node(&(p_s1ap_gb_ctx->
                sctp_socket_desc_search_tree));

    p_next = p_temp;

    /*delete the nodes one by one */
    while(PNULL != p_next)
    {
        p_next = st_get_next_node(p_temp);
        st_delete_node(&(p_s1ap_gb_ctx->sctp_socket_desc_search_tree),
                p_temp);
        rrc_mem_free(p_temp);
        p_temp = p_next;
    }
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::ALL SCTP SDS deleted count %d\n",
            __FUNCTION__,
           st_get_node_count(&p_s1ap_gb_ctx->sctp_socket_desc_search_tree));

    p_temp = PNULL;
    /* To delete the search tree of mme contexts one by one 
       whose key value is mmeues1apid */
    for(mme_count = 0; mme_count < MAX_NUM_MME; mme_count++)
    {
        /* get the first node of the search tree */
        if (PNULL != p_s1ap_gb_ctx->mme_contexts[mme_count])
        {
            /* Free served gummeis */
            if (p_s1ap_gb_ctx->mme_contexts[mme_count]->
                    p_served_gummeis)
            {
                free_served_gummeis( p_s1ap_gb_ctx->mme_contexts
                        [mme_count]->p_served_gummeis, 
                        p_s1ap_gb_ctx->mme_contexts[mme_count]->
                        num_served_gummeis);
                rrc_mem_free(p_s1ap_gb_ctx->mme_contexts[mme_count]->
                           p_served_gummeis);
                p_s1ap_gb_ctx->mme_contexts[mme_count]->p_served_gummeis =
                               PNULL;
            }

            p_temp = st_get_first_node(&(p_s1ap_gb_ctx->
                        mme_contexts[mme_count]->mmeUeS1apId_search_tree));
            p_next = p_temp;
        }

        /*delete the nodes one by one */
        while(PNULL != p_next)
        {
            p_next = st_get_next_node(p_temp);
            st_delete_node(
                    &(p_s1ap_gb_ctx->mme_contexts[mme_count]->
                        mmeUeS1apId_search_tree),
                    p_temp);
            rrc_mem_free(p_temp);
            p_temp = p_next;
        }
        p_temp = PNULL;
    }
    /* To delete the MME contexts */
    for (mme_count = 0; mme_count < MAX_NUM_MME; mme_count++)
    {
        if (PNULL != p_s1ap_gb_ctx->mme_contexts[mme_count])
        {
            socket = p_s1ap_gb_ctx->mme_contexts[mme_count]->sctp_sd;

            if(INVALID_SCTP_SOCKET != socket)
            {
                if(RRC_SUCCESS != l3_sctp_close_association(socket))
                {
                    RRC_S1AP_TRACE(RRC_ERROR,
                        "[%s]L3 sctp close association failed:" , __FUNCTION__);
                }
            }




            /*Stop enb_config_update_timer_id guard timer*/
            if (PNULL != p_s1ap_gb_ctx->mme_contexts[mme_count]->enb_config_update_timer_id)
            {    
                RRC_S1AP_TRACE(RRC_INFO,"[%s]::Stop timer "
                        "[S1AP_ENB_CONFIGURAION_UPDATE_TIMER]",
                        __FUNCTION__);
                rrc_stop_timer(p_s1ap_gb_ctx->mme_contexts[mme_count]->enb_config_update_timer_id);
                p_s1ap_gb_ctx->mme_contexts[mme_count]->enb_config_update_timer_id = PNULL;
            }    

            if (PNULL != p_s1ap_gb_ctx->mme_contexts[mme_count]->mme_timer_id)
            {
                rrc_stop_timer( p_s1ap_gb_ctx->mme_contexts[mme_count]->
                        mme_timer_id);
                p_s1ap_gb_ctx->mme_contexts[mme_count]->mme_timer_id = 
                    PNULL;
            }

            if (p_s1ap_gb_ctx->mme_contexts[mme_count]->p_enb_configuration_update)
            {
                rrc_mem_free(p_s1ap_gb_ctx->mme_contexts[mme_count]->p_enb_configuration_update);
                p_s1ap_gb_ctx->mme_contexts[mme_count]->p_enb_configuration_update = PNULL;
            }

            rrc_mem_free(p_s1ap_gb_ctx->mme_contexts[mme_count]);
            p_s1ap_gb_ctx->mme_contexts[mme_count] = PNULL;
        }
    }

    /* Reseting active_mme_count to zero */
    (p_s1ap_gb_ctx->active_mme_info).active_mme_count = S1AP_ZERO;
    RRC_S1AP_TRACE(RRC_INFO,"Active MME Count is set to zero");

    /* Delete RESET context list */
    p_reset_context_next_node = (reset_context_node_t *)
        list_first_node(&p_s1ap_gb_ctx->
                reset_context_list);

    while(p_reset_context_next_node)
    {
        p_reset_context_node = p_reset_context_next_node;

        p_reset_context_next_node = (reset_context_node_t *)
            list_next_node(&p_reset_context_node->node);

        if (p_reset_context_node->reset_context.timer_id)
        {
            rrc_stop_timer(p_reset_context_node->reset_context.timer_id);
        }

        for (mme_count = 0;
                mme_count < p_reset_context_node->reset_context.mme_count;
                mme_count++)
        {
            if (p_reset_context_node->reset_context.mme_reset_info[mme_count])
            {
                rrc_mem_free(p_reset_context_node->reset_context.
                               mme_reset_info[mme_count]);
                p_reset_context_node->reset_context.mme_reset_info
                                     [mme_count] = PNULL;
            }
        }
        /* Delete reset_context from the reset_context_list */
        list_delete_node(&p_s1ap_gb_ctx->reset_context_list,
                &p_reset_context_node->node);

        if (PNULL != p_reset_context_node)
        {
            rrc_mem_free((void*)p_reset_context_node);
            p_reset_context_node = PNULL;
        }
    }
    p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
        list_first_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list);

    while(p_ho_ongoing_ue_ctx_node)
    {
        /* Delete ho_ongoing_ue_ctx from the ho_ongoing_ue_ctx_list */
        list_delete_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list,
                (void *)p_ho_ongoing_ue_ctx_node);
        RRC_S1AP_TRACE(RRC_INFO,"%s HO ONGOING UE Context DELETED"
                "\n",__FUNCTION__);
        p_ho_ongoing_ue_ctx_node_next = (ho_ongoing_ue_ctx_node_t *)
            list_next_node(&p_ho_ongoing_ue_ctx_node->node);

        if (PNULL != p_ho_ongoing_ue_ctx_node)
        {
            rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
            p_ho_ongoing_ue_ctx_node = PNULL;
        }

        p_ho_ongoing_ue_ctx_node = p_ho_ongoing_ue_ctx_node_next; 
    }

    /* Cleaning the pws info from s1ap ctx */
    if (p_s1ap_gb_ctx->p_pws_info != PNULL)
    {
        rrc_mem_free(p_s1ap_gb_ctx->p_pws_info);
        p_s1ap_gb_ctx->p_pws_info = PNULL;
    }
        
    /*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
    if(PNULL != s1ap_lock)
    {
        qvDestroyLock(s1ap_lock);
        s1ap_lock = PNULL;
    }
#endif
    /*Lockless Implementation stop*/
    S1AP_SET_FSM_STATE(p_s1ap_gb_ctx,S1AP_W_FOR_PROV_REQ);
    /* sending clean respone to OAM */
    s1ap_send_oam_cleanup_resp(RRC_S1AP_MODULE_ID,trans_id);	
	/*bug 115771 begin*/
	s1ap_gb_ctx_reset(p_s1ap_gb_ctx);
	/*bug 115771 end*/
#ifdef LINUX_PC_TEST
	test_s1ap_init();
#endif
    RRC_S1AP_UT_TRACE_EXIT();

}

/*****************************************************************************
 * Function Name  : s1ap_delete_all_ue_contexts
 * Inputs         : p_s1ap_ue_context - Pointer to the ue context
 *                  delete_cell_ctx - boolean value of type rrc_bool_et 
 * Outputs        : None
 * Returns        : none
 * Description    : This function cleans the memory allocated for the 
 *                  ue context of an UE.
 ****************************************************************************/
void s1ap_delete_all_ue_contexts(
        s1ap_gb_context_t   *p_s1ap_gb_ctx,
        rrc_bool_et delete_cell_ctx)    
{
    U32 cell_count = 0;
    U16 ue_count = 0;
    ue_s1ap_context_t     *p_s1ap_context = PNULL;

    if (PNULL == p_s1ap_gb_ctx)
    {
        RRC_S1AP_TRACE(RRC_WARNING," Unable to get "
                "the address of s1ap global context ");
        return ;
    }
    /*Deleting UE context present in all the UECC instances */
    /* To delete the UE contexts cell wise */
/*BUG 604 changes start*/
    if (p_s1ap_gb_ctx->p_p_ue_s1ap_contexts != PNULL )
    {
        for (ue_count = 0; ue_count < p_s1ap_gb_ctx->
                total_ue_supported; ue_count++)
        {
            p_s1ap_context = p_s1ap_gb_ctx->p_p_ue_s1ap_contexts[ue_count];

            if (PNULL != p_s1ap_context)
            {
                SET_UE_INDEX(ue_count);

                rrc_mem_free(p_s1ap_context);

                /*Setting value of p_p_ue_s1ap_contexts in global context as
                 * PNULL */
                p_s1ap_gb_ctx->p_p_ue_s1ap_contexts[ue_count] = PNULL;
            }
        }
        rrc_mem_free(p_s1ap_gb_ctx->p_p_ue_s1ap_contexts);
        p_s1ap_gb_ctx->p_p_ue_s1ap_contexts = PNULL;
        /* Free all s1ap UEID ctx*/
        if(PNULL != p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids)
        {
            rrc_mem_free(p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids);
            p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids = PNULL;
        }
        p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur = 0;
        p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count = 0;
        /*BUG 604 changes stop*/

    }
/*BUG 604 changes stop*/
    
    if (RRC_TRUE == delete_cell_ctx)
    {
        for (cell_count = 0; cell_count < MAX_NUM_CELLS; cell_count++)
        {
            if (PNULL != p_s1ap_gb_ctx-> cell_s1ap_contexts[cell_count])
            {
                SET_CELL_INDEX(cell_count);

                rrc_mem_free(p_s1ap_gb_ctx->cell_s1ap_contexts[cell_count]);
                p_s1ap_gb_ctx->cell_s1ap_contexts[cell_count] = PNULL;
            }
        }
    }
}


/*****************************************************************************
 * Function Name  : s1ap_send_oam_cleanup_resp
 * Inputs         : src_module_id - source id
 *                  trans_id - transaction id
 * Outputs        : None
 * Returns        : none
 * Description    : This function sends the cleanup responce to OAM .
 ****************************************************************************/
void s1ap_send_oam_cleanup_resp(
        U16 src_module_id,
        U16 trans_id)
{
    U8* p_msg = PNULL;

    U16   total_msg_len = CV_HDRSIZE + RRC_INTERFACE_API_HEADER_SIZE;
    U16   msg_len = 0;


    p_msg = (U8*) rrc_msg_mem_get(total_msg_len);

    if (PNULL == p_msg)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s:rrc_msg_mem_get failed",
                __FUNCTION__);
        return;
    }        

    /* Fill CSPL header */
    rrc_construct_api_header(
            p_msg, 
            RRC_VERSION_ID, 
            src_module_id,
            RRC_OAM_MODULE_ID, 
            S1AP_OAM_CLEANUP_RESP,
            total_msg_len);

    /* Fill interface header */
    rrc_construct_interface_api_header(
            p_msg + RRC_API_HEADER_SIZE, 
            trans_id,
            src_module_id,
            RRC_OAM_MODULE_ID,
            S1AP_OAM_CLEANUP_RESP,
            msg_len,
            RRC_NULL);

    /* Send message to */
    /*SPR 20172 Fix Start*/
    rrc_send_message(p_msg,RRC_OAM_MODULE_ID);
    /*SPR 20172 Fix End*/
}    


/*****************************************************************************
 * Function Name  : s1ap_add_and_process_reset_context
 * Inputs         : p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  p_reset_input_info - pointer to s1ap_enb_reset_info_t
 *                  send_resp_to_module - responce 
 *                  no_retries_on_timeout - no retries if time is out 
 *                  is_s1ap_ctx_released - check for s1ap ctx release
 *                  is_ue_ctx_deleted - check for ue ctx deletion
 * Outputs        : None
 * Returns        : None
 * Description    : The function processes reset request from OAM.
 *****************************************************************************/
void s1ap_add_and_process_reset_context(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        s1ap_enb_reset_info_t  *p_reset_input_info,
        U8 send_resp_to_module,
        U8 no_retries_on_timeout,
        U8 is_s1ap_ctx_released,
        U8 is_ue_ctx_deleted)
{
    reset_context_node_t * p_reset_context_node = PNULL;
    s1ap_timer_buffer_t timer_buf = {S1AP_OAM_INIT_TIMER,NULL,RRC_FALSE,0};

    RRC_S1AP_UT_TRACE_ENTER();
    /* Create a reset_context */

    p_reset_context_node = 
        (reset_context_node_t *)rrc_mem_get(sizeof(reset_context_node_t));

    if(PNULL == p_reset_context_node)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }

    memset_wrapper(p_reset_context_node, RRC_NULL, sizeof(reset_context_node_t));

        /* Set send_resp_to_module accordingly */
        p_reset_context_node->reset_context.send_resp_to_module = 
            send_resp_to_module;

        p_reset_context_node->reset_context.is_s1ap_ctx_deleted = 
            is_s1ap_ctx_released;

        p_reset_context_node->reset_context.is_ue_ctx_deleted =
            is_ue_ctx_deleted;


        /* Set retry count to max retries so that
           retry functionality is not performed*/
        if (no_retries_on_timeout)
        {
            p_reset_context_node->reset_context.reset_retry_count =
                p_s1ap_gb_ctx->max_retry_for_reset;
        }

        /* Set mme_reset_info */
        s1ap_set_mme_reset_info(p_s1ap_gb_ctx,
                &(p_reset_context_node->reset_context.mme_reset_info[0]),
                p_reset_input_info,
                &p_reset_context_node->reset_context.mme_count);

        /* Start repeated timer for sending RESET Msg */
        timer_buf.timer_type = S1AP_MME_RESET_TIMER;

        timer_buf.p_timer_specific_data = p_reset_context_node;

        /* Store the timer id in the mme context */
        p_reset_context_node->reset_context.timer_id =
            rrc_start_timer(p_s1ap_gb_ctx->reset_duration,
                    &timer_buf,
                    sizeof(timer_buf),
                    RRC_TRUE);

        list_push_head(&p_s1ap_gb_ctx->reset_context_list,
                &p_reset_context_node->node);

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : s1ap_set_mme_reset_info
 * Inputs         : p_s1ap_gb_ctx - Pointer to the S1AP global context data 
 *                  p_mme_reset_info_arr - pointer to reset_req_t
 *                  p_reset_input_info - The Input information from which
                                         RESET messages are prepared.
                    p_num_reset_msgs_sent - num of reset msgs sent 
 * Outputs        : p_mme_reset_info_arr - Pointer to reset_info_array in
                               global context that is being set as a part of 
                               RESET Procedure
                    p_num_reset_msgs_sent - Number of reset ACKs expected.
 * Returns        : None
 * Description    : The function sets RESET info per MME.
 *****************************************************************************/
void s1ap_set_mme_reset_info(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        reset_req_t    ** p_mme_reset_info_arr,
        s1ap_enb_reset_info_t    *p_reset_input_info,
        U8* p_num_reset_msgs_sent)
{
    mme_context_t * p_mme_context = PNULL;
    reset_req_t    *p_mme_reset_info = PNULL;
    /* SPR21585 Start */
    U16 ue_count = 0;
    /* SPR21585 End */
    U8 mme_count = 0;
    U8 mme_id = 0;
    U8 num_reset_msg_sent = 0;
    U16 asn_msg_len  = 0;
    /*valgrind_fix start*/
    U8* p_asn_msg_buff     = PNULL;
    p_asn_msg_buff = (U8*)rrc_mem_get(S1AP_MAX_ASN1_BUF_LEN);
    if(PNULL == p_asn_msg_buff)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "%s Unable to get the memory for p_asn_msg_buff ",
                __FUNCTION__);
        /*coverity_fix*/
        return;

        /*coverity_fix*/
    }
    memset_wrapper(p_asn_msg_buff,0,S1AP_MAX_ASN1_BUF_LEN);
    /*valgrind_fix start*/

    RRC_ASSERT(p_asn_msg_buff != PNULL);

    /* Check if it is the case of RESET S1 Interface */
    if (!p_reset_input_info->num_of_items)
    {
        for (mme_count = 0; mme_count < MAX_NUM_MME; mme_count++)
        {
            if (PNULL == 
               (p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_count]))
            {
                RRC_TRACE(RRC_INFO,"%s MME context not present at idx %d \n",
                        __FUNCTION__,mme_count);
                p_mme_reset_info_arr[mme_count] = PNULL;
                continue;
            }
            /*SPR 15989 FIX START*/
            /* Check whether MME's sctp assoc is active or not */
            if (p_mme_context->current_mme_state == MME_ACTIVE)
            {
                /*SPR 15989 FIX END*/
            p_mme_reset_info = (reset_req_t *)rrc_mem_get
                (sizeof(reset_req_t));
            if (PNULL != p_mme_reset_info)
            {
                memset_wrapper(p_mme_reset_info,0,sizeof(reset_req_t));

                /**** Set cause in RESET Message****/
                p_mme_reset_info->cause = p_reset_input_info->cause;

                p_mme_reset_info_arr[mme_count] = p_mme_reset_info;
            }
                /*SPR 15989 FIX START*/
            }
            /*SPR 15989 FIX END*/
        }
    }
    else
    {
        /* Check if it is the case of RESET part of S1 Interface */
        for(ue_count = 0; (ue_count < p_reset_input_info->num_of_items);
                ue_count++)
        {
            mme_id = p_reset_input_info->enbUeS1apId_arr[ue_count].mme_id;
            /*SPR 15989 FIX START*/
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];
            /*SPR 15989 FIX END*/

            if (!p_mme_reset_info_arr[mme_id])
            {
                /*SPR 15989 FIX START*/
                /* Check whether MME's sctp assoc is active or not */
                if ((p_mme_context != NULL) && (p_mme_context->current_mme_state == MME_ACTIVE))
                {
                    /*SPR 15989 FIX END*/
                p_mme_reset_info = (reset_req_t *)rrc_mem_get
                    (sizeof(reset_req_t));
                if (PNULL != p_mme_reset_info)
                {

                    memset_wrapper(p_mme_reset_info,0,sizeof(reset_req_t));

                    /**** Set cause in RESET Message****/
                    p_mme_reset_info->cause = p_reset_input_info->cause;

                    p_mme_reset_info_arr[mme_id] = p_mme_reset_info;
                }
                    /*SPR 15989 FIX START*/
                }
                /*SPR 15989 FIX END*/
            }

            /* Set the enbUeS1apId in reset_info corresponding to a
               MME and increment the count */
	    /* SPR 21126 Fix Start */
            if (p_mme_reset_info_arr[mme_id])
            {
            p_mme_reset_info_arr[mme_id]->reset_info.id_pair[ue_count].
                bitmask |= ENB_UE_S1AP_ID_PRESENT;
            p_mme_reset_info_arr[mme_id]->reset_info.id_pair[ue_count].
                enbUeS1apId = p_reset_input_info->enbUeS1apId_arr[
                ue_count].enbUeS1apId;
            p_mme_reset_info_arr[mme_id]->reset_info.num_of_items++;
	        }
	    /* SPR 21126 Fix End */
        }
    }

    /* For all the MMEs to which RESET message is to be sent,
       compose RESET ASN Message and send */
    for (mme_count = 0; mme_count < MAX_NUM_MME ; mme_count++)
    {
        if ((p_mme_reset_info_arr[mme_count]) && (PNULL != p_asn_msg_buff))
        {
            if (RRC_FAILURE ==  
                  compose_asn_reset(p_mme_reset_info_arr[mme_count],
                        p_asn_msg_buff,
                        &asn_msg_len))
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "compose_asn_reset returned failure.",
                        __FUNCTION__);
                continue;
            }

            /*CSR 51442 Start*/
            ALARM_MSG_L3(RRC_MODULE_ID, S1AP_RESET_ALARM_ID, MAJOR_ALARM, INVALID_ERR_CAUSE_ID);
            /*CSR 51442 Stop*/
            
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_count];

            /*SPR 15989 FIX START*/
            /* Code Removed */
            /*SPR 15989 FIX END*/
#ifndef LINUX_PC_TEST
            if (SCTP_SOCKET_ERROR == l3_sctp_send(
                        p_mme_context->sctp_sd,
                        p_asn_msg_buff,
                        asn_msg_len,
                        p_mme_context
                        ->stream_info.stream_ids[
                        SCTP_COMMON_PROC_STREAMS]))
#else
			if(SCTP_SOCKET_ERROR == pctest_forward_mme_message_to_pc(
						RRC_S1AP_MODULE_ID,
						RRC_MME_MODULE_ID,
						MME_MESSAGE_PS_TO_TEST,
						p_mme_context->sctp_sd,
						p_mme_context->stream_info.stream_ids[SCTP_COMMON_PROC_STREAMS], //stream id
						p_asn_msg_buff,
						asn_msg_len))

#endif
            {
                RRC_S1AP_TRACE(RRC_WARNING,"[%s]:l3_sctp_send"
                        " failed\n",__FUNCTION__);
            }
            else
            {
                /*SPR 15989 FIX START*/
                /* Increment The count of S1 RESET messages sent*/
                num_reset_msg_sent++;
                /*SPR 15989 FIX END*/
                RRC_S1AP_TRACE(RRC_INFO,"[%s]:RESET sent to MME\n"
                        ,__FUNCTION__);
            }
        }
    }
    *p_num_reset_msgs_sent = num_reset_msg_sent;

    if (PNULL != p_asn_msg_buff)
    {
        rrc_mem_free((void*)p_asn_msg_buff);
    }
}

/*****************************************************************************
 * Function Name  : delete_reset_ue_s1ap_ctx
 * Inputs         : p_s1ap_gb_ctx - Pointer to the S1AP global context data 
 *                  p_reset_context - The Input information from which
                                      RESET messages are prepared.
                    is_ue_ctx_deleted - check to check about the deletion of 
                                        UE-context 
 * Outputs        : None
 * Returns        : None
 * Description    : The function deleted S1AP UE contexts that are to be
                    released corresponding to a RESET EP.
 *****************************************************************************/
void delete_reset_ue_s1ap_ctx(s1ap_gb_context_t* p_s1ap_gb_ctx,
        reset_req_t *p_reset_req,
        U8 mme_id,
        U8 is_ue_ctx_deleted)
{
    ue_s1ap_context_t * p_s1ap_ue_ctx = PNULL;
    s1ap_reset_info_t * p_reset_info = &p_reset_req->reset_info;
    mmeUeS1apId_search_node_t* p_mmeUeS1apId_st_node = PNULL;
    mmeUeS1apId_search_node_t* p_temp = PNULL;
    mmeUeS1apId_search_node_t* p_next = PNULL;
    mme_context_t *p_mme_context = PNULL;

    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node_temp = PNULL;

    U8 cell_id = 0;
    U16 ue_id = 0;
    U16 ue_count = 0;
    U32 enbUeS1apId = 0;


    void *p_intrl_msg         = PNULL;
    s1ap_del_ctx_msg_ind_t   s1ap_del_ctx_msg;

    memset_wrapper((void*)&s1ap_del_ctx_msg, 0 ,sizeof(s1ap_del_ctx_msg_ind_t));


    if (!p_reset_info->num_of_items)
    {
        p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

        /* check whether the mme context exists or not */
        if (!p_mme_context)
        {
            return;
        }
/* SPR 16149  Fix Start */
        /* Delete all nodes from HO ongoing UE contexts */
	p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
		list_first_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list);

	while(p_ho_ongoing_ue_ctx_node)
	{
		if (mme_id ==
				p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mme_id) 
		{
			/* Match found!! */
			RRC_TRACE(RRC_INFO,"%s HO Node "
					"Match found\n",__FUNCTION__);
			p_ho_ongoing_ue_ctx_node_temp = (ho_ongoing_ue_ctx_node_t *)
				list_next_node(&p_ho_ongoing_ue_ctx_node->node);
			/* Delete ho_ongoing_ue_ctx from the ho_ongoing_ue_ctx_list */
			list_delete_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list, (void
						*)p_ho_ongoing_ue_ctx_node);

			if (PNULL != p_ho_ongoing_ue_ctx_node)
			{
				rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
				p_ho_ongoing_ue_ctx_node = PNULL;
				RRC_TRACE(RRC_INFO,"%s HO ONGOING UE  Context DELETED"
						"\n",__FUNCTION__);
			}
			p_ho_ongoing_ue_ctx_node = p_ho_ongoing_ue_ctx_node_temp;
		}
		else
		{
			p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
				list_next_node(&p_ho_ongoing_ue_ctx_node->node);
		}
	}
/* SPR 16149 Fix Stop */

        /* Delete all nodes from mmeUeS1apId search tree. */

        /* get the first node of the search tree */
        p_temp = (mmeUeS1apId_search_node_t*)
                     st_get_first_node(&p_mme_context->
                mmeUeS1apId_search_tree);

        /*delete the nodes one by one */
        if (p_temp)
        {
            do{
                RRC_S1AP_TRACE(
                        RRC_INFO,
                        "[%s]Deleting entry from search"
                        " tree for mmeUeS1apId %d \n ",
                        __FUNCTION__,
                        ((mmeUeS1apId_search_node_t *)p_temp)
                        ->mmeUeS1apId);

                /* Free All Ue Contexts Related to tht MME */
                enbUeS1apId = p_temp->enbUeS1apId;

                cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, enbUeS1apId);
                ue_id =  s1ap_get_ue_id(enbUeS1apId);
                get_ue_ctx_from_ue_id( p_s1ap_gb_ctx,
                        &p_s1ap_ue_ctx, enbUeS1apId);

                if( (cell_id >= MAX_NUM_CELLS || ue_id >= MAX_NUM_UE) ||
                        (!p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id]) || 
                        (!p_s1ap_ue_ctx))
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                            "enbUeS1apId 0x%x doesnt' exist",
                            __FUNCTION__,enbUeS1apId);
                }
                else
                {
                    /*BUG 604 changes start*/
                    /*Free s1ap_ue_id Ctx*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            p_s1ap_ue_ctx->enb_ue_s1ap_id);
                    /*BUG 604 changes stop*/
                    /*SPR 22789 Fix Start */
                    RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx, p_s1ap_ue_ctx, enbUeS1apId);
                    /*SPR 22789 Fix End */

                    RRC_S1AP_TRACE(
                            RRC_INFO,
                            "[%s]Deleting entry S1AP GB CTX"
                            " at cell id %d ue id %d \n ",
                            __FUNCTION__,cell_id,ue_id);
                }

                p_next = (mmeUeS1apId_search_node_t *)
                                  st_get_next_node(&p_temp->nodeAnchor);
                st_delete_node(&p_mme_context->mmeUeS1apId_search_tree,&p_temp
                        ->nodeAnchor);
                rrc_mem_free(p_temp);
                p_temp = p_next;
            }while(PNULL != p_next);
        }
    }
    else
    {
        for (ue_count = 0; ue_count < p_reset_info->num_of_items; ue_count++)
        {
            enbUeS1apId = p_reset_info->id_pair[ue_count].enbUeS1apId;
            cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, enbUeS1apId);
            ue_id =  s1ap_get_ue_id(enbUeS1apId);
            get_ue_ctx_from_ue_id( p_s1ap_gb_ctx,
                    &p_s1ap_ue_ctx, enbUeS1apId);

            if (cell_id >= MAX_NUM_CELLS || ue_id >= MAX_NUM_UE)
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "Either cell id %d or ue id %d invalid",
                        __FUNCTION__,cell_id,ue_id);
                continue;
            }

            if (!p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id])
            {
                RRC_S1AP_TRACE(RRC_DETAILED, "%s: "
                        "Cell context doesn't exist at id %d",
                        __FUNCTION__,cell_id);
                continue;
            }

            if (!p_s1ap_ue_ctx)
            {
                RRC_S1AP_TRACE(RRC_DETAILED, "%s: "
                        "UE context doesn't exist at id %d",
                        __FUNCTION__,ue_id);
                continue;
            }

            p_mme_context = p_s1ap_gb_ctx->mme_contexts[p_s1ap_ue_ctx->mme_id];

            p_mmeUeS1apId_st_node = (mmeUeS1apId_search_node_t *)
                  st_find_tree_node(&p_mme_context->mmeUeS1apId_search_tree,
                    &(p_s1ap_ue_ctx->mme_ue_s1ap_id));

            if (is_ue_ctx_deleted == 0x01)
            {
                /* The context is to be deleted for a sinlge UE */
                s1ap_del_ctx_msg.enb_ue_s1ap_id = enbUeS1apId;
/*BUG 604 changes start*/
                s1ap_del_ctx_msg.ue_index = p_s1ap_ue_ctx->ue_index;
/*BUG 604 changes stop*/
            }

            /* SPR 16149 Fix Start */
            p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
                list_first_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list);

            while(p_ho_ongoing_ue_ctx_node)
            {
                if ((mme_id ==
                            p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mme_id) && 
                        ((p_s1ap_ue_ctx->mme_ue_s1ap_id == 
                          p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mmeUeS1apId)||
                         (p_s1ap_ue_ctx->enb_ue_s1ap_id == 
                          p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id)))
                {
                    /* Match found!! */
                    RRC_TRACE(RRC_INFO,"%s HO Node "
                            "Match found\n",__FUNCTION__);
                    /* Delete ho_ongoing_ue_ctx from the ho_ongoing_ue_ctx_list */
                    list_delete_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list, (void
                                *)p_ho_ongoing_ue_ctx_node);

                    if (PNULL != p_ho_ongoing_ue_ctx_node)
                    {
                        rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
                        p_ho_ongoing_ue_ctx_node = PNULL;
                        RRC_TRACE(RRC_INFO,"%s HO ONGOING UE  Context DELETED"
                                "\n",__FUNCTION__);
                    }
                    break;
                }
                else
                {
                    p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
                        list_next_node(&p_ho_ongoing_ue_ctx_node->node);
                }
            }
            /* SPR 16149 Fix Stop */
            /*Coverity 94793 Fix Start*/
            if (PNULL != p_s1ap_ue_ctx)
            {
                /*SPR 22789 Fix Start */
                /*BUG 604 changes start*/
                /*Free s1ap_ue_id Ctx*/
                s1ap_ueid_free(p_s1ap_gb_ctx,
                        p_s1ap_ue_ctx->enb_ue_s1ap_id);
                /*BUG 604 changes stop*/
                RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx, p_s1ap_ue_ctx, p_s1ap_ue_ctx->enb_ue_s1ap_id);
                /* Code Deleted */
                /*SPR 22789 Fix End */
            }

            RRC_S1AP_TRACE(RRC_INFO, "%s: "
                    "enbUeS1apId  %d deleted from S1AP GB CTX",__FUNCTION__,
                    enbUeS1apId);
            /*Coverity 93793 Fix End*/

            if (!p_mmeUeS1apId_st_node)
            {
                RRC_S1AP_TRACE(RRC_INFO, "%s: "
                        "mmeUeS1apId is not preset in the search tree",
                        __FUNCTION__);
            }
            else
            {
                RRC_S1AP_TRACE(RRC_INFO, "%s: "
                        "mmeUeS1apId  %d deleted from search tree",__FUNCTION__,
                        p_mmeUeS1apId_st_node->mmeUeS1apId);

                st_delete_node(&p_mme_context->mmeUeS1apId_search_tree,
                        (search_tree_node_t*)p_mmeUeS1apId_st_node);

                if (PNULL != p_mmeUeS1apId_st_node)
                {
                    rrc_mem_free(p_mmeUeS1apId_st_node);
                    p_mmeUeS1apId_st_node = PNULL;
                }
            }

            if (is_ue_ctx_deleted == S1AP_TRUE)
            {

                p_intrl_msg = s1ap_alloc_intrl_msg(
                    RRC_UECC_MODULE_ID,
                    S1AP_DEL_CONTEXT_MSG_IND,
                    sizeof(s1ap_del_ctx_msg_ind_t));

                if (PNULL == p_intrl_msg)
                {
                   /*out of memory*/
                   RRC_S1AP_UT_TRACE_EXIT();
                   return;
                }

                l3_memcpy_wrapper (
                     (void*)((U8*) p_intrl_msg + RRC_API_HEADER_SIZE),
                     (const void*) &s1ap_del_ctx_msg,
                     sizeof(s1ap_del_ctx_msg_ind_t));
        /*BUG 604 changes start*/
                if ((RRC_GET_UECC_MODULE_ID(s1ap_del_ctx_msg.ue_index) - 
                      RRC_UECC_1_MODULE_ID) < p_s1ap_gb_ctx->no_of_uecc_instances)
                {
                    /* Send message */
    /*SPR 20172 Fix Start*/
                    rrc_send_message (p_intrl_msg,RRC_GET_UECC_MODULE_ID(s1ap_del_ctx_msg.ue_index));
    /*SPR 20172 Fix End*/
                }
        /*BUG 604 changes stop*/
                else
		{
		    RRC_S1AP_TRACE(RRC_WARNING,"UECC instance does not exist");
		    /* Coverity Fix 48266 Start */
		    if ( PNULL != p_intrl_msg )
		    {
			/* SPR 15887 Start */
			rrc_msg_mem_free(p_intrl_msg);
			/* SPR 15887 Stop */
			p_intrl_msg = PNULL;
		    }
		    /* Coverity Fix 48266 Stop */
		}
		RRC_S1AP_TRACE(RRC_INFO,"[%s]: SENT : S1AP_DEL_CONTEXT_MSG_IND with S1AP_TRUE"
			,__FUNCTION__);

                /* RESET the flag */

                is_ue_ctx_deleted = S1AP_FALSE;
            }



        }
    }
    /* SPR 16149 Fix Start*/
    /* code removed */
    /* SPR 16149 Fix Stop*/

}

/*****************************************************************************
 * Function Name  : validate_s1ap_oam_ip_addresses
 * Inputs         : p_s1ap_oam_info - pointer to S1AP PROV INFO
 * Outputs        : None 
 * Returns        : RRC_TRUE/RRC_FALSE
 * Description    : The function validates MME/eNB IP Addresses came in S1AP_
 *                  OAM_PROV_REQ      
 * *****************************************************************************/
rrc_bool_et validate_s1ap_oam_ip_addresses(
        s1apInfo_t *p_s1ap_oam_info
        )
{

    U8 mme_counter = 0;
    U8 no_ip_count = 0;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Check for MME IP Addresses */
    for (mme_counter = 0; mme_counter < p_s1ap_oam_info->num_mme; mme_counter++)
    {

		/*bug 114945 begin*/
		if((p_s1ap_oam_info->mme_comm_info[mme_counter].num_ip_addr == 0) &&
			!(p_s1ap_oam_info->mme_comm_info[mme_counter].bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT))
		{
			RRC_S1AP_TRACE(RRC_WARNING,
					"[S1AP_OAM_PROV_REQ]:no ipv4 or ipv6 addr for mmecounter is:%d",mme_counter);
			return RRC_FALSE;
		}
		/*bug 114945 end*/	

	
        if(!(p_s1ap_oam_info->mme_comm_info[mme_counter].bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT))
        {
            for (no_ip_count = 0; 
                    no_ip_count < p_s1ap_oam_info->mme_comm_info[mme_counter].num_ip_addr;
                    no_ip_count++)
            {
                if (RRC_SUCCESS != 
                        validate_ip_addr(
                            p_s1ap_oam_info->mme_comm_info[mme_counter].ip_addr[no_ip_count].ip_addr))
                {
                    RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: MME IP ADDRESS VALIDATION FAILED");
                    return RRC_FALSE;
                }
            }
        }
		/*bug 115029 begin*/
		//check ipv6 addr
		if(p_s1ap_oam_info->mme_comm_info[mme_counter].bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
		{
            for (no_ip_count = 0; 
                    no_ip_count < p_s1ap_oam_info->mme_comm_info[mme_counter].num_ipv6_addr;
                    no_ip_count++)
            {
                if (RRC_SUCCESS != 
                        validate_ipv6_addr(
                            p_s1ap_oam_info->mme_comm_info[mme_counter].ipv6_addr[no_ip_count].ipv6_addr))
                {
                    RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: MME IP ADDRESS VALIDATION FAILED");
                    return RRC_FALSE;
                }
            }			
		}
		/*bug 115029 end*/		
    }

	/*bug 114945 begin*/
	if((p_s1ap_oam_info->enb_comm_info.num_ip_addr == 0) &&
		!(p_s1ap_oam_info->enb_comm_info.bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT))
	{
		RRC_S1AP_TRACE(RRC_WARNING,
				"[S1AP_OAM_PROV_REQ]:no ipv4 or ipv6 addr to eNB ");
		return RRC_FALSE;
	}
	/*bug 114945 end*/	


    /* Check for eNB IP Addresses */
    for (no_ip_count = 0; 
            no_ip_count < p_s1ap_oam_info->enb_comm_info.num_ip_addr;
            no_ip_count++)
    {
        if(!(p_s1ap_oam_info->enb_comm_info.bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT))
        {
            if (RRC_SUCCESS != 
                    validate_ip_addr(
                        p_s1ap_oam_info->enb_comm_info.ip_addr[no_ip_count].ip_addr))
            {
                RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: eNB IP ADDRESS VALIDATION FAILED");
                return RRC_FALSE;
            }
        }
    }
			
	/*bug 115029 begin*/
	/* Check for eNB IPV6 Addresses */
	if(p_s1ap_oam_info->enb_comm_info.bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
		for (no_ip_count = 0; 
            no_ip_count < p_s1ap_oam_info->enb_comm_info.num_ipv6_addr;
            no_ip_count++)		
		{
            if (RRC_SUCCESS != 
                    validate_ipv6_addr(
                        p_s1ap_oam_info->enb_comm_info.ipv6_addr[no_ip_count].ipv6_addr))
            {
                RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: eNB IPV6 ADDRESS VALIDATION FAILED");
                return RRC_FALSE;
            }		
		}		
    }
	/*bug 115029 end*/

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_TRUE;
}


/*****************************************************************************
 * Function Name  : validate_ip_addresses
 * Inputs         : p_ip_addr - pointer to ip address
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : The function validates IP Addresses     
 * *****************************************************************************/
rrc_return_et validate_ip_addr(U8 *p_ip_addr)
{
    struct in_addr inp;
    RRC_S1AP_UT_TRACE_ENTER();
   
    /*coveriry_fix*/

    memset_wrapper(&inp,0,sizeof(struct in_addr));
    /*coveriry_fix*/

    if (RRC_NULL == 
        inet_aton_wrapper(
            (char *)p_ip_addr,
            &inp))
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: IP ADDRESS VALIDATION FAILED"
                       " : Invalid IP Format received: %s",
                       p_ip_addr);
        return RRC_FAILURE;
    }
    /* Check for 0.0.0.0 and 255.255.255.255 */
    if (inp.s_addr == BROADCAST_IP_ADDR) 
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: BROADCAST IP ADDRESS received: %s",
                p_ip_addr);
        return RRC_FAILURE;
    }
    else if (inp.s_addr == INVALID_IP_ADDR)
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: INVALID IP ADDRESS received: %s",
                p_ip_addr);
        return RRC_FAILURE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : validate_ipv6_addr
 * Inputs         : p_ipv6_addr - pointer to ipv6 address
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : The function validates IPv6 Addresses     
 * *****************************************************************************/
rrc_return_et validate_ipv6_addr(U8 *p_ipv6_addr)
{
    struct in6_addr  inp;
    RRC_S1AP_UT_TRACE_ENTER();
   
    /*coveriry_fix*/

    memset_wrapper(&inp,0,sizeof(struct in6_addr));
    /*coveriry_fix*/

    if (RRC_NULL == 
        inet_pton_wrapper(
        	AF_INET6,
            (char *)p_ipv6_addr,
            (void *)&inp))
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: IPV6 ADDRESS VALIDATION FAILED"
                       " : Invalid IP Format received: %s",
                       p_ipv6_addr);
        return RRC_FAILURE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/*****************************************************************************
* Function Name  : s1ap_oam_process_enb_config_update
* Inputs         : p_api - Pointer to input API buffer
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
* Outputs        : None
* Returns        : None
* Description    : This function build and send the eNB Configuration update
*                  to MME
****************************************************************************/
void 
s1ap_oam_process_enb_config_update(
        void *p_api, 
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
    /* Since we have received this message that means
     * we have to update the existing S1Setup info 
     * present in the S1AP Global Context
     */
    s1ap_oam_enb_config_update_t s1ap_oam_enb_config_update; 
    s1ap_oam_enb_config_update_response_t oam_enb_config_update_resp;
    rrc_return_et response = RRC_FAILURE;
    rrc_bool_et send_failure_to_oam = RRC_FALSE;
    U16 mme_counter = RRC_NULL;

    S32 length_read = RRC_NULL;
    U16 trans_id = RRC_NULL;
    mme_context_t *p_mme_context = PNULL;
    U8 mme_id = RRC_NULL;
    s1_setup_req_t *p_s1_setup_info = PNULL;
    enb_config_update_invoked_et invoked_by = INVOKED_BY_OAM;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    /* MEMSET */
    memset_wrapper(&s1ap_oam_enb_config_update, 0, 
            sizeof(s1ap_oam_enb_config_update_t));

    memset_wrapper(&oam_enb_config_update_resp, 0,
            sizeof(s1ap_oam_enb_config_update_response_t));

    /* Get the transaction id */
    trans_id = rrc_get_transaction_id(p_api);

    do
    {
        /* Check the state of s1ap */
        if (S1AP_W_FOR_PROV_REQ == p_s1ap_gb_ctx->s1ap_current_state)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "S1AP module is not provisioned");
	    /*SPR 13932 Fix Start*/
            send_failure_to_oam = RRC_TRUE;
	    /*SPR 13932 Fix Stop*/

            break;
        }

        /* Decode the received message from OAM */
        response = rrc_il_parse_s1ap_oam_enb_config_update(
                &s1ap_oam_enb_config_update,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - 
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read);

        /* Send the failiure message to OAM on Decode failure */
        if (RRC_FAILURE == response) 
        {
            RRC_S1AP_TRACE(RRC_WARNING, 
                    "[S1AP_OAM_ENB_CONFIG_UPDATE] decode failed");
            send_failure_to_oam = RRC_TRUE;

            break;
        }

        RRC_S1AP_TRACE(RRC_INFO, "[S1AP_OAM_ENB_CONFIG_UPDATE] decode success");

        /******************************************************** 
         * Update the S1 Setup info in the 
         * S1AP Global Context to reflect updated values
         * This information will be used to S1 Setup Req if
         * there is new MME Connection is to be brought up.
         * *******************************************************/
        if (&p_s1ap_gb_ctx->s1_setup_info != PNULL)
        {
            p_s1_setup_info = &p_s1ap_gb_ctx->s1_setup_info;
        }

        /* Check if ENB NAME Present */
        if (S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT & 
                s1ap_oam_enb_config_update.bitmask)
        {
            /* Update the bitmask */
            p_s1_setup_info->bitmask |= S1AP_OAM_ENB_NAME_PRESENT;

            /* Update the ENB Name now. */
            l3_memcpy_wrapper(p_s1_setup_info->enb_name,
                    s1ap_oam_enb_config_update.enb_name, 
                    MAX_ENB_NAME_STR_SIZE);
        }

        /* Check if SUPP_TA_LIST Present */
        if (S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT &
                s1ap_oam_enb_config_update.bitmask)
        {
            l3_memcpy_wrapper(&p_s1_setup_info->supp_ta_list,
                    &s1ap_oam_enb_config_update.supp_ta_list,
                    sizeof(supp_ta_list_t));
        }

        /* Check if CSG_ID_LIST Present */
        if (S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT &
                s1ap_oam_enb_config_update.bitmask)
        {
            p_s1_setup_info->bitmask |= S1AP_OAM_CSG_ID_LIST_PRESENT;
            l3_memcpy_wrapper(&p_s1_setup_info->csg_id_list,
                    &s1ap_oam_enb_config_update.csg_id_list,
                    sizeof(csg_id_list_t));
        }

        /* Check if default paging drx present */
        if (S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT & 
                s1ap_oam_enb_config_update.bitmask)
        {
            p_s1_setup_info->default_paging_drx = 
                s1ap_oam_enb_config_update.default_paging_drx;
        }

        /* Update the counter */
        p_s1ap_gb_ctx->enb_config_updated_gb_count = 
            p_s1ap_gb_ctx->enb_config_updated_gb_count + 1;

        /******************************************************
         * Global Context Updation END
         * ***************************************************/

        /* Now this message has to be sent to the Active MMEs */
        /* Check for the Active MME list and send the message */

        /* Checking if there is any Active MME in the active list */
        /* AK Start */
        if (S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT &
                s1ap_oam_enb_config_update.bitmask)
        {
            for (mme_counter = 0; 
                  mme_counter < s1ap_oam_enb_config_update.s1ap_oam_mme_id_list.num_mme_id; 
                  mme_counter++)
            {
                /* Now get the MME ID from the active mme info and 
                 * fetch the MME Context to set the State
                 */
                mme_id = 
                    (U8)(s1ap_oam_enb_config_update.s1ap_oam_mme_id_list.mme_id[mme_counter]);
                p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

                if (PNULL == p_mme_context)
                {
                    RRC_S1AP_TRACE(RRC_WARNING,
                            "MME Context NULL at Index %d", mme_id);
                    /* No Need to break here as we have other 
                     * MME Context to update */
                    build_and_send_enb_config_update_resp_to_oam((U8)(mme_id),
                            OAM_ENB_CONFIG_UPDATE_FAILURE_INVALID_MME_ID,
                            /*SPR 17777 +-*/
                            trans_id);
                }
                else
                {
                    /* Send the update only when the previous procedure is 
                     * complete. If there is failure returned for the 
                     * previous update, we can send the update configuration.
                     * (If there are two immediate configuration update, and 
                     * previous configuration update is still in progress)
                     * The update configuration will be taken from the 
                     * updated S1AP global configuration.
                     */
                    if ((p_mme_context->current_enb_config_update_state == 
                                ENB_STATE_RECV_ENB_CONFIG_FROM_OAM))
                    {
                        /* This is the new config copy the data 
                         * it'll be used under Abnormal Scenario */
                        p_mme_context->p_enb_configuration_update = 
                            (s1ap_oam_enb_config_update_t *)
                            rrc_mem_get(sizeof(s1ap_oam_enb_config_update_t));

                        if (PNULL == p_mme_context->p_enb_configuration_update)
                        {
                            RRC_S1AP_TRACE(RRC_WARNING,
                                    "%s Unable to get the memory for copy data ",
                                    __FUNCTION__);

                    build_and_send_enb_config_update_resp_to_oam((U8)(mme_id),
                            OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                            /*SPR 17777 +-*/
                            trans_id);
                            continue;
                        }

                        /* Copy the data */
                        l3_memcpy_wrapper(p_mme_context->p_enb_configuration_update,
                                &s1ap_oam_enb_config_update,
                                sizeof(s1ap_oam_enb_config_update_t));

                        /* Update the OAM transaction id in global context */
                        p_mme_context->oam_transaction_id = trans_id;

                        /* Set the retry count to Zero before 
                         * Initiating the enb config update
                         */
                        p_mme_context->s1_config_retry_count = 0;

                        enb_fsm_config_update_process_event(
                                S1AP_MME_EV_ENB_CONFIG_SENT,/*coverity fix start*/
                                p_api,/*coverity fix stop*/
                                &invoked_by, 
                                p_s1ap_gb_ctx,
                                p_mme_context);
                    }
                    else
                    {
                        /* Building the response here as we dont have
                         * to maintain the data in the global context */
                        RRC_S1AP_TRACE(RRC_INFO,
                                "eNB Configuration Update not sent to"
                                " MME ID %d, as previous update is not"
                                " Complete", mme_id);
                    build_and_send_enb_config_update_resp_to_oam((U8)(mme_id),
                            OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                            /*SPR 17777 +-*/
                            trans_id);
                    }
                }
            }
        }
        else
        {

        if (RRC_NULL < p_s1ap_gb_ctx->active_mme_info.active_mme_count)
        {
            /* Active MME Availalble, Set the state of ENB Config
             * update FSM as ENB_STATE_RECV_ENB_CONFIG_FROM_OAM
             */
            for (mme_counter = 0; 
                  mme_counter < p_s1ap_gb_ctx->active_mme_info.active_mme_count; 
                  mme_counter++)
            {
                /* Now get the MME ID from the active mme info and 
                 * fetch the MME Context to set the State
                 */
                mme_id = 
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[mme_counter].mme_id;
                p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

                if (PNULL == p_mme_context)
                {
                    RRC_S1AP_TRACE(RRC_WARNING,
                            "MME Context NULL at Index %d", mme_id);
                    /* No Need to break here as we have other 
                     * MME Context to update */ 
                    build_and_send_enb_config_update_resp_to_oam((U8)(mme_id),
                            OAM_ENB_CONFIG_UPDATE_FAILURE_INVALID_MME_ID,
                            /*SPR 17777 +-*/
                            trans_id);

                }
                else
                {
                    /* Send the update only when the previous procedure is 
                     * complete. If there is failure returned for the 
                     * previous update, we can send the update configuration.
                     * (If there are two immediate configuration update, and 
                     * previous configuration update is still in progress)
                     * The update configuration will be taken from the 
                     * updated S1AP global configuration.
                     */
                    if ((p_mme_context->current_enb_config_update_state == 
                                ENB_STATE_RECV_ENB_CONFIG_FROM_OAM))
                    {
                        /* This is the new config copy the data 
                         * it'll be used under Abnormal Scenario */
                        p_mme_context->p_enb_configuration_update = 
                            (s1ap_oam_enb_config_update_t *)
                            rrc_mem_get(sizeof(s1ap_oam_enb_config_update_t));

                        if (PNULL == p_mme_context->p_enb_configuration_update)
                        {
                            RRC_S1AP_TRACE(RRC_WARNING,
                                    "%s Unable to get the memory for copy data ",
                                    __FUNCTION__);

                    build_and_send_enb_config_update_resp_to_oam((U8)(mme_id),
                            OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                            /*SPR 17777 +-*/
                            trans_id);
                            continue;
                        }

                        /* Copy the data */
                        l3_memcpy_wrapper(p_mme_context->p_enb_configuration_update,
                                &s1ap_oam_enb_config_update,
                                sizeof(s1ap_oam_enb_config_update_t));

                        /* Update the OAM transaction id in global context */
                        p_mme_context->oam_transaction_id = trans_id;

                        /* Set the retry count to Zero before 
                         * Initiating the enb config update
                         */
                        p_mme_context->s1_config_retry_count = 0;

                        enb_fsm_config_update_process_event(
                                S1AP_MME_EV_ENB_CONFIG_SENT,/*coverity fix start*/
                                p_api,/*coverity fix stop*/
                                &invoked_by, 
                                p_s1ap_gb_ctx,
                                p_mme_context);
                    }
                    else
                    {
                        /* Building the response here as we dont have
                         * to maintain the data in the global context */
                        RRC_S1AP_TRACE(RRC_INFO,
                                "eNB Configuration Update not sent to"
                                " MME ID %d, as previous update is not"
                                " Complete", mme_id);
                    build_and_send_enb_config_update_resp_to_oam((U8)(mme_id),
                            OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                            /*SPR 17777 +-*/
                            trans_id);
                    }
                }
            }
        }
        else
        {
            /* No Active MME Available */
            RRC_S1AP_TRACE(RRC_WARNING, "[S1AP_OAM_ENB_CONFIG_UPDATE] "
                    "No Active MME Present, ENB Config update is not sent");
            send_failure_to_oam = RRC_TRUE;

            break;
        }
        }
        /* AK Stop */
    } while (0); 


    if (send_failure_to_oam == RRC_TRUE)
    {
        build_and_send_enb_config_update_resp_to_oam(
               RRC_NULL,
               OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
               /*SPR 17777 +-*/
               trans_id);
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return;
}

/*****************************************************************************
* Function Name  : update_oam_mme_connection_status_resp 
* Inputs         : p_oam_mme_connection_status_resp - Response to be sent to OAM
*                  response - Response
*                  mme_id - MME ID
*                  p_index_to_update - Index to be updated
* Outputs        : None
* Returns        : None
* Description    : This function update the response to sent to OAM
****************************************************************************/
rrc_return_et update_oam_mme_connection_status_resp(
        s1ap_oam_mme_conn_status_response_t *p_oam_mme_connection_status_resp,
        rrc_response_et response,
        U8 mme_id,
        U8 index_to_update)
{
    rrc_return_et result = RRC_FAILURE;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oam_mme_connection_status_resp);

    if (MAX_NUM_MME > index_to_update)
    {
        p_oam_mme_connection_status_resp->mme_count++;

            /* SPR 11404 Fix Start */
        p_oam_mme_connection_status_resp->
            oam_mme_conn_info_list[index_to_update].mme_id = (U8)(mme_id);
            /* SPR 11404 Fix Stop */

        p_oam_mme_connection_status_resp->
            oam_mme_conn_info_list[index_to_update].response = response;

        result = RRC_SUCCESS;
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "%s: Index out of bound", __FUNCTION__);
        result = RRC_FAILURE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return result;
}



/*****************************************************************************
* Function Name  : s1ap_oam_process_close_mme_conn 
* Inputs         : p_api - Pointer to input API buffer
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
* Outputs        : None
* Returns        : None
* Description    : This function process the message to close the mme 
*                  connection.
****************************************************************************/
void s1ap_oam_process_close_mme_conn(
        void *p_api, 
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
	s1ap_oam_close_mme_conn_t s1ap_close_mme_conn;
	s1ap_oam_mme_conn_status_response_t oam_mme_connection_status_resp;
	rrc_return_et  response         = RRC_FAILURE;
	rrc_return_et  result           = RRC_FAILURE;
	rrc_return_et  mme_fsm_resp     = RRC_FAILURE;
	S32            length_read      = RRC_NULL;
	U16            trans_id         = RRC_NULL;
	U8             count            = 0;
	U8             index            = 0;
	mme_context_t *p_mme_context    = PNULL;
	U8             mme_id           = 0;
	rrc_bool_et    complete_failure = RRC_FALSE;
	U8             index_to_update  = 0;
    /* SPR-22039 START */
    rrc_bool_et    active_mme_match = RRC_FALSE;
    /* SPR-22039 END */

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

	RRC_S1AP_UT_TRACE_ENTER();

	/* Initialize */
	memset_wrapper(&s1ap_close_mme_conn, 
			RRC_NULL, 
			sizeof(s1ap_oam_close_mme_conn_t));

	memset_wrapper(&oam_mme_connection_status_resp,
			RRC_NULL,
			sizeof(s1ap_oam_mme_conn_status_response_t));

	/* Check the state of s1ap */
	trans_id = rrc_get_transaction_id(p_api);

	do
	{
		/* Check the state of s1ap */
		if (S1AP_W_FOR_PROV_REQ == p_s1ap_gb_ctx->s1ap_current_state)
		{
			RRC_S1AP_TRACE(RRC_WARNING, "S1AP module is not provisioned"); 

			complete_failure = RRC_TRUE;
			break;
		}

		response = rrc_il_parse_s1ap_oam_close_mme_conn(
				&s1ap_close_mme_conn,
				(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
				rrc_get_api_buf_size(p_api) -
				RRC_FULL_INTERFACE_HEADERS_SIZE,
				&length_read);

		/* Send the failiure message to OAM on Decode failure */
		if (RRC_FAILURE == response) 
		{
			RRC_S1AP_TRACE(RRC_WARNING, 
					"[S1AP_OAM_CLOSE_MME_CONN] decode failed");

			complete_failure = RRC_TRUE;
			break;
		}

		RRC_S1AP_TRACE(RRC_INFO, "[S1AP_OAM_CLOSE_MME_CONN] decode success");

		for (count = 0; count < s1ap_close_mme_conn.mme_count; count++)
		{
			/* MME ID Sent from OAM start with index 1 */ 
			/* spr_11584_fix_start */
            /*coverity_64271_fix_start*/
			if (s1ap_close_mme_conn.mme_id[count] < MAX_NUM_MME) 
            /*coverity_64271_fix_stop*/
			{
				/* spr_11584_fix_stop */

				/* mme_id will not be negative because of the 
				 * above if check */
				/* spr_11584_fix_start */
				mme_id = (U8)(s1ap_close_mme_conn.mme_id[count]);
				/* spr_11584_fix_stop */

				/* Check if MME ID is in Active List and 
				 * current state is active */
				for(index = 0; index < MAX_NUM_MME; index++)
				{
					if (mme_id == p_s1ap_gb_ctx->active_mme_info.
							mme_id_arr[index].mme_id)
					{
						mme_fsm_resp = RRC_FAILURE;
						/* Fetch the mme context */
						p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];


						/* Check the mme state */
                        /* Bug 16213_Fix start */
                        if ((p_mme_context->current_mme_state == MME_ACTIVE)||(p_mme_context->current_mme_state == MME_W_CONNECT_SUCCESSFUL)) 
                        /*Bug 16213_Fix stop */
						{
						    mme_fsm_process_event(
							    MME_EVENT_CLOSE_MME_CONNECTION,/*coverity fix start*/
							    p_api,/*coverity fix stop*/
							    &mme_fsm_resp,
							    p_s1ap_gb_ctx,
							    p_mme_context);
			/*SPR 14639 Fix Start*/
			/* Reset S1Ap mode because it is checked when S1AP receives re-start request */
                           p_s1ap_gb_ctx->s1_conn_mode_one_flag = S1AP_FALSE;
			/*SPR 14639 Fix Stop*/
						}
						else
						{
							continue;
						}
                        /* SPR-22039 START */
                        active_mme_match = RRC_TRUE;
                        /* SPR-22039 END */

						if (RRC_SUCCESS == mme_fsm_resp)
						{
							/* update the OAM response */
							/* Send OAM Failure */
							result = update_oam_mme_connection_status_resp(
									&oam_mme_connection_status_resp,
									SUCCESS,
									mme_id,
									index_to_update);

							index_to_update++;
						}
						else
						{
							/* Send OAM Failure */
							result = update_oam_mme_connection_status_resp(
									&oam_mme_connection_status_resp,
									FAILURE,
									mme_id,
									index_to_update);

							index_to_update++;
						}

						if (RRC_FAILURE == result) 
						{
							RRC_S1AP_TRACE(RRC_WARNING,
									"%s: Index out of bound", __FUNCTION__);
						}

					}
				}
                /* SPR-22039 START */
                if(RRC_FALSE == active_mme_match)
                {
                    if(RRC_FAILURE == update_oam_mme_connection_status_resp(
                                &oam_mme_connection_status_resp,
                                FAILURE,
                                mme_id,
                                index_to_update))
                    {
                        RRC_S1AP_TRACE(RRC_WARNING,
                                "%s: Index out of bound", __FUNCTION__);
                    }
                    else
                    {
                        index_to_update++;
                    }
                }
                active_mme_match = RRC_FALSE;
                /* SPR-22039 END */
			}
			else
			{
				RRC_S1AP_TRACE(RRC_WARNING, 
						"[S1AP_OAM_CLOSE_MME_CONN] MME ID %d "
						"is not valid", s1ap_close_mme_conn.mme_id[count]);

				/* This Condition should not occur
				 * Put a check Just in Case */
				if (MAX_NUM_MME >= index_to_update)
				{
					/* Wrap Around */
					index_to_update = 0;
				}

				/* Send OAM Failure */
				if(RRC_FAILURE == update_oam_mme_connection_status_resp(
							&oam_mme_connection_status_resp,
							FAILURE,
							mme_id,
							index_to_update))
				{
					RRC_S1AP_TRACE(RRC_WARNING,
							"%s: Index out of bound", __FUNCTION__);
				}
				else
				{
					index_to_update++;
				}

			}/* end of if */
		} /* end of for */
	}while(0);

	/* If Complete failure, we build the response and send */
	if (RRC_TRUE == complete_failure)
	{
		oam_mme_connection_status_resp.mme_count = 
			s1ap_close_mme_conn.mme_count;

		for (index = 0; index < s1ap_close_mme_conn.mme_count; index++)
		{
			if (MAX_NUM_MME > index_to_update)
			{
				oam_mme_connection_status_resp.
					oam_mme_conn_info_list[index].mme_id =
					s1ap_close_mme_conn.mme_id[index];
				oam_mme_connection_status_resp.
					oam_mme_conn_info_list[index].response =
					RRC_FAILURE;
			}
			else
			{
				RRC_S1AP_TRACE(RRC_WARNING,
						"%s: Index out of bound", __FUNCTION__);
			}
		}

		index_to_update = s1ap_close_mme_conn.mme_count;
	}

	/* this condition should not occur putting the check 
	 * Just in case */
	if (index_to_update != s1ap_close_mme_conn.mme_count)
	{
		RRC_S1AP_TRACE(RRC_WARNING, 
				"Unable to proccess complete OAM Response");
	}

	send_oam_mme_connection_status_resp(&oam_mme_connection_status_resp,
			trans_id);

	RRC_S1AP_UT_TRACE_EXIT();

	return;
}

/*****************************************************************************
* Function Name  : s1ap_oam_process_reestab_mme_conn
* Inputs         : p_api - Pointer to input API buffer
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
* Outputs        : None
* Returns        : None
* Description    : This function process the message to establisth the mme 
*                  connection.
****************************************************************************/
void s1ap_oam_process_reestab_mme_conn(
        void *p_api, 
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
    s1ap_oam_reestab_mme_conn_t s1ap_oam_reestab_mme_conn;
    s1ap_oam_mme_conn_status_response_t oam_mme_connection_status_resp;
    rrc_return_et   response         = RRC_FAILURE;
    mme_context_t   *p_mme_context   = PNULL;
    rrc_bool_et     complete_failure = RRC_FALSE;
    S32             length_read      = RRC_NULL;
    U16             trans_id         = 0;
    U8              count            = 0;
    U8              mme_id           = 0;
    U8              index_to_update  = 0;

    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&s1ap_oam_reestab_mme_conn,
            RRC_NULL,
            sizeof(s1ap_oam_reestab_mme_conn_t));

    memset_wrapper(&oam_mme_connection_status_resp,
            RRC_NULL,
            sizeof(s1ap_oam_mme_conn_status_response_t));

    /* Check the state of s1ap */
    trans_id = rrc_get_transaction_id(p_api);

    do
    {
        /* Check the state of s1ap */
        if (S1AP_W_FOR_PROV_REQ == p_s1ap_gb_ctx->s1ap_current_state)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "S1AP module is not provisioned"); 

            /* Send message to OAM */
            complete_failure = RRC_TRUE;
            break;
        }

        response = rrc_il_parse_s1ap_oam_reestab_mme_conn(
                &s1ap_oam_reestab_mme_conn,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read);

        /* Send the failiure message to OAM on Decode failure */
        if (RRC_FAILURE == response) 
        {
            RRC_S1AP_TRACE(RRC_WARNING, 
                    "[S1AP_OAM_CLOSE_MME_CONN] decode failed");
            /* Send message to OAM */ 
            complete_failure = RRC_TRUE;
            break;
        }

        RRC_S1AP_TRACE(RRC_INFO, "[S1AP_OAM_REESTAB_MME_CONN] decode success");

        for(count = 0; count < s1ap_oam_reestab_mme_conn.mme_count; count++)
        {
            /* SPR 13432 Fix Start */
            if (!p_s1ap_gb_ctx->s1_conn_mode_one_flag)
            {
            /* MME ID Sent from OAM start with index 1 */ 
            /* SPR_16132 Fix Start */
            if(s1ap_oam_reestab_mme_conn.mme_id[count] < MAX_NUM_MME)
            {
                /* Correct the MME ID */
                mme_id = (U8)(s1ap_oam_reestab_mme_conn.mme_id[count]);
            /* SPR_16132 Fix Stop */

                /* Fetch the mme context */
                p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

                if (PNULL == p_mme_context)
                {
                    oam_mme_connection_status_resp.mme_count++;

                    /* SPR_16132 Fix Start */
                    /* build failure at index */
                    oam_mme_connection_status_resp.
                        oam_mme_conn_info_list[index_to_update].
                        mme_id = (U8)(mme_id);
                    /* SPR_16132 Fix Stop */

                    oam_mme_connection_status_resp.
                        oam_mme_conn_info_list[index_to_update].response =
                        RRC_FAILURE;

                    index_to_update++;

                    continue;
                }

                /*Now check if the MME is in IDLE state */
                if (p_mme_context->current_mme_state == MME_IDLE)
                {
                    p_mme_context->s1_config_retry_count = 0;
                    p_mme_context->oam_transaction_id = 0; 

                    /*mmeUeS1apId Tree Initialisation*/
                    st_init(&(p_mme_context->mmeUeS1apId_search_tree),
                            &compare_mmeUeS1apId_search_tree,
                            key_of_mmeUeS1apId_search_tree);

                    /* Init the FSM to open the connection again */ 
                    mme_fsm_process_event(
                            MME_EVENT_S1_MME_PROV_REQ,
                            &(p_mme_context->connection_info),
                            &(p_s1ap_gb_ctx->enb_comm_info),
                            p_s1ap_gb_ctx,
                            p_mme_context);

                    /* The errors are handled by FSM,
                     * We are sending the Success after 
                     * Invoking the MME FSM */
                     /* build failure at index */
                    oam_mme_connection_status_resp.mme_count++;

                   /* SPR_16132 Fix Start */
                    oam_mme_connection_status_resp.
                        oam_mme_conn_info_list[index_to_update].
                        mme_id = (U8)(mme_id);
                   /* SPR_16132 Fix Stop */

                    oam_mme_connection_status_resp.
                        oam_mme_conn_info_list[index_to_update].response =
                        RRC_SUCCESS;

                    index_to_update++;

                }
                else
                {
                    RRC_S1AP_TRACE(RRC_INFO,
                            "[S1AP_OAM_REESTAB_MME_CONN] MME %d "
                            "is not in IDLE State", 
                            s1ap_oam_reestab_mme_conn.mme_id[count]);
                    /* build failure at index */
                    /* Send message to OAM */
                    oam_mme_connection_status_resp.mme_count++;

                   /* SPR_16132 Fix Start */
                    oam_mme_connection_status_resp.
                        oam_mme_conn_info_list[index_to_update].
                        mme_id = (U8)(mme_id);
                   /* SPR_16132 Fix Stop */

                    oam_mme_connection_status_resp.
                        oam_mme_conn_info_list[index_to_update].response =
                        RRC_FAILURE;

                    index_to_update++;
                }
            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, 
                        "[S1AP_OAM_REESTAB_MME_CONN] MME ID %d "
                        "is not valid", s1ap_oam_reestab_mme_conn.mme_id[count]);
                /* build failure at index */
                /* Send message to OAM */
                oam_mme_connection_status_resp.mme_count++;

                /* SPR_16132 Fix Start */
                oam_mme_connection_status_resp.
                    oam_mme_conn_info_list[index_to_update].
                    mme_id = (U8)(mme_id);
                /* SPR_16132 Fix Stop */

                oam_mme_connection_status_resp.
                    oam_mme_conn_info_list[index_to_update].response =
                    RRC_FAILURE;

                index_to_update++;
            } /* end of if */
        }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, 
                        "[S1AP_OAM_REESTAB_MME_CONN] S1ConnectionMode equals to One : "
                        "Cannot reestablish Connection mme_id[%d]", s1ap_oam_reestab_mme_conn.mme_id[count]);
                    /* build failure at index */
                    /* Send message to OAM */
                oam_mme_connection_status_resp.mme_count++;

                oam_mme_connection_status_resp.
                    oam_mme_conn_info_list[index_to_update].
                    mme_id = (U8)(mme_id + 1);

                oam_mme_connection_status_resp.
                    oam_mme_conn_info_list[index_to_update].response =
                    RRC_FAILURE;

                index_to_update++;
            }
            /* SPR 13432 Fix Stop */
        }
    }while (0);

    /* If Complete failure, we build the response and send */
    if (RRC_TRUE == complete_failure)
    {
        oam_mme_connection_status_resp.mme_count = 
            s1ap_oam_reestab_mme_conn.mme_count;

        for (count = 0; count < s1ap_oam_reestab_mme_conn.mme_count; count++)
        {
            oam_mme_connection_status_resp.
                oam_mme_conn_info_list[count].mme_id =
                s1ap_oam_reestab_mme_conn.mme_id[count];
            oam_mme_connection_status_resp.
                oam_mme_conn_info_list[count].response =
                RRC_FAILURE;
        }
    }

    /* this condition should not occur putting the check 
     * Just in case */
    if (index_to_update != (s1ap_oam_reestab_mme_conn.mme_count))
    {
        RRC_S1AP_TRACE(RRC_WARNING, 
                "Unable to proccess complete OAM Response");
    }

    send_oam_mme_connection_status_resp(&oam_mme_connection_status_resp,
            trans_id);

    RRC_S1AP_UT_TRACE_EXIT();
    return; 
}

/**************************************************************************
 * Function Name  : send_oam_mme_connection_status_resp 
 * Inputs         : p_oam_resp - Response to be sent
 *                  trans_id   - OAM Transaction ID
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function send the message to OAM
 ************************************************************************/
rrc_return_et send_oam_mme_connection_status_resp(
        s1ap_oam_mme_conn_status_response_t *p_oam_resp,
        U16 trans_id)
{
    rrc_return_et result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_oam_resp);

    result =
        rrc_s1apOam_il_send_s1ap_oam_mme_conn_status_response(
                p_oam_resp,
                RRC_S1AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                trans_id,
                RRC_NULL);

    if (RRC_FAILURE == result)
    {
        RRC_S1AP_TRACE(RRC_WARNING, 
                "%s: encode failed", __FUNCTION__ );
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO, 
                "%s: sent to OAM", __FUNCTION__ );
    }

    return result;
}

/* SPR 1643 Start */
/****************************************************************************
 * Function Name  : validate_s1ap_oam_plmn_identity 
 * Inputs         : p_s1_setup_req_parameters - pointer points to s1ap setup 
 *                           req parameters received in s1ap oam provisioning 
 *                           request.
 * Outputs        : None 
 * Returns        : RRC_TRUE/RRC_FALSE
 * Description    : This function validates PLMN identiy range
 *                  received in s1ap oam provisioning request.
 ****************************************************************************/
static rrc_bool_et validate_s1ap_oam_plmn_identity(
      s1_setup_req_t  *p_s1_setup_req_parameters)
{
    gb_enb_id_t     *p_gb_enb_id    = PNULL;
    supp_ta_list_t  *p_supp_ta_list = PNULL;
    U32 numSupTas                   = 0;
    U32 numBPLMNs                   = 0;

    p_gb_enb_id    = &(p_s1_setup_req_parameters->gb_enb_id);
    p_supp_ta_list = &(p_s1_setup_req_parameters->supp_ta_list);

   /* Global eNB ID - PLMN Identity */ 
   if (RRC_FAILURE == 
          is_valid_plmn_identity(p_gb_enb_id->plmn_identity.plmn_id))
   {    
         /* Global eNB ID - PLMN ID Error */
         RRC_S1AP_TRACE(RRC_WARNING,
                        "PLMN ID Range Check Fail - Global eNB ID");
         return RRC_FALSE;
   }

   /* Broadcast PLMNs Validation */ 
   for (numSupTas = 0; 
        numSupTas < p_supp_ta_list->num_supported_tais; 
        numSupTas++)
   {
       for (numBPLMNs = 0; 
            numBPLMNs < p_supp_ta_list->supp_tais[numSupTas].bc_plmn_list.num_bplmn; 
            numBPLMNs++)
       {
           if (RRC_FAILURE == 
               is_valid_plmn_identity(p_supp_ta_list->supp_tais[numSupTas].
                   bc_plmn_list.plmn_identity[numBPLMNs].plmn_id))
           {    
               /* Broadcast PLMN ID Error */
               RRC_S1AP_TRACE(RRC_WARNING,
                              "PLMN ID Range Check Fail for Index [%d] - BroadCast PLMNs",numBPLMNs);
               return RRC_FALSE;

           }
       }
   }
   return RRC_TRUE;
}
/* SPR 1643 Stop */


/*****************************************************************************
 * Function Name  : s1ap_build_and_send_oam_s1ap_link_status_ind
 * Inputs         : p_s1ap_sb_context : Pointer to S1AP Global Context
 *                : p_mme_ctx : Pointer to MME Context
 *                : status - Status of S1AP link
 * Outputs        : s1ap_oam_s1ap_link_status_ins message to OAM
 * Returns        : RRC_SUCCESS - Message was successfully composed and sent 
 *                                to OAM.
 *                  RRC_FAILURE - Problem in message composition.
 * Description    : The function creates and send S1AP_OAM_S1AP_LINK_STATUS_IND.
 *****************************************************************************/
rrc_return_et s1ap_build_and_send_oam_s1ap_link_status_ind(
                        s1ap_gb_context_t *p_s1ap_gb_ctx,
                        mme_context_t     *p_mme_ctx,
                        U8                status)
{
    s1ap_oam_s1ap_link_status_ind_t  s1ap_oam_s1ap_link_status_ind;

    RRC_S1AP_UT_TRACE_ENTER();

    /* 15047_start */
    memset_wrapper(&s1ap_oam_s1ap_link_status_ind, RRC_NULL, sizeof(s1ap_oam_s1ap_link_status_ind_t));
    /* 15047_end */

    s1ap_oam_s1ap_link_status_ind.mme_id = p_mme_ctx->mme_id;

    s1ap_oam_s1ap_link_status_ind.status = status;

    /* SPR 13432 Fix Start */
    if (( p_mme_ctx->connection_info.bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT ) &&
            (p_mme_ctx->connection_info.bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT ))        
    {
        s1ap_oam_s1ap_link_status_ind.bitmask |= 
            S1AP_OAM_LINK_STATUS_IND_IPv6_ADDRESS_PRESENT;         

        l3_memcpy_wrapper(s1ap_oam_s1ap_link_status_ind.ipv6_addr, p_mme_ctx->
            connection_info.ipv6_addr[0].ipv6_addr, MAX_IPV6_ADDRESS_LENGTH);
    }
    else
    {
        s1ap_oam_s1ap_link_status_ind.bitmask |=
            S1AP_OAM_LINK_STATUS_IND_IPv4_ADDRESS_PRESENT;

        l3_memcpy_wrapper(s1ap_oam_s1ap_link_status_ind.ip_addr, p_mme_ctx->
            connection_info.ip_addr[0].ip_addr, MAX_IP_ADDRESS_LENGTH);
    }

    s1ap_oam_s1ap_link_status_ind.bitmask |= S1AP_OAM_LINK_STATUS_IND_PORT_PRESENT;
    s1ap_oam_s1ap_link_status_ind.port = p_mme_ctx->connection_info.port;
    /* SPR 13432 Fix Stop */

    /* Encode and send the S1AP link status ind to OAM */
    if (RRC_FAILURE == rrc_s1apOam_il_send_s1ap_oam_s1ap_link_status_ind(
                &s1ap_oam_s1ap_link_status_ind,
                RRC_S1AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                p_s1ap_gb_ctx->s1ap_oam_tx_info.s1ap_oam_transaction_id,
                RRC_NULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
            "build_and_send_s1ap_oam_s1ap_link_status_ind: "
            "rrc_s1apOam_il_send_s1ap_oam_s1ap_link_status_ind returned failure");
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    RRC_S1AP_TRACE(RRC_INFO, "%s Sent Message to OAM for TX ID: %d "
            "\n",__FUNCTION__,p_s1ap_gb_ctx->s1ap_oam_tx_info.
            s1ap_oam_transaction_id);

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/**************************************************************************
 * Function Name  : send_enb_config_update_resp_to_oam 
 * Inputs         : mme_id - MME Id
 *                  response - responce of enb config update 
 *                  p_s1ap_sb_context - pointer to S1AP global context
 *                  trans_id - transaction id
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function builds and send enb config update 
 *                  responce message to OAM
 ************************************************************************/
rrc_return_et build_and_send_enb_config_update_resp_to_oam(
        U8 mme_id,
        s1ap_oam_enb_config_update_resp_cause_et response,
        /*SPR 17777 +-*/
        U16 trans_id)
{
    rrc_return_et result           = RRC_FAILURE;
    s1ap_oam_enb_config_update_response_t    s1ap_oam_enb_config_update_resp;

    RRC_S1AP_UT_TRACE_ENTER();

    /*SPR 17777 +-*/

    memset_wrapper(&s1ap_oam_enb_config_update_resp,
               RRC_NULL,
               sizeof(s1ap_oam_enb_config_update_response_t));

    do
    {

        /* Fill MME ID */
        s1ap_oam_enb_config_update_resp.mme_id   = mme_id;

        /* Fill Response */
        s1ap_oam_enb_config_update_resp.response = response;

        result =
            rrc_s1apOam_il_send_s1ap_oam_enb_config_update_response(
                    &s1ap_oam_enb_config_update_resp,
                    RRC_S1AP_MODULE_ID,
                    RRC_OAM_MODULE_ID,
                    trans_id,
                    RRC_NULL);
        if (RRC_FAILURE == result)
        {
            RRC_S1AP_TRACE(RRC_WARNING, 
                    "[S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE] encode failed");
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO, 
                    "[S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE] sent to OAM");
        }
    } while (0);

    RRC_S1AP_UT_TRACE_EXIT();
    return result;
}

/*sctp parameters start */
/*****************************************************************************
* Function Name  : s1ap_oam_convert_sctp_mib_status
* Inputs         : sctp_mib_status
* Outputs        : None
* Returns        : s1ap_oam_sctp_status
* Description    : This function converts sctp mib status into
*                  s1ap_oam_sctp_status 
****************************************************************************/
#if 0
#ifdef ARICENT_SCTP_IPR
void  s1ap_oam_convert_sctp_mib_status(
        sctpmib_assoc_state_et *p_assoc_state, 
        U32 *p_s1ap_oam_sctp_status )
{
    RRC_S1AP_UT_TRACE_ENTER();
    switch(*p_assoc_state)
    {
        case SM_CLOSED: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_CLOSED;
             break;
        case SM_COOKIE_WAIT:
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_COOKIE_WAIT;
             break;
        case SM_COOKIE_ECHOD: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_COOKIE_ECHOED;
             break;
        case SM_ESTABLISHED: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_ESTABLISHED;
             break;
        case SM_SHUTDOWN_PENDING: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_SHUTDOWN_PENDING;
             break;  
        case SM_SHUTDOWN_SENT: 
            *p_s1ap_oam_sctp_status =  S1AP_OAM_SCTP_SHUTDOWN_SENT;
             break;
        case SM_SHUTDOWN_RECEIVED: 
            *p_s1ap_oam_sctp_status =  S1AP_OAM_SCTP_SHUTDOWN_RECEIVED;
             break;
        case SM_SHUTDOWN_ACK_SENT: 
            *p_s1ap_oam_sctp_status =  S1AP_OAM_SCTP_SHUTDOWN_ACK_SENT;
             break;
        default:
            RRC_TRACE(RRC_WARNING,"Invalid State");
            break;
    }
    RRC_S1AP_UT_TRACE_EXIT();
    
}
#endif
#endif

/*****************************************************************************
 * Function Name  : s1ap_oam_build_get_sctp_status_resp_with_bitmask
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
 * Outputs        : None
 * Returns        : rrc_return_et
 * Description    : This function build the SCTP Status Resp to OAM.
 ****************************************************************************/
rrc_return_et s1ap_oam_build_get_sctp_status_resp_with_bitmask(
	s1ap_gb_context_t *p_s1ap_gb_ctx,
	s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req,
	s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp,
	U8	*failed_mme_count,
	U8	*mme_status_count
		)
{
	U8 mme_counter  = RRC_NULL;		
	U8 mme_id        = RRC_NULL;
	mme_context_t *p_mme_context = PNULL;
	S32 connSock    = RRC_NULL;
	U8 temp_failed_mme_count = RRC_NULL;
	U8 temp_mme_status_count = RRC_NULL;

//#ifdef ARICENT_SCTP_IPR
#if 0
	sctp_sockaddr_st *sctp_sin6;
	sctp_sockaddr_st *sctp_sin;
	sctpmib_assoc_table_st  *p_assoc_tab = PNULL;
	sctp_stats_et            stats_type = SCTP_ASSOC_STATS;
	sctp_error_t             error_code = E_NOERROR;
	U8 ret = RRC_NULL;
	U32 s1ap_sctp_status = RRC_NULL;
	struct in_addr in;
	/*struct in6_addr in6;*/
	char str[MAX_IP_ADDRESS_LENGTH];
#endif	
//#else
	socklen_t optlen = 0;
	struct sctp_status  status;
	struct sctp_paddrinfo *spinfo;
	struct sockaddr_in6 *s_in6;
	struct sockaddr_in6 sa_in6;
	struct sockaddr_in *s_in;
	struct sockaddr_in sa_in;
	char tempname[MAX_IP_ADDRESS_LENGTH];
	U16 local_port = 0;
	U32 socklen = RRC_NULL;
	S32 return_type = RRC_NULL;
//#endif
#if 0
#ifdef ARICENT_SCTP_IPR
	p_assoc_tab = (sctpmib_assoc_table_st*)
		rrc_mem_get(sizeof(sctpmib_assoc_table_st));

	if (PNULL == p_assoc_tab)
	{
		RRC_TRACE(RRC_WARNING,"Unable to allocate memory");
		return RRC_FAILURE;
	}
#endif
#endif

	for (mme_counter = 0; mme_counter < p_s1ap_oam_get_sctp_status_req->
			s1ap_oam_mme_status_list.num_mme_id;mme_counter++)
	{
		/*Get MME Id and MME Context */
		mme_id = (U8)(p_s1ap_oam_get_sctp_status_req->
				s1ap_oam_mme_status_list.mme_id[mme_counter]);
		p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

		if(PNULL == p_mme_context)
		{
			if(!(S1AP_OAM_MME_FAILED_LIST_PRESENT & 
						p_s1ap_oam_get_sctp_status_resp->bitmask))
			{
				p_s1ap_oam_get_sctp_status_resp->bitmask |= 
					S1AP_OAM_MME_FAILED_LIST_PRESENT ;
			}
			p_s1ap_oam_get_sctp_status_resp->mme_failed_list.
				failed_mme_info[temp_failed_mme_count].mme_id = mme_id;
			p_s1ap_oam_get_sctp_status_resp->mme_failed_list.
				failed_mme_info[temp_failed_mme_count].cause = S1AP_OAM_MME_INVALID;
			temp_failed_mme_count++;
			continue;
		}
		if(p_mme_context->current_mme_state != MME_ACTIVE)
		{
			if(!(S1AP_OAM_MME_FAILED_LIST_PRESENT & 
						p_s1ap_oam_get_sctp_status_resp->bitmask))
			{
				p_s1ap_oam_get_sctp_status_resp->bitmask |= 
					S1AP_OAM_MME_FAILED_LIST_PRESENT;
			}
			p_s1ap_oam_get_sctp_status_resp->mme_failed_list.
				failed_mme_info[temp_failed_mme_count].mme_id = mme_id;
			p_s1ap_oam_get_sctp_status_resp->mme_failed_list.
				failed_mme_info[temp_failed_mme_count].cause = S1AP_OAM_MME_NOT_ACTIVE;
			temp_failed_mme_count++;
			continue;
		}
		else
		{
            		/* BUG 11440 Fix Start */
			RRC_S1AP_TRACE(RRC_INFO,
					"MME Context found at Index %d", p_mme_context->mme_id);
            		/* BUG 11440 Fix Stop */
			temp_mme_status_count++;
			connSock = p_mme_context->sctp_sd; 
//#ifdef ARICENT_SCTP_IPR
#if 0
			if(SCTP_FAILURE == sctp_api_enable_stats(stats_type,&error_code))
			{
				RRC_S1AP_TRACE(RRC_WARNING,"Unable to set SCTP Stats:SCTP_ASSOC_STATS");
				if(PNULL != p_assoc_tab)
				{
					rrc_mem_free(p_assoc_tab);
				}
				return RRC_FAILURE;
			}
			ret = sctpmib_get_assoc_table_var(
					connSock,
					SCTP_NULL,
					SCTP_NULL,
					p_assoc_tab,
					&error_code);
#endif			
//#else
			optlen = sizeof(status);
			return_type = getsockopt_wrapper(
					connSock,
					IPPROTO_SCTP,
					SCTP_STATUS,
					&status,
					&optlen);

			spinfo = &status.sstat_primary;
			if (spinfo->spinfo_address.ss_family == AF_INET6) 
			{
				s_in6 = (struct sockaddr_in6 *)&spinfo->spinfo_address;
				inet_ntop_wrapper(AF_INET6, &s_in6->sin6_addr, tempname,
						sizeof (tempname));

				/*getsockname_wrapper to fill in this socket's local address*/
				socklen = sizeof (sa_in6);
				if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in6, (socklen_t *)&socklen) == -1) 
				{
					RRC_S1AP_TRACE(RRC_ERROR, "getsockname_wrapper failed");
					return RRC_FAILURE;
				}
				local_port = (int) ntohs_wrapper(sa_in6.sin6_port);
				RRC_S1AP_TRACE(RRC_DETAILED, "Local port is: %d\n", local_port);
			} 
			else if (spinfo->spinfo_address.ss_family == AF_INET) 
			{
				s_in = (struct sockaddr_in *)&spinfo->spinfo_address;
				inet_ntop_wrapper(AF_INET, &s_in->sin_addr, tempname, sizeof (tempname));

				/*getsockname_wrapper to fill in this socket's local address*/
				socklen = sizeof (sa_in);
				if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in,(socklen_t *)&socklen) == -1) 
				{
					RRC_S1AP_TRACE(RRC_ERROR, "getsockname_wrapper failed");
					return RRC_FAILURE;
				}
				local_port = (int) ntohs_wrapper(sa_in.sin_port);
				RRC_S1AP_TRACE(RRC_INFO, "Local port is: %d\n", local_port);
			} 
			else 
			{
				RRC_S1AP_TRACE(RRC_ERROR, "ss_family is not found");
				return RRC_FAILURE;
			}
//#endif
//#ifdef ARICENT_SCTP_IPR
//			if (SCTP_FAILURE == ret)
//#else
            /* Conerity Fix 86870 start */
		    if (return_type < 0)
            /* Conerity Fix 86870 end */
//#endif
				{
					RRC_S1AP_TRACE(RRC_ERROR,"SCTP : Unable to retreive sctp status parameters");
#if 0
#ifdef ARICENT_SCTP_IPR
					if(PNULL != p_assoc_tab)
					{
						rrc_mem_free(p_assoc_tab);
					}
#endif
#endif
					return RRC_FAILURE;
				}
				else
				{
					if(!(S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT & 
								p_s1ap_oam_get_sctp_status_resp->bitmask))
					{
						p_s1ap_oam_get_sctp_status_resp->bitmask |= 
							S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT;
					}
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].mme_id = p_mme_context->mme_id;

//#ifdef ARICENT_SCTP_IPR
#if 0
					s1ap_oam_convert_sctp_mib_status(&p_assoc_tab->assoc_state,
							&s1ap_sctp_status);
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].status = s1ap_sctp_status;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].in_streams = p_assoc_tab->assoc_in_streams;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].out_streams = p_assoc_tab->assoc_out_streams;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].local_port = p_assoc_tab->local_sctp_port;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].start_time.tv_sec = 
						(U32)p_assoc_tab->assoc_start_time.tv_sec;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].start_time.tv_usec = 
						(U32)p_assoc_tab->assoc_start_time.tv_usec;

					/*Retrieving Primary Peer Address */
					if (p_assoc_tab->rem_prim_add.family == AF_INET6)
					{
						RRC_S1AP_TRACE(RRC_INFO,"Family is AF_INET6");
						sctp_sin6 = &p_assoc_tab->rem_prim_add;
						inet_ntop_wrapper(AF_INET6, &(sctp_sin6->ip.v6.addr),str,INET_ADDRSTRLEN);
						l3_memcpy_wrapper(&p_s1ap_oam_get_sctp_status_resp->
								s1ap_oam_mme_status_params_list.staus_params[mme_counter].
								primary_peer_addr,str,
								(sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
					}
					else if (p_assoc_tab->rem_prim_add.family == AF_INET)
					{
						RRC_S1AP_TRACE(RRC_INFO,"Family is AF_INET");

						sctp_sin = &p_assoc_tab->rem_prim_add;
						in.s_addr = SCTP_HTONL(sctp_sin->ip.v4.addr);
						l3_memcpy_wrapper(&p_s1ap_oam_get_sctp_status_resp->
								s1ap_oam_mme_status_params_list.staus_params[mme_counter].
								primary_peer_addr,inet_ntoa_wrapper(in),
								(sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
					}
					else
					{
						RRC_S1AP_TRACE(RRC_ERROR, "Unknown Family Name");
						if(PNULL != p_assoc_tab)
						{
							rrc_mem_free(p_assoc_tab);
						}
						return RRC_FAILURE;
					}
#endif
//#else
					/* Bug 3826 Start */
					/*Resoning from BLR-BZ*
					  In S1AP_OAM_GET_SCTP_STATUS_RESP, RRC writes the sctp state value (sstat_state)
					  returned by Linux kernel in getsockopt call. As per kernel code, following are
					  the SCP state values:
					  typedef enum {

					  SCTP_STATE_CLOSED               = 0,
					  SCTP_STATE_COOKIE_WAIT          = 1,
					  SCTP_STATE_COOKIE_ECHOED        = 2,
					  SCTP_STATE_ESTABLISHED          = 3,
					  SCTP_STATE_SHUTDOWN_PENDING     = 4,
					  SCTP_STATE_SHUTDOWN_SENT        = 5,
					  SCTP_STATE_SHUTDOWN_RECEIVED    = 6,
					  SCTP_STATE_SHUTDOWN_ACK_SENT    = 7,

					  } sctp_state_t;


					  But, as per API document, following are the status returned by RRC to OAM:

					  typedef enum
					  {
					  S1AP_OAM_SCTP_CLOSED,
					  S1AP_OAM_SCTP_BOUND,
					  S1AP_OAM_SCTP_LISTEN,
					  S1AP_OAM_SCTP_COOKIE_WAIT,
					  S1AP_OAM_SCTP_COOKIE_ECHOED,
					  S1AP_OAM_SCTP_ESTABLISHED,
					  S1AP_OAM_SCTP_SHUTDOWN_PENDING,
					  S1AP_OAM_SCTP_SHUTDOWN_SENT,
					  S1AP_OAM_SCTP_SHUTDOWN_RECEIVED,
					  S1AP_OAM_SCTP_SHUTDOWN_ACK_SENT
					  }s1ap_oam_sctp_status_et

					  So, at RRC side, we need to map the response received from LKSCTP to match with
					  the API documented values		
					  */
					if ( status.sstat_state != 0)
					{
					    //Update the status value to conform to the interface API document.
					    status.sstat_state = status.sstat_state + 2;
					}
					/* Bug 3826 End */
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].mme_id = p_mme_context->mme_id;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].status = (U32)status.sstat_state;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].in_streams = status.sstat_instrms;
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].out_streams = status.sstat_outstrms;

					/*Filling local Port*/
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].local_port = local_port;
					l3_memcpy_wrapper(&p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
							staus_params[mme_counter].primary_peer_addr,tempname,
							(sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
					/*Filling Start Time*/
					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].start_time.tv_sec = 
						p_s1ap_gb_ctx->sctp_start_time.tv_sec;

					p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
						staus_params[mme_counter].start_time.tv_usec = 
						p_s1ap_gb_ctx->sctp_start_time.tv_usec;
//#endif
				}
		}
	}
	if(S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT & 
			p_s1ap_oam_get_sctp_status_resp->bitmask)
	{
		p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
			num_mme_id = temp_mme_status_count;
	}
	else if(S1AP_OAM_MME_FAILED_LIST_PRESENT & 
			p_s1ap_oam_get_sctp_status_resp->bitmask)
	{
		p_s1ap_oam_get_sctp_status_resp->mme_failed_list.count = 
			temp_failed_mme_count;
	}
	*failed_mme_count = temp_failed_mme_count;
	*mme_status_count = temp_mme_status_count;
#if 0
#ifdef ARICENT_SCTP_IPR
    if(PNULL != p_assoc_tab)
    {
        rrc_mem_free(p_assoc_tab);
    }
#endif
#endif
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : s1ap_oam_build_get_sctp_status_resp_without_bitmask 
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
 * Outputs        : None
 * Returns        : rrc_return_et
 * Description    : This function build the SCTP Status Resp to OAM.
 ****************************************************************************/
rrc_return_et s1ap_oam_build_get_sctp_status_resp_without_bitmask(
	s1ap_gb_context_t *p_s1ap_gb_ctx,
	s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp,
	U8	*mme_status_count
		)
{
	U8 mme_counter  = RRC_NULL;		
	mme_context_t *p_mme_context = PNULL;
	S32 connSock    = RRC_NULL;
	U8 temp_mme_status_count = RRC_NULL;

//#ifdef ARICENT_SCTP_IPR
#if 0
	sctp_status_st status;
	sctp_sockaddr_st *sctp_sin6;
	sctp_sockaddr_st *sctp_sin;
	sctpmib_assoc_table_st  *p_assoc_tab = PNULL;
	sctp_stats_et            stats_type = SCTP_ASSOC_STATS;
	sctp_error_t             error_code = E_NOERROR;
	U8 ret = RRC_NULL;
	U32 s1ap_sctp_status = RRC_NULL;
	struct in_addr in;
	/*struct in6_addr in6;*/
	char str[MAX_IP_ADDRESS_LENGTH];
#endif	
//#else
	socklen_t optlen = 0;
	struct sctp_status  status;
	struct sctp_paddrinfo *spinfo;
	struct sockaddr_in6 *s_in6;
	struct sockaddr_in6 sa_in6;
	struct sockaddr_in *s_in;
	struct sockaddr_in sa_in;
	char tempname[MAX_IP_ADDRESS_LENGTH];
	U16 local_port = 0;
	U32 socklen = RRC_NULL;
	S32 return_type = RRC_NULL;
//#endif
    /* Coverity Fix 86878 start */
    memset_wrapper(&status, 0, sizeof(status));
    /* Coverity Fix 86878 end */
#if 0	
#ifdef ARICENT_SCTP_IPR
	p_assoc_tab = (sctpmib_assoc_table_st*)
		rrc_mem_get(sizeof(sctpmib_assoc_table_st));

	if (PNULL == p_assoc_tab)
	{
		RRC_TRACE(RRC_WARNING,"Unable to allocate memory");
		return RRC_FAILURE;
	}
#endif
#endif	
	for (mme_counter = 0; mme_counter < MAX_NUM_MME ; mme_counter++)
	{
		p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_counter];

		if (PNULL == p_mme_context)
		{
			RRC_S1AP_TRACE(RRC_WARNING,
					"MME Context NULL at Index %d", mme_counter);
			continue;
		}
		/* Check whether MME's sctp assoc is active or not */
		if (p_mme_context->current_mme_state != MME_ACTIVE)
		{
			RRC_S1AP_TRACE(RRC_INFO, "%s:"
					" MME context with id %d is not active.",
					__FUNCTION__,p_mme_context->mme_id);
			continue;
		}
		RRC_S1AP_TRACE(RRC_WARNING,
				"MME Context found at Index %d", mme_counter);
		temp_mme_status_count++;
		connSock = p_mme_context->sctp_sd; 
//#ifdef ARICENT_SCTP_IPR
#if 0
		if(SCTP_FAILURE == sctp_api_enable_stats(stats_type,&error_code))
		{
			RRC_S1AP_TRACE(RRC_WARNING,"Unable to set SCTP Stats:SCTP_ASSOC_STATS");
			if(PNULL != p_assoc_tab)
			{
				rrc_mem_free(p_assoc_tab);
			}
			return RRC_FAILURE;
		}
		ret = sctpmib_get_assoc_table_var(
				connSock,
				SCTP_NULL,
				SCTP_NULL,
				p_assoc_tab,
				&error_code);
#endif	
//#else
		optlen = sizeof(status);
		return_type = getsockopt_wrapper(
				connSock,
				IPPROTO_SCTP,
				SCTP_STATUS,
				&status,
				&optlen);

		spinfo = &status.sstat_primary;
		if (spinfo->spinfo_address.ss_family == AF_INET6) 
		{
			s_in6 = (struct sockaddr_in6 *)&spinfo->spinfo_address;
			inet_ntop_wrapper(AF_INET6, &s_in6->sin6_addr, tempname,
					sizeof (tempname));

			/*getsockname_wrapper to fill in this socket's local address*/
			socklen = sizeof (sa_in6);
			if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in6, (socklen_t *)&socklen) == -1) 
			{
				RRC_S1AP_TRACE(RRC_ERROR, "getsockname_wrapper failed");
				return RRC_FAILURE;
			}
			local_port = (int) ntohs_wrapper(sa_in6.sin6_port);
			RRC_S1AP_TRACE(RRC_INFO, "Local port is: %d", local_port);
		} 
		else if (spinfo->spinfo_address.ss_family == AF_INET) 
		{
			s_in = (struct sockaddr_in *)&spinfo->spinfo_address;
			inet_ntop_wrapper(AF_INET, &s_in->sin_addr, tempname, sizeof (tempname));

			/*getsockname_wrapper to fill in this socket's local address*/
			socklen = sizeof (sa_in);
			if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in,(socklen_t *)&socklen) == -1) 
			{
				RRC_S1AP_TRACE(RRC_ERROR, "getsockname_wrapper failed");
				return RRC_FAILURE;
			}
			local_port = (int) ntohs_wrapper(sa_in.sin_port);
			RRC_S1AP_TRACE(RRC_INFO, "Local port is: %d", local_port);
		} 
		else 
		{
			RRC_S1AP_TRACE(RRC_ERROR, "ss_family is not matched");
			return RRC_FAILURE;
		}
//#endif
//#ifdef ARICENT_SCTP_IPR
//		if (SCTP_FAILURE == ret)
//#else
        /* Coverity Fix 86878 start */
		if (return_type < 0)
//#endif
		{
		    RRC_S1AP_TRACE(RRC_ERROR,"SCTP : Unable to retreive sctp status parameters");
#if 0
#ifdef ARICENT_SCTP_IPR
			if(PNULL != p_assoc_tab)
			{
			    rrc_mem_free(p_assoc_tab);
			}
#endif
#endif	
			return RRC_FAILURE;
		}
		else
		{
		    if(!(S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT &
					p_s1ap_oam_get_sctp_status_resp->bitmask))
			{
			    p_s1ap_oam_get_sctp_status_resp->bitmask |= 
					S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT;
			}
//#ifdef ARICENT_SCTP_IPR
#if 0
			s1ap_oam_convert_sctp_mib_status(&p_assoc_tab->assoc_state,
					&s1ap_sctp_status);
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].status = s1ap_sctp_status;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].in_streams = p_assoc_tab->assoc_in_streams;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].out_streams = p_assoc_tab->assoc_out_streams;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].local_port = p_assoc_tab->local_sctp_port;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].start_time.tv_sec = 
				(U32)p_assoc_tab->assoc_start_time.tv_sec;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].start_time.tv_usec = 
				(U32)p_assoc_tab->assoc_start_time.tv_usec;

			/*Retrieving Primary Peer Address */

			if (p_assoc_tab->rem_prim_add.family == AF_INET6)
			{
				RRC_S1AP_TRACE(RRC_INFO,"Family is AF_INET6");
				sctp_sin6 = &p_assoc_tab->rem_prim_add;
				inet_ntop_wrapper(AF_INET6, &(sctp_sin6->ip.v6.addr),str,INET_ADDRSTRLEN);
				l3_memcpy_wrapper(&p_s1ap_oam_get_sctp_status_resp->
						s1ap_oam_mme_status_params_list.staus_params[mme_counter].
						primary_peer_addr,str,
						(sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
			}
			else if (p_assoc_tab->rem_prim_add.family == AF_INET)
			{
				RRC_S1AP_TRACE(RRC_INFO,"Family is AF_INET");

                sctp_sin = &p_assoc_tab->rem_prim_add;
				in.s_addr = SCTP_HTONL(sctp_sin->ip.v4.addr);
				l3_memcpy_wrapper(&p_s1ap_oam_get_sctp_status_resp->
						s1ap_oam_mme_status_params_list.staus_params[mme_counter].
						primary_peer_addr,inet_ntoa_wrapper(in),
						(sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
			}
			else
			{
				RRC_S1AP_TRACE(RRC_ERROR, "Unknown family name");
				if(PNULL != p_assoc_tab)
				{
					rrc_mem_free(p_assoc_tab);
				}
				return RRC_FAILURE;
			}
#endif	
//#else
			/* Bug 3826 Start */
			if ( status.sstat_state != 0)
			{
			    //Update the status value to conform to the interface API document.
			    status.sstat_state = status.sstat_state + 2;
			}
			/* Bug 3826 End */
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].mme_id = p_mme_context->mme_id;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].status = (U32)status.sstat_state;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].in_streams = status.sstat_instrms;
			p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].out_streams = status.sstat_outstrms;

            p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].local_port = local_port;
			l3_memcpy_wrapper(&p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
					staus_params[mme_counter].primary_peer_addr,tempname,
					(sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

            p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].start_time.tv_sec = 
				p_s1ap_gb_ctx->sctp_start_time.tv_sec;

            p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
				staus_params[mme_counter].start_time.tv_usec = 
				p_s1ap_gb_ctx->sctp_start_time.tv_usec;
//#endif
		}
        /* Coverity Fix 86878 end */
	}
	if(S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT &
			p_s1ap_oam_get_sctp_status_resp->bitmask)
	{
		p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list.
			num_mme_id = temp_mme_status_count;
	}

        *mme_status_count = temp_mme_status_count;
#if 0
#ifdef ARICENT_SCTP_IPR
    if(PNULL != p_assoc_tab)
    {
        rrc_mem_free(p_assoc_tab);
    }
#endif
#endif	
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : s1ap_oam_process_get_sctp_status_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function build and send the SCTP Status Resp to OAM.
 ****************************************************************************/
void 
s1ap_oam_process_get_sctp_status_req(
        void *p_api, 
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
    s1ap_oam_get_sctp_status_req_t s1ap_oam_get_sctp_status_req; 
    s1ap_oam_get_sctp_status_resp_t s1ap_oam_get_sctp_status_resp;
    rrc_return_et response = RRC_FAILURE;
    U8 mme_status_count  = RRC_NULL;
    U8 failed_mme_count  = RRC_NULL;

    S32 length_read  = RRC_NULL;
    U16 trans_id     = RRC_NULL;
    
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    
    /* MEMSET */
    memset_wrapper(&s1ap_oam_get_sctp_status_req, 0, 
            sizeof(s1ap_oam_get_sctp_status_req_t));
    memset_wrapper(&s1ap_oam_get_sctp_status_resp, 0,
            sizeof(s1ap_oam_get_sctp_status_resp_t));
    /* Get the transaction id */
    trans_id = rrc_get_transaction_id(p_api);
    do
    {
        /* Decode the received message from OAM */
        response = rrc_il_parse_s1ap_oam_get_sctp_status_req(
                &s1ap_oam_get_sctp_status_req,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - 
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read);

        /* Send the failiure message to OAM on Decode failure */
        if (RRC_FAILURE == response) 
        {
            RRC_S1AP_TRACE(RRC_WARNING, 
                    "[S1AP_OAM_GET_SCTP_STATUS_REQ] decode failed");
            break;
        }

        RRC_S1AP_TRACE(RRC_INFO, "[S1AP_OAM_GET_SCTP_STATUS_REQ] decode success");

        if (S1AP_OAM_MME_STATUS_LIST_PRESENT &
                s1ap_oam_get_sctp_status_req.bitmask)
        {
	    if ( RRC_FAILURE == s1ap_oam_build_get_sctp_status_resp_with_bitmask(
				p_s1ap_gb_ctx,
				&s1ap_oam_get_sctp_status_req,
				&s1ap_oam_get_sctp_status_resp,
				&failed_mme_count,
				&mme_status_count
				))
	     {
                 RRC_S1AP_TRACE(RRC_ERROR, 
                    "[S1AP_OAM_GET_SCTP_STATUS_REQ] build get_sctp_status_resp_with_bitmask failed");
		 return;
             }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,"Bitmask is not set");
	    if ( RRC_FAILURE == s1ap_oam_build_get_sctp_status_resp_without_bitmask(
				p_s1ap_gb_ctx,
				&s1ap_oam_get_sctp_status_resp,
				&mme_status_count
				))
	     {
                 RRC_S1AP_TRACE(RRC_ERROR, 
                    "[S1AP_OAM_GET_SCTP_STATUS_REQ] build get_sctp_status_resp_without_bitmask failed");
		 return;
             }
        }

        if(mme_status_count > 0)
        {
            s1ap_oam_get_sctp_status_resp.response = RRC_SUCCESS;
        }
        else if(failed_mme_count > 0)
        {
            s1ap_oam_get_sctp_status_resp.response = RRC_PARTIAL_SUCCESS;
        }
        else
        {
            s1ap_oam_get_sctp_status_resp.bitmask = RRC_NULL;
            s1ap_oam_get_sctp_status_resp.response = RRC_FAILURE;
        }

        /* Send SCTP Status Resp to OAM */
        if (RRC_FAILURE == rrc_s1apOam_il_send_s1ap_oam_get_sctp_status_resp(
                    &s1ap_oam_get_sctp_status_resp,
                    RRC_S1AP_MODULE_ID,
                    RRC_OAM_MODULE_ID,
                    trans_id,
                    RRC_NULL))
        {
            RRC_S1AP_TRACE(RRC_WARNING, 
                    "s1ap_oam_get_sctp_status_resp encode failed ");
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO, 
                    "s1ap_oam_get_sctp_status_resp sent to OAM" );
        }
    } while (0); 
    RRC_S1AP_UT_TRACE_EXIT();
    return;
}

/*sctp parameters stop */

/******************************************************************************
 *   FUNCTION NAME: s1ap_process_OAM_PROC_SUPP_REQ 
 *   Inputs         :p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *   DESCRIPTION:
 *       This function checks the status of s1ap global health monitoring variable
 *       based on it sends S1AP_OAM_PROC_SUPP_RESP message to OAM
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/

void s1ap_oam_process_proc_req(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
    U16 trans_id = RRC_NULL;
    U8  count = RRC_NULL;
    U8  no_of_active_th = RRC_NULL;

    RRC_S1AP_TRACE(RRC_INFO, 
            "[S1AP_OAM__PROC_SUP_REQ] RECEIVED");
    /* Get the transaction id */
   trans_id = rrc_get_transaction_id(p_api);

    for(count = 0;count < S1AP_SCTP_MAX_THREADS;count++)
    {
        /*SPR 20421 fix start*/
        /*
        if(p_s1ap_gb_ctx->s1ap_health_monitor_val[count] != RRC_TRUE)
        {
            RRC_S1AP_TRACE(RRC_FATAL, 
                    "s1ap_oam_process_proc_req: Status of the thread :%s",
                    s1ap_health_monitor_desriptors[count].thrd_info);
        }
        else
        */
        /*SPR 20421 fix end*/
        {
            no_of_active_th++;
        }
    }

    if(S1AP_SCTP_MAX_THREADS == no_of_active_th)
    {
        s1ap_oam_send_proc_res(RRC_S1AP_MODULE_ID,trans_id);
    }
}

/******************************************************************************
 *   FUNCTION NAME: s1ap_oam_send_proc_res 
 *   DESCRIPTION:
 *       This function constructs and sends S1AP_OAM_PROC_SUPP_RESP message to OAM
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void s1ap_oam_send_proc_res
(
        U16 src_module_id,
        U16 trans_id
)
{
    U8* p_msg = PNULL;

    U16   total_msg_len = CV_HDRSIZE + RRC_INTERFACE_API_HEADER_SIZE;
    U16   msg_len = 0;


    p_msg = (U8*) rrc_msg_mem_get(total_msg_len);

    if (PNULL == p_msg)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s:rrc_msg_mem_get failed",
                __FUNCTION__);
        return;
    }

    /* Fill CSPL header */
    rrc_construct_api_header(
            p_msg,
            RRC_VERSION_ID,
            src_module_id,
            RRC_OAM_MODULE_ID,
            S1AP_OAM_PROC_SUPP_RESP,
            total_msg_len);

    /* Fill interface header */
    rrc_construct_interface_api_header(
            p_msg + RRC_API_HEADER_SIZE,
            trans_id,
            src_module_id,
            RRC_OAM_MODULE_ID,
            S1AP_OAM_PROC_SUPP_RESP,
            msg_len,
            RRC_NULL);

    /* Send message to */
    /*SPR 20172 Fix Start*/
    rrc_send_message(p_msg,RRC_OAM_MODULE_ID);
    /*SPR 20172 Fix End*/

}

/******************************************************************************
 *   FUNCTION NAME: start_s1ap_health_monitor_timer 
 *   DESCRIPTION:
 *       This function starts the s1ap health monitor
 *       timer provided by the user if the duration 
 *       of timer is greater than zero
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/

void start_s1ap_health_monitor_timer
(
 /*SPR 17777 +-*/
   U32                  duration
)
{
    s1ap_timer_buffer_t timer_buf;
    rrc_timer_t timer;

    RRC_S1AP_UT_TRACE_ENTER();
    /*SPR 17777 +-*/

    if(duration == 0)
    {
        return;
    }

    timer_buf.timer_type = S1AP_OAM_HEALTH_MONITOR_TIMER;
    timer_buf.auto_delete = RRC_FALSE;

    timer = rrc_start_timer(duration,
            &timer_buf,
            sizeof(timer_buf),
            RRC_TRUE);
    if ( PNULL == timer )
    {
        RRC_S1AP_TRACE(RRC_WARNING,
           "Error in starting the timer ");
    }

    RRC_S1AP_UT_TRACE_EXIT();
}
