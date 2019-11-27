/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrc_s1ap_uecc_intf.h,v 1.4 2010/10/13 07:02:09 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description :
 *      This file contains types used for
 *      representation of S1AP - UECC API.
 *      Based on LTE_L3_API_Rel_1_0_Rev_0_45.doc
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrc_s1ap_uecc_intf.h,v $
 * Revision 1.4  2010/10/13 07:02:09  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.5  2010/09/03 11:29:28  gur18569
 * bug fixed while HO UT
 *
 * Revision 1.3.4.4  2010/07/26 06:36:24  gur18569
 *  increased asn buff len size
 *
 * Revision 1.3.4.3  2010/07/21 11:53:05  gur18569
 * incorporated code comments
 *
 * Revision 1.3.4.2  2010/06/24 12:42:32  gur22047
 * internal source and s1ap code
 *
 * Revision 1.3.4.1  2010/06/22 05:30:24  gur22047
 * Internal Interface update
 *
 * Revision 1.3  2010/03/24 09:50:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.6  2010/03/05 10:48:10  gur18569
 * updated comments for s1ap_uecc_sig_msg_req
 *
 * Revision 1.2.2.5  2010/03/05 08:28:26  gur18569
 *  change in s1ap_uecc_sig_mesg_req_t
 *
 * Revision 1.2.2.4  2010/02/01 07:58:00  gur18569
 * removed tabs
 *
 * Revision 1.2.2.3  2010/01/22 10:22:52  ukr16032
 * S1U Path failure indication handling feature has been updated according to the code review.
 *
 * Revision 1.2.2.2  2010/01/19 13:50:44  ukr16032
 * S1U PATH_FAILURE_IND message handling has been added (FT updated)
 *
 * Revision 1.2.2.1  2010/01/13 10:34:34  ukr15916
 * S1AP_UE_RELEASE_MSG_REQ added
 *
 * Revision 1.2  2010/01/04 16:10:03  ukr15916
 * no message
 *
 * Revision 1.1.4.1  2009/11/26 18:33:42  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.4  2009/11/19 15:00:37  ukr16018
 * W80.
 *
 * Revision 1.1.2.3  2009/11/19 09:52:11  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.2  2009/11/16 16:11:00  ukr15916
 * removed old messages
 *
 * Revision 1.1.2.1  2009/11/16 13:37:03  ukr16018
 * S1AP is internal module now 1.
 *
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_RRC_S1AP_UECC_INTF_H
#define INCLUDED_RRC_S1AP_UECC_INTF_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "list_wrap.h"
#include "rrc_s1apOam_intf.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define S1AP_MAX_ASN1_BUF_LEN                   8192

/* Values for s1ap_message_type_et */
typedef enum
{
    SUCCESSFUL_OUTCOME,
    UNSUCCESSFUL_OUTCOME,
    INITIATING_MESSAGE
} s1ap_message_type_et;

/****************************************************************************
 * Exported Types
 ****************************************************************************/

#pragma pack(push, 1)

/******************************************************************************
    RRC_S1AP_INITIAL_UE_MSG_REQ
******************************************************************************/

/******************************************************************************
*   S1AP_DEL_CONTEXT_MSG_IND
******************************************************************************/

typedef struct
{
    U32     enb_ue_s1ap_id;
/*BUG 604 changes start*/
    U16     ue_index;
/*BUG 604 changes stop*/ 
} s1ap_del_ctx_msg_ind_t;

typedef struct
{
    s1ap_PLMNidentity plmn_identity;
    s1ap_MME_Group_ID grp_id;
    s1ap_MME_Code mme_code;
} s1ap_gummei_t;

typedef struct
{
    s1ap_gummei_t  gummei;
}gummei_list_t; 

/******************************************************************************
*   S1AP_STATUS_MSG_IND
******************************************************************************/

