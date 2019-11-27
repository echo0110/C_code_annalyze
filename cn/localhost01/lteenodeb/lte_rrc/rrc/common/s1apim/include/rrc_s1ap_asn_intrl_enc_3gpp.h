/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrc_s1ap_asn_intrl_enc_3gpp.h,v 1.13 2010/12/01 12:56:56 gur19827 Exp $
 ****************************************************************************
 *
 *  File Description :
 *   This file contains functions that encode
 *   ASN.1 based internal representation of S1AP messages
 *   into ASN.1 encoded S1AP messages.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rrc_s1ap_asn_intrl_enc_3gpp.h,v $
 * Revision 1.13  2010/12/01 12:56:56  gur19827
 * Merged Inter RAT HO
 *
 * Revision 1.12  2010/10/13 07:02:22  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.4  2010/08/05 08:02:31  gur15697
 * merging round 2
 *
 * Revision 1.3.4.3  2010/07/26 06:37:56  gur18569
 * Added s1ap ho requst intrl enc
 *
 * Revision 1.3.4.2  2010/07/21 08:21:13  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.3.4.1  2010/06/23 13:35:10  gur15697
 * Updated for EUTRAN/UTRAN asn enc dec changes
 *
 * Revision 1.3  2010/03/24 09:50:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.1  2010/01/08 12:52:33  ukr15916
 * error_indication_added
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.1  2009/11/26 18:33:45  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.14  2009/11/19 15:00:37  ukr16018
 * W80.
 *
 * Revision 1.1.2.13  2009/11/19 09:52:20  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.12  2009/11/16 14:45:02  ukr18877
 * S1-AP moved to internal interfaces
 *
 * Revision 1.1.2.11  2009/11/14 16:29:55  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.2.10  2009/11/11 19:04:26  ukr15916
 * rrc_s1ap_initial_context_setup_response_intrl_enc(),
 *  rrc_s1ap_initial_context_setup_failure_intrl_enc() added
 *
 * Revision 1.1.2.9  2009/11/10 16:45:55  ukr15916
 * rrc_s1ap_initial_context_setup_request_intrl_dec(),
 *  rrc_s1ap_initial_context_setup_request_intrl_enc() added
 *
 * Revision 1.1.2.8  2009/11/10 11:42:41  ukr15916
 * downlink_nas_transport and nas_non_delivery_indication updated for ASN S1AP
 *
 * Revision 1.1.2.7  2009/11/09 15:10:51  ukr15916
 * ue_release_request updated for ASN S1AP
 *
 * Revision 1.1.2.6  2009/11/09 11:43:54  ukr15916
 * uplink_nas_transport updated to S1AP ASN
 *
 * Revision 1.1.2.5  2009/11/08 13:58:07  ukr15916
 * rrc_s1ap_initial_ue_message_t added
 *
 * Revision 1.1.2.4  2009/11/08 12:53:15  ukr15916
 * s1ap_ue_context_release_complete ASN version implemented
 *
 * Revision 1.1.2.3  2009/11/03 12:29:41  ukr15916
 * rrc_s1ap_ue_context_release_command_intrl_enc() finished
 *
 *
 ****************************************************************************/

#ifndef _RRC_S1AP_ASN_INTRL_ENC_3GPP_H_
#define _RRC_S1AP_ASN_INTRL_ENC_3GPP_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_s1ap_asn_intrl_3gpp.h"
#include "rrc_s1ap_uecc_intf.h"
#include "uecc_global_ctx.h"
#include "uecc_logger.h"

/****************************************************************************
 * Exported Variable Declarations
 ****************************************************************************/
extern S1AP_PDU s1ap_pdu;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

rrc_return_et rrc_s1ap_initial_context_setup_request_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_initial_context_setup_request_t *p_initial_context_setup_request
);

rrc_return_et rrc_s1ap_initial_context_setup_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_initial_context_setup_response_t *p_initial_context_setup_response
);

rrc_return_et rrc_s1ap_initial_context_setup_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_initial_context_setup_failure_t *p_initial_context_setup_failure
);

rrc_return_et rrc_s1ap_ue_context_release_request_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_release_request_t   *p_ue_context_release_request
);

rrc_return_et rrc_s1ap_ue_context_release_command_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_release_command_t   *p_ue_context_release_command
);

rrc_return_et rrc_s1ap_ue_context_release_complete_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_release_complete_t  *p_ue_context_release_complete
);

