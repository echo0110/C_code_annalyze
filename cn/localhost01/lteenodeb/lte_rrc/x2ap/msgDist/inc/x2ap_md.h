/*******************************************************************************
 *
 *  FILE NAME   : x2ap_msg_dist.h
 *
 *  DESCRIPTION : This file contains the structure and function declaration 
 *                required for x2ap message distributor.
 *
 *  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
 *
 *  Copyright 2007, Aricent Inc.
 *
 *******************************************************************************/
#ifndef _X2AP_MSG_DIST_H_
#define _X2AP_MSG_DIST_H_

#include "commoninc.h"
#include "x2ap_timer.h"
#include "x2ap_msg_framework.h"
//#include "x2ap_db.h"
#include "x2setup_sm.h"
S32 x2ap_process_msg(
        void *p_api,        /* Pointer to input API buffer */
        void *p_gl_ctx);    /* Pointer to the X2AP global context data */

void x2ap_process_timer_msg(
        x2ap_timer_t timer_id,       /* Identity of the expired timer */
        void        *p_timer_buf,   /* Pointer to data associated with the timer */
        void        *p_gl_ctxi);    /* Pointer to the X2AP global context data */

void x2ap_process_oam_messages(
        /* Pointer to input API buffer */
        void            *p_api,

        /* Pointer to the x2AP global context data */
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
void x2ap_process_init_timer(void);


void x2ap_process_csc_messages(
        /* Pointer to input API buffer */
        void            *p_api,

        /* Pointer to the x2AP global context data */
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

void x2ap_process_rrm_messages(
        /* Pointer to input API buffer */
        void            *p_api,         

        /* Pointer to the x2AP global context data */
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
/*x2ap-start*/
void x2ap_process_uecc_messages(
        /* Pointer to input API buffer */
        void            *p_api,         

        /* Pointer to the x2AP global context data */
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
/*x2ap-end*/
/*SPR_16533_START*/
#ifdef LKSCTP
void    x2ap_create_enb_contx_for_new_enb_in_sd_tree(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_cntxt,
        sctp_sd_t          sctp_sd);
x2ap_peer_enb_context_t*   x2ap_create_enb_contx_for_new_enb_in_ip_tree(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2_enb_comm_info_t *p_enb_comm_info,
        sctp_sd_t          sctp_sd);
#endif
/*SPR_16533_END*/
/*SPR_17153_START*/
void x2ap_delete_assoc_id(S32 sctp_sd);
/*SPR_17153_END*/
#endif

