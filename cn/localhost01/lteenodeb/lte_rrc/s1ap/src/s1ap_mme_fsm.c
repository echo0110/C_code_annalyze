/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: s1ap_mme_fsm.c,v 1.6 2010/10/13 07:04:23 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : This file comtains the S1AP MME FSM
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_mme_fsm.c,v $
 * Revision 1.6  2010/10/13 07:04:23  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.5.4.3  2010/10/04 13:31:54  gur04518
 * removed backslash
 *
 * Revision 1.5.4.2  2010/10/02 13:55:27  gur16768
 * put a null check before fetching the next node in reset all ue context process function
 *
 * Revision 1.5.4.1  2010/09/09 10:10:39  gur22047
 * modified_during_s1ap_UT added_stream_id
 *
 * Revision 1.5  2010/03/30 06:48:39  gur18569
 * Optimized Reset Procedure
 *
 * Revision 1.4  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.3  2010/02/18 04:25:20  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.17  2009/12/28 10:14:21  gur21006
 * system calls removed
 *
 * Revision 1.1.2.16  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.15  2009/12/27 08:39:04  gur18569
 * removed tabs
 *
 * Revision 1.1.2.14  2009/12/27 07:57:29  gur18569
 * fixed bug: 634c634 void*)((U32*) p_intrl_msg wrongly typecasted
 *
 * Revision 1.1.2.13  2009/12/26 12:05:01  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.12  2009/12/24 10:29:58  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.11  2009/12/23 06:42:29  gur21006
 * Warning Removed
 *
 * Revision 1.1.2.10  2009/12/22 14:37:20  gur18569
 * fixed 154
 *
 * Revision 1.1.2.9  2009/12/18 11:53:00  gur20470
 * Fixed SPR 174 for Assertion Failing On Timer_id NULL check
 *
 * Revision 1.1.2.8  2009/12/18 11:41:08  gur18569
 * fixed spr: 184
 *
 * Revision 1.1.2.7  2009/12/17 15:08:54  gur18569
 * send S1AP RESET IND msg to UECC is num prov mme ==1
 *
 * Revision 1.1.2.6  2009/12/17 08:01:54  gur18569
 * sending reset all to uecc if num mme ==1
 *
 * Revision 1.1.2.5  2009/12/11 04:46:50  gur21006
 * Added support for sctp shutdown event
 *
 * Revision 1.1.2.4  2009/12/04 11:51:29  gur21006
 * New functions added
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
 * Revision 1.1.2.26  2009/11/24 12:20:40  gur20470
 * Removed Warnings of execution
 *
 * Revision 1.1.2.25  2009/11/23 15:08:11  gur18569
 * ut fixes
 *
 * Revision 1.1.2.24  2009/11/23 10:55:25  gur21006
 * Each timer id initialised to NULL after rrc_stop_timer
 *
 * Revision 1.1.2.22  2009/11/23 07:26:59  gur21006
 * New function added : delete_sctp_sd_from_tree
 *
 * Revision 1.1.2.20  2009/11/22 12:05:43  gur18569
 * freeing deleted nodes from search tree
 *
 * Revision 1.1.2.19  2009/11/21 12:17:58  gur21006
 * FSM changed
 *
 * Revision 1.1.2.18  2009/11/21 06:38:44  gur18569
 * fixed error in process_reset_all
 *
 * Revision 1.1.2.17  2009/11/21 06:37:59  gur21006
 * Converted Timer value in milli sec in S1 setup with TTW
 *
 * Revision 1.1.2.16  2009/11/20 14:06:22  gur21006
 * Segmentation fault removed
 *
 * Revision 1.1.2.15  2009/11/20 09:19:33  gur21006
 * Arguments for s1 setup resp and resp failure changed
 *
 * Revision 1.1.2.14  2009/11/20 08:42:31  gur21006
 * function prototype of l3_open_sctp_association modified
 *
 * Revision 1.1.2.13  2009/11/19 22:33:43  gur20052
 * Updated for the change in typecast of Pointer
 *
 * Revision 1.1.2.12  2009/11/19 21:40:25  gur20052
 *  Added the mme name from S1Setup
 *
 * Revision 1.1.2.11  2009/11/19 13:29:01  gur21006
 * Segmentation faults removed in compose_asn_s1_setup_req function calls
 *
 * Revision 1.1.2.10  2009/11/19 06:29:07  gur21006
 * functionality added for deletion of socket desc from search tree after closing it
 *
 * Revision 1.1.2.9  2009/11/18 12:34:10  gur20470
 * adding sctp sd into search tree upon connection success
 *
 * Revision 1.1.2.8  2009/11/17 10:19:44  gur20052
 *  Review Comments are incorporated
 *
 * Revision 1.1.2.7  2009/11/09 09:18:23  gur18569
 * removed compilation warnings
 *
 * Revision 1.1.2.6  2009/10/29 17:30:22  gur20052
 *  Added deletion mme from active mme info
 *
 * Revision 1.1.2.5  2009/10/29 14:50:35  gur20052
 * change in mme_fsm
 *
 * Revision 1.1.2.4  2009/10/29 14:35:12  gur20052
 * Removed the redundant statement
 *
 * Revision 1.1.2.3  2009/10/23 16:08:53  gur18569
 * removed compilation errors
 *
 * Revision 1.1.2.2  2009/10/23 11:43:19  gur20052
 * Removed compilation errors
 *
 * Revision 1.1.2.1  2009/10/14 13:33:25  gur20052
 * Added initial version
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_mme_fsm.h"
#include "list_wrap.h"
#include "s1ap_enb_mgr_fsm.h"
#include "s1ap_oam.h"
#include "s1ap_uecc.h"
#include "rrc_common_md.h"
#include "rrc_logging.h"
#include "rrc_s1apRrm_il_composer.h"
/*SPR 15570 Fix Start*/
#include "rrc_defines.h"
/*SPR 15570 Fix Stop*/

#ifdef LINUX_PC_TEST
extern S32 pctest_forward_mme_message_to_pc(    U16          src_module_id,U16 dst_module_id,U16 api_id,U32 socketId,U32 streamId,const void * pmsg, U32 msglen);
#endif

 /****************************************************************************
   Private Definitions
  ****************************************************************************/


 /****************************************************************************
   Private Types
  ****************************************************************************/

 /****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/


 /*****************************************************************************
  * Private Constants
  ****************************************************************************/

 /****************************************************************************
  * Exported Variables
  ****************************************************************************/
 /****************************************************************************
  * Private Variables (Must be declared static)
  ****************************************************************************/
 const S8* MME_FSM_STATES_NAMES[] =
{
    (const S8*)"MME_IDLE",
    (const S8*)"MME_W_S1_SETUP_RESP",
    (const S8*)"MME_ACTIVE",
    (const S8*)"MME_W_SCTP_CONN_RECOVERY",
    (const S8*)"MME_W_CONNECT_SUCCESSFUL",
    (const S8*)"MME_W_TIME_TO_WAIT_TIMER_EXPIRY",
    /*SPR 15570 Fix Start*/
    (const S8*)"MME_W_SCTP_CONNECT",
    /*SPR 15570 Fix Stop*/
    (const S8*)"MME_NUM_OF_STATES",
    (const S8*)"MME_INVALID_STATE"
};

const S8* MME_FSM_EVENTS_NAMES[] =
{
    (const S8*)"MME_EVENT_S1_MME_PROV_REQ",
    (const S8*)"MME_EVENT_S1_SETUP_RSP_TIMER_EXPIRY",
    (const S8*)"MME_EVENT_S1_SETUP_RSP",
    (const S8*)"MME_EVENT_S1_SETUP_FAILURE",
    (const S8*)"MME_EVENT_SCTP_CONN_FAILURE_IND",
    (const S8*)"MME_EVENT_SCTP_CONN_RECOVERY_IND",
    (const S8*)"MME_EVENT_SCTP_CONN_REC_TIMER_EXPIRY",
    (const S8*)"MME_EVENT_SCTP_CONNECT_TIMER_EXPIRY",
    (const S8*)"MME_EVENT_S1_SETUP_TIME_TO_WAIT_TIMER_EXPIRY",
    (const S8*)"MME_EVENT_SCTP_SHUTDOWN",
    (const S8*)"MME_EVENT_CLOSE_MME_CONNECTION",
/*Bug 9050 fixes start*/
    (const S8*)"MME_EVENT_SCTP_ASSOC_UP",
    (const S8*)"MME_EVENT_SCTP_ASSOC_DOWN",
/*Bug 9050 fixes end*/
    (const S8*)"MME_NUM_OF_EVENTS",
    (const S8*)"MME_INVALID_EVENT"
};

const S8* MME_LOG_LEVELS_NAMES[] =
{
    (const S8*)"MME_LOG_LEVEL_FATAL",
    (const S8*)"MME_LOG_LEVEL_ERROR",
    (const S8*)"MME_LOG_LEVEL_WARNING",
    (const S8*)"MME_LOG_LEVEL_INFO",
    (const S8*)"MME_LOG_LEVEL_BRIEF",
    (const S8*)"MME_LOG_LEVEL_DETAILED"
};
/* Handlers for events occuring during Initial state */
static mme_fsm_ret mme_fsm[MME_NUM_OF_STATES][MME_NUM_OF_EVENTS] =
{
    /* State MME_IDLE */
    {
        mme_fsm_mme_init_req ,          /* MME_EVENT_S1_MME_PROV_REQ  */
        mme_fsm_invalid_event_handler, /* MME_EVENT_S1_SETUP_RSP_TIMER_EXPIRY*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_FAILURE */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_FAILURE_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_RECOVERY_IND */
        mme_fsm_invalid_event_handler,/* MME_EVENT_SCTP_CONN_REC_TIMER_EXP*/
        mme_fsm_invalid_event_handler, /* MME_EVENT_SCTP_CONNECT_TIMER_EXP*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_TIME_TO_*/
        /*WAIT_TIMER_EXPIRY */
        mme_fsm_invalid_event_handler, /* MME_EVENT_SCTP_SHUTDOWN*/   
/*Bug 9050 fixes start*/
        mme_fsm_invalid_event_handler, /* MME_EVENT_CLOSE_MME_CONNECTION */
        mme_fsm_invalid_event_handler,  /*MME_EVENT_SCTP_ASSOC_UP*/
        mme_fsm_invalid_event_handler   /*MME_EVENT_SCTP_ASSOC_DOWN*/
/*Bug 9050 fixes end*/
    },
    /* State MME_W_S1_SETUP_RESP */
    {
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_MME_PROV_REQ*/
        mme_fsm_s1_setup_rsp_timer_expiry,/*MME_EVENT_S1_SETUP_RSP_TIMER_EXP*/
        mme_fsm_s1_setup_resp,          /* MME_EVENT_S1_SETUP_RSP  */
        mme_fsm_s1_setup_failure,       /* MME_EVENT_S1_SETUP_FAILURE*/
        mme_fsm_w_s1_resp_conn_failure,  /* MME_EVENT_SCTP_CONN_FAILURE_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_RECOVERY_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_REC_TIMER_EXP*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONNECT_TIMER_EXPI*/
        mme_fsm_invalid_event_handler,   /* MME_EVENT_S1_SETUP_TIME_TO_*/
        /*WAIT_TIMER_EXPIRY */
        mme_fsm_shutdown_event_handler, /* MME_EVENT_SCTP_SHUTDOWN*/   
/*Bug 9050 fixes start*/
        mme_fsm_invalid_event_handler, /* MME_EVENT_CLOSE_MME_CONNECTION */
        mme_fsm_invalid_event_handler,  /*MME_EVENT_SCTP_ASSOC_UP*/
        mme_fsm_invalid_event_handler   /*MME_EVENT_SCTP_ASSOC_DOWN*/
/*Bug 9050 fixes end*/
    },
    /* State MME_ACTIVE */
    {
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_MME_PROV_REQ*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP_TIMER_EXP*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP  */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_FAILURE*/
        mme_fsm_sctp_conn_failure_ind,  /* MME_EVENT_SCTP_CONN_FAILURE_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_RECOVERY_IND */
        mme_fsm_invalid_event_handler,/* MME_EVENT_SCTP_CONN_REC_TIMER_EXP*/
        mme_fsm_invalid_event_handler, /* MME_EVENT_SCTP_CONNECT_TIMER_EXP*/
        mme_fsm_invalid_event_handler,   /* MME_EVENT_S1_SETUP_TIME_TO*/
        /*_WAIT_TIMER_EXPIRY */
        mme_fsm_mme_active_shutdown_event_handler, /* MME_EVENT_SCTP_SHUTDOWN*/   
/*Bug 9050 fixes start*/
        mme_fsm_oam_mme_close_conn_handler,     /* MME_EVENT_CLOSE_MME_CONNECTION */
        mme_fsm_invalid_event_handler,  /*MME_EVENT_SCTP_ASSOC_UP*/
        mme_fsm_invalid_event_handler   /*MME_EVENT_SCTP_ASSOC_DOWN*/
/*Bug 9050 fixes end*/
    },
    /* State MME_W_SCTP_CONN_RECOVERY */
    {
        mme_fsm_invalid_event_handler,   /* MME_EVENT_S1_MME_PROV_REQ*/
        mme_fsm_invalid_event_handler,   /* MME_EVENT_S1_SETUP_RSP_TIMER_EXP*/
        mme_fsm_invalid_event_handler,   /* MME_EVENT_S1_SETUP_RSP  */
        mme_fsm_invalid_event_handler,   /* MME_EVENT_S1_SETUP_FAILURE*/
        mme_fsm_sctp_re_conn_try_failure_ind,
                 /* MME_EVENT_SCTP_CONN_FAILURE_IND*/
        mme_fsm_sctp_conn_rec_ind,       /* MME_EVENT_SCTP_CONN_RECOVERY_IND */
        mme_fsm_sctp_conn_rec_timer_expiry,/*MME_EVENT_SCTP_CONN_REC_TIMER_EX*/
        mme_fsm_invalid_event_handler,/* MME_EVENT_SCTP_CONNECT_TIMER_EXPIRY */
        mme_fsm_invalid_event_handler, /* MME_EVENT_S1_SETUP_TIME_TO*/
        /*_WAIT_TIMER_EXPIRY */
        mme_fsm_shutdown_event_handler, /* MME_EVENT_SCTP_SHUTDOWN*/   
/*Bug 9050 fixes start*/
        mme_fsm_invalid_event_handler, /* MME_EVENT_CLOSE_MME_CONNECTION */
        mme_fsm_invalid_event_handler,  /*MME_EVENT_SCTP_ASSOC_UP*/
        mme_fsm_invalid_event_handler   /*MME_EVENT_SCTP_ASSOC_DOWN*/
/*Bug 9050 fixes end*/
    },
    /* State MME_W_CONNECT_SUCCESSFUL */
    {
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_MME_PROV_REQ*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP_TIMER_EXPI*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP  */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_FAILURE*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_FAILURE_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_RECOVERY_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_REC_TIMER_EXP*/
        mme_fsm_sctp_connect_timer_expiry,/*MME_EVENT_SCTP_CONNECT_TIMER_EXP*/
        mme_fsm_invalid_event_handler,   /* MME_EVENT_S1_SETUP_TIME_TO*/
        /*_WAIT_TIMER_EXPIRY */
        mme_fsm_invalid_event_handler, /* MME_EVENT_SCTP_SHUTDOWN*/   
/*Bug 9050 fixes start*/

        /*Bug 16213_Fix start*/       
        /* SPR 21093 fix start */ 
        mme_fsm_invalid_event_handler,       
        /* SPR 21093 fix end */         
/*Bug 16213_Fix stop*/
        mme_fsm_sctp_assoc_up_event_handler,  /*MME_EVENT_SCTP_ASSOC_UP*/
        mme_fsm_sctp_assoc_down_event_handler   /*MME_EVENT_SCTP_ASSOC_DOWN*/
/*Bug 9050 fixes end*/
    },
    /* State MME_W_TIME_TO_WAIT_TIMER_EXPIRY */
    {
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_MME_PROV_REQ*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP_TIMER_EXPIRY*/
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP  */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_FAILURE*/
        mme_fsm_w_s1_resp_conn_failure,  /* MME_EVENT_SCTP_CONN_FAILURE_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_RECOVERY_IND */
        mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_REC_TIMER_EXP*/
        mme_fsm_invalid_event_handler, /*MME_EVENT_SCTP_CONNECT_TIMER_EXPIRY */
        mme_fsm_back_off_timer_expiry,  /*MME_EVENT_S1_SETUP_TIME_TO*/
        /*_WAIT_TIMER_EXPIRY */
        mme_fsm_shutdown_event_handler, /* MME_EVENT_SCTP_SHUTDOWN*/   
/*Bug 9050 fixes start*/
        mme_fsm_invalid_event_handler, /* MME_EVENT_CLOSE_MME_CONNECTION */
        mme_fsm_invalid_event_handler,  /*MME_EVENT_SCTP_ASSOC_UP*/
        mme_fsm_invalid_event_handler   /*MME_EVENT_SCTP_ASSOC_DOWN*/
		/*Bug 9050 fixes end*/
		/*SPR 15570 Fix Start*/
    },
    /* State MME_W_SCTP_CONNECT */
    {
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_MME_PROV_REQ*/
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP_TIMER_EXPIRY*/
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_RSP  */
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_FAILURE*/
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_FAILURE_IND */
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_RECOVERY_IND */
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_SCTP_CONN_REC_TIMER_EXPIRY */
	    mme_fsm_w_sctp_connect_event_handler, /*MME_EVENT_SCTP_CONNECT_TIMER_EXPIRY */
	    mme_fsm_invalid_event_handler,  /* MME_EVENT_S1_SETUP_TIME_TO_WAIT_TIMER_EXPIRY */
	    mme_fsm_shutdown_event_handler, /* MME_EVENT_SCTP_SHUTDOWN*/   
	    mme_fsm_invalid_event_handler, /* MME_EVENT_CLOSE_MME_CONNECTION */
	    mme_fsm_invalid_event_handler,  /*MME_EVENT_SCTP_ASSOC_UP*/
	    mme_fsm_invalid_event_handler   /*MME_EVENT_SCTP_ASSOC_DOWN*/
    }
    /*SPR 15570 Fix Stop*/
};

static void handle_sctp_send_failure(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t     * p_mme_ctx );


static void handle_sctp_open_connection_failure(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_ctx);

/****************************************************************************
 * Function Name  : mme_fsm_invalid_event_handler
 * Inputs         : p_api               : pointer to p_api
 *                  p_msg               : Pointer to message buffer
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None 
 * Description    : This is for handling the invalid events for a state 
 ****************************************************************************/
void mme_fsm_invalid_event_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p,p_s1ap_gb_ctx:%p",
            p_api,p_msg,p_s1ap_gb_ctx);

    RRC_S1AP_TRACE(RRC_WARNING,"[%s]::Invalid Event, state=%s",
            __FUNCTION__,MME_FSM_STATES_NAMES[(MME_GET_FSM_STATE(p_mme_ctx))]);

    RRC_S1AP_UT_TRACE_EXIT();
}



