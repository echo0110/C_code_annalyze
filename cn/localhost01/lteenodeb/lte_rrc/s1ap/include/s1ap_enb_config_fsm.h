/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_enb_config_fsm.h,v 1.5 2010/11/26 07:04:08 gur25381 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file comtains the function prototypes for S1AP 
                       eNB Configuration update FSM
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_enb_config_fsm.h,v $
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#ifndef INCLUDED_ENB_CONFIG_FSM_H
#define INCLUDED_ENB_CONFIG_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "s1ap_global_ctx.h"
#include "sctp_wrappers.h"
#include "s1ap_timers.h"
#include "rrc_common_utils.h"
#include "rrc_s1apOam_intf.h"
#include "rrc_s1ap_uecc_intf.h"
#include "s1ap_api.h"
#include "s1ap_oam.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define ENB_CONFIG_GET_FSM_STATE(gb_mme_ctx) \
    gb_mme_ctx->current_enb_config_update_state

#define ENB_CONFIG_SET_FSM_STATE(gb_mme_ctx,new_state) \
    gb_mme_ctx->current_enb_config_update_state=new_state

#define ENB_CONFIG_FSM_EVENT_TRACE(state, event) \
    RRC_S1AP_TRACE(RRC_BRIEF, "[STATE:%s] [EVENT:%s]", \
            ENB_CONFIG_UPDATE_FSM_STATES_NAMES[(state)], \
            ENB_CONFIG_UPDATE_FSM_EVENTS_NAMES[(event)])

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * ENB CONFIGURATION UPDATE - START
 ****************************************************************************/
typedef enum 
{
    ENB_STATE_RECV_ENB_CONFIG_FROM_OAM, /* S1AP has recived update request from OAM */
    ENB_STATE_W_FOR_ENB_CONFIG_RESP, /* Waiting the response from MME */
    ENB_STATE_ENB_CONFIG_FAILURE, /* S1AP has received failure message from MME */
    ENB_STATE_ABNORMAL_ENB_CONFIG, /* S1AP has not received any response from MME */
    ENB_STATE_ENB_CONFIG_COMPLETE, /* S1AP has received the ACK from MME */
    ENB_STATE_MAX_ENB_CONFIG_VAL, /* Invalid Value*/
    ENB_STATE_ENB_CONFIG_INVALID
} enb_configuration_update_state_et;

typedef enum {
    S1AP_MME_EV_ENB_CONFIG_SENT, /* Event when operator updates the TA list */
    S1AP_MME_EV_ENB_CONFIG_ACK_RECV, /* Event enb Config update is sent to MME */
    S1AP_MME_EV_TIMER_EXPIRY, /* Timer expiry waiting for the response from MME */
    S1AP_MME_EV_ENB_CONFIG_NACK, /* S1AP has received NACK from MME */
    S1AP_MME_EV_ENB_CONFIG_RESEND, /* S1AP Sends the eNB Config Update again */
    S1AP_MME_EV_MAX_RETRIES_OVER, /* Max Retires are over */
    S1AP_OAM_EV_ENB_CONFIG_UPDATE, /* OAM Sends the config update to be sent to MME */
    S1AP_MME_EV_MAX_EVENT_VAL, 
    S1AP_MME_EV_INVALID
} enb_configuration_update_event_et;

typedef void (*enb_config_update_fsm_ret)(
        void *,
        void *,
        s1ap_gb_context_t *,
        mme_context_t *);

/*****************************************************************************
* ENB CONFIGURATION UPDATE - END
****************************************************************************/

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
void enb_fsm_config_update_req_handler(
        void                * p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void enb_fsm_invalid_event_handler(
        void                * p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void enb_fsm_config_update_complete_handler(
        void                * p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void enb_fsm_config_update_nack_handler(
        void                * p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void enb_fsm_config_update_timer_expiry_handler (
        void             *p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

void enb_fsm_config_update_process_event(
        enb_configuration_update_event_et event_id,
        void                *param1,
        void                *param2,
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx);
void enb_fsm_config_update_time_to_wait_timer_expiry_handler(
        void                * p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx);

#endif /* INCLUDED_MME_FSM_H */
/* s1ap_enb_config_fsm.h */


