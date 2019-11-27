/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dprm_update_p1.h,v 1.2.36.2 2010/12/02 08:59:08 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for decompressor
 *                     side DPIB to dsc update operations for profile p1.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dprm_update_p1.h,v $
 * Revision 1.2.36.2  2010/12/02 08:59:08  gur22059
 * Conflicting profile 1 functions were renamed acc to new Naming Convention
 *
 * Revision 1.2.36.1  2010/11/09 04:54:28  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DPRM_UPDATE_P1_
#define _ROHC_DPRM_UPDATE_P1_

/* + SPR 17439 */
rohc_void_t rohc_dprm_p1_type_base_update_pkt
        ROHC_PROTO((rohc_entity_t           *p_entity   ,
                    rohc_dsc_rtp_t          *p_dsc      ,
                    rohc_dprm_update_blk_t  *p_dprm_update_blk
                    ));

rohc_void_t rohc_dprm_p1_type_base_ext3_update_pkt
        ROHC_PROTO((rohc_entity_t   *p_entity   ,
                    rohc_dsc_rtp_t  *p_dsc      ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));
                    
rohc_void_t rohc_dprm_p1_type_ir_irdyn_update_pkt
        ROHC_PROTO((rohc_entity_t   *p_entity   ,
                    rohc_dsc_rtp_t  *p_dsc      ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));
                    
/* - SPR 17439 */
                                                            

#endif /* _ROHC_DPRM_UPDATE_P1_ */