/****************************************************************************
 * Function Name  : mme_fsm_mme_init_req
 * Inputs         : p_api                 : pointer to p_api
 *                  p_msg                 : Pointer to message buffer
 *                  p_s1ap_gb_ctx         : pointer to S1AP global context
 *                  p_mme_ctx             : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when S1AP_OAM_PROV_REQ message is 
 *                  received from OAM.This function is invoked per MME context
 *                  to be created.
 ****************************************************************************/
void  mme_fsm_mme_init_req(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    S32 sockd      = S1AP_SCTP_ERROR;
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    sctp_sd_search_node_t *p_node = PNULL;
    enb_comm_info_t * p_enb_comm_info = PNULL;
/*Bug 9050 fixes start*/
    LOCAL_SCTP_SETUP_ATTEMPT_STRUCT *p_local_sctp_setup_att = PNULL;
//#endif
    LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;
    /*SPR 15570 Fix Start*/
    s1ap_timer_buffer_t conn_establish_timer_buf;  
    p_mme_ctx->sctp_einprogress_initiated_from = MME_INVALID_STATE; 
    /*SPR 15570 Fix Stop*/
    /*Bug 9050 fixes end*/

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(p_s1ap_gb_ctx != PNULL);
    RRC_ASSERT(p_mme_ctx != PNULL);
    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_msg != PNULL);

//#ifndef RRC_UNIT_TEST_FRAMEWORK
    p_enb_comm_info = (enb_comm_info_t *)p_msg;
//#endif


    /* Calling Reset Function for corresponding MME, after receiving this 
       UECC initiate the RRC Connection Release procedure with all UEs 
       corresponds to this MME*/

    process_reset_all(p_s1ap_gb_ctx,
            p_mme_ctx);

    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_MME;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS, p_mme_ctx->
                connection_info.ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_LOCAL;
    }
            
    /* SPR 13432 Fix Start */
    if (p_s1ap_gb_ctx->s1_conn_mode_one_flag)
    {
    	p_s1ap_gb_ctx->last_selected_mme_id = p_mme_ctx->mme_id; 
    }
    /* SPR 13432 Fix Stop */
    
//#ifndef RRC_UNIT_TEST_FRAMEWORK
#ifndef LINUX_PC_TEST
    /* Create the Socket Descriptor.
     Details:  Socket creation , binding , connect */
    sockd = l3_open_sctp_association(
            p_s1ap_gb_ctx,
            p_mme_ctx,
            p_enb_comm_info);
#else
	sockd = s1ap_set_sock_fd_for_test(p_mme_ctx);	
	p_mme_ctx->is_conn_ongoing = RRC_FALSE;
#endif
/*SPR 15570 Fix Start*/
    if(RRC_TRUE == p_mme_ctx->is_conn_ongoing)
    {
	    /*Storing  a SD in  p_mme_ctx */
	    p_mme_ctx->sctp_sd = sockd;
	    p_mme_ctx->sctp_einprogress_retry_count = S1AP_ZERO;
	    p_mme_ctx->sctp_einprogress_initiated_from = MME_IDLE; 


	    /* Start sctp connection establish timer  */
	    conn_establish_timer_buf.timer_type = S1AP_SCTP_CONN_TIMER ;
	    conn_establish_timer_buf.auto_delete = MME_TRUE;
	    conn_establish_timer_buf.mme_id = p_mme_ctx->mme_id;

	    /*Starting the S1AP_SCTP_CONN_TIMER timer*/
	    p_mme_ctx->mme_timer_id =  rrc_start_timer(
			    SCTP_W_CONNECT_TIMER_DURATION, 
			    &conn_establish_timer_buf,
			    sizeof(conn_establish_timer_buf), 
			    RRC_FALSE);
	    MME_SET_FSM_STATE(p_mme_ctx, MME_W_SCTP_CONNECT);
	    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_SCTP_CONNECT]",
			    __FUNCTION__);

    }
    else
    {
	    /*SPR 15570 Fix Stop*/

    /*Opening a SCTP association Failed*/
    if (S1AP_SCTP_ERROR == sockd) 
    {
        handle_sctp_open_connection_failure(
                p_s1ap_gb_ctx,
                p_mme_ctx);
        if (PNULL != p_local_sctp_setup_result)
        {
            p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_FAILURE;
        }
    }
    else
    {
        ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_RECOVERY_ALARM_ID,
                NOTIFICATION,
                "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);
        /* SPR 13471 Fix Start */
        handle_sctp_open_connection_failure(
                p_s1ap_gb_ctx,
                p_mme_ctx);
        /* SPR 13471 Fix Stop */

        /*Generating Protocl Event LOCAL_SCTP_SETUP_ATTEMPT*/ 
        p_local_sctp_setup_att = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT));
        if (PNULL != p_local_sctp_setup_att)
        {
            memset_wrapper(p_local_sctp_setup_att, RRC_NULL, sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT));
            p_local_sctp_setup_att->header.length = sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT);
            p_local_sctp_setup_att->header.event_id = LOCAL_SCTP_SETUP_ATTEMPT;

            l3_memcpy_wrapper(p_local_sctp_setup_att->EVENT_PARAM_IP_ADDRESS ,p_mme_ctx->
                connection_info.ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_att);
        }
       
        if (PNULL != p_local_sctp_setup_result)
        {
            p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_SUCCESS;
        }

/*Bug 9050 fixes start*/ 


        /*Storing  a SD in  p_mme_ctx */
        p_mme_ctx->sctp_sd = sockd;
        p_node = (sctp_sd_search_node_t*)rrc_mem_get
                             (sizeof(sctp_sd_search_node_t));
        if(PNULL == p_node)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"[%s]: Cannot allocate Memory;Unused variable p_api=%p",
                    __FUNCTION__,p_api);/*SPR 17777 +-*/
            RRC_S1AP_UT_TRACE_EXIT();
            /* Change state to MME_IDLE */
            MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);
	     /* Coverity_95482 Fix Start */
        if (PNULL != p_local_sctp_setup_result)
        {
            rrc_mem_free(p_local_sctp_setup_result);
            p_local_sctp_setup_result = PNULL;
        }
           /* Coverity_95482 Fix End */

            return;
        }
        p_node->sctp_sd = sockd;
        p_node->mmeId = p_mme_ctx->mme_id;

        st_insert_node (
                &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree),
                &(p_node->nodeAnchor));

        RRC_S1AP_TRACE(RRC_INFO,"[%s]::SCTP SD %d inserted st addr [0x%p]\n",
                __FUNCTION__,sockd,
                &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree));

        /*Change the state to Wait For Connection to be successful*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
                __FUNCTION__);
/*Bug 9050 fixes end*/ 
	/*SPR 15570 Fix Start*/
    }
    /*SPR 15570 Fix Stop*/
    }
//#else
#if 0
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::MME SET ",
                __FUNCTION__);
   /*Storing  a SD in  p_mme_ctx */
	    p_mme_ctx->sctp_sd = sockd;
	    p_mme_ctx->sctp_einprogress_retry_count = S1AP_ZERO;
	    p_mme_ctx->sctp_einprogress_initiated_from = MME_IDLE; 


	    /* Start sctp connection establish timer  */
	    conn_establish_timer_buf.timer_type = S1AP_SCTP_CONN_TIMER ;
	    conn_establish_timer_buf.auto_delete = MME_TRUE;
	    conn_establish_timer_buf.mme_id = p_mme_ctx->mme_id;

	    /*Starting the S1AP_SCTP_CONN_TIMER timer*/
	    p_mme_ctx->mme_timer_id =  rrc_start_timer(
			    SCTP_W_CONNECT_TIMER_DURATION, 
			    &conn_establish_timer_buf,
			    sizeof(conn_establish_timer_buf), 
			    RRC_FALSE);
	    MME_SET_FSM_STATE(p_mme_ctx, MME_W_SCTP_CONNECT);
	    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_SCTP_CONNECT]",
			    __FUNCTION__);

        MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
                __FUNCTION__);
                mme_fsm_process_event (
                        MME_EVENT_SCTP_ASSOC_UP,
                        NULL,
                        NULL,
                        p_s1ap_gb_ctx,
                        p_s1ap_gb_ctx->mme_contexts[0]);
#endif				
//#endif
    if (PNULL != p_local_sctp_setup_result)
    {
        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
        /* SPR 20633: CID 88645 Start */
        p_local_sctp_setup_result = PNULL;
        /* SPR 20633: CID 88645 End */
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mme_fsm_s1_setup_rsp_timer_expiry
 * Inputs         : timer_id            : timer_id
 *                  p_timer_buf         : pointer to p_s1ap_timer_buf
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when s1 setup request timer is 
 *                  expired and the s1 setup response is not received.
 ****************************************************************************/
void  mme_fsm_s1_setup_rsp_timer_expiry (
        void                * timer_id,           
        void                * p_timer_buf,       
        s1ap_gb_context_t   * p_s1ap_gb_ctx,
        mme_context_t       * p_mme_ctx)
{
    rrc_return_et response          = RRC_FAILURE;
    S32 sctp_send_response          = S1AP_SCTP_ERROR;
    rrc_return_et asn_response      = RRC_FAILURE;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN]  = {0};
    U16 asn_msg_len =0;
    s1ap_timer_buffer_t s1_setup_rsp_timer_buf;  
    LOCAL_S1_SETUP_RESULT_STRUCT    *p_s1_setup_result = PNULL;
    /* SPR 13432 Fix Start */
    mme_context_t        * p_mme_ctx_new = PNULL;
    /* SPR 13432 Fix Stop */

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm timer_id:%p p_timer_buf:%p",
            timer_id,p_timer_buf);

    /* Timer expired and is not valid any more */
    p_mme_ctx->mme_timer_id = PNULL;

    /* If the S1 Setup Req Retries are exhausted */
    if (p_mme_ctx->s1_config_retry_count >= 
            p_s1ap_gb_ctx->max_retry_for_s1_config)
    {
        /*Change the state to MME_IDLE*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_IDLE]",__FUNCTION__);

        /* Generating the Protocal Event LOCAL_S1_SETUP_RESULT*/
        p_s1_setup_result = rrc_mem_get(sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
        if (PNULL != p_s1_setup_result)
        {
            memset_wrapper(p_s1_setup_result, RRC_NULL, sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
            p_s1_setup_result->header.length = sizeof(LOCAL_S1_SETUP_RESULT_STRUCT);
            p_s1_setup_result->header.event_id = LOCAL_S1_SETUP_RESULT;

            p_s1_setup_result->EVENT_PARAM_S1_SETUP_RESULT = S1AP_TIMEOUT;
            l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_IP_ADDRESS ,p_mme_ctx->connection_info.
                ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
            
            l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_NAME, p_mme_ctx->mme_name,
                    (sizeof(U8) * MAX_MME_NAME_STR));
                    
            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_s1_setup_result);
        }

        
        /*Close the SCTP Association*/
        response = l3_sctp_close_association (p_mme_ctx->sctp_sd);

        if (RRC_FAILURE == response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket!!\n",
                    __FUNCTION__);
        }
        
        ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID,
                CRITICAL_ALARM,
                "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);

        delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
        p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;
/* SPR 14887 Fix Start*/
	/*Reset the P_mme_ctx- s1_config_retry_count.*/
    	p_mme_ctx->s1_config_retry_count = S1AP_ZERO;
    	RRC_S1AP_TRACE(RRC_INFO,"[%s]: S1 SETUP retry count set to zero",__FUNCTION__);
