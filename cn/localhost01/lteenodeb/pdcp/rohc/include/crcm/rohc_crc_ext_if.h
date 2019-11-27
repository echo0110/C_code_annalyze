/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_crc_ext_if.h,v 1.2.36.4 2010/11/09 04:47:55 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_crc_ext_if.h,v $
 * Revision 1.2.36.4  2010/11/09 04:47:55  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.3  2010/10/13 04:22:57  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.2  2010/10/04 03:29:50  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.36.1  2010/09/23 06:58:54  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:59:56  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef __ROHC_CRC_EXT_IF__
#define __ROHC_CRC_EXT_IF__

#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"

#define ROHC_CRC_TYPE_3 3
#define ROHC_CRC_TYPE_7 7
#define ROHC_CRC_TYPE_8 8

/* + SPR 17439 */
 /* +- SPR 17777 */
rohc_return_t rohc_c_crc_p1_compute ROHC_PROTO (( 
                    rohc_csc_rtp_t  *csc        , 
                    rohc_U8bit_t    type));

rohc_return_t rohc_c_crc_p2_compute ROHC_PROTO (( 
                    rohc_csc_udp_t  *csc        , 
                    rohc_U8bit_t    type));

rohc_return_t rohc_c_crc_p6_compute ROHC_PROTO (( 
                    rohc_csc_tcp_t  *csc        , 
                    rohc_U8bit_t    type));
 /* +- SPR 17777 */
rohc_return_t rohc_d_crc_p1_compute_comp_pkt ROHC_PROTO ((
                    rohc_entity_t           *p_entity   ,
                    rohc_dsc_rtp_t          *dsc        ,
                    rohc_dprm_update_blk_t  *p_blk));

rohc_return_t rohc_d_crc_p2_compute_comp_pkt ROHC_PROTO ((
                    rohc_entity_t          *p_entity ,
                    rohc_dsc_udp_t         *dsc      ,
                    rohc_dprm_update_blk_t *p_blk ));

rohc_return_t rohc_d_crc_p6_compute_comp_pkt ROHC_PROTO ((
                    rohc_entity_t          *p_entity ,
                    rohc_dsc_tcp_t         *dsc      ,
                    rohc_dprm_update_blk_t *p_blk));
/* +- SPR 17777 */

rohc_U8bit_t rohc_compute_crc ROHC_PROTO((
                              rohc_U8bit_t  type    ,
                              rohc_U8bit_t  *data   , 
                              rohc_U32bit_t length  ,
                              rohc_U8bit_t  crc_init)); 

rohc_void_t rohc_init_crc ROHC_PROTO((void));
/* - SPR 17439 */
#endif