typedef struct
{
    U8      bitmask;             /*^ M  ^*/
#define SERVED_GUMMEI_INFO_PRESENT 0x01
    U32    mme_id;
    U8     status;
    s1ap_gummei_t gummei;
    /*^ O, SERVED_GUMMEI_INFO_PRESENT ^*/
    enb_id_info_t       enb_id;
    U8                  enbName[MAX_ENB_NAME_STR_SIZE];
    mme_comm_info_t     connection_info;
} s1ap_status_msg_ind_t;

/*REL 1.2:HANDOVER PROCEDURE START*/
/******************************************************************************
*   S1AP_UE_ASSOCIATED_SIG_MSG_REQ
******************************************************************************/
typedef struct
{
#define REGISTERED_MMEGI_PRESENT 0x01  /* MMEGI if registeredMME is present */
#define REGISTERED_MMEC_PRESENT 0x02  /* MMEC if registeredMME or S-TMSI is
present */
#define UE_ASSOC_SIG_MSG_REQ_MME_UE_S1AP_ID_PRESENT 0x04
#define UE_ASSOC_SIG_MSG_REQ_MME_ID_PRESENT 0x08
#define UE_ASSOC_SIG_MSG_REQ_ENB_UE_S1AP_ID_PRESENT 0x10

    U8      bitmask;             /*^ M  ^*/
    U8      procedure_code;

    s1ap_message_type_et      message_type; /* s1ap_message_type_et */
    U32     enb_ue_s1ap_id;
 /*^ O, UE_ASSOC_SIG_MSG_REQ_ENB_UE_S1AP_ID_PRESENT ^*/
   
    U16     message_buff_size;

    U32 rrc_establishment_cause;    /*^ O, M in case of Intial UE Msg  ^*/
    rrc_cell_index_t  cell_index;
    s1ap_PLMNidentity plmn_identity;  /*^ O, M in case of Initial UE Msg ^*/
    s1ap_MME_Group_ID mme_group_id;   /*^ O ^*/
    s1ap_MME_Code     mmec;           /*^ O ^*/

    U32     mme_ue_s1ap_id;
/*^ O, UE_ASSOC_SIG_MSG_REQ_MME_UE_S1AP_ID_PRESENT ^*/
    U8      mme_id;
/*^ O, UE_ASSOC_SIG_MSG_REQ_MME_ID_PRESENT ^*/
/*BUG 604 changes start*/
    U16     ue_index;         
/*BUG 604 changes stop*/ 

    U8      message_buff_p[0];
/*^ M, 0, OCTET_STRING, TILL_THE_END ^*/ /* ASN-encoded message */
} s1ap_ue_associated_sig_msg_req_t;
/* from UECC to S1AP */

/******************************************************************************
*   S1AP_UE_ASSOCIATED_SIG_MSG_IND
******************************************************************************/
typedef struct
{
#define UE_ASSOC_SIG_MSG_IND_MME_ID_PRESENT         0x01
#define UE_ASSOC_SIG_MSG_IND_REL_OF_MME_PRESENT     0x02
#define UE_ASSOC_SIG_MSG_IND_MME_UE_S1APID_PRESENT  0x04
/*BUG 604 changes start*/
#define UE_ASSOC_SIG_MSG_IND_ENB_UE_S1APID_PRESENT  0x08
/*BUG 604 changes stop*/
    U8      bitmask;              /*^ M  ^*/
    U8      s1ap_procedure_code ; /*^ M  ^*/
    s1ap_message_type_et      message_type; /* s1ap_message_type_et */

    U8      mme_id;               /*^ O, UE_ASSOC_SIG_MSG_IND_MME_ID_PRESENT ^*/
    U32    mme_ue_s1apid;         /*^ O, UE_ASSOC_SIG_MSG_IND_MME_UE_S1APID_PRESENT ^*/
    U8      rel_of_mme;           /*^ O, UE_ASSOC_SIG_MSG_IND_REL_OF_MME_PRESENT ^*/
/*BUG 604 changes start*/
    U16     ue_index;
    U32     enb_ue_s1ap_id;       /*^ O, UE_ASSOC_SIG_MSG_IND_ENB_UE_S1APID_PRESENT ^*/
/*BUG 604 changes stop*/ 
    U8      message_buff_p[0];
/*^ M, 0, OCTET_STRING, TILL_THE_END ^*/ /* ASN-encoded message */

} s1ap_ue_associated_sig_msg_ind_t;
/* from S1AP to UECC */