/* SPR 14887 Fix Stop*/

        /* SPR 13432 Fix Start */
        s1ap_build_and_send_oam_s1ap_link_status_ind(p_s1ap_gb_ctx,
                p_mme_ctx,
                OAM_S1AP_LINK_DOWN);

        if (p_s1ap_gb_ctx->s1_conn_mode_one_flag)
        {
	        p_s1ap_gb_ctx->last_selected_mme_id++;

            p_s1ap_gb_ctx->last_selected_mme_id = ((p_s1ap_gb_ctx->last_selected_mme_id) % (p_s1ap_gb_ctx->
                    num_of_mme_prov));

            p_mme_ctx_new = p_s1ap_gb_ctx->mme_contexts[p_s1ap_gb_ctx->last_selected_mme_id];

            mme_fsm_process_event(
                MME_EVENT_S1_MME_PROV_REQ,
                &(p_mme_ctx_new->connection_info),
                &(p_s1ap_gb_ctx->enb_comm_info),
                p_s1ap_gb_ctx,
                p_mme_ctx_new);
	    }
	/* SPR 13432 Fix Stop */
    } 
    else
    {
	/*send S1 SETUP REQUEST to MME ; No Need to change the state*/
	asn_response =  compose_asn_s1_setup_req (
		&(p_s1ap_gb_ctx->s1_setup_info),
		asn_msg_buff,
		&asn_msg_len);

        if (RRC_FAILURE == asn_response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ASN Encoding of S1 Setup Failed"
                    ,__FUNCTION__);
            /* Change state to MME_IDLE */
            MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);
            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }

        /*Sending the  S1 SETUP REQUEST to MME */
        sctp_send_response = l3_sctp_send(
                p_mme_ctx->sctp_sd,
                asn_msg_buff,
                asn_msg_len,
                SCTP_COMMON_PROC_STREAMS );

        if ( SCTP_SOCKET_ERROR == sctp_send_response)
        {
            handle_sctp_send_failure(p_s1ap_gb_ctx, 
                    p_mme_ctx);
        }                 
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,"S1AP_MSG: S1 SETUP REQUEST %s", change_data_to_str(asn_msg_len, asn_msg_buff));
            /* Change state and wait for S1 Setup Response */
            MME_SET_FSM_STATE(p_mme_ctx, MME_W_S1_SETUP_RESP);
            RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_S1_SETUP_RESP]",
                      __FUNCTION__);
            
            /* Start guard timer for S1_Setup_Rsp*/
            s1_setup_rsp_timer_buf.timer_type   = S1AP_MME_S1_SETUP_RSP_TIMER ;
            s1_setup_rsp_timer_buf.auto_delete  = MME_TRUE;
            s1_setup_rsp_timer_buf.mme_id       = p_mme_ctx->mme_id;

            p_mme_ctx->mme_timer_id =  rrc_start_timer(
                    p_s1ap_gb_ctx->s1_config_resp_duration,
                    &s1_setup_rsp_timer_buf,
                    sizeof(s1_setup_rsp_timer_buf), 
                    RRC_FALSE);

            RRC_S1AP_TRACE(RRC_INFO,"[%s]::Start timer "
                     "[S1AP_MME_S1_SETUP_RSP_TIMER]",
                    __FUNCTION__);
            /*Increment the P_mme_ctx-> s1_config_retry_count.*/
            p_mme_ctx->s1_config_retry_count++;

            RRC_S1AP_TRACE(RRC_INFO,"[%s]: S1 Setup Retry counter incremented",
                    __FUNCTION__);
        }
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : build_and_send_mme_status_msg_ind
 * Inputs         : p_mme_context         : pointer to MME context
 *                : mme_connection_status : rrc_bool_et type
 *                  p_s1ap_gb_ctx         : pointer to S1AP global context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to build and send mme status msg 
 *                  indication to UECC.
 ****************************************************************************/
void build_and_send_mme_status_msg_ind(
        mme_context_t           *p_mme_context,
        rrc_bool_et      mme_connection_status,
        //Trace Change Start
        s1ap_gb_context_t     * p_s1ap_gb_ctx)
        //Trace Change End
{
    void *p_intrl_msg           = PNULL;
    s1ap_status_msg_ind_t  s1ap_status_ind;
    
    RRC_S1AP_UT_TRACE_ENTER();    
    RRC_ASSERT(PNULL!= p_mme_context);

    memset_wrapper (&s1ap_status_ind,
                RRC_NULL,
                sizeof(s1ap_status_msg_ind_t));

    /*S1AP sends S1AP_STATUS_MSG_IND API to UECC with 
     * status = 1 on successfully 
      establishing first association with MME.*/
        s1ap_status_ind.bitmask = 0;
        s1ap_status_ind.mme_id = p_mme_context->mme_id;
        s1ap_status_ind.status = mme_connection_status;
      
        if(RRC_TRUE == mme_connection_status)
        {
            s1ap_status_ind.bitmask |= SERVED_GUMMEI_INFO_PRESENT;
            s1ap_fill_gummei_info(p_mme_context, &(s1ap_status_ind.gummei));
            
            //Trace Change Start
           //Set the ENB -ID and ENB Name to S1AP Status Ind message
            l3_memcpy_wrapper( &s1ap_status_ind.enb_id,
                &p_s1ap_gb_ctx->s1_setup_info.gb_enb_id.enb_id,
                sizeof(s1ap_status_ind.enb_id));
            
            l3_memcpy_wrapper( &s1ap_status_ind.enbName,
                &p_s1ap_gb_ctx->s1_setup_info.enb_name,
                sizeof(p_s1ap_gb_ctx->s1_setup_info.enb_name));
            
           //Send the MME Info to UECC context via s1ap_status_ind message
           l3_memcpy_wrapper( &s1ap_status_ind.connection_info,
                &p_mme_context->connection_info,
                sizeof(p_mme_context->connection_info));

           //Trace Change End
        }
        p_intrl_msg = s1ap_alloc_intrl_msg(
                RRC_UECC_MODULE_ID,
                S1AP_STATUS_MSG_IND,  
                sizeof(s1ap_status_ind));

        if (PNULL == p_intrl_msg)
        {
            /*out of memory*/
            RRC_S1AP_TRACE(RRC_INFO,"Memory Alloc failure[%s]", __FUNCTION__); 
            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }
        l3_memcpy_wrapper(
                (void*)((U8*) p_intrl_msg + RRC_API_HEADER_SIZE),
                (const void*) &s1ap_status_ind,
                sizeof(s1ap_status_ind));

        /* Send message */
        rrc_send_message(
                p_intrl_msg,
                RRC_UECCMD_MODULE_ID);
        RRC_S1AP_TRACE(RRC_INFO,
                "SENT : S1AP_UECC_STATUS_IND with mme status = %d",
                mme_connection_status);

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : active_mme_list_insert
 * Inputs         : p_s1ap_gb_ctx : pointer to S1AP global context
 *                : mme idex      : to be inserted into active mme array.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to insert mme_id in 
 *                  active_mme_info present in s1ap global Context.
 ****************************************************************************/
void active_mme_list_insert(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        U8     mme_index)
{
    active_mme_info_t * p_active_mme_info = PNULL;
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    p_active_mme_info = &p_s1ap_gb_ctx->active_mme_info;
    if (p_active_mme_info->active_mme_count  < MAX_NUM_MME)
    {
        p_active_mme_info->mme_id_arr[p_active_mme_info->active_mme_count].mme_id
            = mme_index;
        p_active_mme_info->mme_id_arr[p_active_mme_info->active_mme_count].overload_response
            = 
            OVERLOAD_START_INVALID_VALUE;
        p_active_mme_info->active_mme_count++;
        RRC_S1AP_TRACE(RRC_INFO,"Record for Active MME updated for MMEId[%u] at Idx[%u]",
                mme_index, p_active_mme_info->active_mme_count);
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]::Active MME Already "
                "reached to MAX_NUM_MME"
                " Hence cannot insert another MME",__FUNCTION__);
        return ;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : active_mme_list_delete
 * Inputs         : p_s1ap_gb_ctx : pointer to S1AP global context
 *                  mme_index     : mme idex to be inserted into active 
 *                                  mme array.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to delete mme_id in 
 *                  active_mme_info present in s1ap global Context.
 ****************************************************************************/
void active_mme_list_delete(
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        U16         mme_index)
{
    active_mme_info_t * p_active_mme_info = PNULL;

    U16 temp_mme_arr_idx = RRC_NULL;
    U16 flag = S1AP_FALSE;


    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    p_active_mme_info = &p_s1ap_gb_ctx->active_mme_info;
    if ( p_active_mme_info->active_mme_count < S1AP_ONE)
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]:MME Index :%d "
                "to be deleted is NOT Found in"
                "mme_id_arr,Hence cannot Delete MME",__FUNCTION__,mme_index);
        return;
    }
    for (temp_mme_arr_idx = 0;
            temp_mme_arr_idx < p_active_mme_info->active_mme_count;
            temp_mme_arr_idx++)
    {
        if (p_active_mme_info->mme_id_arr[temp_mme_arr_idx].mme_id == mme_index)
        {
            p_active_mme_info->mme_id_arr[temp_mme_arr_idx].mme_id = 
                p_active_mme_info->
                mme_id_arr[p_active_mme_info->active_mme_count-1].mme_id;
            p_active_mme_info->mme_id_arr[p_active_mme_info->active_mme_count-1].mme_id
                = MAX_NUM_MME;
            if ((p_active_mme_info->mme_idx_to_returned) == 
                    (p_active_mme_info->active_mme_count - 1 ))
            {
                p_active_mme_info->mme_idx_to_returned = 0;
            }
            p_active_mme_info->active_mme_count--;
			p_s1ap_gb_ctx->num_of_mme_prov--; //add by wood for fix bug :114110		
            flag = S1AP_TRUE ;
            break;
        }
    }

    if ( S1AP_FALSE == flag )
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]:MME Index :%d "
                "to be deleted is NOT Found in"
                "mme_id_arr, Hence cannot Delete MME",__FUNCTION__,mme_index);
    }
    RRC_S1AP_UT_TRACE_EXIT();
}


/****************************************************************************
 * Function Name  : mme_fsm_s1_setup_resp
 * Inputs         : p_api                  : pointer to p_api
 *                  p_msg                  : Pointer to message buffer
 *                  p_s1ap_gb_ctx          : pointer to S1AP global context
 *                  p_mme_ctx              : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon reception of S1 SETUP 
                    RESPONSE from an MME
 ****************************************************************************/
void mme_fsm_s1_setup_resp (
        void                * p_api,             
        void                * p_msg,
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx)
{
    parsed_s1_setup_rsp_t parsed_s1_setup_response;
    LOCAL_S1_SETUP_RESULT_STRUCT    *p_s1_setup_result = PNULL;
    s1ap_rrm_mme_info_t   s1ap_rrm_mme_info; 
    s1ap_oam_mme_info_t   s1ap_oam_mme_info;
    rrc_return_et result = RRC_FAILURE;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_msg);

    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper((void*)&parsed_s1_setup_response,0,
                          sizeof(parsed_s1_setup_rsp_t));

    memset_wrapper((void*)&s1ap_rrm_mme_info, RRC_NULL,
                          sizeof(s1ap_rrm_mme_info_t));
    memset_wrapper((void*)&s1ap_oam_mme_info, RRC_NULL,
                          sizeof(s1ap_oam_mme_info_t));
    

    if ( RRC_FAILURE == parse_asn_s1_setup_response(
                    p_s1ap_gb_ctx,
                    p_mme_ctx,
                    p_api,
                *((U16*)p_msg),&parsed_s1_setup_response))
    {
        RRC_S1AP_TRACE(RRC_ERROR, 
                "[%s]parse_asn_s1_setup_response returnd Failure\n",
                __FUNCTION__);
        /* Local error handling must be started. */
        if (parsed_s1_setup_response.gummei_info.p_served_gummeis)
        {
            free_served_gummeis( parsed_s1_setup_response.gummei_info.p_served_gummeis,
                         parsed_s1_setup_response.gummei_info.num_served_gummeis);
            rrc_mem_free(parsed_s1_setup_response.gummei_info.p_served_gummeis);
        }
        
        /* Generating the Protocal Event LOCAL_S1_SETUP_RESULT*/
        p_s1_setup_result = rrc_mem_get(sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
        if (PNULL != p_s1_setup_result)
        {
            memset_wrapper(p_s1_setup_result, RRC_NULL, sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
            p_s1_setup_result->header.length = sizeof(LOCAL_S1_SETUP_RESULT_STRUCT);
            p_s1_setup_result->header.event_id = LOCAL_S1_SETUP_RESULT;

            l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_IP_ADDRESS, p_mme_ctx->connection_info.
                    ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

            p_s1_setup_result->EVENT_PARAM_S1_SETUP_RESULT = S1AP_PROTOCOL_ERROR;
            if (parsed_s1_setup_response.bitmask & S1AP_S1_SETUP_RESP_MME_NAME_PRESENT)
            {
                l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_NAME, parsed_s1_setup_response.
                        p_mme_name, (sizeof(U8) * MAX_MME_NAME_STR));
            }
            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_s1_setup_result);
        }
        return;
    }

    l3_memcpy_wrapper((void*) p_mme_ctx->mme_name,
                (const void*) parsed_s1_setup_response.p_mme_name , 
                MAX_MME_NAME_STR);

    /* If Served Gummeis info is already present then free it
     * and update it with latest one */
    if (p_mme_ctx->p_served_gummeis)
    {
        /* Local error handling must be started. */
        free_served_gummeis( p_mme_ctx->p_served_gummeis,
                   p_mme_ctx->num_served_gummeis); 
        rrc_mem_free(p_mme_ctx->p_served_gummeis);
        p_mme_ctx->p_served_gummeis = PNULL;
    }

    p_mme_ctx->num_served_gummeis = parsed_s1_setup_response.gummei_info.num_served_gummeis;
    p_mme_ctx->p_served_gummeis = parsed_s1_setup_response.gummei_info.p_served_gummeis;
    
    /* storing relative mme capacity */
    p_mme_ctx->relative_mme_capacity = parsed_s1_setup_response.relative_mme_capacity;

    /*Adding the MME into the active mme in the global context*/

    /*Stop s1 setup guard timer*/
    if (PNULL != p_mme_ctx->mme_timer_id)
    {
        RRC_S1AP_TRACE(RRC_BRIEF,"[%s]::Stop timer "
                    "[S1AP_MME_S1_SETUP_RSP_TIMER]",
                __FUNCTION__);
        rrc_stop_timer(p_mme_ctx->mme_timer_id);
        p_mme_ctx->mme_timer_id = PNULL;
    }
    /*Reset the P_mme_ctx-> s1_config_retry_count.*/
    p_mme_ctx->s1_config_retry_count = S1AP_ZERO;

    /*Change the state to MME_ACTIVE*/
    MME_SET_FSM_STATE(p_mme_ctx, MME_ACTIVE);
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::S1 SETUP retry count set to zero Set State [MME_ACTIVE]",__FUNCTION__);

