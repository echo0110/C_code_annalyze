/******************************************************************************
*
*   FILE NAME:
*       csc_bch_m_sibs.h
*
*   DESCRIPTION:
*       This is the header file of functions that conver SIBs representation
*       from RRM to ASN.1. Necessary for UT of these functions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   12 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_BCH_M_SIBS_H_
#define _CSC_BCH_M_SIBS_H_

#include "rrc_rrm_intf.h"
#include "rrc_asn_enc_dec.h"
#include "csc_cell_ctx.h"
#include "rrc_defines.h"

/* Functions declaration for UT */
rrc_return_et bch_m_check_and_convert_sib1
(
    OSCTXT                      *pctxt,
    SystemInformationBlockType1 *p_asn1_sib1,
    sib_type_1_Info_t           *p_sib1,
    rrc_bitmask_t               presence_bitmask,
    csc_cell_ctx_t      *p_cell_ctx
);

rrc_return_et bch_m_check_and_convert_sib2
(
 OSCTXT                      *p_asn1_ctx,
 SystemInformationBlockType2 *p_asn1_sib2,
 /*** TDD changes start ***/
 sib_type_2_Info_t           *p_sib2,
 sib_type_1_Info_t           *p_sib1
 /*** TDD changes stop ***/

 );





rrc_return_et bch_m_check_and_convert_sib3
(
    SystemInformationBlockType3 *p_asn1_sib3,
    sib_type_3_Info_t           *p_sib3
);

rrc_return_et bch_m_check_and_convert_sib4
(
    OSCTXT                      *pctxt,
    SystemInformationBlockType4 *p_asn1_sib4,
    sib_type_4_Info_t           *p_sib4
);

rrc_return_et bch_m_check_and_convert_sib5
(
    OSCTXT                      *pctxt,
    SystemInformationBlockType5 *p_asn1_sib5,
    sib_type_5_Info_t           *p_sib5,
    csc_cell_ctx_t              *p_cell_ctx
);

rrc_return_et bch_m_check_and_convert_sib6
(
    OSCTXT                      *p_asn1_ctx,
    SystemInformationBlockType6 *p_asn1_sib6,
    sib_type_6_Info_t           *p_sib6,
    csc_cell_ctx_t              *p_cell_ctx
);

rrc_return_et bch_m_check_and_convert_sib7
(
    OSCTXT                      *p_asn1_ctx,
    SystemInformationBlockType7 *p_asn1_sib7,
    sib_type_7_Info_t           *p_sib7
);

rrc_return_et bch_m_check_and_convert_sib8
(
    OSCTXT                      *pctxt,
    SystemInformationBlockType8 *p_asn1_sib8,
    sib_type_8_Info_t           *p_sib8,
    csc_cell_ctx_t              *p_cell_ctx,
    U8                          si_segment_counter
);

rrc_return_et bch_m_check_and_convert_sib9
(
    SystemInformationBlockType9 *p_asn1_sib9,
    sib_type_9_Info_t           *p_sib9
);

rrc_return_et bch_m_check_and_convert_sib10
(
    SystemInformationBlockType10    *p_asn1_sib10,
    sib_type_10_Info_t              *p_sib10
);

rrc_return_et bch_m_check_and_convert_sib11
(
    OSCTXT                        *pctxt,         /* for memory allocation */
    SystemInformationBlockType11  *p_asn1_sib11,  /* destination */
    sib_type_11_Info_t              *p_sib11,        /* source */
    U8                            segment_number,  /* Segment number */
    U16                            warning_msg_segment_size, /* warning msg segment size */
    /* Bug_15359 fix starts */
    U32                           warning_msg_segment_type, /* warning msg segment type */
    U16                            segment_size /* last msg segment start */ 
    /* Bug_15359 fix ends */
);

rrc_return_et bch_m_check_and_convert_sib12
(
    OSCTXT                        *pctxt,         /* for memory allocation */
    SystemInformationBlockType12_r9  *p_asn1_sib12,  /* destination */
    sib_type_12_Info_t              *p_sib12,        /* source */
    U8                            segment_number,  /* Segment number */
    U16                            warning_msg_segment_size, /* warning msg segment size */
    U32                           warning_msg_segment_type /* warning msg segment type */
);

#endif /* _CSC_BCH_M_SIBS_H_ */

