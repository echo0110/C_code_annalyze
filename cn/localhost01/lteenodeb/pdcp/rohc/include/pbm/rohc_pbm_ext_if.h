/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_pbm_ext_if.h,v 1.2.36.4 2010/11/09 04:51:47 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function prototypes for C-PPM
 *                     be exported out of the stack.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_pbm_ext_if.h,v $
 * Revision 1.2.36.4  2010/11/09 04:51:47  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.3  2010/10/13 04:30:52  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.2  2010/10/04 03:29:09  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.36.1  2010/09/23 07:02:11  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:29:58  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef ROHC_PBM_EXT_IF_H
#define ROHC_PBM_EXT_IF_H

#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_limits.h"
/* + SPR 17439 */
/* +- SPR 17777 */
rohc_return_t  
rohc_make_p0_rohc_compr_pkt
    ROHC_PROTO((
    struct rohc_entity_t        * p_entity              ,
    struct rohc_csc_uncomp_t    * p_csc                 ,
    rohc_U8bit_t                pkt_type                ,
    rohc_U8bit_t                * p_in_buff             ,
    rohc_U32bit_t               * p_pkt_len             ,
    rohc_U8bit_t                * p_rohc_pkt            ,
    rohc_U8bit_t                * p_len_skip_from_pl));

rohc_return_t  
rohc_make_p1_rohc_compr_pkt
    ROHC_PROTO((
    struct rohc_entity_t    * p_entity              ,
    struct rohc_csc_rtp_t   * p_csc                 ,
    rohc_U8bit_t            pkt_type                ,
    rohc_U8bit_t            * p_in_buff             ,
    rohc_U32bit_t           * p_pkt_len             ,
    rohc_U8bit_t            * p_rohc_pkt            ,
    rohc_U8bit_t            * p_len_skip_from_pl    ,
    rohc_error_code_t       * p_ecode));

rohc_return_t  
rohc_make_p2_rohc_compr_pkt
    ROHC_PROTO((
    struct rohc_entity_t  * p_entity ,
    struct rohc_csc_udp_t * p_csc               ,
    rohc_U8bit_t          pkt_type              ,
    rohc_U8bit_t          * p_in_buff           ,
    rohc_U32bit_t         * p_pkt_len           ,
    rohc_U8bit_t          * p_rohc_pkt          ,
    rohc_U8bit_t          * p_len_skip_from_pl  ,
    rohc_error_code_t     * p_ecode));

rohc_return_t  
rohc_make_p6_rohc_compr_pkt
    ROHC_PROTO((
    struct rohc_entity_t    * p_entity              ,
    struct rohc_csc_tcp_t   * p_csc                 ,
    rohc_U8bit_t            pkt_type                ,
    rohc_U32bit_t           * p_pkt_len             ,
    rohc_U8bit_t            * p_rohc_pkt            ,
    rohc_error_code_t       * p_ecode));

rohc_return_t  
getOptionIndex
    ROHC_PROTO((
     rohc_entity_t   * p_entity                 ,
     rohc_U8bit_t            kind               ,
     rohc_c_tcp_opt_t  *p_tcp_opt               ,
     rohc_U8bit_t *p_item_list                  ,
     rohc_csc_tcp_t * p_csc                     ,
     rohc_U8bit_t *p_len,
     rohc_U8bit_t *p_gen_opt_idx));
/* D-PBM */        
rohc_return_t rohc_d_make_p1_ip_pkt
        ROHC_PROTO((
        rohc_U8bit_t            * p_pkt             ,
        rohc_U32bit_t           * p_len             ,
        struct rohc_dsc_rtp_t   * p_dsc             ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)); 
        
/* D-PBM */        
rohc_return_t rohc_d_make_p2_ip_pkt
        ROHC_PROTO((
        rohc_U8bit_t           * p_pkt            ,
        rohc_U32bit_t          * p_len            ,
        struct rohc_dsc_udp_t  * p_dsc            ,
        rohc_dprm_update_blk_t *p_dprm_update_blk)); 
/* - SPR 17439 */        
/* D-PBM */        
rohc_return_t rohcDMakeP6IpPkt(
        rohc_U8bit_t           * p_pkt           ,
        rohc_U32bit_t          * p_len           ,
        struct rohc_dsc_tcp_t  * p_dsc           ,
        rohc_dprm_update_blk_t *p_dprm_update_blk); 
/* +- SPR 17777 */
/* + SPR 17439 */        
/* Decompressor - Make feedback packet */       
rohc_return_t
rohc_d_make_feedback_packet 
        ROHC_PROTO((
        struct rohc_entity_t    * p_entity      ,
        struct rohc_feedback_t  * p_feedback    ,   
        rohc_error_code_t       * p_ecode));
/* - SPR 17439 */
/* Decompressor - Make feedback packet for Profile 6 */       
rohc_return_t
rohcDP6MakeFeedbackPacket( 
        struct rohc_entity_t    * p_entity,
        struct rohc_feedback_t  * p_feedback,   
        rohc_error_code_t       * p_ecode);
/* + SPR 17439 */
void rohc_c_pbm_init
    ROHC_PROTO((void));
rohc_U16bit_t rohc_cal_ip_checksum ROHC_PROTO((
                                            rohc_U8bit_t  *a_buffer , 
                                            rohc_U32bit_t a_size));
/* - SPR 17439 */    

#endif /* ROHC_PBM_EXT_IF_H */