/* Send the LINK UP Indication to the OAM */
    s1ap_build_and_send_oam_s1ap_link_status_ind(p_s1ap_gb_ctx,
                                                 p_mme_ctx,
                                                 OAM_S1AP_LINK_UP);
            
    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_CONNECTION_SUCCESS_ALARM_ID, NOTIFICATION,
            "MME IP Address = %s\n",
            p_mme_ctx->connection_info.ip_addr[0].ip_addr);

    /* Generating the Protocal Event LOCAL_S1_SETUP_RESULT*/
    p_s1_setup_result = rrc_mem_get(sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
    if (PNULL != p_s1_setup_result)
    {
        memset_wrapper(p_s1_setup_result, RRC_NULL, sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
        p_s1_setup_result->header.length = sizeof(LOCAL_S1_SETUP_RESULT_STRUCT);
        p_s1_setup_result->header.event_id = LOCAL_S1_SETUP_RESULT;

        l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_IP_ADDRESS, p_mme_ctx->connection_info.
                ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

        s1ap_fill_gummei_info(p_mme_ctx,
                &p_s1_setup_result->EVENT_PARAM_GUMMEI);
        p_s1_setup_result->EVENT_PARAM_S1_SETUP_RESULT = S1AP_SUCCESS;
        if (parsed_s1_setup_response.bitmask & S1AP_S1_SETUP_RESP_MME_NAME_PRESENT)
        {
            l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_NAME, parsed_s1_setup_response.
                    p_mme_name, (sizeof(U8) * MAX_MME_NAME_STR));
        }
        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_s1_setup_result);
    }

    s1ap_rrm_mme_info.mme_id = p_mme_ctx->mme_id;
    
    l3_memcpy_wrapper(s1ap_rrm_mme_info.ip_addr, p_mme_ctx->connection_info.ip_addr[0].
            ip_addr, MAX_IP_ADDRESS_LENGTH);
    
    s1ap_rrm_mme_info.port = p_mme_ctx->connection_info.port;
    
    if (parsed_s1_setup_response.bitmask & S1AP_S1_SETUP_RESP_MME_NAME_PRESENT)
    {        
        s1ap_rrm_mme_info.mme_info.bitmask |= S1AP_MME_NAME_PRESENT;
        l3_memcpy_wrapper(s1ap_rrm_mme_info.mme_info.mme_name, parsed_s1_setup_response.
                p_mme_name, MAX_MME_NAME);
    }
    
    s1ap_fill_served_gummei_info(p_mme_ctx,&s1ap_rrm_mme_info.mme_info.served_gummei_info);
    s1ap_rrm_mme_info.mme_info.bitmask |= S1AP_SERVED_GUMMEI_INFO_PRESENT;

    s1ap_rrm_mme_info.mme_info.relative_mme_capacity = parsed_s1_setup_response.
        relative_mme_capacity;
    s1ap_rrm_mme_info.mme_info.bitmask |= S1AP_RELATIVE_MME_CAPACITY_PRESENT;

    /* PUP encode and send message to RRM */
    result = rrc_s1apRrm_il_send_s1ap_rrm_mme_info(&s1ap_rrm_mme_info,
            RRC_S1AP_MODULE_ID,
            RRC_RRM_MODULE_ID,
            RRC_TRANSACTION_ID_ABSENT,
            RRC_NULL);

      if (RRC_SUCCESS == result)
      {
        RRC_S1AP_TRACE(RRC_DETAILED,
          "s1ap_rrm_mme_info Message sent to RRM");
      }
      else
      {
        RRC_S1AP_TRACE(RRC_INFO,
          "Unable to send s1ap_rrm_mme_info message to RRM");
      }
    
      l3_memcpy_wrapper(&s1ap_oam_mme_info, &s1ap_rrm_mme_info, sizeof(s1ap_rrm_mme_info_t));
      /* PUP encode and send message to OAM */
      result = rrc_s1apOam_il_send_s1ap_oam_mme_info(&s1ap_oam_mme_info,
              RRC_S1AP_MODULE_ID,
              RRC_OAM_MODULE_ID,
              RRC_TRANSACTION_ID_ABSENT,
              RRC_NULL);

      if (RRC_SUCCESS == result)
      {
          RRC_S1AP_TRACE(RRC_DETAILED,
                  "s1ap_oam_mme_info Message sent to OAM");
      }
      else
      {
          RRC_S1AP_TRACE(RRC_INFO,
                  "Unable to send s1ap_oam_mme_info message to OAM");
      }


    /*Adding the MME into the active mme in the global context*/
    active_mme_list_insert(
            p_s1ap_gb_ctx, 
            p_mme_ctx->mme_id);

    /* Send MME Info to UECC */
    build_and_send_mme_status_msg_ind(p_mme_ctx, S1AP_TRUE,p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : free_served_gummeis
 * Inputs         : p_served_gummeis   : Served Gummei Info per RAT.
 *                  num_served_gummeis : Number of entries in 
 *                                       p_served_gummeis
 * Outputs        : None
 * Returns        : None
 * Description    : This function frees memory related to served gummeis info
 ****************************************************************************/
void free_served_gummeis( served_gummei_info_t * p_served_gummeis, U8
                                   num_served_gummeis)
{
    list_t * p_mme_grp_id_list = PNULL;
    list_node_t * p_node = PNULL;
    U8 gummei_count = 0;
    U32 num_mme_group_id= 0;
    U32 group_id_count= 0;

    /*Freeing list elements of mme_group_id_list */
    for (gummei_count =0 ; 
            gummei_count < num_served_gummeis;
            gummei_count++)
    {
        p_mme_grp_id_list = &p_served_gummeis[gummei_count].mme_group_id_list;
        num_mme_group_id = list_count(p_mme_grp_id_list);

        for (group_id_count =0 ; 
                group_id_count < num_mme_group_id;
                group_id_count++)
        {
            p_node = list_pop_head(p_mme_grp_id_list);

            if (p_node)
            {
                rrc_mem_free(p_node);
                p_node = PNULL;
            }
        }
    }
}

/****************************************************************************
 * Function Name  : mme_fsm_s1_setup_failure
 * Inputs         : p_api                  : pointer to p_api
 *                  p_msg                  : Pointer to message buffer
 *                  p_s1ap_gb_ctx          : pointer to S1AP global context
 *                  p_mme_ctx              : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon reception of S1 SETUP 
 *                  FAILURE from an MME.
 ****************************************************************************/
void mme_fsm_s1_setup_failure (
        void                * p_api,             
        void                * p_msg,
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx)
{
    rrc_return_et response          = RRC_FAILURE;
/* SPR 10858 Fix Start */
    s1ap_timer_buffer_t time_to_wait_timer_buf;        
    U32 time_to_wait_timer_duration = RRC_NULL;
    LOCAL_S1_SETUP_RESULT_STRUCT    *p_s1_setup_result = PNULL;
    s1ap_Cause  cause;
    /* SPR 13432 Fix Start */
    mme_context_t        * p_mme_ctx_new = PNULL;
    /* SPR 13432 Fix Stop */
    
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_msg);


    /* Coverity Fix 83182 Start */
    memset_wrapper(&cause, RRC_NULL, sizeof(s1ap_Cause));
    /* Coverity Fix 83182 Stop */

    RRC_S1AP_TRACE(RRC_INFO,"S1AP_MSG: S1 SETUP FAILURE");
    
    /*Stop s1 setup guard timer*/
    if (PNULL != p_mme_ctx->mme_timer_id)
    {
        rrc_stop_timer(p_mme_ctx->mme_timer_id);
        p_mme_ctx->mme_timer_id = PNULL;
    }

    if (RRC_FAILURE == parse_asn_s1_setup_failure(
                    p_s1ap_gb_ctx,
                    p_mme_ctx,
                    p_api,
                    *((U16*)p_msg),
                    &(time_to_wait_timer_duration),
                    &cause))
    {
       RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ASN Parsing is "
                    "not Done correctly",
                    __FUNCTION__);
    }
    if ( (p_mme_ctx->s1_config_retry_count >= 
            p_s1ap_gb_ctx->max_retry_for_s1_config) ||
         (!(time_to_wait_timer_duration)) )
    {
        /*Change the state to MME_IDLE*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_IDLE]",__FUNCTION__);
        /* Generating the Protocal Event LOCAL_S1_SETUP_RESULT*/
        p_s1_setup_result = rrc_mem_get(sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
        if (PNULL != p_s1_setup_result)
        {
            memset_wrapper(p_s1_setup_result, RRC_NULL, sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
            p_s1_setup_result->header.length = sizeof(LOCAL_S1_SETUP_RESULT_STRUCT);
            p_s1_setup_result->header.event_id = LOCAL_S1_SETUP_RESULT;

            p_s1_setup_result->EVENT_PARAM_S1_SETUP_RESULT = S1AP_FAILURE;

            l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_IP_ADDRESS, p_mme_ctx->connection_info.
                    ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
            l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_NAME, p_mme_ctx->mme_name,
                    (sizeof(U8) * MAX_MME_NAME_STR));

            if (RRC_SUCCESS == parse_asn_s1_setup_failure(
                        p_s1ap_gb_ctx,
                        p_mme_ctx,
                        p_api,
                        *((U16*)p_msg),
                        &(time_to_wait_timer_duration),
                        &cause))
            {
                p_s1_setup_result->EVENT_PARAM_3GPP_CAUSE_GROUP = (U32)(cause.t);

                switch(p_s1_setup_result->EVENT_PARAM_3GPP_CAUSE_GROUP)
                { 
                    case T_s1ap_Cause_radioNetwork: 
                        p_s1_setup_result->EVENT_PARAM_3GPP_CAUSE = cause.u.radioNetwork;
                        break;

                    case T_s1ap_Cause_transport: 
                        p_s1_setup_result->EVENT_PARAM_3GPP_CAUSE = cause.u.transport;
                        break;

                    case T_s1ap_Cause_nas: 
                        p_s1_setup_result->EVENT_PARAM_3GPP_CAUSE = cause.u.nas;
                        break; 

                    case T_s1ap_Cause_protocol:
                        p_s1_setup_result->EVENT_PARAM_3GPP_CAUSE = cause.u.protocol;
                        break; 

                    case T_s1ap_Cause_misc:
                        p_s1_setup_result->EVENT_PARAM_3GPP_CAUSE = cause.u.misc;
                        break; 
                }
            }

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_s1_setup_result);
        }

        /*Close the SCTP Association*/

        response = l3_sctp_close_association (p_mme_ctx->sctp_sd);

        if (RRC_FAILURE == response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket !!\n",
                    __FUNCTION__ );
        }
        
        ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID, 
                CRITICAL_ALARM, "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);
        
        delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
        p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;
        /* SPR 14887 Fix Start*/	
        /*Reset the P_mme_ctx- s1_config_retry_count.*/
        p_mme_ctx->s1_config_retry_count = S1AP_ZERO;
        RRC_S1AP_TRACE(RRC_INFO,"[%s]: S1 SETUP retry count set to zero",__FUNCTION__);
        /* SPR 14887 Fix Stop*/	
        
        /* SPR 13432 Fix Start */
        s1ap_build_and_send_oam_s1ap_link_status_ind(p_s1ap_gb_ctx,
                p_mme_ctx,
                OAM_S1AP_LINK_DOWN);

        if (p_s1ap_gb_ctx->s1_conn_mode_one_flag)
        {
     	    p_s1ap_gb_ctx->last_selected_mme_id++;

            p_s1ap_gb_ctx->last_selected_mme_id = ((p_s1ap_gb_ctx->last_selected_mme_id) % (p_s1ap_gb_ctx->
                    num_of_mme_prov));

            p_mme_ctx_new = p_s1ap_gb_ctx->mme_contexts[p_s1ap_gb_ctx->last_selected_mme_id];

            mme_fsm_process_event(
                MME_EVENT_S1_MME_PROV_REQ,
                &(p_mme_ctx_new->connection_info),
                &(p_s1ap_gb_ctx->enb_comm_info),
                p_s1ap_gb_ctx,
                p_mme_ctx_new);
        }
        /* SPR 13432 Fix Stop */
    }
    else
    {
        /*Change the state to MME_W_TIME_TO_WAIT_TIMER_EXPIRY*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_W_TIME_TO_WAIT_TIMER_EXPIRY );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State "
                    "[MME_W_TIME_TO_WAIT_TIMER_EXPIRY]",
                    __FUNCTION__);

        /*Start timer with timeout value equal to time to wait*/
        time_to_wait_timer_buf.timer_type = S1AP_MME_TIME_TO_WAIT_TIMER ;
        time_to_wait_timer_buf.auto_delete = MME_TRUE;
        time_to_wait_timer_buf.mme_id = p_mme_ctx->mme_id;

        /* The timer value after parsing is in milliseconds so no need 
           of multiplying it with 1000
        */
        p_mme_ctx->mme_timer_id =  rrc_start_timer(
                    time_to_wait_timer_duration ,
                     /* Passing time in milli seconds*/
                    &time_to_wait_timer_buf,
                    sizeof(time_to_wait_timer_buf), 
                    RRC_FALSE);
    }
    /* SPR 10858 Fix Stop */
    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mme_fsm_back_off_timer_expiry
 * Inputs         : timer_id            : timer_id 
 *                  p_timer_buf         : pointer to p_s1ap_timer_buf
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon expiry of back off timer which
 *                  was equal to the time_to_wait IE received in the S1 Setup 
 *                  Failure
 ****************************************************************************/
void  mme_fsm_back_off_timer_expiry(
        rrc_timer_t         timer_id,
        void                *p_timer_buf,       
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx)
{
    S32 sctp_send_response          = S1AP_SCTP_ERROR;
    rrc_return_et asn_response      = RRC_FAILURE;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_msg_len = 0;
    s1ap_timer_buffer_t s1_setup_rsp_timer_buf;  

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_timer_buf);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm timer_id:%p p_timer_buf:%p",
            timer_id,p_timer_buf);
    /* Timer expired and is not valid any more */
    p_mme_ctx->mme_timer_id = PNULL;


    /*1.Send S1 SETUP REQUEST to MME*/
    /* ASN : Compose s1_setup_req IE  */
    asn_response =  compose_asn_s1_setup_req (
            &(p_s1ap_gb_ctx->s1_setup_info),
            asn_msg_buff,
            &asn_msg_len);

    if ( RRC_FAILURE == asn_response)
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ASN Encoding of S1 Setup Failed",
                __FUNCTION__);
        /* Change state to MME_IDLE */
        MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);
        return;
    }

    sctp_send_response = l3_sctp_send(
            p_mme_ctx->sctp_sd,
            asn_msg_buff,
            asn_msg_len,
            SCTP_COMMON_PROC_STREAMS );

    if ( SCTP_SOCKET_ERROR == sctp_send_response)
    {
        handle_sctp_send_failure(p_s1ap_gb_ctx, 
                p_mme_ctx);
    }                 
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"S1AP_MSG: S1 SETUP REQUEST %s", change_data_to_str(asn_msg_len, asn_msg_buff));
        /* Change state and wait for S1 Setup Response */
        MME_SET_FSM_STATE(p_mme_ctx, MME_W_S1_SETUP_RESP);
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_S1_SETUP_RESP]",
                __FUNCTION__);

        /* Start guard timer for S1_Setup_Rsp*/
        s1_setup_rsp_timer_buf.timer_type = S1AP_MME_S1_SETUP_RSP_TIMER ;
        s1_setup_rsp_timer_buf.auto_delete = MME_TRUE;
        s1_setup_rsp_timer_buf.mme_id = p_mme_ctx->mme_id;

        p_mme_ctx->mme_timer_id =  rrc_start_timer(
                p_s1ap_gb_ctx->s1_config_resp_duration , 
                &s1_setup_rsp_timer_buf,
                sizeof(s1_setup_rsp_timer_buf), 
                RRC_FALSE);

        /*Increment the P_mme_ctx-> s1_config_retry_count.*/
        p_mme_ctx->s1_config_retry_count++;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*SPR 15570 Fix Start*/
/****************************************************************************
 * Function Name  : mme_fsm_w_sctp_connect_event_handler
 * Inputs         : timer_id            : timer_id 
 *                  p_timer_buf         : pointer to p_s1ap_timer_buf
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon expiry of timer which
 *                  was started on receiving error code EINPROGRESS 
  ****************************************************************************/

