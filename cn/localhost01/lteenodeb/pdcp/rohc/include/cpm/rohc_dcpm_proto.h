/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_proto.h,v 1.2.36.2 2010/11/09 04:45:37 gur22059 Exp $
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
 * $Log: rohc_dcpm_proto.h,v $
 * Revision 1.2.36.2  2010/11/09 04:45:37  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.1  2010/10/04 03:35:23  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DCPM_PROTO_
#define _ROHC_DCPM_PROTO_

#include "rohc_dcpm_ext_if.h"
#include "rohc_dcpm_u_done.h"
#include "rohc_dcpm_u_pending.h"
#include "rohc_dcpm_u_initiated.h"
#include "rohc_dcpm_o_done.h"
#include "rohc_dcpm_o_pending.h"
#include "rohc_dcpm_o_initiated.h"
#include "rohc_dcpm_r_done.h"
#include "rohc_dcpm_r_pending.h"
#include "rohc_dcpm_r_initiated.h"
#include "rohc_dcpm_p1_o_done.h"
#include "rohc_dcpm_p1_o_initiated.h"
#include "rohc_dcpm_p1_o_pending.h"
#include "rohc_dcpm_p1_r_done.h"
#include "rohc_dcpm_p1_r_initiated.h"
#include "rohc_dcpm_p1_r_pending.h"
#include "rohc_dcpm_p1_u_done.h"
#include "rohc_dcpm_p1_u_initiated.h"
#include "rohc_dcpm_p1_u_pending.h"
#include "rohc_dcpm_p2_o_done.h"
#include "rohc_dcpm_p2_o_initiated.h"
#include "rohc_dcpm_p2_o_pending.h"
#include "rohc_dcpm_p2_r_done.h"
#include "rohc_dcpm_p2_r_initiated.h"
#include "rohc_dcpm_p2_r_pending.h"
#include "rohc_dcpm_p2_u_done.h"
#include "rohc_dcpm_p2_u_initiated.h"
#include "rohc_dcpm_p2_u_pending.h"
#include "rohc_dcpm_event.h"
#include "rohc_trace_mcr.h"
#include "rohc_dcom_mcr.h"
#include "rohc_def.h"
#include "rohc_dcpm_p6_r_done.h"

typedef rohc_void_t (*rohc_dcpm_fprtable)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_dsc_t      *p_dsc
                      );
typedef rohc_void_t (*rohc_dcpm_fprtable_rtp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_dsc_rtp_t      *p_dsc
                      );
typedef rohc_void_t (*rohc_dcpm_fprtable_udp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_dsc_udp_t      *p_dsc
                      );
typedef rohc_void_t (*rohc_dcpm_fprtable_tcp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_dsc_tcp_t      *p_dsc
                      );

/******************************************************************************
 *
 *
 * Function Poniter table for CPM Events.
 *
 *
 *****************************************************************************/

extern rohc_dcpm_fprtable rohc_dppu_fpt[ROHC_MODE_R - ROHC_MODE_U + 1]
    	[ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 ]
    	[ROHC_D_R_MAX_ET - ROHC_D_U_START + 1]
    	[ROHC_D_ST_FC - ROHC_D_ST_NC+ 1]
        [ROHC_ALL_PROFILE - ROHC_UNCOMP]; 
    
extern rohc_U8bit_t rohc_dcpm_map_tbl[ROHC_MAX_PKT_TYPES][3];  
  
rohc_void_t rohc_dcpm_in_evt_tbl ROHC_PROTO(ROHC_NO_PARAMS());


/* + SPR 17439 */
rohc_void_t rohc_dcpm_send_ack
			ROHC_PROTO(( 
			rohc_entity_t *p_entity , 
			rohc_dsc_t *p_dsc ,
			rohc_U8bit_t crc_flag));

rohc_void_t rohc_dcpm_p1_send_ack
			ROHC_PROTO(( 
			rohc_entity_t   *p_entity   , 
			rohc_dsc_rtp_t  *p_dsc      ,
			rohc_U8bit_t    crc_flag));

rohc_void_t rohc_dcpm_p2_send_ack
			ROHC_PROTO(( 
			rohc_entity_t  *p_entity , 
			rohc_dsc_udp_t *p_dsc    ,
			rohc_U8bit_t   crc_flag));

rohc_void_t rohcDcpmP6SendAck( 
			rohc_entity_t  *p_entity ,
			rohc_dsc_tcp_t *p_dsc   
);
            
rohc_void_t rohc_dcpm_send_nack 
			ROHC_PROTO( (
			rohc_entity_t *p_entity , 
			rohc_dsc_t *p_dsc ,
			rohc_U8bit_t crc_flag));
			
rohc_void_t rohc_dcpm_p1_send_nack 
			ROHC_PROTO( (
			rohc_entity_t   *p_entity   , 
			rohc_dsc_rtp_t  *p_dsc      ,
			rohc_U8bit_t    crc_flag));
			
rohc_void_t rohc_dcpm_p2_send_nack 
			ROHC_PROTO( (
			rohc_entity_t  *p_entity , 
			rohc_dsc_udp_t *p_dsc    ,
			rohc_U8bit_t   crc_flag));
			
rohc_void_t rohcDcpmP6SendNack(
			rohc_entity_t  *p_entity , 
			rohc_dsc_tcp_t *p_dsc   
);

rohc_void_t rohc_dcpm_send_static_nack 
			ROHC_PROTO( (
			rohc_entity_t *p_entity , 
			rohc_dsc_t *p_dsc ,
			rohc_U8bit_t crc_flag));

rohc_void_t rohc_dcpm_p1_send_static_nack 
			ROHC_PROTO( (
			rohc_entity_t   *p_entity   , 
			rohc_dsc_rtp_t  *p_dsc      ,
			rohc_U8bit_t    crc_flag));

rohc_void_t rohc_dcpm_p2_send_static_nack 
			ROHC_PROTO( (
			rohc_entity_t  *p_entity , 
			rohc_dsc_udp_t *p_dsc    ,
			rohc_U8bit_t   crc_flag));

rohc_void_t rohcDcpmP6SendStaticNack(
			rohc_entity_t  *p_entity , 
			rohc_dsc_tcp_t *p_dsc    
);
            
/* - SPR 17439 */
#endif /*_ROHC_DCPM_PROTO_*/

