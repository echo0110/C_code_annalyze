/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dprm_update_p23.h,v 1.1.1.1.16.1 2010/11/03 06:48:43 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for decompressor
 *                     side DPIB to dsc update operations for profile p23.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dprm_update_p23.h,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:48:43  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.2.36.2  2010/10/13 04:36:15  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/10/04 03:28:35  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DPRM_UPDATE_P23_
#define _ROHC_DPRM_UPDATE_P23_

/* + SPR 17439 */
rohc_void_t rohc_dprm_p2_base_update_pkt
        ROHC_PROTO((rohc_entity_t          *p_entity ,
                    rohc_dsc_udp_t         *p_dsc    ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));

rohc_void_t rohc_dprm_p2_base_ext3_update_pkt
        ROHC_PROTO((rohc_entity_t *p_entity ,
                    rohc_dsc_udp_t *p_dsc  ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));
                    
rohc_void_t rohc_dprm_p2_ir_irdyn_update_pkt
        ROHC_PROTO((rohc_entity_t          *p_entity ,
                    rohc_dsc_udp_t         *p_dsc    ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));

/* - SPR 17439 */

#endif /* _ROHC_DPRM_UPDATE_P23_ */

