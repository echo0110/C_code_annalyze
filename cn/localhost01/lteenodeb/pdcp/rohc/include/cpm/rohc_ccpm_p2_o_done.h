/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccpm_p2_o_done.h,v 1.1.2.1 2010/11/03 09:01:37 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for O-mode
 *                     compressor CPM, when compressor transition state is DONE.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ccpm_p2_o_done.h,v $
 * Revision 1.1.2.1  2010/11/03 09:01:37  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/13 04:03:44  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.1.2.1  2010/09/23 06:54:50  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CCPM_P2_O_DONE_
#define _ROHC_CCPM_P2_O_DONE_

/* + SPR 17439 */
rohc_void_t rohc_c_p2_o_ir_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_ir_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_ir_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_irdyn_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_irdyn_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_irdyn_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_0_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_0_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_0_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_1_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_1_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_1_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_2_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_2_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_uo_2_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_ack_o_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_ack_o_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_ack_o_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_static_nack_o_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_static_nack_o_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_static_nack_o_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_nack_o_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_nack_o_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_nack_o_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_mt_o_to_u_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_mt_o_to_u_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_mt_o_to_u_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_mt_o_to_r_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_mt_o_to_r_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));

rohc_void_t rohc_c_p2_o_mt_o_to_r_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_udp_t *p_csc));


/* - SPR 17439 */

#endif /*ROHC_CCPM_P2_O_DONE*/

