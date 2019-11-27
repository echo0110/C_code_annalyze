/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_mme_fsm.h,v 1.5 2010/10/13 07:04:08 gur18569 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file comtains the function prototypes for S1AP 
                       MME Module FSM
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_mme_fsm.h,v $
 * Revision 1.5  2010/10/13 07:04:08  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.1  2010/08/05 08:10:44  gur15697
 * merging round 2
 *
 * Revision 1.4  2010/07/24 07:18:48  gur04733
 * Checking On 24 July
 *
 * Revision 1.3.2.2  2010/07/01 05:29:19  gur10248
 * retrive old version before s1ap asn
 *
 * Revision 1.3.2.1  2010/06/30 07:28:16  gur10248
 * new s1ap folder
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.7  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.6  2009/12/27 08:32:51  gur18569
 * removed tabs
 *
 * Revision 1.1.2.5  2009/12/22 14:38:53  gur18569
 * fixed 154
 *
 * Revision 1.1.2.4  2009/12/11 04:47:12  gur21006
 * Added support for sctp shutdown event
 *
 * Revision 1.1.2.3  2009/12/04 11:51:38  gur21006
 * New functions added
 *
 * Revision 1.1.2.2  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.6  2009/11/23 07:27:12  gur21006
 * New function added : delete_sctp_sd_from_tree
 *
 * Revision 1.1.2.5  2009/11/21 12:18:35  gur21006
 * FSM changed
 *
 * Revision 1.1.2.4  2009/11/17 10:20:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.1.2.3  2009/10/29 14:48:53  gur20052
 * s1ap_mme_fsm.h
 *
 * Revision 1.1.2.2  2009/10/23 16:11:39  gur18569
 * Initial version
 *
 * Revision 1.1.2.1  2009/10/14 13:52:41  gur20052
 * Added initial version
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#ifndef INCLUDED_MME_FSM_H
#define INCLUDED_MME_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "s1ap_global_ctx.h"
#include "sctp_wrappers.h"
#include "s1ap_timers.h"
#include "rrc_common_utils.h"
#include "rrc_s1apOam_intf.h"
#include "s1ap_utils.h"
#include "rrc_s1ap_uecc_intf.h"
#include "s1ap_asn_codec.h"
#include "s1ap_api.h"
#include "s1ap_oam.h"



/******************************************************************************
  Private Definitions
 *****************************************************************************/

#define MME_GET_FSM_STATE(gb_mme_ctx) gb_mme_ctx->current_mme_state

#define MME_SET_FSM_STATE(gb_mme_ctx,new_state) \
    gb_mme_ctx->current_mme_state=new_state

#define MME_FSM_EVENT_TRACE(state, event) \
    RRC_S1AP_TRACE(RRC_BRIEF, "[STATE:%s] [EVENT:%s]", \
            MME_FSM_STATES_NAMES[(state)], \
            MME_FSM_EVENTS_NAMES[(event)])

/******************************************************************************
  Private Types
 *****************************************************************************/


typedef enum {
    S1AP_ZERO,
    S1AP_ONE }s1ap_bool_t ;
#define s1ap_conf_t rrc_bool_et
#define S1AP_FALSE RRC_FALSE
#define S1AP_TRUE RRC_TRUE
typedef enum
{
    MME_IDLE,                /*This is the initial state of MME FSM*/
    MME_W_S1_SETUP_RESP,     /* Waiting for S1 Setup RSP request */
    MME_ACTIVE,              /* S1AP association between eNB and MME is active*/
    MME_W_SCTP_CONN_RECOVERY,/*After detecting connection failure a timer is 
                               started to wait for SCTP connection recovery.*/
    MME_W_CONNECT_SUCCESSFUL,/*Wait for connection MME after connection failure*/
    MME_W_TIME_TO_WAIT_TIMER_EXPIRY,/*In this state we wait for expiry of back 
                                      off timer which is started upon reception 
                                      of S1 SETUP FAILURE with IE timetowait value*/
/*SPR 15570 Fix Start*/
     MME_W_SCTP_CONNECT,   /* Wait for sctp connection after receiving error code EINPROGRESS*/
/*SPR 15570 Fix Stop*/
    MME_NUM_OF_STATES,       /* Number of states */
    MME_INVALID_STATE        /* Invalid State */
}mme_state_et;

