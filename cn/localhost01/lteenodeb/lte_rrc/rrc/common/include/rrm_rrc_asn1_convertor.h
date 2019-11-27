/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrm_rrc_asn1_convertor.h,v 1.2 2010/03/24 10:42:40 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : Converts from RRM interface structures into RRC ASN1
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrm_rrc_asn1_convertor.h,v $
 * Revision 1.2  2010/03/24 10:42:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.1.2.1  2010/01/14 14:34:17  ukr15916
 * redirectedCarrierInfo and idleModeMobilityControlInfo RRM->RRC ASN message construction added
 *
 *
 *
 ****************************************************************************/

#ifndef RRM_RRC_ASN1_CONVERTOR_H
#define RRM_RRC_ASN1_CONVERTOR_H

/****************************************************************************
 * Inculdes
 ****************************************************************************/
#include "uecc_ue_ctx.h"
#include "rrc_common_utils.h"
#include "rrc_asn_enc_dec.h"
#include "rrc_rrm_intf.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: check_and_convert_carrier_freqs_geran_following_arfcns
*
*   DESCRIPTION:
*       This function convert geran_following_arfcns_t to
*       CarrierFreqsGERAN_followingARFCNs
*       and check if input data are valid.
*
*   RETURNS:
*       RRC_SUCCESS - if all data are valid
*       RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et check_and_convert_carrier_freqs_geran_following_arfcns
(
    /* for memory allocation */
    OSCTXT                            *p_asn1_ctx,
    CarrierFreqsGERAN_followingARFCNs *p_asn1_arfcns,  /* destination */
    geran_following_arfcns_t          *p_arfcns        /* source */
);

/******************************************************************************
*   FUNCTION NAME: check_and_convert_carrier_freqs_geran
*
*   DESCRIPTION:
*       This function convert carrier_freqs_geran_t to
*       CarrierFreqsGERAN
*       and check if input data are valid.
*
*   RETURNS:
*       RRC_SUCCESS - if all data are valid
*       RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et check_and_convert_carrier_freqs_geran
(
    OSCTXT                  *p_asn1_ctx,       /* for memory allocation */
    CarrierFreqsGERAN       *p_asn1_cf_geran,  /* destination */
    carrier_freqs_geran_t   *p_cf_geran        /* source */
);

rrc_return_et rrm_rrc_fill_cell_info_geran_r9
(
    /* for memory allocation */
    OSCTXT                                    *p_asn1_ctx,
    CellInfoListGERAN_r9        *p_asn_geran_r9, /* destination */
    cell_info_list_geran_r9_t   *p_geran_r9       /* source */
);

rrc_return_et rrm_rrc_fill_cell_info_utra_tdd_r9
(
    /* for memory allocation */
    OSCTXT                                    *p_asn1_ctx,
    CellInfoListUTRA_TDD_r9        *p_asn_utra_tdd_r9, /* destination */
    cell_info_list_utra_tdd_r9_t   *p_utra_tdd_r9       /* source */
);

rrc_return_et rrm_rrc_fill_cell_info_utra_fdd_r9
(
    /* for memory allocation */
    OSCTXT                                    *p_asn1_ctx,
    CellInfoListUTRA_FDD_r9        *p_asn_utra_fdd_r9, /* destination */
    cell_info_list_utra_fdd_r9_t   *p_utra_fdd_r9       /* source */
);

rrc_return_et uecc_llim_check_and_convert_cell_info_list_r9
(
    uecc_ue_context_t *p_uecc_ue_context,        /* UE context */
    /* for memory allocation */
    OSCTXT                            *p_asn1_ctx,
    RRCConnectionRelease_v920_IEs_cellInfoList_r9 *p_asn1_cell_info_list,  /* destination */
    cell_info_list_r9_t          *p_cell_info_list        /* source */
);

#endif  /* RRM_RRC_ASN1_CONVERTOR_H */
