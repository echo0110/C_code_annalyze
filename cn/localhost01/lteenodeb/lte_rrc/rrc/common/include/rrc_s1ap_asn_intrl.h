/******************************************************************************
*
*   FILE NAME:
*       rrc_s1ap_asn_intrl.h
*
*   DESCRIPTION:
*       This file contains ASN.1 based types for internal representation
*       of S1AP messages in more regular way.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   07 Oct 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_S1AP_ASN_INTRL_H_
#define _RRC_S1AP_ASN_INTRL_H_

#include "rrc_defines.h"
#include "s1ap_asn_enc_dec_3gpp.h"


typedef struct EXTERN TAIListIntrl_element {
    s1ap_TAIItem         value;
} TAIListIntrl_element;

/* List of TAIListIntrl_element */
typedef OSRTDList TAIListIntrl;


typedef struct _rrc_s1ap_asn_intrl_paging_t
{
    rrc_bitmask_t           bitmask;                    /*^ BITMASK ^*/
#define RRC_S1AP_INTRL_PAGING_DRX_PRESENT       0x01
#define RRC_S1AP_INTRL_CSG_ID_LIST_PRESENT      0x02
#define RRC_S1AP_INTRL_PAGING_PRIORITY_PRESENT  0x04

    s1ap_UEIdentityIndexValue    ue_identity_index_value;
    s1ap_UEPagingID              ue_paging_id;
    s1ap_PagingDRX               paging_drx;
/*^ O, RRC_S1AP_INTRL_PAGING_DRX_PRESENT ^*/

    s1ap_CNDomain                cn_domain;
    s1ap_TAIList            tai_list;
    s1ap_CSG_IdList              csg_id_list;
/*^ O, RRC_S1AP_INTRL_CSG_ID_LIST_PRESENT ^*/
    s1ap_PagingPriority         paging_priority;
/*^ O, RRC_S1AP_INTRL_PAGING_PRIORITY_PRESENT ^*/

} rrc_s1ap_asn_intrl_paging_t;

#endif /* _RRC_S1AP_ASN_INTRL_H_ */