/*REL 1.2:HANDOVER PROCEDURE END*/

/******************************************************************************
*   S1AP_RESET_MSG_IND
******************************************************************************/

#define MAX_RESET_UE_S1AP_ID_COUNT   256

typedef struct
{
    U16 count;
    U32 enb_ue_s1ap_id[MAX_RESET_UE_S1AP_ID_COUNT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} enb_ue_s1ap_id_list_t;

typedef struct
{
    U16 count;
    U32 mme_ue_s1ap_id[MAX_RESET_UE_S1AP_ID_COUNT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} mme_ue_s1ap_id_list_t;

/*BUG 604 Fixes start*/
typedef struct
{
    U16 count;
    U16 ue_index[MAX_RESET_UE_S1AP_ID_COUNT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} ue_index_list_t;
/*BUG 604 Fixes stop*/

typedef struct
{
    rrc_bitmask_t       bitmask;
/*BUG 604 Fixes start*/
#define S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT      0x01
/*BUG 604 Fixes stop*/
#define S1AP_RESET_MSG_IND_MME_UE_S1AP_ID_LIST_PRESENT      0x02

/*BUG 604 Fixes start*/
    ue_index_list_t   ue_index_list;
/*^ O, S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT ^*/
/*BUG 604 Fixes stop*/

    mme_ue_s1ap_id_list_t   mme_ue_s1ap_id_list;
/*^ O, S1AP_RESET_MSG_IND_MME_UE_S1AP_ID_LIST_PRESENT ^*/

} s1ap_reset_msg_ind_t;

/******************************************************************************
*   S1AP_UE_RELEASE_MSG_REQ
******************************************************************************/

typedef struct
{
    U32 enb_ue_s1ap_id;
    U8  mme_release_required; /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
    /*SPR 15610 Fix Start*/
    U8  release_cause_valid; /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
    s1ap_Cause cause; /* cause shall be read only 
                       * if release_cause_valid is set to TRUE */
    /*SPR 15610 Fix Stop*/
} s1ap_ue_release_msg_req_t;
/* from UECC to S1AP */

/******************************************************************************
*   S1AP_FAILURE_MSG_REQ
******************************************************************************/

typedef struct
{
    rrc_bitmask_t       bitmask;
#define S1AP_FAILURE_MSG_REQ_MME_UE_S1AP_ID_LIST_PRESENT      0x01

    rrc_cell_index_t    cell_index;

    enb_ue_s1ap_id_list_t   enb_ue_s1ap_id_list;

    mme_ue_s1ap_id_list_t   mme_ue_s1ap_id_list;
/*^ O, S1AP_FAILURE_MSG_REQ_MME_UE_S1AP_ID_LIST_PRESENT ^*/

} s1ap_failure_msg_req_t;

/******************************************************************************
* S1AP_OVERLOAD_MSG_IND
*****************************************************************************/
typedef struct
{
    U32 enb_ue_s1ap_id;
    s1ap_Cause cause;
/*BUG 604 changes start*/
    U16     ue_index;
/*BUG 604 changes stop*/ 
} s1ap_overload_msg_ind_t;

typedef struct served_gummei_info
{
    s1ap_ServedPLMNs servedPLMNs;
    U32 num_of_mme_group_ids;
    list_t mme_group_id_list;
    s1ap_ServedMMECs served_mmecs;
} served_gummei_info_t;

/*****************************************************************************
*  S1AP_UE_ASSOCIATED_MME_UPDATE_STATUS_IND
*****************************************************************************/
typedef struct
{
    U32 enb_ue_s1ap_id;
    s1ap_gummei_t gummei;
/*BUG 604 changes start*/
    U16     ue_index;
/*BUG 604 changes stop*/ 
} s1ap_ue_associated_mme_update_status_ind_t;

/******************************************************************************
*   S1AP_UE_RELEASE_IND
******************************************************************************/
typedef struct
{
    U32     enb_ue_s1ap_id;
    U32     mme_ue_s1ap_id;
    U8      mme_id;
}s1ap_ue_release_ind_t;

/******************************************************************************
*   S1AP_UE_REL_REQ_INT
******************************************************************************/

typedef struct
{
    U32 enb_ue_s1ap_id;
/*BUG 604 changes start*/ 
    U16 ue_index;
/*BUG 604 changes stop*/ 
} s1ap_ue_rel_req_int_t;
/* from S1AP to UECC */
/******************************************************************************
*   S1AP_HO_ONGOING_UE_RELEASE_IND
******************************************************************************/

typedef struct
{
    mme_ue_s1ap_id_list_t   mme_ue_s1ap_id_list;

} s1ap_ho_ongoing_ue_release_ind_t;

/******************************************************************************
*   UECCMD_UE_CONTEXT_RELEASE_ONGOING_IND
******************************************************************************/

typedef struct
{
    U32  mme_ue_s1ap_id;
    U8   result;

} ueccmd_ue_context_release_ongoing_ind_t;

/******************************************************************************
*   S1AP_UECCMD_UECC_INFO
******************************************************************************/
typedef struct
{
    U8     num_uecc_instances;
    U16    total_ue_supported;

} s1ap_ueccmd_uecc_info_t;

/******************************************************************************
*   S1AP_ALLOCATE_MME_REQ
******************************************************************************/

#define REGISTERED_MMEGI_PRESENT 0x01  /* MMEGI if registeredMME is present */
#define REGISTERED_MMEC_PRESENT 0x02  /* MMEC if registeredMME or S-TMSI is present */
typedef struct
{
    U8                 bitmask;
    rrc_cell_index_t   cell_index;
/*BUG 604 changes start*/
    //U32                enb_ue_s1ap_id;
/*BUG 604 changes start*/
    U32                rrc_establishment_cause;   
    s1ap_PLMNidentity  plmn_identity;  
    s1ap_MME_Group_ID  mme_group_id;   /*^ O ^*/
    s1ap_MME_Code      mmec;           /*^ O ^*/
/*BUG 604 changes start*/
    U16                ue_index;
/*BUG 604 changes stop*/
} s1ap_allocate_mme_req_t;

/******************************************************************************
*   S1AP_ALLOCATE_MME_RESP
******************************************************************************/
typedef struct
{
    U32        enb_ue_s1ap_id;

    /* release of MME (Rel8/Rel9) */
    U8          rel_of_mme; /*^ M,0,H,0,1 ^*/ /* s1ap_oam_rel_of_mme_et */
    U8          mme_id;
/*BUG 604 changes start*/
    U16         ue_index;
/*BUG 604 changes stop*/
    /* mme grp id feature changes start */
    U8          matching_gummei_to_be_sent;
    /* mme grp id feature changes stop */

} s1ap_allocate_mme_resp_t;

/*BUG 604 changes start*/
/******************************************************************************
*   S1AP_INTRA_CELL_HO_SUCCESS_IND
******************************************************************************/

/* from UECC to S1AP */
typedef struct
{
    U32 enb_ue_s1ap_id;
    U16 ue_index; 
} s1ap_intra_cell_ho_success_ind_t;

/******************************************************************************
*   S1AP_ALLOCATE_S1AP_UEID_REQ
******************************************************************************/

/* from UECC to S1AP */
typedef struct
{
    U16 ue_index; 
} s1ap_allocate_s1ap_ueid_req_t;

/******************************************************************************
*   S1AP_ALLOCATE_S1AP_UEID_RESP
******************************************************************************/

/* from S1AP to UECC*/
typedef struct
{
    U16 ue_index; 
    U32 enb_ue_s1ap_id;
} s1ap_allocate_s1ap_ueid_resp_t;
/*BUG 604 changes stop*/
#pragma pack(pop)

#endif  /* INCLUDED_RRC_S1AP_UECC_INTF_H */