rrc_return_et rrc_s1ap_initial_ue_message_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_initial_ue_message_t   *p_initial_ue_message   /* source */
);

rrc_return_et rrc_s1ap_downlink_nas_transport_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_downlink_nas_transport_t   *p_downlink_nas_transport   /* source */
);

rrc_return_et rrc_s1ap_uplink_nas_transport_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_uplink_nas_transport_t *p_uplink_nas_transport /* source */
);

rrc_return_et rrc_s1ap_nas_non_delivery_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_nas_non_delivery_indication_t  *p_nas_non_delivery_indication
);

rrc_return_et rrc_uecc_error_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_error_indication_t  *p_error_indication
);

/* Trace Start Procedure start */
rrc_return_et rrc_s1ap_trace_failure_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_trace_failure_indication_t *p_trace_failure_indication
);
/* Trace Start Procedure end */

rrc_return_et rrc_s1ap_ue_capability_info_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_capability_info_indication_t  *p_ue_capability_info_indication
);


rrc_return_et rrc_s1ap_handover_required_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT   *pctxt,         /* for memory allocation */
    U8           *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_required_t *p_handover_required
);

rrc_return_et rrc_s1ap_rrc_container_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_RRC_Container  *p_rrc_container,

    /* source */
    HandoverPreparationInformation *p_ho_preparation_information
);

rrc_return_et rrc_s1ap_source_enb_to_targer_enb_container_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_Source_ToTarget_TransparentContainer  *p_source_to_target_transparent_container,

    /* source */
    s1ap_SourceeNB_ToTargeteNB_TransparentContainer *p_container
);

rrc_return_et rrc_s1ap_enb_status_transfer_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT   *pctxt,         /* for memory allocation */
    U8           *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_enb_status_transfer_t *p_enb_status_transfer
);

rrc_return_et rrc_s1ap_handover_cancel_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_cancel_t   *p_ue_handover_cancel
);

rrc_return_et rrc_s1ap_handover_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_failure_t   *p_ue_handover_failure
);

rrc_return_et rrc_s1ap_handover_notify_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_notify_t   *p_ue_handover_notify
);

rrc_return_et rrc_s1ap_handover_request_ack_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_request_ack_t *p_ue_handover_request_ack
);

rrc_return_et rrc_s1ap_target_enb_to_source_enb_container_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    s1ap_Target_ToSource_TransparentContainer  *p_container,
    s1ap_TargeteNB_ToSourceeNB_TransparentContainer *p_enb_container,     /* source */
    OSCTXT      *pctxt         /* for memory allocation */
);

rrc_return_et rrc_var_short_mac_input_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    VarShortMAC_Input  *p_var_short_mac_input
);

rrc_return_et rrc_s1ap_last_visited_utran_cell_information_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */

     /* source */
    ranap_LastVisitedUTRANCell_Item  *p_last_visited_utran_cell_Item
);

rrc_return_et rrc_s1ap_handover_request_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U32         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_request_test_t *p_handover_request
);

/* ERB RELEASE COMMAND START */
rrc_return_et rrc_s1ap_e_rab_release_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT*      pctxt,         /* for memory allocation */
    U8*          p_buff,        /* ASN.1 encoded message */
    U16*         p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_release_response_t* p_e_rab_release_response
);
/* ERB RELEASE COMMAND END */

/* ERB RELEASE INDICATION START */
rrc_return_et rrc_s1ap_e_rab_release_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT*      pctxt,         /* for memory allocation */
    U8*          p_buff,        /* ASN.1 encoded message */
    U16*         p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_e_rab_release_indication_t* p_e_rab_release_indication
);

/* ERB RELEASE INDICATION END */

rrc_return_et rrc_s1ap_initial_ue_context_modification_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_modification_response_t *p_ue_context_mod_response
 );


rrc_return_et rrc_s1ap_ue_context_modification_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_modification_failure_t *p_ue_context_mod_response
 );


rrc_return_et rrc_s1ap_erab_setup_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_setup_response_t *p_erab_setup_response
);

rrc_return_et rrc_s1ap_erab_modify_request_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_modify_request_t *p_erab_modify_request
);
 rrc_return_et rrc_s1ap_erb_modify_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message*/

    U16         *p_buff_size,   /* size of result*/

    /* source */
    rrc_s1ap_erab_modify_response_t *p_erb_modify_response
);