void  mme_fsm_w_sctp_connect_event_handler(
        rrc_timer_t         timer_id,
        void                *p_timer_buf,       
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx)
{
	S32 sctp_connect_response      = S1AP_SCTP_ERROR;
	rrc_return_et response          = RRC_FAILURE;
	s1ap_timer_buffer_t conn_establish_timer_buf;  
	sctp_sd_search_node_t *p_node = PNULL;
	mme_context_t        * p_mme_ctx_new = PNULL;
	LOCAL_SCTP_SETUP_ATTEMPT_STRUCT *p_local_sctp_setup_att = PNULL;
	LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;

	RRC_S1AP_UT_TRACE_ENTER();
	RRC_ASSERT(PNULL != p_timer_buf);
	RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
	RRC_ASSERT(PNULL != p_mme_ctx);


	RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm timer_id:%p p_timer_buf:%p",
			timer_id,p_timer_buf);
	/* Timer expired and is not valid any more */
	RRC_S1AP_TRACE(RRC_WARNING,"[%s]:: SCTP TIMER EXPIRED : retrying to connect",
			__FUNCTION__);
	p_mme_ctx->mme_timer_id = PNULL;


	sctp_connect_response = l3_retry_connect(
			p_s1ap_gb_ctx,
			p_mme_ctx);
	p_mme_ctx->sctp_einprogress_retry_count++ ;

    /* coverity_fix 86808 ++ */
    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_MME;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS ,p_mme_ctx->
                connection_info.ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_LOCAL;
    }
    /* coverity_fix 86808 -- */

    if (INVALID_SCTP_SOCKET == sctp_connect_response)
    {
        if(p_mme_ctx->sctp_einprogress_retry_count == SCTP_SELECT_RETRY_COUNT)
        {
			RRC_S1AP_TRACE(RRC_WARNING,"[%s]::SCTP Association is not UP , max retries[%u] for EINPROGRESS done",
					__FUNCTION__, p_mme_ctx->sctp_einprogress_retry_count);
			p_mme_ctx->sctp_einprogress_retry_count = S1AP_ZERO ;
			p_mme_ctx->is_conn_ongoing =  S1AP_FALSE;
			if ((MME_W_CONNECT_SUCCESSFUL == p_mme_ctx->sctp_einprogress_initiated_from) && (S1AP_TRUE == p_s1ap_gb_ctx->s1_conn_mode_one_flag))
			{
				/*Close the SCTP Association*/
				response = l3_sctp_close_association (p_mme_ctx->sctp_sd);

				if (RRC_FAILURE == response)
				{ 
					RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket !!\n",
							__FUNCTION__ );
				} 


				p_mme_ctx->sctp_einprogress_initiated_from = MME_INVALID_STATE; 
				MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);

				RRC_S1AP_TRACE(RRC_WARNING,"[%s]::RAISING ALARM , max retries for EINPROGRESS done",
						__FUNCTION__);
				ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID,
						CRITICAL_ALARM,
						"MME IP Address = %s\n",
						p_mme_ctx->connection_info.ip_addr[0].ip_addr);


				delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
				p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;

				/* Send the LINK Down Indication to the OAM */
				s1ap_build_and_send_oam_s1ap_link_status_ind(p_s1ap_gb_ctx,
						p_mme_ctx,
						OAM_S1AP_LINK_DOWN);


				p_s1ap_gb_ctx->last_selected_mme_id = ((p_s1ap_gb_ctx->last_selected_mme_id + 1) % (p_s1ap_gb_ctx->
							num_of_mme_prov ));

				p_mme_ctx_new = p_s1ap_gb_ctx->mme_contexts[p_s1ap_gb_ctx->last_selected_mme_id];

				mme_fsm_process_event(
						MME_EVENT_S1_MME_PROV_REQ,
						&(p_mme_ctx_new->connection_info),
						&(p_s1ap_gb_ctx->enb_comm_info),
						p_s1ap_gb_ctx,
						p_mme_ctx_new);

			}
			else 
			{
				handle_sctp_open_connection_failure(
						p_s1ap_gb_ctx,
						p_mme_ctx);
				if (PNULL != p_local_sctp_setup_result)
				{
					p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_FAILURE;
				}
			}
		}
		else
		{
			/* Start sctp connection establish timer  */
			conn_establish_timer_buf.timer_type = S1AP_SCTP_CONN_TIMER ;
			conn_establish_timer_buf.auto_delete = MME_TRUE;
			conn_establish_timer_buf.mme_id = p_mme_ctx->mme_id;

			/*Starting the S1AP_SCTP_CONN_TIMER timer*/
			p_mme_ctx->mme_timer_id =  rrc_start_timer(
					SCTP_W_CONNECT_TIMER_DURATION, 
					&conn_establish_timer_buf,
					sizeof(conn_establish_timer_buf), 
					RRC_FALSE);
            /* SPR 20633: CID 88645 Start */

		}
        /* SPR 20633: CID 88645 End */

	}
	else
	{
		p_mme_ctx->sctp_einprogress_retry_count = S1AP_ZERO ;



		ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_RECOVERY_ALARM_ID,
				NOTIFICATION,
				"MME IP Address = %s\n",
				p_mme_ctx->connection_info.ip_addr[0].ip_addr);

		/*Generating Protocl Event LOCAL_SCTP_SETUP_ATTEMPT*/ 
		p_local_sctp_setup_att = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT));
		if (PNULL != p_local_sctp_setup_att)
		{
			memset_wrapper(p_local_sctp_setup_att, RRC_NULL, sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT));
			p_local_sctp_setup_att->header.length = sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT);
			p_local_sctp_setup_att->header.event_id = LOCAL_SCTP_SETUP_ATTEMPT;

			l3_memcpy_wrapper(p_local_sctp_setup_att->EVENT_PARAM_IP_ADDRESS ,p_mme_ctx->
					connection_info.ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

			rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_att);
		}

		if (PNULL != p_local_sctp_setup_result)
		{
			p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_SUCCESS;
		}



		/*Storing  a SD in  p_mme_ctx */
		p_node = (sctp_sd_search_node_t*)rrc_mem_get
			(sizeof(sctp_sd_search_node_t));
		if(PNULL == p_node)
		{
			RRC_S1AP_TRACE(RRC_FATAL,"[%s]: Cannot allocate Memory",
					__FUNCTION__);
			RRC_S1AP_UT_TRACE_EXIT();
			/* Change state to MME_IDLE */
			MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);

            /* SPR 20633: CID 88645 Start */
            }
        else
        {
            /* SPR 20633: CID 88645 End */
		p_node->sctp_sd = p_mme_ctx->sctp_sd;
		p_node->mmeId = p_mme_ctx->mme_id;

		st_insert_node (
				&(p_s1ap_gb_ctx->sctp_socket_desc_search_tree),
				&(p_node->nodeAnchor));

		RRC_S1AP_TRACE(RRC_INFO,"[%s]::SCTP SD %d inserted st addr [0x%p]\n",
				__FUNCTION__,p_mme_ctx->sctp_sd,
				&(p_s1ap_gb_ctx->sctp_socket_desc_search_tree));

		/*Change the state to Wait For Connection to be successful*/
		MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
		RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
				__FUNCTION__);
		if (PNULL != p_local_sctp_setup_result)
		{
			rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
            /* SPR 20633: CID 88645 Start */
            p_local_sctp_setup_result = PNULL;
		}
        }
        /* SPR 20633: CID 88645 End */

	}
    /* SPR 20633: CID 88645 Start */
    if(PNULL != p_local_sctp_setup_result)
    {
        rrc_mem_free(p_local_sctp_setup_result);
        p_local_sctp_setup_result = PNULL;
    }
    /* SPR 20633: CID 88645 End */
	RRC_S1AP_UT_TRACE_EXIT();
}
/*SPR 15570 Fix Stop*/

/****************************************************************************
 * Function Name  : mme_fsm_sctp_conn_failure_ind
 * Inputs         : p_api                  : pointer to p_api
 *                  p_msg                  : Pointer to message buffer
 *                  p_s1ap_gb_ctx          : pointer to S1AP global context
 *                  p_mme_ctx              : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon reception of sctp connection 
 *                  failure indication from SCTP layer.
 ****************************************************************************/
void  mme_fsm_sctp_conn_failure_ind(
        void                * p_api,             
        void                * p_msg,
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx)
{
    s1ap_timer_buffer_t conn_recovery_timer_buf;        
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p",
            p_api,p_msg);
    
    /*Change the state to Wait For Connection to be successful*/
    MME_SET_FSM_STATE(p_mme_ctx, MME_W_SCTP_CONN_RECOVERY );
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_SCTP_CONN_RECOVERY]",
                                                              __FUNCTION__);

    /* Start sctp connection recovery timer  */
    conn_recovery_timer_buf.timer_type = S1AP_MME_SCTP_CONN_REC_TIMER ;
    conn_recovery_timer_buf.auto_delete = MME_TRUE;
    conn_recovery_timer_buf.mme_id = p_mme_ctx->mme_id;

    /*Starting the S1AP_MME_SCTP_CONN_REC_TIMER timer*/
    p_mme_ctx->mme_timer_id =  rrc_start_timer(
            p_s1ap_gb_ctx->s1ap_conn_recovery_duration , 
            &conn_recovery_timer_buf,
            sizeof(conn_recovery_timer_buf), 
            RRC_FALSE);

    /*Deleting the MME from the active mme list in the global context*/
    active_mme_list_delete(
            p_s1ap_gb_ctx, 
            p_mme_ctx->mme_id);

    /* Send MME Info to UECC */
    build_and_send_mme_status_msg_ind(p_mme_ctx, S1AP_FALSE,p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mme_fsm_sctp_re_conn_try_failure_ind
 * inputs         : p_api                  : pointer to p_api
 *                  p_msg                  : Pointer to message buffer
 *                  p_s1ap_gb_ctx          : pointer to S1AP global context
 *                  p_mme_ctx              : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon reception of sctp connection 
 *                  failure indication from SCTP layer while its was in 
 *                  MME_W_SCTP_CONN_RECOVERY.
 ****************************************************************************/
void  mme_fsm_sctp_re_conn_try_failure_ind(
        void                * p_api,             
        void                * p_msg,
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx)
{
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p p_s1ap_gb_ctx:%p",
            p_api,p_msg,p_s1ap_gb_ctx);

    /*No Need to Change the state as it is already in Wait For Connection 
      to be successful*/
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Failure Occured during the"
            " try to SCTP Connection "
            "recovery Present State [MME_W_SCTP_CONN_RECOVERY][%d]",
             __FUNCTION__,
            MME_GET_FSM_STATE(p_mme_ctx));
    RRC_S1AP_UT_TRACE_EXIT();
}



/****************************************************************************
 * Function Name  : mme_fsm_sctp_conn_rec_ind
 * Inputs         : p_api           : pointer to api data
 *                  p_msg           : pointer to msg buffer
 *                  p_s1ap_gb_ctx   : pointer to S1AP global context
 *                  p_mme_ctx       : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon reception sctp connection 
 *                   recovery indication from SCTP layer
 ****************************************************************************/
void  mme_fsm_sctp_conn_rec_ind (
        void                * p_api,             
        void                * p_msg,
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx)
{
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p ",
            p_api,p_msg);

    /*1.    Stop SCTP recovery timer.*/
    if (PNULL != p_mme_ctx->mme_timer_id)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Stop timer "
               "[S1AP_MME_SCTP_CONN_REC_TIMER]",
                __FUNCTION__);
        rrc_stop_timer(p_mme_ctx->mme_timer_id);
        p_mme_ctx->mme_timer_id = PNULL;
    }

    /*2. Change the state to MME_IDLE*/
    MME_SET_FSM_STATE(p_mme_ctx, MME_ACTIVE);
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_ACTIVE]",__FUNCTION__);

    /*Adding the MME into the active mme in the global context*/
    active_mme_list_insert(
            p_s1ap_gb_ctx, 
            p_mme_ctx->mme_id);
    
    /* Send MME Info to UECC */
    build_and_send_mme_status_msg_ind(p_mme_ctx, S1AP_TRUE,p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mme_fsm_sctp_conn_rec_timer_expiry
 * Inputs         : p_api               : pointer to api data
 *                  p_msg               : pointer to message buffer
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon reception of S1 SETUP 
 *                  FAILURE from an MME.
 ****************************************************************************/
void  mme_fsm_sctp_conn_rec_timer_expiry (
        void                * p_api,             
        void                * p_msg,
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx)
{
    sctp_sd_search_node_t *p_node = PNULL;
    rrc_return_et response = RRC_FAILURE;
    S32 sctp_open_assoc_resp = S1AP_SCTP_ERROR;
    LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;
    /*SPR 15570 Fix Start*/
    s1ap_timer_buffer_t conn_establish_timer_buf;  
    p_mme_ctx->sctp_einprogress_initiated_from = MME_INVALID_STATE; 
    /*SPR 15570 Fix Stop*/
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p ",
            p_api,p_msg);

    /* Bug 9582 Fix Start */
    /* Timer expired and is not valid any more */
    p_mme_ctx->mme_timer_id = PNULL;
    /* Bug 9582 Fix End */

    /*Close SCTP association*/
    response = l3_sctp_close_association (p_mme_ctx->sctp_sd);

    if (RRC_FAILURE == response)
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket !!\n"
                                    ,__FUNCTION__);
    }

    /* Send the LINK Down Indication to the OAM */
    s1ap_build_and_send_oam_s1ap_link_status_ind(p_s1ap_gb_ctx,
                                                 p_mme_ctx,
                                                 OAM_S1AP_LINK_DOWN);
                                                                            
    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID, 
            CRITICAL_ALARM,
            "MME IP Address = %s\n",
            p_mme_ctx->connection_info.ip_addr[0].ip_addr);
    
    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_CONNECTION_FAILURE_ALARM_ID, 
            CRITICAL_ALARM,
            "MME IP Address = %s\n",
            p_mme_ctx->connection_info.ip_addr[0].ip_addr);

    delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);    
    p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;

    /* Calling Reset Function for corresponding MME, after receiving this 
       UECC initiate the RRC Connection Release procedure 
       with all UEs corresponds to this MME*/
    process_reset_all( 
            p_s1ap_gb_ctx, 
            p_mme_ctx);

    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_MME;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS ,p_mme_ctx->
                connection_info.ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_LOCAL;
    }

    /* SPR 13432 Fix Start */
    if (p_s1ap_gb_ctx->s1_conn_mode_one_flag)
    {
        MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE );

    	p_s1ap_gb_ctx->last_selected_mme_id++;

        p_s1ap_gb_ctx->last_selected_mme_id = ((p_s1ap_gb_ctx->last_selected_mme_id) % (p_s1ap_gb_ctx->
                    num_of_mme_prov));

        p_mme_ctx = p_s1ap_gb_ctx->mme_contexts[p_s1ap_gb_ctx->last_selected_mme_id];

    }
    /* SPR 13432 Fix Stop */

    /* create the Socket SD details  Socket creation , binding , connect */
    sctp_open_assoc_resp = l3_open_sctp_association(
		    p_s1ap_gb_ctx,
		    p_mme_ctx,
		    &(p_s1ap_gb_ctx->enb_comm_info)); 
    /*SPR 15570 Fix Start*/
    if(RRC_TRUE == p_mme_ctx->is_conn_ongoing)
    {
	    /*Storing  a SD in  p_mme_ctx */
	    p_mme_ctx->sctp_sd = sctp_open_assoc_resp;
	    p_mme_ctx->sctp_einprogress_retry_count = S1AP_ZERO;
	    p_mme_ctx->sctp_einprogress_initiated_from = MME_W_SCTP_CONN_RECOVERY; 


	    /* Start sctp connection establish timer  */
	    conn_establish_timer_buf.timer_type = S1AP_SCTP_CONN_TIMER ;
	    conn_establish_timer_buf.auto_delete = MME_TRUE;
	    conn_establish_timer_buf.mme_id = p_mme_ctx->mme_id;

	    /*Starting the S1AP_SCTP_CONN_TIMER timer*/
	    p_mme_ctx->mme_timer_id =  rrc_start_timer(
			    SCTP_W_CONNECT_TIMER_DURATION, 
			    &conn_establish_timer_buf,
			    sizeof(conn_establish_timer_buf), 
			    RRC_FALSE);
	    MME_SET_FSM_STATE(p_mme_ctx, MME_W_SCTP_CONNECT);
	    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_SCTP_CONNECT]",
			    __FUNCTION__);

    }
    else 
    {

	    /*SPR 15570 Fix Stop*/

    /*Opening a SCTP association Failed*/
    if (INVALID_SCTP_SOCKET == sctp_open_assoc_resp) 
    {
        handle_sctp_open_connection_failure(
                p_s1ap_gb_ctx,
                p_mme_ctx);
        if (PNULL != p_local_sctp_setup_result)
        {
            p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_FAILURE;
        }
    }
    else
    {
        /* SPR 13471 Fix Start */
        handle_sctp_open_connection_failure(
                p_s1ap_gb_ctx,
                p_mme_ctx);
        /* SPR 13471 Fix Stop */
        ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_RECOVERY_ALARM_ID,NOTIFICATION,
                "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);
        
        if (PNULL != p_local_sctp_setup_result)
        {
            p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_SUCCESS;
        }

        /*Storing  a SD in  p_mme_ctx */
        p_mme_ctx->sctp_sd = sctp_open_assoc_resp;

        p_node = (sctp_sd_search_node_t*)rrc_mem_get
                         (sizeof(sctp_sd_search_node_t));
        if (PNULL != p_node)
        {
            p_node->sctp_sd = sctp_open_assoc_resp;
            p_node->mmeId = p_mme_ctx->mme_id;

            st_insert_node (
                    &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree),
                    &(p_node->nodeAnchor));
            RRC_S1AP_TRACE(RRC_INFO,"[%s]::SCTP SD %d inserted st addr [0x%p]\n",
                    __FUNCTION__,sctp_open_assoc_resp,
                    &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree));
        }
