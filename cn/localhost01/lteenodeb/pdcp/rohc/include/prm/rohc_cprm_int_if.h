/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_cprm_int_if.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains functions declaration for internal
 *                     interface C-PRM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_cprm_int_if.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CPRM_INT_IF_H_
#define _ROHC_CPRM_INT_IF_H_

typedef struct rohc_p1_pkt_typ_info
{   
    rohc_U8bit sn_bits;
    rohc_U8bit ts_bits;
    rohc_U8bit ip_id_bits;
    rohc_U8bit pbm_packet_type;
} rohc_p1_pkt_typ_info_t;

/*rohc_p1_pkt_typ_info_t rohc_p1_pkt_typ_info_uo[MAX_UO_P1_PACKETS];*/

/*rohc_p1_pkt_typ_info_t rohc_p1_pkt_typ_info_r[MAX_R_P1_PACKETS];*/

typedef struct rohc_p23_pkt_typ_info
{   
    rohc_U8bit sn_bits;
    rohc_U8bit ip_id_bits;
    rohc_U8bit pbm_packet_type;
}rohc_p23_pkt_typ_info_t;

/*rohc_p23_pkt_typ_info_t rohc_p23_pkt_typ_info_uo[MAX_UO_P23_PACKETS];*/

/*rohc_p23_pkt_typ_info_t rohc_p23_pkt_typ_info_r[MAX_R_P23_PACKETS];*/


static rohc_U8bit rohc_cprm_resolve_pkt_typ_p1_uo (rohc_entity_t *p_entity,\
                                            rohc_csc_t *p_csc,
                rohc_p23_pkt_typ_info_t[] *p_rohc_pkt_typ_info_p23_uo,\
                                            rohc_U8bit sn_bits,\
                                            rohc_U8bit ts_bits,\
                                            rohc_U8bit ip_id_bits)

static rohc_U8bit rohc_cprm_resolve_pkt_typ_p1_r (rohc_entity_t *p_entity,\
                                           rohc_csc_t  *p_csc,\
                rohc_p23_packet_type_info_t[] *p_rohc_pkt_typ_info_p23_r,\
                                           rohc_U8bit sn_bits,\
                                           rohc_U8bit ts_bits,\
                                           rohc_U8bit ip_id_bits)

static rohc_U8bit rohc_c_resolve_packet_type_p23_r (rohc_entity_t *p_entity,\
                                            rohc_glb_ctx_blk_t * p_glb_ctx_blk,\
                                            rohc_csc_t *p_csc,\
                rohc_p1_pkt_typ_info_t *p_rohc_pkt_typ_info_p1_uo,\
                                            rohc_U8bit sn_bits,\
                                            rohc_U8bit ts_bits,\
                                            rohc_U8bit ip_id_bits)

static rohc_U8bit rohc_c_resolve_packet_type_p23_r(rohc_entity_t *p_entity,\
                                           rohc_glb_ctx_blk_t * p_glb_ctx_blk,\
                                           rohc_csc_t *p_csc,\
                rohc_p1_pkt_typ_info_t *p_rohc_pkt_typ_info_p1_r,\
                                           rohc_U8bit sn_bits,\
                                           rohc_U8bit ts_bits,\
                                           rohc_U8bit ip_id_bits)

static rohc_U8bit rohc_c_resolve_packet_type_uncomp(rohc_entity_t *p_entity,\
                                            rohc_csc_t *p_csc)

#endif /* _ROHC_CPRM_INT_IF_H_ */
