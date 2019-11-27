/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ***************************************************************************
 * File Details
 * ------------
 *  $Id: csc_paging_m.h,v 1.3 2010/04/20 05:43:24 gur21006 Exp $
 ***************************************************************************
 *
 *  File Description : RRC Paging procedure implementation with
 *                     multi UE paging supporting.
 *                     Paging records storage implementation.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * Jan 8, 2010   grag -   initial creation
 * $Log: csc_paging_m.h,v $
 * Revision 1.3  2010/04/20 05:43:24  gur21006
 * paging for si mod period added
 *
 * Revision 1.2  2010/03/11 09:25:40  ukr16032
 * CSC - Multi UE Paging functionality updated according to new requirements.
 * -- CSC_SFN_OFFSET added
 * -- UE Paging records are rearranged now on receiving new SFN indication or due to periodical SFN synchronization.
 *
 * Revision 1.1  2010/01/13 16:38:56  ukr16032
 * CSC Multi UE Paging feature has been implemented.
 *
 *
 ***************************************************************************/

#ifndef CSC_PAGING_M_H
#define CSC_PAGING_M_H

/****************************************************************************
 * Inculdes
 ****************************************************************************/
#include "rrc_defines.h"
#include "csc_cell_ctx.h"
#include "rrc_asn_enc_dec.h"
//#include "s1ap_asn_enc_dec.h"
#include "s1ap_asn_enc_dec_3gpp.h"

#include "rrc_s1ap_asn_intrl.h"
#include "csc_global_ctx.h"

#define CSC_SFN_OFFSET                  5
#define CSC_SI_PWS_SFN_OFFSET           5
#define CSC_SFN_REARRANGE_LIMIT         100


/******************************************************************************
*   FUNCTION NAME: csc_paging_storage_init
*
*   DESCRIPTION:
*       Allocates and initializes CSC Paging storage.
*
*   RETURNS:
*       pointer on paging storage
*
******************************************************************************/
void* csc_paging_storage_init(void);

/******************************************************************************
*   FUNCTION NAME: csc_paging_storage_free
*
*   DESCRIPTION:
*       Clean up CSC Paging storage and frees all allocated resources.
*
*   RETURNS:
*       None
*
******************************************************************************/
void csc_paging_storage_free(void* p_storage,
        rrc_bool_et is_cell_stopped);

/******************************************************************************
*   FUNCTION NAME: csc_paging_process_ue_paging_request
*
*   DESCRIPTION:
*       Processes UE Paging request.
*       If UE is suitable for paging the corresponding Paging record will be
*       stored in the CSC Paging storage and UE will be paged
*       within appropriate System frame.
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_t csc_paging_process_ue_paging_request(
        csc_cell_ctx_t              *p_cell_ctx,
        rrc_s1ap_asn_intrl_paging_t *p_s1ap_intrl_paging,
        /* Current SFN in RRC cell */
        rrc_sfn_t       rrc_curr_sfn,
        csc_gl_ctx_t    *p_csc_gl_ctx
);

/******************************************************************************
*   FUNCTION NAME: csc_paging_page_ues
*
*   DESCRIPTION:
*       Makes and sends UE Paging requests to all UEs for which
*       Paging records with appropriate SFN are stored in CSC Paging storage.
*
*   RETURNS:
*       None
*
******************************************************************************/
void csc_paging_page_ues(
        csc_cell_ctx_t      *p_cell_ctx
        /*SPR 17777 +-*/
);

/******************************************************************************
*   FUNCTION NAME: csc_paging_rearrange_ues
*
*   DESCRIPTION:
*       Rearranges UE Paging requests in case when CSC current SFN is
*       changed by MAC.
*
*   RETURNS:
*       None
*
******************************************************************************/
void csc_paging_rearrange_ues(
        /*SPR 17777 +-*/
        csc_cell_ctx_t      *p_cell_ctx,
        U16                 sfn_mac
);

void csc_paging_build_and_send_broadcast_info(
    rrc_sfn_t    sfn,
    U8          paging_type,
    csc_cell_ctx_t  *p_cell_ctx
);
/*SPR 21554 Start*/
void csc_paging_plmn_identity_rrc_to_s1ap
(
    /* Destination - size must be NUM_OCTET_IN_S1AP_PLMN_ID */
    U8              *p_plmn_id_as_s1ap,
    /* Source - C struct that represent PLMN Id from RRC specification */
    plmn_identity_t *p_plmn_identity
);
/*SPR 21554 End*/



#ifdef RRC_CSC_DEBUG_FT
rrc_return_t csc_paging_add_paging_record_to_rrc_paging_message_ut
(
    OSCTXT                      *p_asn1_ctx, /* ASN.1 context */
    PCCH_Message                *p_pcch_message,
    rrc_s1ap_asn_intrl_paging_t *p_s1ap_intrl_paging
);
#endif /*RRC_CSC_DEBUG_FT*/
#endif /* CSC_PAGING_M_H */