/*Bug 9050 fixes start*/ 


        /*Change the state to Wait For Connection to be successful*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
                __FUNCTION__);
	/*Bug 9050 fixes end*/ 
	/*SPR 15570 Fix Start*/
    }
    /*SPR 15570 Fix Stop*/

    }
    if (PNULL != p_local_sctp_setup_result)
    {
        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
        /* SPR 20633: CID 88645 Start */
        p_local_sctp_setup_result = PNULL;
        /* SPR 20633: CID 88645 End */
    }
}

/****************************************************************************
 * Function Name  : mme_fsm_sctp_connect_timer_expiry
 * Inputs         : p_api               : pointer to api data 
 *                  p_msg               : pointer to msg 
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon expiry of sctp connect timer.
 ****************************************************************************/
void  mme_fsm_sctp_connect_timer_expiry (
        void                * p_api,             
        void                * p_msg,
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx)
{
    sctp_sd_search_node_t *p_node = PNULL;
    S32 sctp_open_assoc_resp = S1AP_SCTP_ERROR;
    LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;
    /* SPR 13432 Fix Start */
    mme_context_t        * p_mme_ctx_new = PNULL;
    /* SPR 13432 Fix Stop */
    /* SPR 13471 Fix Start */
    rrc_return_et response = RRC_FAILURE;
    /* SPR 13471 Fix Stop */
    /*SPR 15570 Fix Start*/
    s1ap_timer_buffer_t conn_establish_timer_buf;  
    p_mme_ctx->sctp_einprogress_initiated_from = MME_INVALID_STATE; 
    /*SPR 15570 Fix Stop*/

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p ",
            p_api,p_msg);

    /* SPR 13471 Fix Start */
    p_mme_ctx->mme_timer_id = PNULL;
    /* SPR 13471 Fix Stop */

    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_MME;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS ,p_mme_ctx->
                connection_info.ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_LOCAL;

    }
    /* SPR 13471 Fix Start */
    /*SPR 14658 FIX start*/
    if (INVALID_SCTP_SOCKET != p_mme_ctx->sctp_sd)
    {
        /*Close the SCTP Association*/
        response = l3_sctp_close_association (p_mme_ctx->sctp_sd);

        if (RRC_FAILURE == response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket !!\n",
                    __FUNCTION__ );
        }
        delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
        p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;
    }
    /*SPR 14658 FIX stop*/

    if (S1AP_SCTP_ERROR != p_mme_ctx->sctp_sd) 
    {
        response = l3_sctp_close_association(p_mme_ctx->sctp_sd);

        if (RRC_FAILURE == response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket!!\n" 
                    ,__FUNCTION__);
        }

        ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID, 
                CRITICAL_ALARM,
                "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);

        delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
        p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;

        /*Change the state to Wait For Connection to be successful*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_SCTP_CONNECT]",
                __FUNCTION__);

    }
    /* SPR 13471 Fix Stop */
    /* create the Socket SD details  Socket creation , binding , connect */
    sctp_open_assoc_resp = l3_open_sctp_association(
		    p_s1ap_gb_ctx,
		    p_mme_ctx,
		    &(p_s1ap_gb_ctx->enb_comm_info));
    /*SPR 15570 Fix Start*/
    if(RRC_TRUE == p_mme_ctx->is_conn_ongoing)
    {
	    /*Storing  a SD in  p_mme_ctx */
	    p_mme_ctx->sctp_sd = sctp_open_assoc_resp;
	    p_mme_ctx->sctp_einprogress_retry_count = S1AP_ZERO;
	    p_mme_ctx->sctp_einprogress_initiated_from = MME_W_CONNECT_SUCCESSFUL; 


	    /* Start sctp connection establish timer  */
	    conn_establish_timer_buf.timer_type = S1AP_SCTP_CONN_TIMER ;
	    conn_establish_timer_buf.auto_delete = MME_TRUE;
	    conn_establish_timer_buf.mme_id = p_mme_ctx->mme_id;

	    /*Starting the S1AP_SCTP_CONN_TIMER timer*/
	    p_mme_ctx->mme_timer_id =  rrc_start_timer(
			    SCTP_W_CONNECT_TIMER_DURATION, 
			    &conn_establish_timer_buf,
			    sizeof(conn_establish_timer_buf), 
			    RRC_FALSE);
	    MME_SET_FSM_STATE(p_mme_ctx, MME_W_SCTP_CONNECT);
	    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_SCTP_CONNECT]",
			    __FUNCTION__);

    }
    else 
    {
	    /*SPR 15570 Fix Stop*/

    /*Opening a SCTP association Failed*/
    if (INVALID_SCTP_SOCKET == sctp_open_assoc_resp) 
    {   /* SPR 13432 Fix Start */
        if ( p_s1ap_gb_ctx->s1_conn_mode_one_flag == S1AP_TRUE )
        {
            MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);
            
            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID,
                CRITICAL_ALARM,
                "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);
           
            delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
            p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;

            /* Send the LINK Down Indication to the OAM */
            s1ap_build_and_send_oam_s1ap_link_status_ind(p_s1ap_gb_ctx,
                                                 p_mme_ctx,
                                                 OAM_S1AP_LINK_DOWN);
                                                 
    	    p_s1ap_gb_ctx->last_selected_mme_id++;
                           
            p_s1ap_gb_ctx->last_selected_mme_id = ((p_s1ap_gb_ctx->last_selected_mme_id) % (p_s1ap_gb_ctx->
                        num_of_mme_prov ));

            p_mme_ctx_new = p_s1ap_gb_ctx->mme_contexts[p_s1ap_gb_ctx->last_selected_mme_id];

            mme_fsm_process_event(
                    MME_EVENT_S1_MME_PROV_REQ,
                    &(p_mme_ctx_new->connection_info),
                    &(p_s1ap_gb_ctx->enb_comm_info),
                    p_s1ap_gb_ctx,
                    p_mme_ctx_new);
        }
        else
        {
             handle_sctp_open_connection_failure(
                p_s1ap_gb_ctx,
                p_mme_ctx);
        }
        /* SPR 13432 Fix Stop */

        if (PNULL != p_local_sctp_setup_result)
        {
            p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_FAILURE;
        }
    }
    else
    {
        /* SPR 13471 Fix Start */
        handle_sctp_open_connection_failure(
                p_s1ap_gb_ctx,
                p_mme_ctx);
        /* SPR 13471 Fix Stop */
        /*Build S1 SETUP REQUEST TO send to MME*/
        ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_RECOVERY_ALARM_ID,NOTIFICATION,
                "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);

        if (PNULL != p_local_sctp_setup_result)
        {
            p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_SUCCESS;
        }
        
/*Bug 9050 fixes start*/ 
        /*Storing  a SD in  p_mme_ctx */
        p_mme_ctx->sctp_sd = sctp_open_assoc_resp;
        p_node = (sctp_sd_search_node_t*)rrc_mem_get
                              (sizeof(sctp_sd_search_node_t));
        if (PNULL != p_node)
        {
            p_node->sctp_sd = sctp_open_assoc_resp;
            p_node->mmeId = p_mme_ctx->mme_id;

            st_insert_node (
                    &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree),
                    &(p_node->nodeAnchor));
            RRC_S1AP_TRACE(RRC_INFO,"[%s]::SCTP SD %d inserted st addr[0x%p]\n",
                    __FUNCTION__,sctp_open_assoc_resp,
                    &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree));
        }
        /*Change the state to Wait For Connection to be successful*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
                __FUNCTION__);
	/*Bug 9050 fixes end*/ 
	/*SPR 15570 Fix Start*/
    }
    /*SPR 15570 Fix Stop*/
    }
    if (PNULL != p_local_sctp_setup_result)
    {
        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
        /* SPR 20633: CID 88645 Start */
        p_local_sctp_setup_result = PNULL;
        /* SPR 20633: CID 88645 End */
    }
}

/*****************************************************************************
 * Function Name  : mme_fsm_process_event
 * Inputs         : event_id            : event id related to the mme
 *                  param1              : pointer to p_api or timer_id
 *                  param2              : pointer to p_msg or p_s1ap_timer_buf
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This is entry point function for S1AP MME FSM. This function
 *                  calls appropriate function handler based on state and event
 *                  combination.
 *****************************************************************************/
void mme_fsm_process_event(
        mme_event_et     event_id,
        void                *param1,            
        void                *param2,            
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx)
{
    mme_state_et state = MME_INVALID_STATE;

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    
    
    RRC_S1AP_UT_TRACE_ENTER();
    
    state = (mme_state_et)MME_GET_FSM_STATE (p_mme_ctx);

    MME_FSM_EVENT_TRACE (state, event_id);

    if ( (state < MME_NUM_OF_STATES) && (event_id < MME_NUM_OF_EVENTS) )
    {
        /* Get appropriate event handler */
        (*mme_fsm[state][event_id])(
                                    param1, 
                                    param2, 
                                    p_s1ap_gb_ctx, 
                                    p_mme_ctx);
             
        RRC_S1AP_TRACE(RRC_INFO, "#### MME: %u Event: %s Change State: %s -> %s ####",
                     p_mme_ctx->mme_id,
                     MME_FSM_EVENTS_NAMES[event_id],
                     MME_FSM_STATES_NAMES[state],
                     MME_FSM_STATES_NAMES[p_mme_ctx->current_enb_config_update_state]);
    }
    else
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]::Invalid Event : "
                "unexpected state or event!",
                __FUNCTION__);
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mme_fsm_shutdown_event_handler
 * Inputs         : p_api               : pointer to api
 *                  p_msg               : Pointer to message buffer
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon the occurance of shutdown 
 *                  event when MME state is not active.
 ****************************************************************************/

void  mme_fsm_shutdown_event_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    LOCAL_S1_SETUP_RESULT_STRUCT    *p_s1_setup_result = PNULL;
    mme_fsm_w_s1_resp_conn_failure(
            p_api,
            p_msg,
            p_s1ap_gb_ctx,
            p_mme_ctx);

    /* Generating the Protocal Event LOCAL_S1_SETUP_RESULT*/
    p_s1_setup_result = rrc_mem_get(sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
    if (PNULL != p_s1_setup_result)
    {
        memset_wrapper(p_s1_setup_result, RRC_NULL, sizeof(LOCAL_S1_SETUP_RESULT_STRUCT));
        p_s1_setup_result->header.length = sizeof(LOCAL_S1_SETUP_RESULT_STRUCT);
        p_s1_setup_result->header.event_id = LOCAL_S1_SETUP_RESULT;

        p_s1_setup_result->EVENT_PARAM_S1_SETUP_RESULT = SCTP_SHUTDOWN;
        l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_IP_ADDRESS, p_mme_ctx->connection_info.
            ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
        l3_memcpy_wrapper(p_s1_setup_result->EVENT_PARAM_MME_NAME, p_mme_ctx->mme_name,
                (sizeof(U8) * MAX_MME_NAME_STR));

        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_s1_setup_result);
    }
}

/****************************************************************************
 * Function Name  : mme_fsm_mme_active_shutdown_event_handler
 * Inputs         : p_api               : pointer to api
 *                  p_msg               : Pointer to message buffer
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon thee occurance of shutdown 
 *                  event when MME state is active.
 ****************************************************************************/

void  mme_fsm_mme_active_shutdown_event_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    /* Send the LINK Down Indication to the OAM */
    s1ap_build_and_send_oam_s1ap_link_status_ind(p_s1ap_gb_ctx,
                                                 p_mme_ctx,
                                                 OAM_S1AP_LINK_DOWN);
    
    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_CONNECTION_FAILURE_ALARM_ID, 
            CRITICAL_ALARM,
            "MME IP Address = %s\n",
            p_mme_ctx->connection_info.ip_addr[0].ip_addr);
    
    /* Calling Reset Function for corresponding MME, after receiving this 
       UECC initiate the RRC Connection Release procedure with all UEs 
       corresponds to this MME*/

    process_reset_all(p_s1ap_gb_ctx,
            p_mme_ctx);

    mme_fsm_w_s1_resp_conn_failure(
            p_api,
            p_msg,
            p_s1ap_gb_ctx,
            p_mme_ctx);

    /*Deleting the MME from the active mme list in the global context*/
    active_mme_list_delete(
            p_s1ap_gb_ctx, 
            p_mme_ctx->mme_id);

    /* Send MME Info to UECC */
    build_and_send_mme_status_msg_ind(p_mme_ctx, S1AP_FALSE,p_s1ap_gb_ctx);
}


/****************************************************************************
 * Function Name  : mme_fsm_oam_mme_close_conn_handler 
 * Inputs         : p_api               : pointer to api
 *                  p_msg               : Pointer to message buffer
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when request if received to close
 *                  the SCTP connection for requested MME.
 ****************************************************************************/
void  mme_fsm_oam_mme_close_conn_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    rrc_return_et *result;

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    /*coverity fix start*/
    /*coverity fix stop*/

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    result = (rrc_return_et *)p_msg;

    /* Trigger Reset for associated Context */
    process_reset_all(p_s1ap_gb_ctx, p_mme_ctx);

    /* Delete the MME from the active List */
    active_mme_list_delete(
            p_s1ap_gb_ctx, 
            p_mme_ctx->mme_id);

    /* Send MME Info to UECC */
    build_and_send_mme_status_msg_ind(p_mme_ctx, S1AP_FALSE,p_s1ap_gb_ctx);

    MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);

    /*Close the SCTP Association*/	
    *result = l3_sctp_close_association (p_mme_ctx->sctp_sd);

    delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
    p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;

    if (RRC_FAILURE == *result)
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket!!\n"
                                  ,__FUNCTION__);
    }
}