typedef enum
{
    MME_EVENT_S1_MME_PROV_REQ,      /* S1AP has received PROV request from OAM*/
    MME_EVENT_S1_SETUP_RSP_TIMER_EXPIRY,/* This event is triggered on expiry of timer
                                           started when S1 SETUP REQUEST is sent*/
    MME_EVENT_S1_SETUP_RSP,        /* Received S1 SETUP RESPONSE from MME  */
    MME_EVENT_S1_SETUP_FAILURE,     /* Received S1 SETUP FAILURE from MME   */
    MME_EVENT_SCTP_CONN_FAILURE_IND,/* failure of SCTP association */
    MME_EVENT_SCTP_CONN_RECOVERY_IND,/*Recovery of SCTP assoc after failure.*/
    MME_EVENT_SCTP_CONN_REC_TIMER_EXPIRY,/*Expiry of guard timer that was started 
                                           after detection of SCTP assoc failure*/
    MME_EVENT_SCTP_CONNECT_TIMER_EXPIRY,/*Expiry of timer started upon failure 
                                          of sctp connect has expired. */
    MME_EVENT_S1_SETUP_TIME_TO_WAIT_TIMER_EXPIRY,/*Expiry of guard timer started 
                                                   upon reception of S1 SET UP FAILURE 
                                                   message which had IE time to wait has*/
    MME_EVENT_SCTP_SHUTDOWN,
    MME_EVENT_CLOSE_MME_CONNECTION,   /* Message received from OAM to close
                                      * the SCTP Connection */
/*Bug 9050 fixes start*/
    MME_EVENT_SCTP_ASSOC_UP,       /* SCTP association is UP */
    MME_EVENT_SCTP_ASSOC_DOWN,       /* SCTP association is DOWN */
/*Bug 9050 fixes end*/
    MME_NUM_OF_EVENTS,               /* Number of Events */
    MME_INVALID_EVENT                /* Invalid Event */
}mme_event_et;

typedef void (*mme_fsm_ret)(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

/* Values for mme_return_et, mme_return_t */
typedef enum {
    MME_FAILURE,
    MME_SUCCESS
} mme_return_et;

/* Values for rrc_bool_et, rrc_bool_t */
#define mme_bool_et rrc_bool_et
#define MME_FALSE RRC_FALSE
#define MME_TRUE RRC_TRUE

/*****************************************************************************
  Private Function Prototypes
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/


/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

void mme_fsm_process_event(
        mme_event_et     event_id,
        void              *param1,
        void              *param2,
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx);


void mme_fsm_invalid_event_handler(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

void  mme_fsm_mme_init_req(
        void                * p_api,
        void                * p_msg,
        s1ap_gb_context_t   * p_s1ap_gb_ctx,
        mme_context_t       * p_mme_ctx);

void  mme_fsm_s1_setup_rsp_timer_expiry (
        void                * timer_id,
        void                * p_timer_buf,
        s1ap_gb_context_t   * p_s1ap_gb_ctx,
        mme_context_t       * p_mme_ctx);

void  mme_fsm_s1_setup_resp(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

void mme_fsm_s1_setup_failure(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

void  mme_fsm_sctp_conn_failure_ind(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

void  mme_fsm_sctp_conn_rec_ind(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

void  mme_fsm_sctp_conn_rec_timer_expiry(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

void  mme_fsm_sctp_connect_timer_expiry(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

/*SPR 15570 Fix Start*/
void  mme_fsm_w_sctp_connect_event_handler(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

/*SPR 15570 Fix Stop*/

void  mme_fsm_back_off_timer_expiry(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);


void s1ap_process_mme_initiated_reset_proc( 
        void* p_msg , 
        mme_context_t *p_mme_ctx);


void active_mme_list_delete(
        s1ap_gb_context_t   * p_s1ap_gb_ctx,
        U16      mme_index);

void active_mme_list_insert(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        U8  mme_index);

void  mme_fsm_sctp_re_conn_try_failure_ind(
        void                * p_api,
        void                * p_msg,
        s1ap_gb_context_t   * p_s1ap_gb_ctx,
        mme_context_t       * p_mme_ctx);

void  mme_fsm_w_s1_resp_conn_failure(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void  mme_fsm_shutdown_event_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void  mme_fsm_mme_active_shutdown_event_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void mme_fsm_oam_mme_close_conn_handler(
        void                * p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void delete_sctp_sd_from_tree(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_ctx);


S8 s1ap_mme_msg_send_to(
        sctp_sd_t ,
        U8 stream_id, 
        U8 *p_asn_msg,
        U8 asn_msg_len);

U8 get_stream_id(mme_context_t* p_mme_ctx);

void free_served_gummeis( served_gummei_info_t * p_served_gummeis, U8
                                   num_served_gummeis);

void process_reset_all(s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t     *p_mme_ctx);

void build_and_send_mme_status_msg_ind(
        mme_context_t           *p_mme_context,
        rrc_bool_et      mme_connection_status,
        //Trace Change Start
        s1ap_gb_context_t     * p_s1ap_gb_ctx);
        //Trace Change End

void s1ap_fill_served_gummei_info(
        mme_context_t*    p_mme_context, 
        s1ap_served_gummei_info_t*    p_gummei);

/*Bug 9050 fixes start*/
void  mme_fsm_sctp_assoc_up_event_handler(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);

void  mme_fsm_sctp_assoc_down_event_handler(
        void   *,
        void   * ,
        s1ap_gb_context_t* ,
        mme_context_t*);
/*Bug 9050 fixes end*/

#endif /* INCLUDED_MME_FSM_H */
/* s1ap_mme_fsm.h */

