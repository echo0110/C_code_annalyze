/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_cprm_ext_if.h,v 1.2.36.3 2010/11/09 04:53:23 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains functions' declaration of C-PRM for
 *                     external modules.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_cprm_ext_if.h,v $
 * Revision 1.2.36.3  2010/11/09 04:53:23  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/13 04:33:22  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/09/23 07:03:10  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CPRM_EXT_IF_H_
#define _ROHC_CPRM_EXT_IF_H_

#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_ed_proto.h"

#define ROHC_CONT_UOR2  253 
#define ROHC_CONT_R0CRC 254
/* SPR 9591 fix start */
#define ROHC_FB_CODE_MASK       0x07
/* SPR 9591 fix end */
/*
typedef struct rohc_encoded_value_t 
{
    rohc_U32bit_t value;
    rohc_U8bit_t  bits; 
}rohc_encoded_value_t;

*/
/* + SPR 17439 */
rohc_return_t rohc_cprm_p0_resolve_pkt_typ
    ROHC_PROTO((rohc_entity_t       *p_entity   ,
                rohc_csc_uncomp_t   *p_csc      ,
                rohc_U8bit_t        *p_pkt_typ  ,
                rohc_error_code_t   *p_ecode));

rohc_return_t rohc_cprm_p1_resolve_pkt_typ
    ROHC_PROTO((rohc_entity_t       *p_entity   ,
                rohc_csc_rtp_t      *p_csc      ,
                rohc_U8bit_t        *p_pkt_typ  ,
                rohc_error_code_t   *p_ecode));

rohc_return_t rohc_cprm_p2_resolve_pkt_typ
    ROHC_PROTO((rohc_entity_t       *p_entity   ,
                rohc_csc_udp_t      *p_csc      ,
                rohc_U8bit_t        *p_pkt_typ  ,
                rohc_error_code_t   *p_ecode));

rohc_return_t rohc_cprm_p6_resolve_pkt_typ
    ROHC_PROTO((rohc_entity_t       *p_entity   ,
                rohc_csc_tcp_t      *p_csc      ,
                rohc_U8bit_t        *p_pkt_typ  ,
                rohc_error_code_t   *p_ecode));

rohc_return_t rohc_cprm_p1_process_feedback
        ROHC_PROTO((rohc_entity_t       * p_entity      ,
                    rohc_csc_common_t      *p_csc_ctx          ,
                    rohc_U8bit_t         *p_fp_buffer    ,
                    rohc_U32bit_t        len             ,
                    rohc_error_code_t   *p_ecode
                    ));
rohc_return_t rohc_cprm_p2_process_feedback
        ROHC_PROTO((rohc_entity_t       * p_entity    ,
                    rohc_csc_common_t      *p_csc_ctx        ,
                    rohc_U8bit_t         *p_fp_buffer  ,
                    rohc_U32bit_t        len           ,
                    rohc_error_code_t   *p_ecode
                    ));
                    
rohc_return_t rohc_cprm_p6_process_feedback
        ROHC_PROTO((rohc_entity_t       * p_entity      ,
                    rohc_csc_common_t      *p_csc_ctx          ,
                    rohc_U8bit_t         *p_fp_buffer    ,
                    rohc_U32bit_t        len             ,
                    rohc_error_code_t   *p_ecode
                    ));
rohc_void_t rohc_cprm_init ROHC_PROTO(ROHC_NO_PARAMS());                

/* SPR 9591 fix start */
rohc_return_t rohc_cprm_p0_process_feedback
        ROHC_PROTO((rohc_entity_t       * p_entity      ,
                    rohc_csc_common_t      *p_csc_ctx          ,
                    rohc_U8bit_t         *p_fp_buffer    ,
                    rohc_U32bit_t        len             ,
                    rohc_error_code_t   *p_ecode
                    ));
/* - SPR 17439 */
/* SPR 9591 fix end */
#endif /* _ROHC_CPRM_EXT_IF_H_ */
