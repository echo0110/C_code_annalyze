/******************************************************************************
*
*   FILE NAME:
*       rrc_s1ap_asn_intrl_dec.h
*
*   DESCRIPTION:
*       This file contains declaration of decode functions that convert
*       S1AP ASN.1 encoded messages into ASN.1 based internal representation.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   07 Oct 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_S1AP_ASN_INTRL_DEC_H_
#define _RRC_S1AP_ASN_INTRL_DEC_H_

#include "rrc_s1ap_asn_intrl.h"

rrc_return_et rrc_s1ap_asn_intrl_dec_paging
(
 rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging,   /* destination */
 /* ASN.1 encoded message from S1AP */
 s1ap_Paging                    *p_3gpp
);


#endif /* _RRC_S1AP_ASN_INTRL_DEC_H_ */