/****************************************************************************
 * Function Name  : mme_fsm_w_s1_resp_conn_failure
 * Inputs         : p_api               : pointer to api
 *                  p_msg               : Pointer to message buffer
 *                  p_s1ap_gb_ctx       : pointer to S1AP global context
 *                  p_mme_ctx           : Pointer to mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called upon the occurance of SCTP 
 *                  connection failure event when MME state is W_S1_SETUP_RESP.
 ****************************************************************************/

void  mme_fsm_w_s1_resp_conn_failure(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    rrc_return_et response = RRC_FAILURE;
    s1ap_timer_buffer_t wait_for_connect_timer_buf ;    

    /*coverity fix start*/
    /*coverity fix stop*/

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p",p_api,p_msg);

    if (PNULL != p_mme_ctx->mme_timer_id)
    {
        rrc_stop_timer(p_mme_ctx->mme_timer_id);
        p_mme_ctx->mme_timer_id = PNULL;
    }
    
    /*Close the SCTP Association*/
    response = l3_sctp_close_association (p_mme_ctx->sctp_sd);
    
    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID, 
            CRITICAL_ALARM,
            "MME IP Address = %s\n",
            p_mme_ctx->connection_info.ip_addr[0].ip_addr);
    
    delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
    p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;

    if (RRC_FAILURE == response)
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket!!\n"
                                  ,__FUNCTION__);
    }


    /* SPR 13432 Fix Start */
    if (p_s1ap_gb_ctx->s1_conn_mode_one_flag)
    {
        MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE );

    	p_s1ap_gb_ctx->last_selected_mme_id++;
        p_s1ap_gb_ctx->last_selected_mme_id = ((p_s1ap_gb_ctx->last_selected_mme_id) % (p_s1ap_gb_ctx->
                    num_of_mme_prov));

        p_mme_ctx = p_s1ap_gb_ctx->mme_contexts[p_s1ap_gb_ctx->last_selected_mme_id];

    }
    /* SPR 13432 Fix Stop */

    /*Change the state to Wait For Connection to be successful*/
    MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
            __FUNCTION__);

    /* Start sctp connection recovery timer  */
    wait_for_connect_timer_buf.timer_type  = S1AP_MME_SCTP_CONNECT_TIMER ;
    wait_for_connect_timer_buf.auto_delete = MME_TRUE;
    wait_for_connect_timer_buf.mme_id      = p_mme_ctx->mme_id;

    p_mme_ctx->mme_timer_id =  rrc_start_timer(
            p_s1ap_gb_ctx->s1ap_conn_recovery_duration, 
            &wait_for_connect_timer_buf,
            sizeof(wait_for_connect_timer_buf), 
            RRC_FALSE);
}


/*****************************************************************************
 * Function Name  : delete_sctp_sd_from_tree
 * Inputs         : p_s1ap_gb_ctx  - Pointer to the S1AP global context data
 *                  p_mme_ctx      - Pointer to MME context
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes the sctp sd from desc tree.
 ******************************************************************************/
void delete_sctp_sd_from_tree(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_ctx)
{
    sctp_sd_search_node_t *p_sd_node = PNULL;
    /* Searching & Deleting Socket Descriptor Entry 
       from socket descriptor search tree */
    p_sd_node = (sctp_sd_search_node_t* )
        st_find_tree_node(
                &(p_s1ap_gb_ctx->
                    sctp_socket_desc_search_tree),
                &(p_mme_ctx->sctp_sd));

    st_delete_node(
            &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree),
            &(p_sd_node->nodeAnchor));
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::SCTP SD %d deleted st addr [0x%p]\n",
            __FUNCTION__,p_mme_ctx->sctp_sd,
            &(p_s1ap_gb_ctx->sctp_socket_desc_search_tree));

    if (PNULL != p_sd_node)
    {
	rrc_mem_free(p_sd_node);
	/*coverity fix 54705 start*/
	p_sd_node = PNULL;
	/*coverity fix 54705 stop*/
    }

    RRC_S1AP_TRACE(RRC_DETAILED,"SCTP Socket Node deleted from the search "
            "tree \n");
}


/*****************************************************************************
 * Function Name  : handle_sctp_send_failure
 * Inputs         : p_s1ap_gb_ctx  - Pointer to the S1AP global context data
 *                  p_mme_ctx      - Pointer to MME context
 * Outputs        : None
 * Returns        : None
 * Description    : This function closes the SCTP association,
 *                  changes the state to MME_W_CONNECT_SUCCESSFUL
 *                  and starts the SCTP CONNECT TIMER.  
 ******************************************************************************/

static void handle_sctp_send_failure(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t     * p_mme_ctx )
{
    rrc_return_et response = RRC_FAILURE;
    s1ap_timer_buffer_t wait_for_connect_timer_buf ;
    RRC_S1AP_TRACE(RRC_WARNING,"[%s]::S1AP Setup message send failed\n",
            __FUNCTION__);

    /*Close the SCTP Association*/
    response = l3_sctp_close_association(p_mme_ctx->sctp_sd);

    if (RRC_FAILURE == response)
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket!!\n" 
                                  ,__FUNCTION__);
    }
    
    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID, 
            CRITICAL_ALARM,
            "MME IP Address = %s\n",
            p_mme_ctx->connection_info.ip_addr[0].ip_addr);

    delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
    p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;

    /*Change the state to Wait For Connection to be successful*/
    MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
            __FUNCTION__);

    /* Start sctp connection recovery timer  */
    wait_for_connect_timer_buf.timer_type  = S1AP_MME_SCTP_CONNECT_TIMER ;
    wait_for_connect_timer_buf.auto_delete = MME_TRUE;
    wait_for_connect_timer_buf.mme_id      = p_mme_ctx->mme_id;
    p_mme_ctx->mme_timer_id =  rrc_start_timer(
            p_s1ap_gb_ctx->s1ap_conn_recovery_duration, 
            &wait_for_connect_timer_buf,
            sizeof(wait_for_connect_timer_buf), 
            RRC_FALSE);
}

/*****************************************************************************
 * Function Name  : process_reset_all
 * Inputs         : p_s1ap_gb_context  - Pointer to the S1AP global context data
 *                  p_mme_context      - Pointer to MME context
 * Outputs        : None
 * Returns        : None
 * Description    : This message Prepares a list enbUeS1apId and send them to
 *                  all supported cells. It also clears the ue context from the
 *                  s1ap global context.
 ******************************************************************************/


void process_reset_all(s1ap_gb_context_t * p_s1ap_gb_context,
        mme_context_t     * p_mme_context )
{
	/* coverity_fix_54706_start */
	ue_s1ap_context_t *p_ue_s1ap_context = PNULL ;   /* UE context in s1ap */
	void *p_mmeUeS1apId_search_node   = PNULL;
	void *p_temp_mmeUeS1apId_search_node   = PNULL;
	/*REL 1.2:HANDOVER PROCEDURE START*/
	ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
	ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node_next = PNULL;
	/*REL 1.2:HANDOVER PROCEDURE END*/

    /*SPR_21818_START*/
    rrc_return_t    result;
    /*SPR_21818_END*/

	U8 cell_id      = RRC_NULL;
	U16 cell_index  = RRC_NULL;
/* SPR 19286 Fix Start */
/* Code Deleted */
/* SPR 19286 Fix End */
	s1ap_ENB_UE_S1AP_ID enbUeS1apId = MAX_NUM_UE; /* Id to identify ue over
							 s1 interface in enB */


	static s1ap_reset_msg_ind_t s1ap_reset_ind[MAX_NUM_CELLS] ;  /* Data structure for 
									S1AP_RESET_MSG_IND */
    /*SPR_21818_START*/
    U16 enbUeS1apId_index = RRC_NULL;
    /*SPR_21818_END*/
	RRC_S1AP_UT_TRACE_ENTER();
	memset_wrapper((void*)&s1ap_reset_ind,0,
			sizeof(s1ap_reset_msg_ind_t) * MAX_NUM_CELLS);

	/* Iterating through the mmeUeS1apID search tree */
	for(
			p_mmeUeS1apId_search_node = (mmeUeS1apId_search_node_t *)
			st_get_first_node( 
				(search_tree_t *)
				&(p_mme_context ->
					mmeUeS1apId_search_tree));

			p_mmeUeS1apId_search_node != PNULL;

	   ) 

	{
                /*SPR_20833_FIX_START*/
                /*code deleted*/
                /*SPR_20833_FIX_END*/
		/* Getting the enbUeS1apId */         
		enbUeS1apId  = ((mmeUeS1apId_search_node_t *)
				p_mmeUeS1apId_search_node)
			-> enbUeS1apId;
                /*SPR_20833_FIX_START*/
                RRC_S1AP_TRACE(RRC_INFO, 
                "[%s]:Getting the enbUeS1apId[%u]",
                    __FUNCTION__,enbUeS1apId);
                /*SPR_20833_FIX_END*/
		/* Get cell id from enbUeS1apId */
		cell_id = s1ap_get_cell_id(p_s1ap_gb_context, enbUeS1apId);

		if (cell_id >= MAX_NUM_CELLS ||
				(!p_s1ap_gb_context->cell_s1ap_contexts[cell_id]))
		{
			RRC_S1AP_TRACE(RRC_ERROR, "%s: "
					"Either Cell id %d is not in valid range (%d) "
					"Or it's not active "
					"Hence ignoring enbUeS1apId 0x%x",__FUNCTION__,
					cell_id,MAX_NUM_CELLS,enbUeS1apId);
            /* SPR 19286 Fix Start */
            cell_id = RRC_NULL; 
            if (PNULL != (p_s1ap_gb_context->p_p_ue_s1ap_contexts[enbUeS1apId]))
            {    
                p_s1ap_gb_context->p_p_ue_s1ap_contexts[enbUeS1apId]->cell_index = RRC_NULL;
            }    
            RRC_S1AP_TRACE(RRC_INFO, "%s: "
                    "Cell id is not in valid ranage Hence setting the cell index to first cell index i.e zero[%d] \n"  
                   ,__FUNCTION__,cell_id);

                /* Code Deleted */
            /* SPR 19286 Fix End */ 
		}
        /* SPR 19286 Fix  Start */
        /* Code Deleted */
		RRC_S1AP_TRACE(RRC_INFO, "%s: "
                "Deleting Entry for enbUeS1apId 0%x cellid %d\n",
				__FUNCTION__,
				enbUeS1apId,
                cell_id
                );

        /* Get the s1ap context related to a particular UE in a cell */
        get_ue_ctx_from_ue_id(p_s1ap_gb_context, &p_ue_s1ap_context, enbUeS1apId);

        if (PNULL != p_ue_s1ap_context)
        {
            /* Preparing the list of UeIndex */                            
            s1ap_reset_ind[cell_id].ue_index_list.ue_index[
                s1ap_reset_ind[cell_id].ue_index_list.count]
                = p_ue_s1ap_context->ue_index; 

            /* Bug 4064 Chages Start */            
            s1ap_ueid_free(p_s1ap_gb_context,enbUeS1apId);
            /* Bug 4064 Chages Stop */            
            /* Incrementing count for UeIndex */
            s1ap_reset_ind[cell_id].ue_index_list.count++;

            /* Deleting ue_s1ap_ccontext */
            /*SPR 22789 Fix Start */
            RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_context, p_ue_s1ap_context, enbUeS1apId);
            /*SPR 22789 Fix End */
            /* SPR 19286 Fix End */
        }

		/* Get the next node of the mmeUeS1ap_search tree */ 
		p_temp_mmeUeS1apId_search_node = p_mmeUeS1apId_search_node;
		p_mmeUeS1apId_search_node = (mmeUeS1apId_search_node_t *) 
			st_get_next_node(
					(search_tree_node_t*)
					p_mmeUeS1apId_search_node);

		/* Deleting mmeUeS1apId search node */
		st_delete_node((search_tree_t*)
				&p_mme_context->mmeUeS1apId_search_tree,
				p_temp_mmeUeS1apId_search_node);

		if (PNULL != p_temp_mmeUeS1apId_search_node)
		{
			rrc_mem_free(p_temp_mmeUeS1apId_search_node);
			p_temp_mmeUeS1apId_search_node = PNULL;
		}


		if (p_s1ap_gb_context->num_of_mme_prov == 1)
		{
			/* If MAX NUMBER of MME is equal to one 
			 * then send RESET all IND message  to UECC */
			continue;
		}


		if( (MAX_RESET_UE_S1AP_ID_COUNT -1) == 
				/*BUG 604 changes start*/
				s1ap_reset_ind[cell_id].ue_index_list.count)
			/*BUG 604 changes stop*/
		{
			RRC_S1AP_TRACE(RRC_INFO, "%s: "
					"LIST size is 256 hence sending S1AP RESET IND"
					"count %d\n",__FUNCTION__,
					/*BUG 604 changes start*/
					s1ap_reset_ind[cell_id].ue_index_list.count);
			/*BUG 604 changes stop*/

			s1ap_reset_ind[cell_id].bitmask = 0;

			/*If Num of MME =1, then send S1AP_RESET_IND ALL*/
			if (p_s1ap_gb_context->num_of_mme_prov > 1)   
			{
				/*BUG 604 changes start*/
				s1ap_reset_ind[cell_id].bitmask |=
					S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
				/*BUG 604 changes stop*/
			}

			s1ap_build_and_send_reset_ind_msg(
					&s1ap_reset_ind[cell_id],RRC_UECCMD_MODULE_ID);
			memset_wrapper((void*)&s1ap_reset_ind[cell_id],RRC_NULL,
					sizeof(s1ap_reset_msg_ind_t));
		}

	}  
	/*REL 1.2:HANDOVER PROCEDURE START*/
	p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
		list_first_node(&p_s1ap_gb_context->ho_ongoing_ue_ctx_list);

	RRC_S1AP_TRACE(RRC_INFO, "%s HO ONGOING UE CONTEXT LIST count %d : ",
			__FUNCTION__,
			list_count(&p_s1ap_gb_context->ho_ongoing_ue_ctx_list));

	while(p_ho_ongoing_ue_ctx_node)
	{
		p_ho_ongoing_ue_ctx_node_next = (ho_ongoing_ue_ctx_node_t *)
			list_next_node(&p_ho_ongoing_ue_ctx_node->node);

		if (p_mme_context->mme_id ==
				p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mme_id) 
		{
			/* Match found!! */
			RRC_S1AP_TRACE(RRC_INFO,"%s HO node "
					"Match found\n",__FUNCTION__);

			s1ap_reset_ind[0].
				mme_ue_s1ap_id_list.mme_ue_s1ap_id[
				s1ap_reset_ind[0].
				mme_ue_s1ap_id_list.count++] = 
				p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mmeUeS1apId;

			/* Delete ho_ongoing_ue_ctx from the ho_ongoing_ue_ctx_list */
			list_delete_node(&p_s1ap_gb_context->ho_ongoing_ue_ctx_list, (void
						*)p_ho_ongoing_ue_ctx_node);

			if ( PNULL != p_ho_ongoing_ue_ctx_node)
			{
                /*SPR_21818_START*/
                get_ue_ctx_from_ue_id(p_s1ap_gb_context, &p_ue_s1ap_context, 
                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                if (PNULL != p_ue_s1ap_context)
                {
                    /* SPR 22789 Fix Start */
                    RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_context,p_ue_s1ap_context,
                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                    /* SPR 22789 Fix End */
                }
                /*SPR_21818_END*/
                /*SPR 21220 Start*/
                /*BUG 604 changes start*/
                /*Free s1ap_ue_id Ctx*/
                s1ap_ueid_free(p_s1ap_gb_context,
                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                /*BUG 604 changes stop*/
                rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
                p_ho_ongoing_ue_ctx_node = PNULL;
                RRC_S1AP_TRACE(RRC_INFO,"%s HO ONGOING UE  Context DELETED\n", __FUNCTION__);
                RRC_S1AP_TRACE(RRC_INFO, 
                    "%s HO ONGOING UE CONTEXT LIST count %d : ",
                    __FUNCTION__, 
                    list_count(&p_s1ap_gb_context->ho_ongoing_ue_ctx_list));
                /*SPR 21220 Stop*/
			}
		}
		p_ho_ongoing_ue_ctx_node = p_ho_ongoing_ue_ctx_node_next;
	}
	/*REL 1.2:HANDOVER PROCEDURE END*/


	RRC_S1AP_TRACE(RRC_INFO, "%s: "
			"After Deletion of search tree nodes"
			"num of entries in mme search tree %d\n",
			__FUNCTION__,st_get_node_count(&p_mme_context->
				mmeUeS1apId_search_tree));
//add for debug
	RRC_S1AP_TRACE(RRC_INFO, "%s: "
			"p_s1ap_gb_context->num_of_mme_prov=%d"
			"p_s1ap_gb_context->active_mme_info.active_mme_count = %d\n",
			__FUNCTION__,p_s1ap_gb_context->num_of_mme_prov,
				p_s1ap_gb_context->active_mme_info.active_mme_count);


	/* Preparing S1AP_UECC_RESET_IND message for each supported cell */
	for(cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
	{

		if ((p_s1ap_gb_context->num_of_mme_prov &&  //"== 1" modify by wood for 114110
					p_s1ap_gb_context->active_mme_info.active_mme_count) 					
				/*BUG 604 changes start*/
				//||(s1ap_reset_ind[cell_index].ue_index_list.count) //remove by wood
			/*BUG 604 changes stop*/
			)
		{

			if (p_s1ap_gb_context->num_of_mme_prov ==1) 
			{

				RRC_S1AP_TRACE(RRC_INFO, "%s: "
						"Sending S1AP RESET IND ALL since Num MME  = 1",
						__FUNCTION__);
				s1ap_reset_ind[cell_index].bitmask = 0;
                /*SPR_21818_START*/
                /* As RESET ALL will be sent, reset eNB_UE_S1AP_Id DB */
                if (PNULL != p_s1ap_gb_context->p_p_ue_s1ap_contexts)
                {
                    for(enbUeS1apId_index = RRC_NULL; 
                        enbUeS1apId_index < p_s1ap_gb_context->total_ue_supported;
                        enbUeS1apId_index++)
                    {
                        if(PNULL !=
                                p_s1ap_gb_context->p_p_ue_s1ap_contexts[enbUeS1apId_index])
                        {
                            rrc_mem_free(p_s1ap_gb_context->p_p_ue_s1ap_contexts[enbUeS1apId_index]);
                        }
                    }
                    memset_wrapper(p_s1ap_gb_context->p_p_ue_s1ap_contexts, RRC_NULL, 
                            sizeof(ue_s1ap_context_t*)*p_s1ap_gb_context->total_ue_supported);
                    result = s1ap_ueid_ctx_init(p_s1ap_gb_context); 
                    if (RRC_FAILURE == result )
                    {
                        RRC_S1AP_TRACE(RRC_WARNING,"unable to initialize s1ap ueid context ");
                        return ;
                    }
                }
                /*SPR_21818_END*/
			}
			else
			{
				s1ap_reset_ind[cell_index].bitmask |=
					S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
				/*BUG 604 changes stop*/
			}
			if(s1ap_reset_ind[cell_index].ue_index_list.count) //add by wood for 114110
			{
				s1ap_build_and_send_reset_ind_msg(
						&s1ap_reset_ind[cell_index],RRC_UECCMD_MODULE_ID);
				RRC_S1AP_TRACE(RRC_INFO, "%s: "
					"Sending S1AP RESET IND msg to UECC for cell id %d"
					"count is %d",
					__FUNCTION__,cell_index,
					/*BUG 604 changes start*/
					s1ap_reset_ind[cell_index].ue_index_list.count);
			/*BUG 604 changes stop*/
			}
		}
	}
}
/* coverity_fix_54706_stop */

/*****************************************************************************
 * Function Name  : handle_sctp_open_connection_failure
 * Inputs         : p_s1ap_gb_context  - Pointer to the S1AP global context data
 *                  p_mme_context      - Pointer to MME context
 * Outputs        : None
 * Returns        : none
 * Description    : This function changes the state to MME_W_CONNECT_SUCCESSFUL
 *                  and starts the SCTP CONNECT TIMER.  
 ******************************************************************************/

static void handle_sctp_open_connection_failure(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_ctx)
{
    s1ap_timer_buffer_t wait_for_connect_timer_buf = {S1AP_OAM_INIT_TIMER,NULL,RRC_FALSE,0};
    /*Change the state to Wait For Connection to be successful*/
    MME_SET_FSM_STATE(p_mme_ctx, MME_W_CONNECT_SUCCESSFUL );
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_W_CONNECT_SUCCESSFUL]",
            __FUNCTION__);

    /* SPR 13471 Fix Start */
    if(PNULL != p_mme_ctx->mme_timer_id)
    {
        rrc_stop_timer(p_mme_ctx->mme_timer_id);
        p_mme_ctx->mme_timer_id = PNULL;
    }
    /* SPR 13471 Fix Stop */

    /* Start sctp connection recovery timer  */
    wait_for_connect_timer_buf.timer_type  = S1AP_MME_SCTP_CONNECT_TIMER ;
    wait_for_connect_timer_buf.auto_delete = MME_TRUE;
    wait_for_connect_timer_buf.mme_id      = p_mme_ctx->mme_id;

    p_mme_ctx->mme_timer_id =  rrc_start_timer(
            p_s1ap_gb_ctx->s1_config_resp_duration, 
            &wait_for_connect_timer_buf,
            sizeof(wait_for_connect_timer_buf), 
            RRC_FALSE);
}


