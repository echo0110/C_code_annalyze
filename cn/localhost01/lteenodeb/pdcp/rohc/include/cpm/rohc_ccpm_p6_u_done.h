/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for U-mode
 *        compressor CPM, when compressor transition state is DONE.
 *
 ****************************************************************************/

#ifndef _ROHC_CCPM_P6_U_DONE_
#define _ROHC_CCPM_P6_U_DONE_


/* + SPR 17439 */
rohc_void_t rohc_c_p6_ir_ir_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_dyn_fo_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_rnd_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_common_comp_so_dn ROHC_PROTO((rohc_entity_t *p_entity ,\
                                                       rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_ir_rtn_tmr_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_fo_rtn_tmr_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_ack_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));
rohc_void_t rohc_c_p6_ack_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));
rohc_void_t rohc_c_p6_ack_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_nack_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));

rohc_void_t rohc_c_p6_nack_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));
rohc_void_t rohc_c_p6_nack_so_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));
rohc_void_t rohc_c_p6_static_nack_ir_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));
rohc_void_t rohc_c_p6_static_nack_fo_dn ROHC_PROTO((rohc_entity_t *p_entity\
                                                 , rohc_csc_tcp_t *p_csc));
/* - SPR 17439 */
#endif /*ROHC_CCPM_P6_U_DONE*/

