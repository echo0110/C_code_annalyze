/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccpm_p2_u_done.h,v 1.1.2.1 2010/11/03 09:03:00 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for U-mode
 *        compressor CPM, when compressor transition state is DONE.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ccpm_p2_u_done.h,v $
 * Revision 1.1.2.1  2010/11/03 09:03:00  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/13 04:06:53  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.1.2.1  2010/09/23 06:57:39  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CCPM_P2_U_DONE_
#define _ROHC_CCPM_P2_U_DONE_


/* + SPR 17439 */
rohc_void_t rohc_c_p2_u_ir_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ir_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ir_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_irdyn_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_irdyn_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_irdyn_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_0_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_0_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_0_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_1_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_1_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_1_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_2_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_2_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_uo_2_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ir_rtn_tmr_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ir_rtn_tmr_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ir_rtn_tmr_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_fo_rtn_tmr_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_fo_rtn_tmr_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_fo_rtn_tmr_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ack_u_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ack_u_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_ack_u_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_mt_u_to_o_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_mt_u_to_o_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_mt_u_to_o_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_mt_u_to_r_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_mt_u_to_r_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_u_mt_u_to_r_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));


/* - SPR 17439 */

#endif /*ROHC_CCPM_P2_U_DONE*/

