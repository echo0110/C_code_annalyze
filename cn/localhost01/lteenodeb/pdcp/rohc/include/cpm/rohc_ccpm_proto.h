/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccpm_proto.h,v 1.1.1.1.16.1 2010/11/03 06:28:22 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for
 *                     initialization and entry point for C-CPM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ccpm_proto.h,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:28:22  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.2.36.2  2010/10/13 04:08:39  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/09/23 06:54:06  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CCPM_PROTO_
#define _ROHC_CCPM_PROTO_

#include "rohc_ccpm_ext_if.h"
#include "rohc_ccpm_u_done.h"
#include "rohc_ccpm_u_pending.h"
#include "rohc_ccpm_o_done.h"
#include "rohc_ccpm_o_pending.h"
#include "rohc_ccpm_r_done.h"
#include "rohc_ccpm_r_pending.h"
#include "rohc_ccpm_p2_u_done.h"
#include "rohc_ccpm_p2_u_pending.h"
#include "rohc_ccpm_p2_o_done.h"
#include "rohc_ccpm_p2_o_pending.h"
#include "rohc_ccpm_p2_r_done.h"
#include "rohc_ccpm_p2_r_pending.h"
#include "rohc_ccpm_p1_u_done.h"
#include "rohc_ccpm_p1_u_pending.h"
#include "rohc_ccpm_p1_o_done.h"
#include "rohc_ccpm_p1_o_pending.h"
#include "rohc_ccpm_p1_r_done.h"
#include "rohc_ccpm_p1_r_pending.h"
#include "rohc_ccpm_p6_u_done.h"
#include "rohc_ccpm_event.h"
#include "rohc_ccom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_def.h"

typedef rohc_void_t (*rohc_ccpm_fprtable)
                    (
                        rohc_entity_t    *p_entity_t,
                        /* + SPR 17439 */
                        rohc_csc_t	     *p_csc
                        /* - SPR 17439 */
                      );
typedef rohc_void_t (*rohc_ccpm_fprtable_fpt_rtp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_csc_rtp_t	     *p_csc
                      );
typedef rohc_void_t (*rohc_ccpm_fprtable_fpt_udp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_csc_udp_t	     *p_csc
                      );
typedef rohc_void_t (*rohc_ccpm_fprtable_fpt_tcp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_csc_tcp_t	     *p_csc
                      );

/******************************************************************************
 *
 *
 * Function Poniter table for CPM Events.
 *
 *
 *****************************************************************************/

rohc_void_t rohc_ccpm_init_evt_tbl ROHC_PROTO(ROHC_NO_PARAMS());
#endif /*_ROHC_CCPM_PROTO_*/

