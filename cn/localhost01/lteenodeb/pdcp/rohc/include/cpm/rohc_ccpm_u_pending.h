/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccpm_u_pending.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for U-mode 
 *        compressor CPM, when compressor transition state is PENDING.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ccpm_u_pending.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CCPM_U_PENDING_
#define _ROHC_CCPM_U_PENDING_


/* + SPR 17439 */
rohc_void_t rohc_c_u_ir_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ir_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ir_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_irdyn_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_irdyn_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_irdyn_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_0_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_0_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_0_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_1_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_1_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_1_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_2_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_2_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_uo_2_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ir_rtn_tmr_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ir_rtn_tmr_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ir_rtn_tmr_so_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_fo_rtn_tmr_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_fo_rtn_tmr_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_fo_rtn_tmr_so_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ack_u_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ack_u_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_ack_u_so_pnd ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_t *p_csc));

/* - SPR 17439 */
rohc_void_t rohc_c_u_mt_u_to_o_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_mt_u_to_o_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_mt_u_to_o_so_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_mt_u_to_r_ir_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_mt_u_to_r_fo_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));

rohc_void_t rohc_c_u_mt_u_to_r_so_pnd ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_t *p_csc));



#endif /*ROHC_CCPM_U_PENDING*/