/*****************************************************************************
 * Function Name  : s1ap_fill_served_gummei_info 
 * Inputs         : p_mme_context      : Pointer to mme context of a MME
 *                  p_gummei           : pointer to s1ap_gummei_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills served gummei information.
 ****************************************************************************/
void s1ap_fill_served_gummei_info(
        mme_context_t*    p_mme_context, 
        s1ap_served_gummei_info_t*    p_gummei)
{
    list_t * p_mme_grp_id_list = PNULL;
    list_node_t * p_node = PNULL;
    U16      index = RRC_NULL;
    served_gummei_info_t * p_served_gummei_info = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_context);
    RRC_ASSERT(PNULL != p_mme_context->p_served_gummeis);
    RRC_ASSERT(PNULL != p_gummei);

    p_served_gummei_info = &p_mme_context->p_served_gummeis[0];
/*KlockWork fix*/
    if (PNULL == p_served_gummei_info)
    {
        RRC_S1AP_TRACE(RRC_INFO,"p_served_gummei_info is NULL");
        return;
    }
/*KlockWork fix*/

    /* filling plmn id */
    p_gummei->num_served_plmns 
        = (U8)p_served_gummei_info->servedPLMNs.n;
        
    for (index = 0; (index < p_gummei->num_served_plmns) && 
            (index < MAX_NUM_SERVED_PLMN); index++)
    {        
        l3_memcpy_wrapper((void *)(&(p_gummei->plmn_identity[index].plmn_id)),
            (const void*)p_served_gummei_info->servedPLMNs.
            elem[index].data,
            p_served_gummei_info->servedPLMNs.
                elem[index].numocts);
    }

    /* filling mme grp id */
    p_gummei->num_served_group_ids = (U16)p_served_gummei_info->num_of_mme_group_ids;

    p_mme_grp_id_list = &(p_served_gummei_info->mme_group_id_list);
    if (PNULL != p_mme_grp_id_list)
    {
        p_node = list_first_node(p_mme_grp_id_list);
        for(index = 0; ((index < p_gummei->num_served_group_ids) &&
                    (index < MAX_NUM_SERVED_GROUP_ID) && (PNULL != p_node)); index++)
        {
            l3_memcpy_wrapper((void *)(p_gummei->served_group_id[index].mmegi),
                    (const void*)((((mme_group_id_list_node_t*)p_node)->
                            mme_group_id).data),
                    (((mme_group_id_list_node_t*)p_node)->mme_group_id).
                    numocts);
            p_node = p_node->next;
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"MME Group Id list is NULL");
    }

    /* filling mme code */
    p_gummei->num_served_mmecs 
        = (U8)p_served_gummei_info->served_mmecs.n;
    
    for (index = 0; (index < p_gummei->num_served_mmecs) &&
            (index < MAX_NUM_SERVED_MMEC); index++)
    {
        l3_memcpy_wrapper((void *)(&(p_gummei->served_mmec[index].mmec)),
            (const void*)p_served_gummei_info->served_mmecs.
            elem[index].data,
            p_served_gummei_info->served_mmecs.
                elem[index].numocts);
    }
    
    RRC_S1AP_UT_TRACE_EXIT();
}
/*Bug 9050 fixes start*/
/****************************************************************************
 * Function Name  : mme_fsm_sctp_assoc_up_event_handler
 * Inputs         : p_api               : pointer to p_api
 * p_msg          : Pointer to message buffer
 * p_s1ap_gb_ctx  : pointer to S1AP global context
 * p_mme_ctx      : Pointer to mme context
 * Outputs        : None
 * Returns        : None 
 * Description    : This is for handling the MME_EVENT_SCTP_ASSOC_UP 
 *                  events for a state 
 ****************************************************************************/
void mme_fsm_sctp_assoc_up_event_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN];
    U16 asn_msg_len = RRC_NULL;
    rrc_return_et asn_response  = RRC_FAILURE;
    S32 sctp_send_response      = S1AP_SCTP_ERROR;
//#ifndef RRC_UNIT_TEST_FRAMEWORK
#ifdef LKSCTP
    S32 sctp_setup_assoc_resp   = S1AP_SCTP_ERROR;
#endif    
//#endif 
    s1ap_timer_buffer_t s1_setup_rsp_timer_buf;  
    LOCAL_S1_SETUP_ATTEMPT_STRUCT   *p_local_s1_setup_attempt = PNULL;
    
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p",
            p_api,p_msg);
    
    /* BUG 11440 Fix Start */
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::SCTP Association is UP",
            __FUNCTION__);
    /* BUG 11440 Fix Stop */
    /* SPR 13471 Fix Start */  
    if(PNULL != p_mme_ctx->mme_timer_id)
    {
        rrc_stop_timer(p_mme_ctx->mme_timer_id);
        p_mme_ctx->mme_timer_id = PNULL;
    }
//#ifndef RRC_UNIT_TEST_FRAMEWORK
#ifdef LKSCTP
#ifndef LINUX_PC_TEST
     /* Coverity_86875 Fix Start */
   sctp_setup_assoc_resp = l3_sctp_association_setup(
            p_s1ap_gb_ctx,
            p_mme_ctx,
            &(p_s1ap_gb_ctx->enb_comm_info));
    /* Coverity_86875 Fix End */

    if ( SCTP_SOCKET_ERROR == sctp_setup_assoc_resp)
    {
        handle_sctp_send_failure(p_s1ap_gb_ctx, 
                p_mme_ctx);
        return;
    }        
#endif	
#endif    
//#endif 
   /* SPR 13471 Fix Stop */
    /*Build S1 SETUP REQUEST TO send to MME*/
    /* ASN : Compose s1_setup_req IE  */
    asn_response =  compose_asn_s1_setup_req (
            &(p_s1ap_gb_ctx->s1_setup_info),
            asn_msg_buff,
            &asn_msg_len);

    if ( RRC_FAILURE == asn_response)
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]::ASN Encoding of S1 Setup Failed"
                ,__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        /* Change state to MME_IDLE */
        MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE);
        return;
    }

#ifdef LINUX_PC_TEST
	sctp_send_response = pctest_forward_mme_message_to_pc(
			RRC_S1AP_MODULE_ID,
			RRC_MME_MODULE_ID,
			MME_MESSAGE_PS_TO_TEST,
			p_mme_ctx->sctp_sd,
			p_mme_ctx->stream_info.stream_ids[SCTP_COMMON_PROC_STREAMS], //stream id
			asn_msg_buff,
			asn_msg_len);
#else
    /*Sending the  S1 SETUP REQUEST to MME */
    sctp_send_response = l3_sctp_send(
            p_mme_ctx->sctp_sd,
            asn_msg_buff,
            asn_msg_len,
            SCTP_COMMON_PROC_STREAMS );
#endif
    if ( SCTP_SOCKET_ERROR == sctp_send_response)
    {
        handle_sctp_send_failure(p_s1ap_gb_ctx, 
                p_mme_ctx);
    }                 
    else
    {
        /* Change state and wait for S1 Setup Response */
        MME_SET_FSM_STATE(p_mme_ctx, MME_W_S1_SETUP_RESP);
        RRC_S1AP_TRACE(RRC_INFO,"------Set State [MME_W_S1_SETUP_RESP]: S1 setup request is sent.------");

        p_local_s1_setup_attempt = rrc_mem_get(sizeof(LOCAL_S1_SETUP_ATTEMPT_STRUCT));
        if (PNULL != p_local_s1_setup_attempt)
        {
            memset_wrapper(p_local_s1_setup_attempt, RRC_NULL, 
                    sizeof(LOCAL_S1_SETUP_ATTEMPT_STRUCT));
            p_local_s1_setup_attempt->header.length = sizeof(LOCAL_S1_SETUP_ATTEMPT_STRUCT);
            p_local_s1_setup_attempt->header.event_id = LOCAL_S1_SETUP_ATTEMPT;

            l3_memcpy_wrapper(p_local_s1_setup_attempt->EVENT_PARAM_MME_IP_ADDRESS ,p_mme_ctx->
                    connection_info.ip_addr[0].ip_addr, (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_s1_setup_attempt);
        }

        /* Start guard timer for S1_Setup_Rsp*/
        s1_setup_rsp_timer_buf.timer_type   = S1AP_MME_S1_SETUP_RSP_TIMER ;
        s1_setup_rsp_timer_buf.auto_delete  = MME_TRUE;
        s1_setup_rsp_timer_buf.mme_id       = p_mme_ctx->mme_id;

        p_mme_ctx->mme_timer_id =  rrc_start_timer(
                p_s1ap_gb_ctx->s1_config_resp_duration , 
                &s1_setup_rsp_timer_buf,
                sizeof(s1_setup_rsp_timer_buf), 
                RRC_FALSE);
    }


    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mme_fsm_sctp_assoc_down_event_handler
 * Inputs         : p_api               : pointer to p_api
 * p_msg          : Pointer to message buffer
 * p_s1ap_gb_ctx  : pointer to S1AP global context
 * p_mme_ctx      : Pointer to mme context
 * Outputs        : None
 * Returns        : None 
 * Description    : This is for handling the MME_EVENT_SCTP_ASSOC_DOWN 
 *                  events for a state 
 ****************************************************************************/
void mme_fsm_sctp_assoc_down_event_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm p_api:%p p_msg:%p",
            p_api,p_msg);

    RRC_S1AP_TRACE(RRC_INFO,"[%s]::SCTP Association is Down",
            __FUNCTION__);

    /* SPR 13471 Fix Start */
    if(PNULL != p_mme_ctx->mme_timer_id)
    {
        rrc_stop_timer(p_mme_ctx->mme_timer_id);
        p_mme_ctx->mme_timer_id = PNULL;
    }
    /* SPR 13471 Fix Stop */

    handle_sctp_send_failure(p_s1ap_gb_ctx, 
            p_mme_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
}
/*Bug 9050 fixes end*/