rrc_return_et rrc_s1ap_erab_setup_request_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_s1ap_erab_setup_request_t *p_erab_setup_request
);
rrc_return_et rrc_s1ap_e_rab_release_command_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_s1ap_erab_release_command_t *p_erab_release_command
);

rrc_return_et rrc_s1ap_ue_context_modification_request_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_modification_request_t *p_ue_context_modification_request
);
rrc_return_et rrc_s1ap_old_bss_to_new_bss_information_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_Source_ToTarget_TransparentContainer  *p_source_to_target_transparent_container,

    /* source */
   s1ap_OldBSS_ToNewBSS_Information  container
);

rrc_return_et rrc_s1ap_source_bss_to_target_bss_container_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_Source_ToTarget_TransparentContainer  *p_source_to_target_transparent_container,

    /* source */
   s1ap_SourceBSS_ToTargetBSS_TransparentContainer container
);

rrc_return_et uecc_s1ap_location_report_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    rrc_s1ap_location_report_t* p_report_msg,
    U8*     p_buff,
    U16*    p_size
);

rrc_return_et uecc_s1ap_ue_associated_location_report_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    rrc_s1ap_failure_report_t*      p_failure_msg,
    U8*                             p_buff,
    U16*                            p_size
 );


/****************************************************************************
* Function Name  : rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc
* Inputs         : pctxt (for memory allocation),
*                   p_handover_required
* Outputs        : p_buff (ASN.1 encoded message),
*                  p_buff_size (size of result buffer)
* Returns        : RRC_SUCCESS / RRC_FAILURE
* Description    : packs  ranap_SourceRNC_ToTargetRNC_TransparentContainer
*                   into s1ap_Source_ToTarget_TransparentContainer ASN
*****************************************************************************/
rrc_return_et rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 OSCTXT                                    *pctxt,   /* for memory allocation */
 s1ap_Source_ToTarget_TransparentContainer *p_source_to_target_transparent_container,
 ranap_SourceRNC_ToTargetRNC_TransparentContainer *p_container /* source */
);


/****************************************************************************
 * Function Name  : uecc_s1ap_handover_command_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  p_handover_command
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_handover_request into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_command_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U32         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_s1ap_handover_command_test_t *p_handover_command
 );
/****************************************************************************
 * Function Name  : rrc_s1ap_cell_traffic_trace_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  p_cell_traffic_trace
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_cell_traffic_trace into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_cell_traffic_trace_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT*      pctxt,         /* for memory allocation */
    U8*          p_buff,        /* ASN.1 encoded message */
    U16*         p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_cell_traffic_trace_t* p_cell_traffic_trace
);


/* CDMA + CSFB Changes start:  */
rrc_return_et rrc_s1ap_uplink_s1_cdma2000_tunneling_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */ 
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_uplink_s1_cdma2000_tunneling_t *p_uplink_s1_cdma2000_tunneling /* source */
);
/* CDMA + CSFB Changes end:  */

/*x2ap-start*/
rrc_return_et rrc_s1ap_path_switch_request_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_path_switch_request_t   *p_path_switch_request   /* source */
);

/* CSR 101813 Fix Start */
rrc_return_et rrc_ranap_target_RNC_to_source_RNC_container_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    s1ap_Target_ToSource_TransparentContainer  *p_container,
    ranap_TargetRNC_ToSourceRNC_TransparentContainer *p_rnc_container,     /* source */
    OSCTXT      *pctxt         /* for memory allocation */
);
rrc_return_et rrc_ranap_target_BSS_to_source_BSS_container_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    s1ap_Target_ToSource_TransparentContainer  *p_container,
    ranap_TargetBSS_ToSourceBSS_TransparentContainer *p_bss_container,     /* source */
    OSCTXT      *pctxt         /* for memory allocation */
);
/* CSR 101813 Fix Stop */
/*x2ap-end*/
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED

rrc_return_et compose_s1ap_secondary_rat_data_usage_report_list(
    OSCTXT * p_asn1_ctx,
    OSRTDList * p_IE_secondary_rat_data_usage_report_list,
    rrc_s1ap_SecRATDataReport_list_t * p_secondary_rat_data_usage_report_list
);

rrc_return_et rrc_s1ap_erab_modification_ind_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_modification_ind_t *p_erab_modification_ind
);
#endif
/* OPTION3X Changes End */
#endif /* _RRC_S1AP_ASN_INTRL_ENC_3GPP_H_ */

