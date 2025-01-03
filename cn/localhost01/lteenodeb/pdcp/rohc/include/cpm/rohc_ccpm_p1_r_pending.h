/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccpm_p1_r_pending.h,v 1.1.2.1 2010/11/09 04:32:49 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for R-mode
 *           compressor CPM, when compressor transition state is PENDING.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ccpm_p1_r_pending.h,v $
 * Revision 1.1.2.1  2010/11/09 04:32:49  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.1.2.1  2010/10/13 04:06:07  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.1.2.1  2010/09/23 06:56:58  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CCPM_P1_R_PENDING_
#define _ROHC_CCPM_P1_R_PENDING_


/* + SPR 17439 */
rohc_void_t rohc_c_p1_r_ir_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_ir_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_ir_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_irdyn_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_irdyn_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_irdyn_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_0_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_0_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_0_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_0_crc_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_0_crc_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_0_crc_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_1_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_1_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_1_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_2_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_2_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_r_2_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_ack_r_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_ack_r_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_ack_r_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_static_nack_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_static_nack_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_static_nack_so_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_nack_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_nack_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_nack_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_mt_r_to_u_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_mt_r_to_u_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_mt_r_to_u_so_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_mt_r_to_o_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_mt_r_to_o_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));

rohc_void_t rohc_c_p1_r_mt_r_to_o_so_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_rtp_t *p_csc));


/* - SPR 17439 */

#endif /*ROHC_CCPM_P2_R_PENDING*/

