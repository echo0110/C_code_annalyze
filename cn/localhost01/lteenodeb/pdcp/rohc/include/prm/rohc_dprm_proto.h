/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dprm_proto.h,v 1.2.36.4 2010/12/02 08:57:40 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for internal
 *                     interface D-PRM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dprm_proto.h,v $
 * Revision 1.2.36.4  2010/12/02 08:57:40  gur22059
 * Conflicting profile 1 functions were renamed acc to new Naming Convention
 *
 * Revision 1.2.36.3  2010/11/09 04:54:10  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/13 04:35:43  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/10/04 03:27:54  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DPRM_PROTO_H_
#define _ROHC_DPRM_PROTO_H_

#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_d_typ.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_sdvl.h"
#include "rohc_protocol_header.h"
#include "rohc_trace_mcr.h"
#include "rohc_dprm_ext_if.h"
#include "rohc_dprm_update_p1.h"
#include "rohc_dprm_update_p23.h"
#include "rohc_dprm_update_p6.h"
#include "rohc_dcpm_event.h"
#include "rohc_dcom_mcr.h"
#include "rohc_dcpm_ext_if.h"
#include "rohc_ed_ext_if.h"
#include "rohc_crc_ext_if.h"
#include "rohc_led_ext_if.h"

#define ROHC_BASE_PKT               0
#define ROHC_NONE_UPDATING_PKT      ROHC_BASE_PKT+0
#define ROHC_NOT_SUPPORTED_PKT      ROHC_BASE_PKT+1
#define ROHC_BASE_UPDATE_PKT        ROHC_BASE_PKT+2
#define ROHC_BASE_EXT3_UPDATE_PKT   ROHC_BASE_PKT+3
#define ROHC_IR_IRDYN_UPDATE_PKT    ROHC_BASE_PKT+4
#define ROHC_IRREGULAR_UPDATE_PKT   ROHC_BASE_PKT+5
#define ROHC_CR_BASE_UPDATE_PKT     ROHC_BASE_PKT+6
#define ROHC_MAX_PKT                ROHC_BASE_PKT+7

typedef rohc_void_t (*rohc_dprm_fprtable_rtp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_dsc_rtp_t      *p_dsc ,
                        rohc_dprm_update_blk_t *p_dprm_update_blk
                      );
typedef rohc_void_t (*rohc_dprm_fprtable_udp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_dsc_udp_t      *p_dsc ,
                        rohc_dprm_update_blk_t *p_dprm_update_blk
                    );
typedef rohc_void_t (*rohc_dprm_fprtable_tcp)
                    (
                        rohc_entity_t    *p_entity_t,
                        rohc_dsc_tcp_t      *p_dsc ,
                        rohc_dprm_update_blk_t *p_dprm_update_blk
                    );
/******************************************************************************
 *
 *
 * Function Poniter table for prm udpate functions.
 *
 *
 *****************************************************************************/


extern rohc_dprm_fprtable_rtp rohc_dprm_p1_type_fpt[ROHC_MAX_PKT]; 

extern rohc_dprm_fprtable_udp rohc_dprm_p2_fpt[ROHC_MAX_PKT]; 

extern rohc_dprm_fprtable_tcp rohcDprmP6FptG[ROHC_MAX_PKT]; 

extern rohc_U8bit_t rohc_dprm_p1_pkt_cat[ROHC_MAX_PKT_TYPES];

extern rohc_U8bit_t rohc_dprm_p2_pkt_cat[ROHC_MAX_PKT_TYPES];

extern rohc_U8bit_t rohc_dprm_p23_pkt_cat[ROHC_MAX_PKT_TYPES];

extern rohc_U8bit_t rohc_dprm_p6_pkt_cat_g[ROHC_MAX_PKT_TYPES];

extern rohc_U8bit_t rohc_dprm_pkt_crc_bit[ROHC_MAX_PKT_TYPES];

rohc_void_t rohc_dprm_init_fpt ROHC_PROTO(ROHC_NO_PARAMS());
 
/* + SPR 17439 */
rohc_U8bit_t rohc_dprm_p1_is_updation_req ROHC_PROTO((
                                        rohc_entity_t       * p_entity  ,
                                        rohc_dsc_rtp_t      *p_dsc      ,
                                        rohc_U8bit_t        *p_in_buff  ,
                                        rohc_U32bit_t       *p_len      ,
                                        rohc_error_code_t   *p_ecode
                                        ) );

rohc_U8bit_t rohc_dprm_p2_is_updation_req ROHC_PROTO((
                                        rohc_entity_t     * p_entity ,
                                        rohc_dsc_udp_t    *p_dsc     ,
                                        rohc_U8bit_t      *p_in_buff ,
                                        rohc_U32bit_t     *p_len     ,
                                        rohc_error_code_t *p_ecode
                                        ) );

rohc_U8bit_t rohcDprmP6IsUpdationReq(
                                        rohc_entity_t     * p_entity ,
                                        rohc_dsc_tcp_t    *p_dsc     ,
                                        rohc_U8bit_t      *p_in_buff ,
                                        rohc_U32bit_t     *p_len     ,
                                        rohc_error_code_t *p_ecode
                                        );

rohc_void_t rohc_dprm_p1_none_updating_pkt
        ROHC_PROTO((rohc_entity_t           *p_entity ,
                    rohc_dsc_rtp_t          *p_dsc    ,
                    rohc_dprm_update_blk_t  *p_dprm_update_blk
                    ));

rohc_void_t rohc_dprm_p2_none_updating_pkt
        ROHC_PROTO((rohc_entity_t           *p_entity ,
                    rohc_dsc_udp_t          *p_dsc    ,
                    rohc_dprm_update_blk_t  *p_dprm_update_blk
                    ));

rohc_void_t rohc_dprm_p6_none_updating_pkt
        ROHC_PROTO((rohc_entity_t           *p_entity ,
                    rohc_dsc_tcp_t          *p_dsc    ,
                    rohc_dprm_update_blk_t  *p_dprm_update_blk
                    ));

rohc_void_t rohc_dprm_p1_not_supported_pkt
        ROHC_PROTO((rohc_entity_t           *p_entity ,
                    rohc_dsc_rtp_t          *p_dsc,
                    rohc_dprm_update_blk_t  *p_dprm_update_blk
                    ));
                    
rohc_void_t rohc_dprm_p2_not_supported_pkt
        ROHC_PROTO((rohc_entity_t           *p_entity ,
                    rohc_dsc_udp_t          *p_dsc,
                    rohc_dprm_update_blk_t  *p_dprm_update_blk
                    ));
                    
rohc_void_t rohc_dprm_p6_not_supported_pkt
        ROHC_PROTO((rohc_entity_t           *p_entity ,
                    rohc_dsc_tcp_t          *p_dsc,
                    rohc_dprm_update_blk_t  *p_dprm_update_blk
                    ));
                    
rohc_return_t rohc_dprm_p2_decode
         ROHC_PROTO((
                rohc_entity_t     *p_entity ,
                rohc_dsc_udp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode
				));

rohc_return_t rohcDprmP6Decode
                (
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode
				);

rohc_return_t rohc_dprm_p1_type_decode
         ROHC_PROTO((
                rohc_entity_t       *p_entity   ,
                rohc_dsc_rtp_t      *p_dsc      ,
                rohc_error_code_t   *p_ecode
				));
/* + SPR 17439 */
#endif /* _ROHC_DPRM_INT_IF_H_ */
