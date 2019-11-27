/*******************************************************************************
 *
 *  FILE NAME   : x2ap_sm_entry.h
 *
 *  DESCRIPTION : This file function declaration of diffrent event handler.
 *
 *  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
 *
 *  Copyright 2007, Aricent Inc.
 *
 *******************************************************************************/
#ifndef _SM_ENTRY_H_
#define _SM_ENTRY_H_

#include "commoninc.h"
#include "x2ap_db.h"

x2ap_return_et
x2ap_enbconf_sm_entry
(
 U16                api,
 U8                 *p_msg_ptr,
 x2ap_gb_context_t  *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t *p_enb_cntxt_arr
 );



x2ap_return_et
x2ap_sctp_sm_entry
(
 U16                api,
 U8                 *p_msg_ptr,
 x2ap_gb_context_t *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t *p_enb_cntxt_arr);
x2ap_return_et
x2setup_sm_entry
(
 U16                api,
 U8                 *p_msg_ptr,
 x2ap_gb_context_t *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t *p_enb_cntxt_arr);

#endif

