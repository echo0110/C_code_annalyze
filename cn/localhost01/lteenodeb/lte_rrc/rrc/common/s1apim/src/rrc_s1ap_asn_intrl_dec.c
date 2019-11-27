/******************************************************************************
*
*   FILE NAME:
*       rrc_s1ap_asn_intrl_dec.c
*
*   DESCRIPTION:
*       This file contains decode functions that convert
*       S1AP ASN.1 encoded messages into ASN.1 based internal representation.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   07 Oct 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#include "rrc_s1ap_asn_intrl_dec.h"
#include "rrc_common_utils.h"


/****************************************************************************
 * FUNCTION       :  rrc_s1ap_asn_intrl_dec_paging
 * INPUT          :  OSCTXT                      *pctxt
 *                   rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging
 *                   s1ap_Paging                    *p_3gpp
 * OUTPUT         :  None
 * RETURN         :  RRC_SUCCESS
 * DESCRIPTION    :  This Function decodes the S1AP internal Paging Message.
 ****************************************************************************/
rrc_return_et rrc_s1ap_asn_intrl_dec_paging
(
    rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging,   /* destination */
    /* ASN.1 encoded message from S1AP */
    s1ap_Paging                    *p_3gpp
)
{

    s1ap_Paging_protocolIEs_element *p_protocol_IE = RRC_NULL;
    OSRTDListNode            *p_node       = PNULL;

    U32                      index = 0;

    /* Clear Bitmask */
    p_s1ap_asn_intrl_paging->bitmask = 0;



    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_Paging_protocolIEs_element *)p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_UEIdentityIndexValue:
                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_UEIdentityIndexValue",__FUNCTION__);

                        l3_memcpy_wrapper(&p_s1ap_asn_intrl_paging->ue_identity_index_value,
                                (p_protocol_IE->value.u._PagingIEs_1),
                                sizeof(p_s1ap_asn_intrl_paging->ue_identity_index_value));

                    break;

                case ASN1V_s1ap_id_UEPagingID:

                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_UEPagingID",__FUNCTION__);
                        p_s1ap_asn_intrl_paging->ue_paging_id.t = p_protocol_IE->value.u._PagingIEs_2->t;
                        if ( T_s1ap_UEPagingID_s_TMSI == p_s1ap_asn_intrl_paging->ue_paging_id.t)
                        {
                          p_s1ap_asn_intrl_paging->ue_paging_id.u.s_TMSI =
                              p_protocol_IE->value.u._PagingIEs_2->u.s_TMSI;
                        }
                        else if ( T_s1ap_UEPagingID_iMSI == p_s1ap_asn_intrl_paging->ue_paging_id.t)
                        {
                          p_s1ap_asn_intrl_paging->ue_paging_id.u.iMSI =
                              p_protocol_IE->value.u._PagingIEs_2->u.iMSI;
                        }
                        else if ( T_s1ap_UEPagingID_extElem1 == p_s1ap_asn_intrl_paging->ue_paging_id.t)
                        {
                          p_s1ap_asn_intrl_paging->ue_paging_id.u.extElem1->numocts =
                              p_protocol_IE->value.u._PagingIEs_2->u.extElem1->numocts;
                          p_s1ap_asn_intrl_paging->ue_paging_id.u.extElem1->data =
                              p_protocol_IE->value.u._PagingIEs_2->u.extElem1->data;
                        }

                    break;

                case ASN1V_s1ap_id_pagingDRX:

                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_pagingDRX",__FUNCTION__);

                        p_s1ap_asn_intrl_paging->paging_drx = 
                                p_protocol_IE->value.u._PagingIEs_3;

                        p_s1ap_asn_intrl_paging->bitmask |=
                            RRC_S1AP_INTRL_PAGING_DRX_PRESENT;
                    break;

                case ASN1V_s1ap_id_CNDomain:
                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_CNDomain",__FUNCTION__);

                        l3_memcpy_wrapper(
                        &p_s1ap_asn_intrl_paging->cn_domain,
                         &p_protocol_IE->value.u._PagingIEs_4,
                         sizeof(p_s1ap_asn_intrl_paging->paging_drx));

                    break;


                case ASN1V_s1ap_id_TAIList:

                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_TAIList",__FUNCTION__);

                        l3_memcpy_wrapper(&p_s1ap_asn_intrl_paging->tai_list,
                                   p_protocol_IE->value.u._PagingIEs_5,
                                   sizeof(p_s1ap_asn_intrl_paging->tai_list));



                    break;

                case ASN1V_s1ap_id_CSG_IdList:

                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_CSG_IdList",__FUNCTION__);

                        l3_memcpy_wrapper(&p_s1ap_asn_intrl_paging->csg_id_list,
                            p_protocol_IE->value.u._PagingIEs_6,
                            sizeof(p_s1ap_asn_intrl_paging->csg_id_list));


                        p_s1ap_asn_intrl_paging->bitmask |=
                            RRC_S1AP_INTRL_CSG_ID_LIST_PRESENT;

                    break;

                case ASN1V_s1ap_id_PagingPriority:

                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_PagingPriority",__FUNCTION__);

                        p_s1ap_asn_intrl_paging->paging_priority =
                            p_protocol_IE->value.u._PagingIEs_7;

                        p_s1ap_asn_intrl_paging->bitmask |=
                            RRC_S1AP_INTRL_PAGING_PRIORITY_PRESENT;

                    break;

            }

            p_node = p_node->next;
        }
    }while(0);


    return RRC_SUCCESS;

}


