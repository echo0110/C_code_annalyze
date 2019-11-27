/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dprm_ext_if.h,v 1.2.36.3 2010/11/09 04:53:54 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains functions' prototypes of C-PRM for
 *                     external modules.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dprm_ext_if.h,v $
 * Revision 1.2.36.3  2010/11/09 04:53:54  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/13 04:34:41  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/10/04 03:34:37  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DPRM_EXT_IF_H_
#define _ROHC_DPRM_EXT_IF_H_

#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_d_typ.h"


                    
rohc_void_t rohc_dprm_init 
                ROHC_PROTO(ROHC_NO_PARAMS());               


/* + SPR 17439 */
rohc_return_t rohc_dprm_p1_exec  
                ROHC_PROTO((
                rohc_entity_t       * p_entity  ,
                rohc_dsc_rtp_t      *p_dsc      , 
                rohc_U8bit_t        *p_in_buff  ,
                rohc_U32bit_t       *p_len      ,
                rohc_error_code_t   *p_ecode));

rohc_return_t rohc_dprm_p2_exec  
                ROHC_PROTO((
                rohc_entity_t     * p_entity ,
                rohc_dsc_udp_t    *p_dsc     , 
                rohc_U8bit_t      *p_in_buff ,
                rohc_U32bit_t     *p_len     ,
                rohc_error_code_t *p_ecode));

/* - SPR 17439 */
rohc_return_t rohcDprmP6Exec  (
                rohc_entity_t     * p_entity ,
                rohc_dsc_tcp_t    *p_dsc     , 
                rohc_U8bit_t      *p_in_buff ,
                rohc_U32bit_t     *p_len     ,
                rohc_error_code_t *p_ecode);
/* + SPR 17439 */

rohc_return_t rohc_exec_dprm_jitter 
                ROHC_PROTO((
                rohc_entity_t       * p_entity  ,
                rohc_dsc_t          *p_dsc      ,
                rohc_U8bit_t        jitter      ,
                rohc_error_code_t   *p_ecode 
                ));

rohc_return_t rohc_exec_dprm_cl_resol  
                ROHC_PROTO((
                rohc_entity_t       * p_entity  ,
                rohc_dsc_t          *p_dsc      ,
                rohc_U8bit_t        clck_resol  ,
                rohc_error_code_t   *p_ecode 
                ));

rohc_return_t rohc_exec_dprm_p1_md_transl 
                ROHC_PROTO((
                rohc_entity_t       * p_entity  ,
                rohc_dsc_rtp_t      *p_dsc      ,
                rohc_U8bit_t        new_mode    ,
                rohc_error_code_t   *p_ecode));

rohc_return_t rohc_exec_dprm_p2_md_transl 
                ROHC_PROTO((
                rohc_entity_t     * p_entity ,
                rohc_dsc_udp_t    *p_dsc     ,
                rohc_U8bit_t      new_mode   ,
                rohc_error_code_t *p_ecode));

/* - SPR 17439 */
#endif /* _ROHC_CPRM_EXT_IF_H_ */
