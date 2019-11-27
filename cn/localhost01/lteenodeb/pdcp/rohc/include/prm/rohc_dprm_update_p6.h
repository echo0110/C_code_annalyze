/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dprm_update_p6.h,v 1.1.1.1.16.1 2010/11/03 06:48:43 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for decompressor
 *                     side DPIB to dsc update operations for profile p6.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef _ROHC_DPRM_UPDATE_P6_
#define _ROHC_DPRM_UPDATE_P6_

rohc_void_t rohcDprmP6BaseUpdatePkt
                    (rohc_entity_t          *p_entity,
                    rohc_dsc_tcp_t         *p_dsc,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    );

rohc_void_t rohcDprmP6CRBaseUpdatePkt
                    (rohc_entity_t          *p_entity,
                    rohc_dsc_tcp_t         *p_dsc,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    );

/* + SPR 17439 */
rohc_void_t rohc_dprm_p6_base_ext3_update_pkt
        ROHC_PROTO((rohc_entity_t *p_entity ,
                    rohc_dsc_tcp_t *p_dsc  ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));
                    
rohc_void_t rohc_dprm_p6_ir_irdyn_update_pkt
        ROHC_PROTO((rohc_entity_t          *p_entity ,
                    rohc_dsc_tcp_t         *p_dsc    ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));

rohc_void_t rohc_dprm_p6_irregular_update_pkt
        ROHC_PROTO((rohc_entity_t          *p_entity ,
                    rohc_dsc_tcp_t         *p_dsc    ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk
                    ));

rohc_void_t rohc_dprm_p6_ir_irdyn_tcp_option
        ROHC_PROTO((rohc_dsc_tcp_t         *p_dsc    ,
                    rohc_dprm_update_blk_t *p_dprm_update_blk ,
                    rohc_U8bit_t XI ,
                    rohc_U8bit_t *p_size ,
                    rohc_U8bit_t *p_opt_list_index
                    ));
/* - SPR 17439 */
#endif /* _ROHC_DPRM_UPDATE_P6_ */
